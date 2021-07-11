
#include <iostream>

using namespace std;
class Solution {
public:
    string longestPalindrome(string s) 
    {
        if (s.length() == 0) 
        {
            return "";
        }
//         保存起始位置，测试了用数组似乎能比全局变量稍快一点
        int result[2] = {0};
        for (int i = 0; i < s.length(); i++) 
        {
//             把回文看成中间的部分全是同一字符，左右部分相对称
//             找到下一个与当前字符不同的字符
            i = findLongest(s, i, result);
        }
        return s.substr(result[0], result[1]+1);  
    }

    int findLongest(string str, int low, int range[]) 
    {
//         查找中间部分
        int high = low;
        while (high < str.length() - 1 && str[high + 1] == str[low]) {
            high++;
        }
//         定位中间部分的最后一个字符
        int ans = high;
//         从中间向左右扩散
        while (low > 0 && high < str.length() - 1 && str[low - 1] == str[high + 1]) {
            low--;
            high++;
        }
//         记录最大长度
        if (high - low > range[1] - range[0]) {
            range[0] = low;
            range[1] = high;
        }
        return ans;
    }

};

#include <vector>
class NSolution
{
public:
    std::string convert(std::string s, int nRows)
    {
        std::vector<std::string> resultVec;
        
        std::string res;
        if(s.empty() || nRows < 1) return res;
        if(nRows == 1) return s;

        int iRow = 0, iColumn = 0;
        int iDir = 0; // 表示向下，还是斜着往上

        for(int i = 0; i < s.length() ; i++)
        {
            resultVec[iRow].push_back(s[i]);
            if ( iDir == 0) //向下
            {
                if( iRow + 1 == nRows )
                {
                    iRow --;
                    iDir = 1;
                }
                else
                {
                    iRow++;
                }
            }
            else //向上
            {
                if( iRow == 0 )
                {
                    iRow ++;
                    iDir = 0;
                }
                else
                {
                    iRow -- ;
                }
            }
        }

        for(int i = 0; i < resultVec.size(); i++){
               res += s[i];
        }

        return res;
    }
};


int main()
{
    std::string str = "cbbd";
    Solution s;
    std::cout << "result : " << s.longestPalindrome(str) << std::endl; 
}