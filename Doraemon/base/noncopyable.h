
/* To avoid drity namespace , we can control our namespace with a new namespace */
namespace Doraemon{
    namespace base{
        
        //非侵入式设计,通过继承，使用；当然也可以使用宏
        //类似还有模板的描述性质，使用模板测试
        class noncopyable
        {
        public:
        noncopyable(const noncopyable&) = delete;
        void operator=(const noncopyable&) = delete;

        protected:
        noncopyable() = default;
        ~noncopyable() = default;
        };
#if 0
        #define NON_COPYABLE(classname) \ 
               public: \
                classname(const classname&) = delete; \
                void operator=(const classname&) = delete;
#endif
    }

    using noncopyable = base::noncopyable;
}
