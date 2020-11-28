

class CExector
{
public:
    size_t RunClosures(const char* executor_name,
                             grpc_closure_list list) {
  size_t n = 0;

  // In the executor, the ExecCtx for the thread is declared in the executor
  // thread itself, but this is the point where we could start seeing
  // application-level callbacks. No need to create a new ExecCtx, though,
  // since there already is one and it is flushed (but not destructed) in this
  // function itself. The ApplicationCallbackExecCtx will have its callbacks
  // invoked on its destruction, which will be after completing any closures in
  // the executor's closure list (which were explicitly scheduled onto the
  // executor).
  grpc_core::ApplicationCallbackExecCtx callback_exec_ctx(
      GRPC_APP_CALLBACK_EXEC_CTX_FLAG_IS_INTERNAL_THREAD);

  grpc_closure* c = list.head;
  while (c != nullptr) {
    grpc_closure* next = c->next_data.next;
    grpc_error* error = c->error_data.error;
#ifndef NDEBUG
    EXECUTOR_TRACE("(%s) run %p [created by %s:%d]", executor_name, c,
                   c->file_created, c->line_created);
    c->scheduled = false;
#else
    EXECUTOR_TRACE("(%s) run %p", executor_name, c);
#endif
    c->cb(c->cb_arg, error);
    GRPC_ERROR_UNREF(error);
    c = next;
    n++;
    grpc_core::ExecCtx::Get()->Flush();
  }

  return n;
}
};