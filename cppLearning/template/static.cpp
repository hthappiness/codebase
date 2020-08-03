
#include <iostream>

using namespace std;

template< typename _class >

class Base
{

    static int itest;

public:

    void Set()
    {

        itest++;

    }

    void Show()
    {

        cout << itest << endl;

    }

};

//即使没实例化

template < typename _class >
int Base< _class >::itest = 100;  //没有这句，编译就过不了^_^

class Derive: public Base<Derive>
{

};

class Derive2: public Base<Derive2>
{

};


int main(int argc, char* argv[])
{
    Derive d1;
    Derive2 d2;

    d1.Set();
    d2.Set();

    d1.Show();
    d2.Show();

    return 0;
}
