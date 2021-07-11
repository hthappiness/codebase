#include <vector>
#include <map>

class solution
{
public:

    bool jump(int curPos, int lastStepNum)
    {
        for(int i = lastStepNum - 1 ; i < lastStepNum; i++)
        {

            if( curPos + i == stepEnd_)
            {
                //刚好达到目标
                return true;
            }

            //检查到达地是否有踏板
            if( dest(curPos+i) )
            {
                jump(curPos + i, i);
            }
            else
            {     
                continue;
            }
            
        }

        return false;
    }

    void initDestMap(const std::vector<int>& grid)
    {
        for( auto& item : grid )
        {
            map_[item] = 1;
            stepEnd_++;
        }
    }

    bool dest(int step)
    {
        auto iter = map_.find(step);
        if( iter == map_.end() )
        {
            return false;
        }
        return true;
    }

private:
    std::map<int, bool> map_;
    int stepEnd_ = 0;
};