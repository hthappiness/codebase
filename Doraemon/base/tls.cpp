#include "tls.h"

#include <assert.h>

intptr_t tls_set(struct base_pthread_thread_local* tls, intptr_t value) {

  assert( 0 == pthread_setspecific(tls->key, (void*)value));
  return value;
}
