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
private:
    int dp[100][256];
    std::string m_pat;
};

int main()
{
    KMP test("ABCABCE");

    std::cout << "result: " << test.search("C45ABCABCABCE") << std::endl;
}