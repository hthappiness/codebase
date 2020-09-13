
#include <iostream>
#include <cstdlib>
#include <array>

static constexpr auto ROOM_NUM = 10;
std::array<unsigned int, ROOM_NUM> arrRoom = { 4, 2, 4, 6, 9, 1, 3, 2, 4, 9};

std::array<unsigned int, ROOM_NUM> arrTotal;

int getN(unsigned int n)
{
    if( arrTotal[n - 1] != 0)
    {
        return arrTotal[n - 1];
    }
    if( n == 1 )
    {
        return arrRoom[0];
    }

    if( n == 2 )
    {
        return  arrRoom[0] > arrRoom[1] ? arrRoom[0] : arrRoom[1];
    }

    unsigned int a = arrRoom[n - 1] + getN(n-2);
    unsigned int b = getN(n - 1);

    return  a > b ? a : b;
}

int main()
{
    arrTotal.fill(0);
    arrTotal[ 0 ] = arrRoom[0];
    arrTotal[ 1 ] = arrRoom[0] > arrRoom[1] ? arrRoom[0] : arrRoom[1];
    for(int i = 0 ; i < ROOM_NUM; i++)
    {
        arrTotal[i] = getN(i+1);
        std::cout << "index " << i << " :" << arrTotal[i] << std::endl;
    }
    
    std::system("pause");

    return 0;
}