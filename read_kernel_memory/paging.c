/*
  Henry Wong <henry@stuffedcow.net>
  http://blog.stuffedcow.net/2015/08/pagewalk-coherence/
  
  2015-08-12
*/

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>

#define PAE 0				// Sorry, no runtime detection of 32-bit PAE. Too lazy.

#define DELAY_REMAP 0		
/* 
  Mode 0: Delay the remapping store to give more time for the pagewalk to be speculatively executed
  before the store executes, which should encourage more speculative pagewalks. In practice, doesn't
  seem to affect results.
 
  Mode 1: Make the remap store dependent on the imul chain, so it can't execute early.
  This causes the load DTLB-miss to execute before the preceding store-address operation.
  Can distinguish processors with non-speculative memory systems because the load (and page walk)
  can't pass the earlier store that got delayed (that modifies the page table). 
  Makes latency for case 2 increase to roughly match case 3.
  Caution: On Intel (Core2 through Haswell), if the page walk occurs too early, there is a penalty of some sort 
  (DTLB miss count is doubled, and it takes longer.) This appears to be the same penalty
  when there is a true page table coherence violation. This might hint at how
  page walk coherence violation detection happens...
     Sandy Bridge: Penalty if DTLB miss occurs > 10 cycles before store-address of preceding store.
     Ivy Bridge: >9 cycles
     Haswell: >9 cycles
*/


#define REPEAT2(X) X X
#define REPEAT3(X) X X X
#define REPEAT4(X) X X X X
#define REPEAT8(X) REPEAT4(X) REPEAT4(X)
#define REPEAT12(X) REPEAT4(X) REPEAT4(X) REPEAT4(X)
#define REPEAT14(X) REPEAT4(X) REPEAT4(X) REPEAT4(X) REPEAT2(X)
#define REPEAT16(X) REPEAT4(X) REPEAT4(X) REPEAT4(X) REPEAT4(X)
#define REPEAT32(X) REPEAT16(X) REPEAT16(X)

#if __x86_64__
	typedef uint64_t pte_t;	// 64-bit always uses 8-byte PTEs
	typedef uint64_t reg_t;	// 64-bit integer registers
	typedef uint64_t paddr_t;	// 52-bit physical address
	int pte_per_pde = 512;		// 512 for PAE/64bit, 1024 for 32-bit.
	const int translation_levels = 4;
	const int translation_shifts[4] = { 12, 21, 30, 39 };

#elif __i386__
	typedef uint32_t reg_t;	// 32-bit integer registers
#if PAE
	typedef uint64_t pte_t;	// 32-bit PAE uses 8-byte PTEs
	typedef uint64_t paddr_t;	// 52-bit physical address
	int pte_per_pde = 512;		// 512 for PAE/64bit, 1024 for 32-bit.
	const int translation_levels = 3;
	const int translation_shifts[3] = { 12, 21, 30 };
#else
	typedef uint32_t pte_t;	// 32-bit non-PAE uses 4-byte PTEs.
	typedef uint32_t paddr_t;	// 32-bit physical address
	int pte_per_pde = 1024;		// 512 for PAE/64bit, 1024 for 32-bit.
	const int translation_levels = 2;
	const int translation_shifts[2] = { 12, 22 };
	
#endif  // PAE

#else
#error "Unknown architecture"
#endif

#define ITS 1024


struct s_opts {
	int preload;	// 0 = flush TLB, 1 = page 1, 2 = page 2.
	unsigned int asize;		// array size, in 4K pages
	int mode;		// Which test to run.
	int chain_deps;	// Adjust data dependencies for mode 1
	bool dont_remap;	// Change pointer to page table to avoid the remap. Applies to mode 0 and 1.
} opts;


inline uint64_t rdtsc()
{
	uint32_t lo, hi;

	__asm__ volatile (".byte 0x0f, 0x31" : "=a" (lo), "=d" (hi));
	return (uint64_t)(((uint64_t)hi)<<32LL) | (uint64_t) lo;
}

int fd_devmem = -1;

void read_physical(paddr_t addr, void* buf, size_t size)
{
	if (fd_devmem < 0)
		return;
		
	lseek64(fd_devmem, addr, SEEK_SET);
	read(fd_devmem, buf, size);
}

void write_physical(paddr_t addr, const void* buf, size_t size)
{
	if (fd_devmem < 0)
		return;
	if (addr < 65536)
		printf ("Something seems wrong. Writing to address %p refused.\n", (void*)addr);
		
	lseek64(fd_devmem, addr, SEEK_SET);
	write(fd_devmem, buf, size);
}


void pgmod_flush_tlb()
{
	int f = open("/proc/pgmod", O_WRONLY);
	write(f, "hi", 1);
	close(f);
}



