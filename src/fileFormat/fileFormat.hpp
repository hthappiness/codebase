

enum fileType{
        FILE_ASCII,
        FILE_BINARY,
        FILE_UNKNOWN_TYPE
    };

enum fileFormat{
        FILE_FMT_ELF,
        FILE_FMT_PCAP,
        FILE_FMT_LEVELDB,
        FILE_FMT_WROD,
        FILE_FMT_XML,
        FILE_FMT_JSON,
        FILE_FMT_GIT
    };

class CFileBase
{
public:
    CFileBase();
    ~CFileBase();

public:
    //文件读写
    virtual int read();
    
    virtual int write();
};

/*
CFileBase *pstFile = new ;
pstFile->data_persistence();

//定义各种存储格式

*/

/* 描述内存数据流 */
class CStreamBase
{
public:
    //持久化数据
    virtual int data_persistence() = 0;

    //附加数据
    virtual int append() = 0;

private:
    CFileBase* m_fileImpl;

    fileType m_fileType;
    fileFormat m_fileFormat;
};

//TLV, tag/len/var      int| 100 | xx-x-x-x-x-x-x-x-x--x

struct CItemHdr{
    unsigned int magic;
    unsigned int iLen;
    unsigned int iType;
};

struct CFileHdr{
    //range
    char acKeyRangeLow[64];
    char acKeyRangeHigher[64];
};