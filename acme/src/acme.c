#include <acme.h>

#include <stdint.h>
#include <pthread.h>
#include <math.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// fwd decl from musl
void __init_libc(char **envp, char *pn);

void acme_init(char** envp) { __init_libc(envp, "libacme"); }

double acme_math(double x) { return cos(x) + sin(x); }

int acme_get_errno(void) { return errno; }

void acme_set_errno(int e) { errno = e; }

void *acme_worker_main(void* d)
{
    int t = (int64_t)d;
    sleep(t);
    printf("worker %d\n", t);
}

void acme_start_worker(worker** p, int t)
{
    pthread_t** pt = (pthread_t**)p;
    printf("pt: %p\n", p);
    printf("*pt: %p\n", *pt);
    pthread_t* thr = malloc(sizeof(pthread_t));
    printf("thr: %p\n", thr);
    *pt = thr;
    printf("*pt: %p\n", *pt);
    pthread_create(*pt, NULL, *acme_worker_main, (void*)((int64_t)t));
}

void acme_join_worker(worker** p)
{
    pthread_t** pt = (pthread_t**)p;
    pthread_join(**pt, NULL);
    free(*pt);
}