// Translates virtual address to physical.
// 4KB paging in 32-bit, PAE, and 64-bit modes are similar enough this can be done with 
// one routine and a bunch of architecture-specific typedefs.
// If level > 0, stop early by "level" lookups and return the PTE. Useful for getting
//   a phyaddr pointer to a page table to edit it.
//   0 : Do a complete translation
//   1 : Stop at the final PTE. Return the final PTE
//   2 : Return the PDE, one level up the tree
pte_t translate(uintptr_t cr3, uintptr_t vaddr, int level=0)
{
	// Canonical form only applies to 64-bit pointers with 48-bit virtual address space.
	if ((sizeof(vaddr) == 8) && (uint64_t)vaddr >> 48 != 0 && (uint64_t)vaddr >> 48 != 0xffff)
	{
		printf ("vaddr %016llx is not in canonical form\n", (uint64_t)vaddr);
		return 0;
	}

	pte_t pte = 0;
	paddr_t cur_ptr = cr3;
	
	for (int l = translation_levels-1; l >= 0; l--)
	{
		int cur_offset = (vaddr >> translation_shifts[l]) & (pte_per_pde-1);
		read_physical(cur_ptr + cur_offset*sizeof(pte), &pte, sizeof(pte));

		if ((l != 0) && (pte & 0x80)) {
			printf ("Large page at level %d detected. Only 4 KB pages supported\n", l);
			exit(1);	// Page tables not cleaned up before exit. Expect lots of log messages.
			return 0;
		}

		if (level == l+1) return pte;

		if (~pte & 1) {
			printf ("Page table level %d entry %016llx not present. Fail to translate %016llx\n", l, (uint64_t)pte, (uint64_t)vaddr);
			exit(1);
			return 0;
		}
		cur_ptr = pte & 0x000ffffffffff000ULL;
	}
	
	return cur_ptr | (vaddr&0xfff);		// pte_t is always at least as big as paddr_t
}

// Changes a PTE. Returns the old PTE. Only for the lowest level of page tables.
pte_t change_pte(uintptr_t cr3, uintptr_t vaddr, pte_t new_pte)
{
	pte_t pde = translate(cr3, vaddr, 2) & 0x000ffffffffff000ULL;
	int cur_offset = (vaddr>>12) & (pte_per_pde-1);
	
	pte_t old_pte; 
	read_physical(pde+cur_offset*sizeof(pte_t), &old_pte, sizeof(pte_t));
	write_physical(pde+cur_offset*sizeof(pte_t), &new_pte, sizeof(pte_t));
	//printf ("Updating PTE from %016llx to %016llx\n", (uint64_t)old_pte, (uint64_t)new_pte);
	
	return old_pte;
}


