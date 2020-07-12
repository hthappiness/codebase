
//MPSC
class MultiProducerSingleConsumerQueue {
 public:
  // List node.  Application node types can inherit from this.
  struct Node {
    Atomic<Node*> next;
  };

  MultiProducerSingleConsumerQueue() : head_{&stub_}, tail_(&stub_) {}
  ~MultiProducerSingleConsumerQueue() {
    GPR_ASSERT(head_.Load(MemoryOrder::RELAXED) == &stub_);
    GPR_ASSERT(tail_ == &stub_);
  }

  // Push a node
  // Thread safe - can be called from multiple threads concurrently
  // Returns true if this was possibly the first node (may return true
  // sporadically, will not return false sporadically)
  bool Push(Node* node);
  // Pop a node (returns NULL if no node is ready - which doesn't indicate that
  // the queue is empty!!)
  // Thread compatible - can only be called from one thread at a time
  Node* Pop();
  // Pop a node; sets *empty to true if the queue is empty, or false if it is
  // not.
  Node* PopAndCheckEnd(bool* empty);

 private:
  // make sure head & tail don't share a cacheline
  union {
    char padding_[GPR_CACHELINE_SIZE];
    Atomic<Node*> head_;
  };
  Node* tail_;
  Node stub_;
};

// An mpscq with a lock: it's safe to pop from multiple threads, but doing
// only one thread will succeed concurrently.
class LockedMultiProducerSingleConsumerQueue {
 public:
  typedef MultiProducerSingleConsumerQueue::Node Node;

  // Push a node
  // Thread safe - can be called from multiple threads concurrently
  // Returns true if this was possibly the first node (may return true
  // sporadically, will not return false sporadically)
  bool Push(Node* node);

  // Pop a node (returns NULL if no node is ready - which doesn't indicate that
  // the queue is empty!!)
  // Thread safe - can be called from multiple threads concurrently
  Node* TryPop();

  // Pop a node.  Returns NULL only if the queue was empty at some point after
  // calling this function
  Node* Pop();

 private:
  MultiProducerSingleConsumerQueue queue_;
  Mutex mu_;
};