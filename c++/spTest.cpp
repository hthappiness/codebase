
#include <iostream>
#include <memory>

class CSpTest
{
public:
	CSpTest()
	{
		std::cout<<"construct"<<std::endl;
	}
	
	~CSpTest()
	{
		std::cout<<"deconstruct"<<std::endl;
	}
};

int main()
{
	auto spTest = std::make_shared<CSpTest>();
	
	spTest = std::make_shared<CSpTest>();
	
	spTest = std::make_shared<CSpTest>();
	spTest = std::make_shared<CSpTest>();
	spTest = std::make_shared<CSpTest>();
}