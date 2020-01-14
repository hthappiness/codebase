// enable_shared_from_this的实现
// 基于(/usr/include/c++/7.3.0/bits/shared_ptr.h)
// 此代码是对gcc实现的简化版本, 仅作为描述原理用.    
// https://www.cnblogs.com/yang-wen/p/8573269.html

template<typename T>
class enable_shared_from_this
{
public:
    shared_ptr<T> shared_from_this()
    {
        return shared_ptr<T>(this->weak_this);
    }
    shared_ptr<const T> shared_from_this() const
    {
        return shared_ptr<const T>(this->weak_this);
    }
private:
    template<typename>
    friend class shared_ptr;

    template<typename T1>
    void _M_weak_assign(T1* p, const shared_count<>& n)
    {
    　　weak_this._M_assign(p, n);
    }

    mutable weak_ptr<T> weak_this;
};