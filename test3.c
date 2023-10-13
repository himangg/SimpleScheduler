#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define N 1000000000

int main() {
    bool *isPrime = (bool *)malloc((N + 1) * sizeof(bool));
    
    if (isPrime == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i <= N; i++) {
        isPrime[i] = true;
    }

    isPrime[0] = isPrime[1] = false;

    for (int p = 2; p <= sqrt(N); p++) {
        if (isPrime[p]) {
            for (int i = p * p; i <= N; i += p) {
                isPrime[i] = false;
            }
        }
    }

    int count = 0;
    for (int i = 2; i <= N; i++) {
        if (isPrime[i]) {
            count++;
        }
    }

    printf("Number of prime numbers up to 1e9: %d\n", count);

    free(isPrime);

    return 0;
}

