#pragma once

#include <pthread.h>

/** Thread local storage based on pthread library calls.
   #include tls.h to use this - and see that file for documentation
   #perthread variable */

//c++ thread_local 

struct base_pthread_thread_local {
  pthread_key_t key;
};

/** Use GPR_TLS_DECL to declare tls static variables outside a class */
#define TLS_DECL(name) static struct base_pthread_thread_local name = {0}

/** Use GPR_TLS_CLASS_DECL to declare tls static variable members of a class.
 *  GPR_TLS_CLASS_DEF needs to be called to define this member. */
#define TLS_CLASS_DECL(name) static struct base_pthread_thread_local name

/** Use TLS_CLASS_DEF to declare tls static variable members of a class.
 *  TLS_CLASS_DEF needs to be called to define this member. */
#define TLS_CLASS_DEF(name) struct base_pthread_thread_local name = {0}

#define base_tls_init(tls) GPR_ASSERT(0 == pthread_key_create(&(tls)->key, NULL))
#define base_tls_destroy(tls) pthread_key_delete((tls)->key)
#define base_tls_get(tls) ((intptr_t)pthread_getspecific((tls)->key))
#ifdef __cplusplus
extern "C" {
#endif
intptr_t tls_set(struct base_pthread_thread_local* tls, intptr_t value);
#ifdef __cplusplus
}
#endif