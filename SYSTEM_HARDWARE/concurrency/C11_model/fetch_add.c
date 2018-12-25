#include<threads.h>
#include<stdatomic.h>

atomic_int a1 = ATOMIC_VAR_INIT(0);

void func_adder() 
{
    int x = atomic_fetch_add(&a1, 1);
    printf("Thread %d saw x = %d\n",(int)thrd_current(), x);
}


int main(void)
{
thrd_t tid;
thrd_create(&tid, func_adder, NULL);

return 0;
}
