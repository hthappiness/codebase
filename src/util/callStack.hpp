

template <typename Key, typename Value = unsigned char>
class Callstack {
public:
    class Iterator;
 
    class Context {
    public:
        Context(const Context&) = delete;
        Context& operator=(const Context&) = delete;
        explicit Context(Key* k)
            : m_key(k), m_next(Callstack<Key, Value>::ms_top) {
            m_val = reinterpret_cast<unsigned char*>(this);
            Callstack<Key, Value>::ms_top = this;
        }
 
        Context(Key* k, Value& v)
            : m_key(k), m_val(&v), m_next(Callstack<Key, Value>::ms_top) {
            Callstack<Key, Value>::ms_top = this;
        }
 
        ~Context() {
            Callstack<Key, Value>::ms_top = m_next;
        }
 
        Key* getKey() {
            return m_key;
        }
 
        Value* getValue() {
            return m_val;
        }
 
    private:
        friend class Callstack<Key, Value>;
        friend class Callstack<Key, Value>::Iterator;
        Key* m_key;
        Value* m_val;
        Context* m_next;
    };
 
    class Iterator {
    public:
        Iterator(Context* ctx) : m_ctx(ctx) {}
        Iterator& operator++() {
            if (m_ctx)
                m_ctx = m_ctx->m_next;
            return *this;
        }
 
        bool operator!=(const Iterator& other) {
            return m_ctx != other.m_ctx;
        }
 
        Context* operator*() {
            return m_ctx;
        }
 
    private:
        Context* m_ctx;
    };
 
    // Determine if the specified owner is on the stack
    // \return
    //  The address of the value if present, nullptr if not present
    static Value* contains(const Key* k) {
        Context* elem = ms_top;
        while (elem) {
            if (elem->m_key == k)
                return elem->m_val;
            elem = elem->m_next;
        }
        return nullptr;
    }
 
    static Iterator begin() {
        return Iterator(ms_top);
    }
 
    static Iterator end() {
        return Iterator(nullptr);
    }
 
private:
    static thread_local Context* ms_top;
};
 
template <typename Key, typename Value>
thread_local typename Callstack<Key, Value>::Context*
    Callstack<Key, Value>::ms_top = nullptr;
	
#if 0
struct Foo {
    template <typename F>
    void run(F f) {
        // Place marker in the callstack, so any called code knows we are
        // executing "run" on this Foo instance
        Callstack<Foo>::Context ctx(this);
        f();
    }
};
 
Foo globalFoo;
 
void func1() {
    printf("%s\n", Callstack<Foo>::contains(&globalFoo) ? "true" : "false");
}
 
int main() {
    func1();               // Will print "false"
    globalFoo.run(func1);  // Will print "true"
    return 0;
}
#endif

// 如下是boost的实现
// 1、每次在栈上声明一个callStack::context，然后其next赋值为上一个context的位置，这样栈上的context就形成了一个链表，便于查找
// 2、每一个callStack根据其key/value的差异，模板类实例化不同，每个类对应一个static变量
// 3、context的这种创建和删除行为，非常适合链表这种数据结构；只是查询的复杂度偏高
// 4、call_stack<Key, Value>::top_ 作为一个全局的访问对象，用于context的生成和销毁

//
// detail/call_stack.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_DETAIL_CALL_STACK_HPP
#define BOOST_ASIO_DETAIL_CALL_STACK_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/config.hpp>
#include <boost/asio/detail/noncopyable.hpp>
#include <boost/asio/detail/tss_ptr.hpp>

#include <boost/asio/detail/push_options.hpp>

namespace boost {
namespace asio {
namespace detail {

// Helper class to determine whether or not the current thread is inside an
// invocation of io_context::run() for a specified io_context object.
template <typename Key, typename Value = unsigned char>
class call_stack
{
public:
  // Context class automatically pushes the key/value pair on to the stack.
  class context
    : private noncopyable
  {
  public:
    // Push the key on to the stack.
    explicit context(Key* k)
      : key_(k),
        next_(call_stack<Key, Value>::top_)
    {
      value_ = reinterpret_cast<unsigned char*>(this);
      call_stack<Key, Value>::top_ = this;
    }

    // Push the key/value pair on to the stack.
    context(Key* k, Value& v)
      : key_(k),
        value_(&v),
        next_(call_stack<Key, Value>::top_)
    {
      call_stack<Key, Value>::top_ = this;
    }

    // Pop the key/value pair from the stack.
    ~context()
    {
      call_stack<Key, Value>::top_ = next_;
    }

    // Find the next context with the same key.
    Value* next_by_key() const
    {
      context* elem = next_;
      while (elem)
      {
        if (elem->key_ == key_)
          return elem->value_;
        elem = elem->next_;
      }
      return 0;
    }

  private:
    friend class call_stack<Key, Value>;

    // The key associated with the context.
    Key* key_;

    // The value associated with the context.
    Value* value_;

    // The next element in the stack.
    context* next_;
  };

  friend class context;

  // Determine whether the specified owner is on the stack. Returns address of
  // key if present, 0 otherwise.
  static Value* contains(Key* k)
  {
    context* elem = top_;
    while (elem)
    {
      if (elem->key_ == k)
        return elem->value_;
      elem = elem->next_;
    }
    return 0;
  }

  // Obtain the value at the top of the stack.
  static Value* top()
  {
    context* elem = top_;
    return elem ? elem->value_ : 0;
  }

private:
  // The top of the stack of calls for the current thread.
  static tss_ptr<context> top_;
};

template <typename Key, typename Value>
tss_ptr<typename call_stack<Key, Value>::context>
call_stack<Key, Value>::top_;

} // namespace detail
} // namespace asio
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_DETAIL_CALL_STACK_HPP
