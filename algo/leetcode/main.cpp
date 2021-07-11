
#include <iostream>
#include <stdio.h>

using namespace std;
class Solution {
public:
	string longestPalindrome(string s)
	{
		if (s.length() == 0)
		{
			return "";
		}
		//         ������ʼλ�ã��������������ƺ��ܱ�ȫ�ֱ����Կ�һ��
		int result[2] = { 0 };
		for (int i = 0; i < s.length(); i++)
		{
			//             �ѻ��Ŀ����м�Ĳ���ȫ��ͬһ�ַ������Ҳ�����Գ�
			//             �ҵ���һ���뵱ǰ�ַ���ͬ���ַ�
			i = findLongest(s, i, result);
                        std::cout<< "result:" << result[0] << " " << result[1] << std::endl;
		}
		return s.substr(result[0], result[1] - result[0] + 1);
	}

	int findLongest(string str, int low, int range[])
	{
		//         �����м䲿��
		int high = low;
		while (high < str.length() - 1 && str[high + 1] == str[low]) {
			high++;
		}
		//         ��λ�м䲿�ֵ����һ���ַ�
		int ans = high;
		//         ���м���������ɢ
		while (low > 0 && high < str.length() - 1 && str[low - 1] == str[high + 1]) {
			low--;
			high++;
		}
		//         ��¼��󳤶�
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
        std::vector<std::string> resultVec(nRows);

        int iRow = 0, iColumn = 0;
        int iDir = 0; // ��ʾ���£�����б������

        for(int i = 0; i < s.length() ; i++)
        {
			
            resultVec[iRow].push_back(s[i]);
            if ( iDir == 0) //����
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
            else //����
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

        std::string res;
        for(int i = 0; i < resultVec.size(); i++){
			   std::cout << "line: "<< resultVec[i]<<std::endl;
               res += resultVec[i];
        }

        return res;
    }
};


int main()
{
	std::string str = "cbbd";
	Solution s;
	std::cout << "result: " << s.longestPalindrome(str) << std::endl;
	
	std::string str1 = "PAYPALISHIRING";
	NSolution s2;
	std::cout << "result: " << s2.convert(str1, 3) << std::endl;
	
	getchar();
}
