#include<iostream> 
#include<cstring>
#include<cstdio>
#include<string>
#include<cmath>
#include<algorithm>
#include<stack>
#include<climits>
#include<queue>
#define eps 1e-7
#define ll long long
#define inf 0x3f3f3f3f
#define pi 3.141592653589793238462643383279
using namespace std;
struct node{
    double x,y;
}; 

double cmp(node a,node b) //给线段的坐标排序 
{
    if(a.x != b.x)
        return a.x < b.x;
    else
        return a.y < b.y;
}

double compute(double x1,double y1,double x2,double y2) //计算叉乘的结果 
{
    return x1*y2 - y1*x2;
}

int compare(node a,node b) //比较坐标的大小  
{
    if(a.x < b.x || a.x == b.x && a.y < b.y)
        return -1;
    else if(a.x == b.x && a.y == b.y)
        return 0;
    else return 1;
}

int main()
{
    int t;
    node po[4];
    cin>>t;
    while(t--)
    {
        for(int i=0; i<4; ++i)
            scanf("%lf%lf",&po[i].x,&po[i].y);
        
        sort(po,po+2,cmp); //给第一条线段的坐标排序 
        sort(po+2,po+4,cmp); //给第二条排序 
        /*for(int i=0; i<4; ++i)
            cout<<po[i].x<<' '<<po[i].y<<endl;*/
        
        int flag;
        if(!compare(po[0],po[2]) || !compare(po[0],po[3]) || !compare(po[1],po[2]) || !compare(po[1],po[3])) //若有某一点重合，则肯定相交 
            flag = 1;
        
        else if(compute(po[0].x-po[1].x , po[0].y-po[1].y , po[2].x-po[3].x , po[2].y-po[3].y) ==0 ) //若两线段平行 
        {
            if(compute(po[0].x-po[1].x , po[0].y-po[1].y , po[0].x-po[3].x , po[0].y-po[3].y) == 0) //若两线段共线 
            {
                if(compare(po[0],po[2]) <= 0 && compare(po[1],po[2]) >= 0) //第一条起点小于第二条起点，第一条终点大于第二条起点 
                    flag = 1;
                else if(compare(po[2],po[0]) >= 0 && compare(po[3],po[0]) <= 0) //第二条起点小于第一条起点，第二条终点大于第一条起点 
                    flag = 1;
                else flag = 0;
            }
            else flag = 0;
            cout <<"same line\n";
        }
        
        else if(compute(po[0].x-po[1].x , po[0].y-po[1].y , po[2].x-po[3].x , po[2].y-po[3].y) !=0 ) //若不平行 
        {
            double num1,num2,num3,num4;
            num1 = compute(po[0].x-po[1].x , po[0].y-po[1].y , po[0].x-po[2].x , po[0].y-po[2].y); //计算第一条的两个端点 
            num2 = compute(po[0].x-po[1].x , po[0].y-po[1].y , po[0].x-po[3].x , po[0].y-po[3].y); //在第二条线段的两边 
            num3 = compute(po[0].x-po[2].x , po[0].y-po[2].y , po[2].x-po[3].x , po[2].y-po[3].y); //计算第二条的两个端点 
            num4 = compute(po[1].x-po[2].x , po[1].y-po[2].y , po[2].x-po[3].x , po[2].y-po[3].y); //在第一条线段的两边 
            cout<<num1<<' '<<num2<<' '<<num3<<' '<<num4<<endl;
            if(num1*num2 < 0 && num3*num4 <= 0 || num1*num2 <= 0 && num3*num4 < 0) //等于0表示成180度角
                flag = 1;
            else
                flag = 0;
        }
        
        else flag = 0;
        if(flag) cout<<"YES\n";
        else cout<<"NO\n";
        
    }
}
