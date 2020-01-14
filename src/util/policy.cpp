#include <iostream>
#include <stdlib.h>
#include <stdio.h>

namespace SHFTest
{
    template <class T>
    struct OpNewCreator
    {
        static T* Create()
        {
            return new T;
        }
    protected:
        ~OpNewCreator(){};
    };

    template <class T>
    struct MallocCreator
    {
        static T* Create()
        {
            T* buf = (T*)std::malloc(sizeof(T));
            if ( !buf )
            {
                return 0;
            }
            return buf;
        }
    protected:
        ~MallocCreator(){};
    };

    template <class T>
    struct ProtypeCreator 
    {
    public:
        T* Create()
        {
            return /*pPrototype_ ? pPrototype_->Clone() :*/ 0;
        }

        T* GetPrototype() { return pPrototype_; }

        void SetPrototype(T* pObj) { pPrototype_ = pObj; }
    protected:
        ~ProtypeCreator(){};
    private:
        T* pPrototype_;
    };

    //默认值
    //嵌套模板 template< template class >
    //
    template<
            class T,
            template <class> class CreatePolicy = OpNewCreator
            >
    class CommonObj : public CreatePolicy<T>
    {
    public:
        CommonObj()
        {
            CreatePolicy<T>::Create();
        }

        ~CommonObj(){}

        void Test()
        {
            std::cout<<"Hello World";
        }
    protected:
    };
}





int _tmain(int argc, _TCHAR* argv[])
{
    typedef SHFTest::CommonObj<int,SHFTest::ProtypeCreator> myProtypeObj;
    myProtypeObj Testobj1;
    Testobj1.Test();

    typedef SHFTest::CommonObj<int> myObj; //两个typedef可以看做，定义一个新的类A/B,而ab的共性如模板变成代码所呈现的那样，这就起到了精简代码的作用
    myObj Testobj2;
    Testobj2.Test();

    return 0;
}