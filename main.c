#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <pthread.h>
#include <string.h>
//#include <inttypes.h>
#include <assert.h>

struct arg_struct {
    int a, b;
};

void calculate(struct arg_struct *args)
{
    char filename[256];
    strcpy(filename, begin);
    strcat(filename, "-");
    strcat(filename, end);
    strcat(filename, ".csv");
    printf("%s", filename);
    mpz_t n;
    mpz_t c;
    mpz_t m;
    mpz_t i;
    mpz_t zero;
    mpz_t one;
    mpz_t two;
    mpz_t three;
    mpz_t rem;

    mpz_init_set_str(zero, "0", 10);
    mpz_init_set_str(one, "1", 10);
    mpz_init_set_str(two, "2", 10);
    mpz_init_set_str(three, "3", 10);
    mpz_init_set_str(m, end, 10);
    mpz_init(c);
    mpz_init(rem);
    mpz_init(n);
    mpz_init_set_str(i, begin, 10);

    FILE * pFile;
    pFile = fopen(filename,"w");
    fputs("", pFile);
    fclose(pFile);

    for(; mpz_cmp(i, m) != 0; mpz_add(i, i, one))
    {
        mpz_set(n, i);
        mpz_set(c, zero);
        for(; mpz_cmp(n, one) != 0; mpz_add(c, c, one))
        {
            mpz_mod(rem, n, two);
            if(mpz_cmp(rem, zero) == 0)
            {
                mpz_divexact(n, n, two);
            }
            else
            {
                mpz_mul(n, n, three);
                mpz_add(n, n, one);
            }
        }
        pFile = fopen(filename,"a");
        if(pFile == NULL)
        {
            perror("Error opening file.");
            return;
        }
        mpz_out_str(pFile, 10, i);
        fputs(",", pFile);
        mpz_out_str(pFile, 10, c);
        fputs("\n", pFile);
        fclose(pFile);
    }
}

int main()
{
    printf("Sequence of operations: \nx mod 2 == 0 ? x /= 2 : x *= 3 + 1\n");
    printf("From 1 to _\b");
    int max = 0;
    scanf("%d", &max);
    printf("Number of threads: ");
    int threadNum = 1;
    scanf("%d", &threadNum);

    pthread_t threads[threadNum];

    char arguments[threadNum][2][125];
    memset(arguments, '\0', 125*2*threadNum);
    int rc, i;

    struct arg_struct *args = arguments;

    for(i = 0; i < threadNum; ++i)
    {
        int a, b;
        a = i*(max/threadNum)+1;
        b = (i+1)*(max/threadNum);
        printf("Creating thread %d for %d to %d\n", i, a, b);
        rc = pthread_create(&threads[i], NULL, calculate, (void*)arguments);
        assert(rc == 0);
    }

    return 0;
}
