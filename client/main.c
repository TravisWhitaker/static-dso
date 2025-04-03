#include <errno.h>
#include <stdio.h>
#include <acme.h>


int main (int argc, char** argv, char** envp)
{
    printf("Entered main()\n");

    acme_init(envp);
    
    printf("Set main's errno = 0\n");

    errno = 0;

    printf("acme_get_errno: %d\n", acme_get_errno());

    printf("Set main's errno = 5\n");

    errno = 5;

    printf("acme_get_errno: %d\n", acme_get_errno());

    printf("acme_set_errno(10)\n");

    acme_set_errno(10);

    printf("main errno: %d, acme errno: %d\n", errno, acme_get_errno());

    worker* j1 = (void*)1;
    worker* j2;
    worker* j3;

    printf("j1 %p\n", j1);
    
    acme_start_worker(&j1, 1);
    acme_start_worker(&j2, 2);
    acme_start_worker(&j3, 3);

    printf("j1 %p\n", j1);

    // pthread_join(j1, NULL);
    // pthread_join(j2, NULL);
    // pthread_join(j3, NULL);


    acme_join_worker(&j1);
    acme_join_worker(&j2);
    acme_join_worker(&j3);

    return 0;
}