bool parse_opts(int argc, char *argv[])
{

	memset(&opts, 0, sizeof(opts));
	opts.asize = 4096;
	opts.chain_deps = 4;	// Default to imul dependent on previous imul, but the load is independent.

	bool fail = false;
	for (int i=1;i<argc;i++) {
		if (!strcmp(argv[i], "--preload")) {
			if (i+1 >= argc) {
				printf ("--preload needs an argument\n");
				fail = true;
			} else {
				sscanf(argv[++i], "%i", &opts.preload);
			}
		}
		else if (!strcmp(argv[i], "--asize")) {
			if (i+1 >= argc) {
				printf ("--asize needs an argument\n");
				fail = true;
			} else {
				sscanf(argv[++i], "%u", &opts.asize);
				if (opts.asize > 16384 || opts.asize <= 0)
				{
					printf ("--asize must not exceed 16K pages because randindex[] uses 16-bit page numbers\n");
					fail = true;
				}
			}
		}
		else if (!strcmp(argv[i], "--mode")) {
			if (i+1 >= argc) {
				printf ("--mode needs an argument\n");
				fail = true;
			} else {
				sscanf(argv[++i], "%i", &opts.mode);
				if (opts.mode > 1 || opts.mode < 0)
				{
					printf ("--mode out of range\n");
					fail = true;
				}
			}
		}
		else if (!strcmp(argv[i], "--dont_remap")) {
			opts.dont_remap = 1;
		}
		else if (!strcmp(argv[i], "--chain_deps")) {
			if (i+3 >= argc) {
				printf ("--chain_deps needs three arguments\n");
				fail = true;
			} else {
				int t1, t2, t3;
				sscanf(argv[++i], "%i", &t1);
				sscanf(argv[++i], "%i", &t2);
				sscanf(argv[++i], "%i", &t3);
				opts.chain_deps = (t1 ? 4 : 0) | (t2 ? 2 : 0) | (t3 ? 1:0) ;
			}
		}		
		else if (!strcmp(argv[i], "--chain_depsb")) {
			if (i+1 >= argc) {
				printf ("--chain_depsb needs one argument\n");
				fail = true;
			} else {
				int t1;
				sscanf(argv[++i], "%i", &t1);
				opts.chain_deps = t1 & 7;
			}
		}		
		else {
			printf ("unknown option '%s'\n", argv[i]);
			fail = true;
		}
	}
	
	
	
	
	if (fail)
	{
			printf (
				"\n"
				"Usage: paging [options]\n"
				"  Creates some arrays, changes page table mappings to map the page tables of a large\n"
				"  array to be visible through the small array. Then changes the page table mappings\n"
				"  of the large array and see which mapping is actually used by the processor\n"
				" Options:\n"
				"    --asize <n>        Array size, in 4 KB pages. Default is 4096. Normally chosen\n"
				"                       to be big enough to overflow all TLBs and cause page walks\n"
				"                       Memory footprint is roughly 4 KB + asize * (8 byte + one PTE)\n"
				"    --preload <n>      Before the start of the test, try to preload the TLB with mappings\n"
				"                       pointing to page 1 or 2. 0 means flush the TLB before tests.\n"
				"                       Used to test how long PTEs can stay in the TLB with smaller asize.\n"
			 	"    --mode <0-1>       Which test to run. Mode 0 tests for speculative page walks. Mode 1\n"
			 	"                       measures whether page walks can be overlapped with arithmetic\n"
			 	"    --dont_remap       Remove PTE remapping to test for any TLB coherence mechanisms.\n"
			 	"    --chain_deps a b c\n"
			 	"    --chain_depsb n\n"
			 	"                       For mode 1 (see source!), controls the data dependenies between the\n"
			 	"                       imul chain and the load. 1 means dependent. There are three dependencies:\n"
			 	"                       a: Controls whether the imul chain is dependent on the previous imul chain\n"
			 	"                       b: Controls whether the imul chain is dependent on the previous load\n"
			 	"                       c: Controls whether the load address is dependent on the imul chain\n"
			 	"                       n: A single integer where bits [2:0] are a, b, and c\n"
			 	"                       Default is 1 0 0 (or n=4)\n"
			 	"                       This allows detecting under which cases arithmetic operations can be\n"
			 	"                       overlapped with the page walk.\n"			 	
			);
	}
	return !fail;
}

