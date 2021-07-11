#include <iostream>
#include <string>
#include <memory>
#include <string.h>

class KMP {
 
public:
    KMP(std::string pat) {
        m_pat = pat;
        int M = pat.length();
        // dp[状态][字符] = 下个状态

        memset(dp, 0, sizeof(int)*100*256);
        // base case
        dp[0][pat[0]] = 1;
        // 影子状态 X 初始为 0
        int X = 0;
        // 构建状态转移图（稍改的更紧凑了）
        for (int j = 1; j < M; j++) 
        {
            for (int c = 0; c < 256; c++)
                dp[j][c] = dp[X][c];
            
            dp[j][pat[j]] = j + 1; //j模式

            //影子状态描述的是遍历前面j个字符之后，此时的影子状态是什么，这意味着后缀的匹配偏移量；记录当不匹配的时候，回退的状态
            // 更新影子状态的逻辑是，0状态的迁移
            // 对状态0来说，只有遇到了pat[0],状态才会向前推进，也就是说我们在遍历pat[1, M]的过程中，再次
            X = dp[X][pat[j]];// 该赋值意味着，当遍历到pat[j]的时候，如果此时的影子状态是X，该如何跳变，是顺序推进，还是回退
            std::cout << "shadow state is:" << X <<std::endl;

            //影子状态意味着，后缀有匹配前缀的，suffix和prefix，因为遍历后面遇到的字符串的信息都记录在前面了，相当于子问题，动态规划的思想
            //一个变量就可以描述影子状态，并达到相应的功能
            //出现不匹配的情况时候，回撤可以马上到达影子状态；影子状态是对后缀匹配的结果，就是说后缀所能达到的
            // X是最大的后缀匹配之后可以达到的状态，
            //dp数组类似于next数组
        }
#if 0
        for (int j = 0; j < M; j++) 
        {
            std::cout << "round=" << j << std::endl;
             for (int c = 0; c < 256; c++)
             {
                 std::cout << " " << dp[j][c];
             }
             std::cout << std::endl;
        }
#endif
    }

     int search(std::string txt) {
        int M = m_pat.length();
        int N = txt.length();
        // pat 的初始态为 0
        int j = 0;
        for (int i = 0; i < N; i++) {
            // 计算 pat 的下一个状态
            j = dp[j][txt[i]];
            // 到达终止态，返回结果
            if (j == M) return i - M + 1;
        }
        // 没到达终止态，匹配失败
        return -1;
    }

    /* ------------------------------------------这是原始的kmp算法------------------------------------------------------ */

    int KMPOriginal(int start,char S[],char T[])
    {
        int i=start,j=0;
        while(S[i]!='\0'&&T[j]!='\0')
        {
            if(j==-1||S[i]==T[j])
            {
                i++;         //继续对下一个字符比较 
                j++;         //模式串向右滑动 
            }
            else j=next[j];
        }
        if(T[j]=='\0') return (i-j);    //匹配成功返回下标 
        else return -1;                 //匹配失败返回-1 
    }

    //KMP的next数组告诉我们：当模式串中的某个字符跟主串中的某个字符失配时，模式串下一步应该跳到哪个位置。
    //如模式串中在j 处的字符跟主串在i 处的字符匹配失配时，下一步用next [j] 处的字符继续跟主串i 处的字符匹配，
    //相当于模式串向右移动 j - next[j] 位。 意味着当不匹配的时候，主串的j下一次开始匹配模式串的哪一个字符
    void GetNext(char T[])
    {
        /* j是主串的下标，k是模式串的下标 */
        int j=0,k=-1; /* -1是哨兵 */
        next[j]=k;
        while(T[j]!='\0')
        {
            if(k==-1||T[j]==T[k])
            {
                j++;
                k++;
                next[j]=k; /* 记录后缀的匹配状态 */
            }
            else 
            {
                k=next[k];/* 意味着前k个字符是匹配的，累进的一个状态 */
            }

            /* 看next的赋值，很显然，每次匹配，next才会增加，否则就回溯，也就是寻找最长公共前后缀的过程 */
        }
    }

    int GetNextII(int j,char T[])
    {
        if(j==0)return -1;
        if(j>0)
        {
            int k=GetNextII(j-1,T);
            while(k>=0)
            {
                if(T[j-1]==T[k])return k+1;
                else k=GetNextII(k,T);
            }
            return 0;
        }
        return 0;
    }


private:
    int dp[100][256];
    std::string m_pat;

      int next[100];
};

int main()
{
    KMP test("ABCABCE");

    std::cout << "result: " << test.search("C45ABCABCABCE") << std::endl;
}

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
        int result[2];
        for (int i = 0; i < s.length(); i++) 
        {
//             把回文看成中间的部分全是同一字符，左右部分相对称
//             找到下一个与当前字符不同的字符
            i = findLongest(s, i, result);
        }
        return s.substr(result[0], result[1]+1);  
    }

    int findLongest(string str, int low, int range[2]) 
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