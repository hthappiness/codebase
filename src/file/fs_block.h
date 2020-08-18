#pragma once

/* 磁盘数据结构和内存数据结构 */
/* block */

class CBlockTable
{
public:
    int tableSync();
    int load();
};

class CBlockManager : public CSingleton<CBlockManager>
{
public:
    CBlockManager() = default;

    /* index block */

    /* record block allocation table */

    int requestBlock(unsigned int& u32Offset);
    int requestBatchBlock(int block);

private:
    enum class resize_type{
        TIMES,
        APPEND,
        BUTT
    };
    int resizeBlock(); 
    int loadBlock();
private:
    constexpr auto block_size = 4*1024;
}