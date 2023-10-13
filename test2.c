#include <stdio.h>

int main() {
    int n = 10;
    long long fib1 = 0, fib2 = 1, nextTerm;

    printf("Fibonacci Series for n = 10:\n");

    for (int i = 0; i < n; ++i) {
        if (i <= 1) {
            nextTerm = i;
        } else {
            nextTerm = fib1 + fib2;
            fib1 = fib2;
            fib2 = nextTerm;
        }
        printf("%d: %lld\n", i + 1, nextTerm);
    }

    return 0;
}
