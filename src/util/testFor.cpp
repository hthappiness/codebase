#include <iostream>

class CTest
{
public:
	CTest(){
		iCnt = 10;
	}
	~CTest(){
		std::cout<<"delete call"<<std::endl;
	}
	void testOut(){
		std::cout<<"loop out"<<std::endl;
	}
	int getNext(){return iCnt;}
	int addNext(){std::cout<<"plus plus"<<std::endl;iCnt --;}
private:
	int iCnt;
};

int main()
{
	for(CTest tmp;tmp.getNext();tmp.testOut(),tmp.addNext())
	{	
		if(tmp.getNext() < 5)
		{
			std::cout<<"break"<<std::endl;
			return 0;
		}
	}
	return 0;
}
