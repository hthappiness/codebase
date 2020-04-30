
#include <iostream>
#include <memory>

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

void testArray(std::array<std::shared_ptr<CSpTest>, 8>& arrPointer)
{
	auto spTest = std::make_shared<CSpTest>();

	arrPointer[0] = spTest; 

	std::cout<<"clear .";

	arrPointer[0] = nullptr;
}

int main()
{
	auto spTest = std::make_shared<CSpTest>();

	std::array<std::shared_ptr<CSpTest>, 8> arr;
	testArray(arr);

	std::cout << "out of spTest" << std::endl;
	
	spTest = std::make_shared<CSpTest>();
	
	spTest = std::make_shared<CSpTest>();
	spTest = std::make_shared<CSpTest>();
	spTest = std::make_shared<CSpTest>();
}