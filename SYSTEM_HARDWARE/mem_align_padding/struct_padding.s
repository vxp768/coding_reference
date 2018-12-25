	.file	"struct_union_padding.c"
	.globl	b_test
	.data
	.align 8
	.type	b_test, @object
	.size	b_test, 12
b_test:
	.byte	12
	.zero	3
	.long	24
	.value	14
	.zero	2
	.globl	b_reorder_test
	.align 8
	.type	b_reorder_test, @object
	.size	b_reorder_test, 8
b_reorder_test:
	.byte	12
	.zero	1
	.value	24
	.long	14
	.globl	b_packed_test
	.type	b_packed_test, @object
	.size	b_packed_test, 7
b_packed_test:
	.byte	12
	.long	24
	.value	14
	.globl	c_test
	.align 8
	.type	c_test, @object
	.size	c_test, 12
c_test:
	.byte	12
	.zero	3
	.long	24
	.value	14
	.value	25
	.globl	c_packed_8
	.align 16
	.type	c_packed_8, @object
	.size	c_packed_8, 16
c_packed_8:
	.byte	12
	.long	24
	.value	14
	.value	25
	.zero	7
	.section	.rodata
	.align 8
.LC0:
	.string	"Size of struct b with padding :%lu\n"
	.align 8
.LC1:
	.string	"Size of struct b_reorder with padding :%lu\n"
.LC2:
	.string	"Size of struct b_packed :%lu\n"
.LC3:
	.string	"Size of struct c: %lu\n"
	.align 8
.LC4:
	.string	"Size of struct c_packed_8 :%lu\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movl	$12, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movl	$8, %esi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	movl	$7, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	movl	$12, %esi
	movl	$.LC3, %edi
	movl	$0, %eax
	call	printf
	movl	$16, %esi
	movl	$.LC4, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.2.1-22ubuntu2) 5.2.1 20151010"
	.section	.note.GNU-stack,"",@progbits
