void acme_init(char**);

double acme_math(double);

int acme_get_errno(void);

void acme_set_errno(int);

void *acme_worker_main(void*);

typedef void * worker;

void acme_start_worker(worker**, int);

void acme_join_worker(worker**);
