

//封装函数对象，并将其运行必要的信息包含起来

struct dm_closure;
typedef struct dm_closure dm_closure;

typedef struct dm_closure_list {
  dm_closure* head;
  dm_closure* tail;
} dm_closure_list;

/** gRPC Callback definition.
 *
 * \param arg Arbitrary input.
 * \param error GRPC_ERROR_NONE if no error occurred, otherwise some grpc_error
 *              describing what went wrong.
 *              Error contract: it is not the cb's job to unref this error;
 *              the closure scheduler will do that after the cb returns */
typedef void (*mgr_cb_func)(void* arg, dm_error* error);

/** A closure over a grpc_iomgr_cb_func. */
struct dm_closure {
  /** Once queued, next indicates the next queued closure; before then, scratch
   *  space */
  union {
    dm_closure* next;
    //grpc_core::ManualConstructor<grpc_core::MultiProducerSingleConsumerQueue::Node>mpscq_node;
    uintptr_t scratch;
  } next_data;

  /** Bound callback. */
  mgr_cb_func cb;

  /** Arguments to be passed to "cb". */
  void* cb_arg;

  /** Once queued, the result of the closure. Before then: scratch space */
  union {
    dm_error* error;
    uintptr_t scratch;
  } error_data;

// extra tracing and debugging for grpc_closure. This incurs a decent amount of
// overhead per closure, so it must be enabled at compile time.
#ifndef NDEBUG
  bool scheduled;
  bool run;  // true = run, false = scheduled
  const char* file_created;
  int line_created;
  const char* file_initiated;
  int line_initiated;
#endif
};