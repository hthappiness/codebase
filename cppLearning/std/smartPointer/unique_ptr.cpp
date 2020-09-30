

/* 
 *1.exclusive ownship, 独占权。关注其使用方法
 * 
 * */

#include <iostream>
#include <memory>

class ClassA
{
    public:
    ClassA () = default;
    ~ClassA()
    {
        std::cout << "call deconstuct" << std::endl;
    }
};

/* 类型的艺术 */
class CNewDeleter
{
    public:
		void operator () (ClassA* p) {
			std::cout << "call delete for ClassA object" << std::endl;
            std::cout << "call delete for ClassA object begin:" << std::endl;
			delete p;
             std::cout << "call delete for ClassA object end:" << std::endl;
        }

};

int main()
{
    std::unique_ptr<ClassA, CNewDeleter> test(new ClassA());
}