
//题目描述
//现在有一个整数类型的数组，数组中素只有一个元素只出现一次，其余的元素都出现两次。
//注意：
//你需要给出一个线性时间复杂度的算法，你能在不使用额外内存空间的情况下解决这个问题么？

//用ones来记录只出现过一次的bits，用twos来记录只出现过两次的bits，ones&twos实际上就记录了出现过三次的bits，
class Solution {
public:
    int singleNumber(int A[], int n) {
        int ones = 0; //关注初始值，根据异或运算的规则以及ones的语义，初始值为0
        int twos = 0;
        int threes;

        //用如下算法计算ones/twos/threes的数值
        for (int i=0; i<n;i++)
        {
            int t = A[i];

            //先更新twos，这意味着ones和t对应的位都为1，则two对应的位为1;同时ones^t会抵消掉1^1
            twos |= ones&t;
            
            //按顺序异或，运算
            ones ^= t;

            //更新threes，当threes更新，则修改ones和twos上对应的位
            threes = ones & twos; //默认threes是0;
            ones &= ~threes;
            twos &= ~threes;
        }
        return ones;
    }
};

/*若一个数出现三次，则其对应的二进制数每一位相加必为3或0。
统计数组中所有元素的每一位，若为3的倍数，所求数的该二进制位对3取余为0，否则为1。
**/
class Solution {
    int singleNumber(int[] A) {      
       int result=0;
        for(int i=0;i<32;++i){
            int bits=0;
            for(int j=0;j<A.length;++j){
                bits+=(A[j]>>i)&1;//依次获取元素的每一位，并将数组元素相同位相加               
            }
            result|=(bits%3)<<i;
        }
        return result;
    }
};