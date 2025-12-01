#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "sync_lib.h"

#define LMITE_SUPERIO 3000000
#define NUM_THREADS 4

long cont_primo_serial = 0;
long cont_primo_paralelo = 0;
sync_mutex_t mutex;