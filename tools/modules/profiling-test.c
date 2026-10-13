#include <stdio.h>
int fibonacci(int n)
{
    if (n == 0)
        return 0;
    else if (n == 1)
        return 1;
    else
        return fibonacci(n - 1) + fibonacci(n - 2);
}

int loop100M(void)
{
    int val = 0;
    for (int i = 0; i < 100000000; i++) {
        if (i % 10 == 0)
            val++;
        else if (i % 3)
            val--;
    }
    return val;
}

int main(void)
{
    printf("Fibonacci value = %u\n", fibonacci(40));
    printf("Loop value = %u\n", loop100M());
    return 0;
}
