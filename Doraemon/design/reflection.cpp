#if 0
#include <iostream>
#include <map>
#include <memory>

#define FUNC function<void*(void)>

using namespace std;

class Reflector
{
private:
    map<std::string, FUNC>objectMap;
    static shared_ptr<Reflector> ptr;

public:
    void* CreateObject(const string &str)
    {
        for (auto & x : objectMap)
        {
            if(x.first == str)
                return x.second();
        }
        return nullptr;
    }

    void Register(const string &class_name, FUNC && generator)
    {
        objectMap[class_name] = generator;
    }

    static shared_ptr<Reflector> Instance()
    {
        if(ptr == nullptr)
        {
            ptr.reset(new Reflector());
        }

        return ptr;
    }

};

shared_ptr<Reflector> Reflector::ptr = nullptr;

class RegisterAction
{
public:
    RegisterAction(const string &class_name, FUNC && generator)
    {
        Reflector::Instance()->Register(class_name, forward<FUNC>(generator));
    }
};

#define REGISTER(CLASS_NAME) \
RegisterAction g_register_action_##CLASS_NAME(#CLASS_NAME, []()\
{\
    return new CLASS_NAME(); \
});


class Base
{
public:
    explicit Base() = default;
    virtual void Print()
    {
        cout << "Base" << endl;
    }
};
REGISTER(Base);

class DeriveA : public Base
{
public:
    void Print() override
    {
        cout << "DeriveA" << endl;
    }
};
REGISTER(DeriveA);

class DeriveB : public Base
{
public:
    void Print() override
    {
        cout << "DeriveB" << endl;
    }
};
REGISTER(DeriveB);

int main()
{
    shared_ptr<Base> p1((Base*)Reflector::Instance()->CreateObject("Base"));
    p1->Print();

    shared_ptr<Base> p2((Base*)Reflector::Instance()->CreateObject("DeriveA"));
    p2->Print();

    shared_ptr<Base> p3((Base*)Reflector::Instance()->CreateObject("DeriveB"));
    p3->Print();
}
#endif

#include <iostream>
#include <string>
#include <map>

using namespace std;

#define BASE_CLASS Test
#define GLOBAL_FUN_MAP FunMap<BASE_CLASS>::get_fun_map()
#define DEFINE_CLASS(class_name, fun_name) \
    class_name(std::string) \
    {\
        GLOBAL_FUN_MAP.registerFun(#fun_name, class_name::fun_name);\
    }\
    class_name(){}\
    static class_name class_name##_;\
    static BASE_CLASS* fun_name()\
    {\
        return new class_name;\
    }
#define REGIST_CLASS(class_name) \
    class_name class_name::class_name##_(#class_name);

template <class T>
class FunMap
{
    typedef T* (*FUN)(void);
    map<std::string, FUN> fun_map_;
    public:
    // register是关键字， register修饰符暗示编译程序相应的变量将被频繁地使用
    void registerFun(string fun_name, FUN fun)
    {
        fun_map_[fun_name] = fun;  
    }

    T* get(const string fun_name)
    {
        if (fun_map_.end() != fun_map_.find(fun_name))
        {
            return fun_map_[fun_name](); 
        }
        else
        {
            return NULL;
        }
    }

    static FunMap<T>& get_fun_map()
    {
        static FunMap<T> fun_map;
        return fun_map;
    }
};

class Test
{};

class Test1 : public Test
{
    public:
    DEFINE_CLASS(Test1, test1)
};

class Test2 : public Test
{
    public:
    DEFINE_CLASS(Test2, test2)
};

REGIST_CLASS(Test1)
REGIST_CLASS(Test2)

int main()
{
    if( nullptr == GLOBAL_FUN_MAP.get("test1") )
    {
        std::cout<< "not register param" << std::endl;
    }
    GLOBAL_FUN_MAP.get("test2");   
    GLOBAL_FUN_MAP.get("11111111");

    if( nullptr == GLOBAL_FUN_MAP.get("lixi") )
    {
        std::cout<< "not register lixi" << std::endl;
    }
}