int main(int argc, char *argv[])
{
	if (!parse_opts(argc, argv)) {
		printf ("Bad options.\n");
		return 1;
	}

	const unsigned int ASIZE = opts.asize;
	// Size of page tables for bigbuf, in pages.
	int num_pde_to_map = ((ASIZE+2*pte_per_pde-2)/pte_per_pde);		
	

	// Lets see if our module to retrieve cr3 exists...
	FILE *f = fopen("/proc/pgmod","rt");
	if (!f) {
		printf("Can't open /proc/pgmod. Module not loaded?\n");
		return -1;
	}
	
	fd_devmem = open("/dev/mem", O_RDWR | O_SYNC);
	if (fd_devmem == -1)
	{
		printf ("Can't open /dev/mem. Permissions?\n");
		return -2;
	}
		
	uint64_t cr3, cr4;
	fscanf(f, "%llx ", &cr3);
	fscanf(f, "%llx ", &cr4);
	fclose(f);

	printf ("Compiled for %s\n", translation_levels == 4 ? "x86-64" : translation_levels == 3 ? "x86 PAE" : "x86 non-PAE");
	printf ("CR3 appears to be %016llx\n", cr3);
	printf ("  num_pde_to_map = %d\n", num_pde_to_map);
	printf ("  array size = %d pages\n", opts.asize);
	printf ("  translation levels = %d\n", translation_levels);
	printf ("CR4 appears to be %016llx\n", cr4);
	printf ("  PAE = %s\n", (cr4 & 0x20) ? "Enabled" : "Disabled");
	if ((pte_per_pde == 512) != !!(cr4 & 0x20)) {
		printf ("PAE state unexpected. Quitting.\n");		// PAE has 512 PTEs per page table, non-PAE has 1024.
		return -3;
	}
	
	pte_t *old_pagetable = (pte_t*) malloc(ASIZE*(sizeof(pte_t)));
	pte_t *old_pgmap_ptes = (pte_t*) malloc(num_pde_to_map*(sizeof(pte_t)));
	pte_t *scratch_memory = (pte_t*) malloc(ASIZE*(sizeof(pte_t)));		// For mode = 2
	
	void *bigbuf = mmap(NULL, ASIZE*4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS | MAP_POPULATE, -1, 0);
	
	if (bigbuf == MAP_FAILED) {
		printf ("mmap of %d MB fail\n", ASIZE*4096/1048576);
	}
	else {
		printf ("mmap of %d MB at %p\n", ASIZE*4096/1048576, bigbuf);
	}
	if (mlock (bigbuf, ASIZE*4096) != 0)
	{
		printf ("mlock of %d MB failed.\n", ASIZE*4096/1048576);
	}
	

	pte_t *pt_window = (pte_t*) mmap(NULL, num_pde_to_map*4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS | MAP_POPULATE, -1, 0);
	
	if (pt_window == MAP_FAILED) {
		printf ("mmap of pt_window array failed\n");
	}
	else {
		printf ("mmap of pt_window at %p\n", pt_window);
	}
	if (mlock (pt_window, num_pde_to_map*4096) != 0)
	{
		printf ("mlock of pt_window failed.\n");
	}	
	
	// Sanity check: Do a translation for every page in bigbuf to check for hugepages before 
	// we start tinkering with page table mappings.
	for (unsigned int i = 0; i < ASIZE; i++) {
		translate(cr3, (uintptr_t)bigbuf + (i<<12), 0);
	}
	

	// Back up 9 mappings used for the page table window and remap it to the page tables for bigbuf.
	for (int i=0;i<num_pde_to_map;i++) {
		old_pgmap_ptes[i] = change_pte(cr3, (uintptr_t)pt_window+(i<<12), 
			translate(cr3, ((uintptr_t)bigbuf & ~(4096*pte_per_pde-1)) + i*(4096*pte_per_pde), 2) );
	}
	
	// Flushing TLB didn't seem necessary. Does accessing /dev/mem flush TLBs?
	pgmod_flush_tlb();

	// Get base of the 4096-entry array of PTEs
	volatile pte_t *const p_pte = &pt_window[(((uintptr_t)bigbuf >> 12) & (pte_per_pde-1)) + 0];
	
	// dont_remap causes the PTE update to write into unrelated memory.
	// The operations are the same. This tests for PTE coherence (whether writing into the PTE 
	// behaves differently than writing into some other location in memory)
	volatile pte_t *p_tinkered_pte = opts.dont_remap ? scratch_memory : p_pte;
	
	
	// Back up page table that was exposed by pt_window
	for (unsigned int i=0;i<ASIZE;i++) {
		old_pagetable[i] = p_pte[i];
	}
	printf ("Page mapping setup complete\n");


	// We now have a bunch of arrays:
	//   bigbuf: An array of 4096 pages. Iterate through this at random to reduce TLB hits
	//   pt_window: Page table window. A 9-page space that is mapped to the 9 page tables that control the 
	//      mapping of bigbuf. bigbuf is usually not 2 or 4 MB aligned, so pt_window[0] usually is not the 
	//      PTE for bigbuf[0]. Accessing the pt_window array can edit the last level of page tables that maps bigbuf.
	//   old_pagetable: A backup of the 4096 PTEs that map bigbuf. This was copied out of pt_window,
	//      and is used to restore all of the page mappings before quitting, so the OS doesn't complain
	//      of corrupted page tables.
	//   old_pgmap_ptes: A backup of the 9 PTEs that mapped pt_window. Restored before exit.
	
	printf ("Sanity check: Translating bigbuf %016llx:\n    %016llx cr3", (uint64_t)bigbuf, (uint64_t)cr3);
	
	for (int i=translation_levels;i>=1;i--) {
		printf ("\n -> %016llx", (uint64_t)translate(cr3, (uintptr_t)bigbuf, i));
	}
	printf (" pte\n");
	
	pte_t map_pte1 = old_pagetable[0];	// Use the first two pages of bigbuf as real backing memory.
	pte_t map_pte2 = old_pagetable[1];
	
	for (int i=0;i<1024;i++) {
		((int*)(bigbuf))[i] = 1;
		((int*)(bigbuf))[i+1024] = 2;
	}
	
	
	// Initialize array with random permutation of pages to visit.
	uint16_t *randindex = new uint16_t[ASIZE];
	uint32_t *randindex2 = new uint32_t[ASIZE*2];	// struct of 2 elements
	for (unsigned int i=0;i<ASIZE;i++) {
		int j = (rand() % (i+1));		// Slight bias acceptable.
		randindex[i] = randindex[j];
		randindex[j] = i;
	}
	
	
	// Different optimization for 32-bit: 32-bit uses smaller pointers, so randindex can store
	// an entire pointer at half the L1 footprint vs. 64-bit. But 32-bit is really short on 
	// registers, so precomputing the pointers saves a register.
#if __x86_64
	for (unsigned int i=0;i<ASIZE;i++) {
		randindex2[2*i] = randindex[i] * sizeof(pte_t);
		// If the L1 cache is PIPT, we don't need to worry about conflict misses, because the memory footprint is only
		// 8 KB (two pages). VIPT caches with alias detection (AMD K7 through K10/Phenom II) has performance problems
		// (lots of aliases in the cache). Cycling through cache lines using the lower 6 bits of the page number to
		// select the cache line within a page means each physical cache line is only accessible by addresses
		// of the same bits [17:12], which should eliminate cache misses due to aliasing in a VIPT cache, if the
		// cache way size is less than 256 KB (2^18).
		// There is probably no 4K aliasing between bigbuf and p_pte accesses. Page offset is 
		// 64*randindex here, and 8 or 4*randindex for p_pte.
		randindex2[2*i+1] = ((randindex[i]<<12) | ((randindex[i]&0x3f)<<6));	
	}
	volatile void *p_pte_opt = p_tinkered_pte;
	void *bigbuf_opt = bigbuf;
#elif __i386__
	for (unsigned int i=0;i<ASIZE;i++) {
		randindex2[2*i] = (uint32_t)((char*)p_tinkered_pte + randindex[i]*sizeof(pte_t));
		randindex2[2*i+1] = (uint32_t)((char*)bigbuf + ((randindex[i]<<12) | ((randindex[i]&0x3f)<<6)));
	}
	volatile void *const p_pte_opt = 0;
	void *const bigbuf_opt = 0;
#else
#error "Unknown architecture"
#endif
	
	
	unsigned int counts[ITS][4] = {{0}};
	
	uint64_t start_time=0, end_time=0;
	int inner_its;


	// Prefetch initial mappings into TLB.
	if (opts.preload != 0)
	{
		unsigned int lcounts[2] = {0};
		for (unsigned int i=0;i<ASIZE;i++) {
			p_pte[i] = (opts.preload == 1) ? map_pte1 : map_pte2;
		}
		pgmod_flush_tlb();
		for (unsigned int i=0;i<ASIZE;i++) {
			unsigned int n;
			unsigned int index = randindex[i];
			volatile unsigned int* p_array =  (volatile unsigned int*)((char*)bigbuf + ((index<<12) | ((index&0x3f)<<6)));
			n = *p_array;
			lcounts[n-1]++;
		}
		//printf ("Preload pass: %d %d\n", lcounts[0], lcounts[1]);		// Don't print: printf might flush the TLB
		if (lcounts[0] + lcounts[1] != ASIZE) {
			printf ("Why doesn't %u + %u add up to %d?\n", lcounts[0], lcounts[1], ASIZE);
			goto cleanup;
		}
	}
	else {
		pgmod_flush_tlb();
	}
	

	// Always initialize PTE to point to first page, even if TLB was preloaded with second page.
	for (unsigned int i=0;i<ASIZE;i++) {
		p_pte[i] = map_pte1;
	}

	inner_its = 4096/ASIZE;
	if (inner_its < 1) inner_its = 1;
	if (inner_its > 4096) inner_its = 4096;

	if (opts.mode == 0) {
	
		unsigned int lcounts[4] = {0};		// 32-bit: We're out of registers anyway. Accumulate results in a memory variable to avoid branches.
	
		start_time = rdtsc();

		for (int j=0;j<ITS;j++) {
			unsigned int n = 0;
			for (int k=0; k< inner_its;k++) {
			if (!opts.preload)
				pgmod_flush_tlb();		// Flush TLB between iterations to guarantee no stray TLB hits across iterations. Don't flush when preloading.
			for (unsigned int* i=randindex2;i != &randindex2[2*ASIZE];) {
				
				char * p_array = (char*)bigbuf_opt + *(i+1);
				volatile pte_t * p_thispte = (pte_t*)((char*)p_pte_opt+(*i));
				i+=2;

				// Be careful with volatile: Must stop compiler from reordering the memory operations
				// but also not force the compiler to do unnecessary load/stores. Verify the disassembly.
	#if DELAY_REMAP
				asm volatile (
					"shr $16,%0\n"				// This makes n = 0, but delayed
					REPEAT2("imul %0,%0\n")
					 : "+r"(n)); 
				// Try to delay the store to increase opportunity for speculative page walk to 
				// occur before the store executes. Make it dependent on previous iteration.
				asm volatile("mov %0, (%1,%q2)\n" : : "r"(map_pte2), "r"(p_thispte), "r"(n));
	#else
				*p_thispte = map_pte2;
	#endif
				n = *(volatile unsigned int*)p_array;		// n is always 1 or 2.
				lcounts[n+1]++;

				*p_thispte = map_pte1;
				n = *(volatile unsigned int*)p_array;
				lcounts[n-1]++;
			}		
		
			counts[j][0] = lcounts[0];
			counts[j][1] = lcounts[1];
			counts[j][2] = lcounts[2];
			counts[j][3] = lcounts[3];
			lcounts[0] = 0; lcounts[1] = 0; lcounts[2] = 0; lcounts[3] = 0;
		}
		}
	
		end_time = rdtsc();
	}
	else if (opts.mode == 1)
	{
		printf ("chain_deps: %d\n", opts.chain_deps);
	
		start_time = rdtsc();
		
		uint32_t x = 0;		// AMD is slow at 64-bit multiplications. x always remains zero.
		unsigned int n = 1;

		// Be careful about alignment of inner loops. AMD Bulldozer/Piledriver is extremely sensitive to alignment.
		// Compile with -falign-labels=8 or greater. GCC doesn't seem to recognize the loops as... loops.

		if (opts.chain_deps == 0) {
			for (int j=0;j<ITS;j++) {
				for (int k=0; k< inner_its;k++)
				for (unsigned int* i=randindex2;i != &randindex2[2*ASIZE];) {
					// Long dependence chain of imuls, fractured to improve scheduling for processors with small schedulers.
					// Should be short enough to not overflow ROB and schedulers,
					// but long enough to have latency comparable to a page walk, and avoid interfering with the memory system.
					// IMUL r,r is good because it has few uops (1) and fairly long latency (~3).		
					asm volatile (
						"mov $0, %0\n"		// Not dependent on either previous imul nor load. 
											// PPro doesn't recognize xor r,r as dependency-breaking.
						REPEAT2("imul %0, %0\n")
						: "+r"(x));

					volatile pte_t * p_thispte = (pte_t*)((char*)p_pte_opt+(*i));
					char * p_array = (char*)bigbuf_opt + *(i+1); //randindex2[2*i+1];
					i+=2;
					asm volatile (
						REPEAT14("imul %0, %0\n")
						: "+r"(x) : "r"(p_thispte),"r"(p_array));

#if DELAY_REMAP
					asm volatile("mov %0, (%1,%q2)\n" : : "r"(map_pte1), "r"(p_thispte), "r"(x));
#else
					asm volatile("mov %0, (%1)\n" : : "r"(map_pte1), "r"(p_thispte));
#endif	
					asm volatile("mov (%1), %0\n" : "=r"(n) : "r"(p_array));					
				}		
		
				counts[j][0] = n;			// Meaningless. Just to prevent optimization.
				counts[j][1] = x;
			}
		}	
		else if (opts.chain_deps == 1) {
			for (int j=0;j<ITS;j++) {
				for (int k=0; k< inner_its;k++) {
				for (unsigned int* i=randindex2;i != &randindex2[2*ASIZE];) {
					asm volatile (
						"mov $0, %0\n"		
						REPEAT2("imul %0, %0\n")
						: "+r"(x));

					volatile pte_t * p_thispte = (pte_t*)((char*)p_pte_opt+(*i));
					char * p_array = (char*)bigbuf_opt + *(i+1);
					i+=2;
					asm volatile (
						REPEAT14("imul %0, %0\n")
						: "+r"(x) : "r"(p_thispte),"r"(p_array));

#if DELAY_REMAP
					asm volatile("mov %0, (%1,%q2)\n" : : "r"(map_pte1), "r"(p_thispte), "r"(x));
#else
					asm volatile("mov %0, (%1)\n" : : "r"(map_pte1), "r"(p_thispte));
#endif	

					// Compiler likes to generate 3 instructions for this.
					// n = *((volatile unsigned int*)(p_array + x));	
					asm volatile("mov (%1,%q2), %0\n" : "=r"(n) : "r"(p_array),"r"(x));	// Load address depends on result of imul chain x
				}		
				}
		
				counts[j][0] = n;
				counts[j][1] = x;
			}
		}	
		else if (opts.chain_deps == 2) {
			for (int j=0;j<ITS;j++) {
				for (int k=0; k< inner_its;k++)
				for (unsigned int* i=randindex2;i != &randindex2[2*ASIZE];) {
					asm volatile (
						"lea -1(%1), %0\n"		// Dependent on previous load, but not previous imul chain.
						REPEAT2("imul %0, %0\n")
						: "+r"(x) : "r"(n));

					volatile pte_t * p_thispte = (pte_t*)((char*)p_pte_opt+(*i));
					char * p_array = (char*)bigbuf_opt + *(i+1);
					i+=2;
					asm volatile (
						REPEAT14("imul %0, %0\n")
						: "+r"(x) : "r"(p_thispte),"r"(p_array));

#if DELAY_REMAP
					asm volatile("mov %0, (%1,%q2)\n" : : "r"(map_pte1), "r"(p_thispte), "r"(x));
#else
					asm volatile("mov %0, (%1)\n" : : "r"(map_pte1), "r"(p_thispte));
#endif					
					asm volatile("mov (%1), %0\n" : "=r"(n) : "r"(p_array));
				}		
		
				counts[j][0] = n;
				counts[j][1] = x;
			}
		}							
		else if (opts.chain_deps == 3) {
			for (int j=0;j<ITS;j++) {
				for (int k=0; k< inner_its;k++)
				for (unsigned int* i=randindex2;i != &randindex2[2*ASIZE];) {
					asm volatile (
						"lea -1(%1), %0\n"		// Dependent on previous load, but not previous imul chain.
						REPEAT2("imul %0, %0\n")
						: "+r"(x) : "r"(n));

					volatile pte_t * p_thispte = (pte_t*)((char*)p_pte_opt+(*i));
					char * p_array = (char*)bigbuf_opt + *(i+1);
					i+=2;
					asm volatile (
						REPEAT14("imul %0, %0\n")
						: "+r"(x) : "r"(p_thispte),"r"(p_array));
#if DELAY_REMAP
					asm volatile("mov %0, (%1,%q2)\n" : : "r"(map_pte1), "r"(p_thispte), "r"(x));
#else
					asm volatile("mov %0, (%1)\n" : : "r"(map_pte1), "r"(p_thispte));
#endif		
					asm volatile("mov (%1,%q2), %0\n" : "=r"(n) : "r"(p_array),"r"(x));
				}		
		
				counts[j][0] = n;
				counts[j][1] = x;
			}
		}		
		else if (opts.chain_deps == 4) {
			for (int j=0;j<ITS;j++) {
				for (int k=0; k< inner_its;k++)
				for (unsigned int* i=randindex2;i != &randindex2[2*ASIZE];) {
					asm volatile (
						"add %0, %0\n"		// Dependent on previous imul chain
						REPEAT2("imul %0, %0\n")
						: "+r"(x));

					volatile pte_t * p_thispte = (pte_t*)((char*)p_pte_opt+(*i));
					char * p_array = (char*)bigbuf_opt + *(i+1);
					i+=2;
					asm volatile (
						REPEAT14("imul %0, %0\n")				
						: "+r"(x) : "r"(p_thispte),"r"(p_array));										

#if DELAY_REMAP
					asm volatile("mov %0, (%1,%q2)\n" : : "r"(map_pte1), "r"(p_thispte), "r"(x));
#else
					asm volatile("mov %0, (%1)\n" : : "r"(map_pte1), "r"(p_thispte));
#endif	
					asm volatile("mov (%1), %0\n" : "=r"(n) : "r"(p_array));					
				}		
		
				counts[j][0] = n;
				counts[j][1] = x;
			}
				asm("nop");
		}
		else if (opts.chain_deps == 5) {
			for (int j=0;j<ITS;j++) {
				for (int k=0; k< inner_its;k++)
				for (unsigned int* i=randindex2;i != &randindex2[2*ASIZE];) {
					asm volatile (
						"add %0, %0\n"		// Dependent on previous imul chain
						REPEAT2("imul %0, %0\n")
						: "+r"(x));

					volatile pte_t * p_thispte = (pte_t*)((char*)p_pte_opt+(*i));
					char * p_array = (char*)bigbuf_opt + *(i+1);
					i+=2;
					asm volatile (
						REPEAT14("imul %0, %0\n")
						: "+r"(x) : "r"(p_thispte),"r"(p_array));	

#if DELAY_REMAP
					asm volatile("mov %0, (%1,%q2)\n" : : "r"(map_pte1), "r"(p_thispte), "r"(x));
#else
					asm volatile("mov %0, (%1)\n" : : "r"(map_pte1), "r"(p_thispte));
#endif	
					asm volatile("mov (%1,%q2), %0\n" : "=r"(n) : "r"(p_array),"r"(x));
				}		
		
				counts[j][0] = n;
				counts[j][1] = x;
			}
		}
		else if (opts.chain_deps == 6) {
			for (int j=0;j<ITS;j++) {
				for (int k=0; k< inner_its;k++)
				for (unsigned int* i=randindex2;i != &randindex2[2*ASIZE];) {
					asm volatile (
						"and %1, %0\n"		// Dependent on previous load and imul chain
						REPEAT2("imul %0, %0\n")
						: "+r"(x) : "r"(n));

					volatile pte_t * p_thispte = (pte_t*)((char*)p_pte_opt+(*i));
					char * p_array = (char*)bigbuf_opt + *(i+1);
					i+=2;
					asm volatile (
						REPEAT14("imul %0, %0\n")
						: "+r"(x) : "r"(p_thispte),"r"(p_array));

#if DELAY_REMAP
					asm volatile("mov %0, (%1,%q2)\n" : : "r"(map_pte1), "r"(p_thispte), "r"(x));
#else
					asm volatile("mov %0, (%1)\n" : : "r"(map_pte1), "r"(p_thispte));
#endif		
					asm volatile("mov (%1), %0\n" : "=r"(n) : "r"(p_array));
				}		
		
				counts[j][0] = n;
				counts[j][1] = x;
			}
		}
		else if (opts.chain_deps == 7) {
			for (int j=0;j<ITS;j++) {
				for (int k=0; k< inner_its;k++)
				for (unsigned int* i=randindex2;i != &randindex2[2*ASIZE];) {
					asm volatile (
						"and %1, %0\n"		// Dependent on previous load and imul chain
						REPEAT2("imul %0, %0\n")
						: "+r"(x) : "r"(n));

					volatile pte_t * p_thispte = (pte_t*)((char*)p_pte_opt+(*i));
					char * p_array = (char*)bigbuf_opt + *(i+1);
					i+=2;
					asm volatile (
						REPEAT14("imul %0, %0\n")
						: "+r"(x) : "r"(p_thispte),"r"(p_array));
						
#if DELAY_REMAP
					asm volatile("mov %0, (%1,%q2)\n" : : "r"(map_pte1), "r"(p_thispte), "r"(x));
#else
					asm volatile("mov %0, (%1)\n" : : "r"(map_pte1), "r"(p_thispte));
#endif		
					asm volatile("mov (%1,%q2), %0\n" : "=r"(n) : "r"(p_array),"r"(x));
				}		
		
				counts[j][0] = n;
				counts[j][1] = x;
			}
		}
	
		end_time = rdtsc();	
	}
	

	pgmod_flush_tlb();

	// counts[i][0] : Expect 1, got 1
	// counts[i][1] : Expect 1, got 2
	// counts[i][2] : Expect 2, got 1
	// counts[i][3] : Expect 2, got 2
	// counts[0 and 1] often has the same results as count[2 and 3] because counts[2 and 3]'s 
	// page walk loaded the entry into the TLB, which the later counts[0 and 1] uses. The counts for 
	// counts[2 and 3] depend on whether the processor does non-coherent speculative page table walks.
	printf ("Final counts: (expect 1 got 1, 1 2, 2 1, 2 2)\n");

	// Tabulate results per iteration, printing out a new row only if it is 
	// different from the previous iteration, and compute a final sum.
	{
		unsigned int lcounts[4] = {0};
		for (int i=0;i < ITS;i++) {
			int repeat_count = 0;
			for (int j=0; j+i < ITS; j++) 	{
				if (counts[i+j][0] == counts[i][0] &&
					counts[i+j][1] == counts[i][1] &&
					counts[i+j][2] == counts[i][2] &&
					counts[i+j][3] == counts[i][3]) {
						repeat_count++;
				}
				else {
					break;
				}
			}
			printf ("  %u %u   %u %u  (x%d)\n", counts[i][0], counts[i][1], counts[i][2], counts[i][3], repeat_count);
			lcounts[0] += counts[i][0] * repeat_count;
			lcounts[1] += counts[i][1] * repeat_count;
			lcounts[2] += counts[i][2] * repeat_count;
			lcounts[3] += counts[i][3] * repeat_count;
			i += repeat_count - 1;
		}
		printf ("Sum:  %u %u   %u %u\n", lcounts[0], lcounts[1],  lcounts[2], lcounts[3]);
		printf ("Runtime: %llu rdtsc clocks, %.1f clocks/iteration, %llu iterations\n", end_time - start_time, (float)(end_time - start_time)/ITS/ASIZE/inner_its, (unsigned long long)ITS*ASIZE*inner_its);
	}
	
	
cleanup:	
	printf ("Restoring old page table mappings...\n");
	// Restore page table exposed by pt_window	
	for (unsigned int i=0;i<ASIZE;i++) {
		p_pte[i] = old_pagetable[i];
	}

	// Restore 9 mappings used for pt_window (i.e., remove the pt_window)
	for (int i=0;i<num_pde_to_map;i++) {
		change_pte(cr3, (uintptr_t)pt_window+(i<<12), old_pgmap_ptes[i]);
	}	

	pgmod_flush_tlb();

	
	
	delete [] randindex;

	
	
		
	munlock (bigbuf, ASIZE*4096);
	munmap(bigbuf, ASIZE*4096);

	free (scratch_memory);
	free (old_pagetable);
	free (old_pgmap_ptes); 
	close(fd_devmem); fd_devmem = -1;
	
	
	return 0;
}
