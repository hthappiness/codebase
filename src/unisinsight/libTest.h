
typedef int (*TEST)(int , int );

int regTestFunc(TEST pfnReg);

class CallBack{
public:
	CallBack();
	~CallBack(){}
};