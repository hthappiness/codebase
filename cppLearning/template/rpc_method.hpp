

/// A wrapper class of an application provided rpc method handler.
template <class ServiceType, class RequestType, class ResponseType>
class RpcMethodHandler : public ::grpc::internal::MethodHandler {
 public:
  RpcMethodHandler(
      std::function<::grpc::Status(ServiceType*, ::grpc_impl::ServerContext*,
                                   const RequestType*, ResponseType*)>
          func,
      ServiceType* service)
      : func_(func), service_(service) {}

  void RunHandler(const HandlerParameter& param) final {
    ResponseType rsp;
    ::grpc::Status status = param.status;
    if (status.ok()) {
      status = CatchingFunctionHandler([this, &param, &rsp] {
        return func_(service_, param.server_context,
                     static_cast<RequestType*>(param.request), &rsp);
      });
      static_cast<RequestType*>(param.request)->~RequestType();
    }

    GPR_CODEGEN_ASSERT(!param.server_context->sent_initial_metadata_);
    ::grpc::internal::CallOpSet<::grpc::internal::CallOpSendInitialMetadata,
                                ::grpc::internal::CallOpSendMessage,
                                ::grpc::internal::CallOpServerSendStatus>
        ops;
    ops.SendInitialMetadata(&param.server_context->initial_metadata_,
                            param.server_context->initial_metadata_flags());
    if (param.server_context->compression_level_set()) {
      ops.set_compression_level(param.server_context->compression_level());
    }
    if (status.ok()) {
      status = ops.SendMessagePtr(&rsp);
    }
    ops.ServerSendStatus(&param.server_context->trailing_metadata_, status);
    param.call->PerformOps(&ops);
    param.call->cq()->Pluck(&ops);
  }

  void* Deserialize(grpc_call* call, grpc_byte_buffer* req,
                    ::grpc::Status* status, void** /*handler_data*/) final {
    ::grpc::ByteBuffer buf;
    buf.set_buffer(req);
    auto* request =
        new (::grpc::g_core_codegen_interface->grpc_call_arena_alloc(
            call, sizeof(RequestType))) RequestType();
    *status =
        ::grpc::SerializationTraits<RequestType>::Deserialize(&buf, request);
    buf.Release();
    if (status->ok()) {
      return request;
    }
    request->~RequestType();
    return nullptr;
  }

 private:
  /// Application provided rpc handler function.
  std::function<::grpc::Status(ServiceType*, ::grpc_impl::ServerContext*,
                               const RequestType*, ResponseType*)>
      func_;
  // The class the above handler function lives in.
  ServiceType* service_;
};
