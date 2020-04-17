
#include <string>

typedef unsigned char U8;
typedef char S8;
typedef unsigned int U32;
typedef int S32;

#define ALIGN（size，a） (((size + (a - 1))/a ) * a)

#define MAX_SUB_FILE_NUM 8

typedef struct tagSubFileInfo
{
    char name[16];
    U32  u32FileSize; // 文件大小四字节对齐
}SUB_FILE_INFO_S;

typedef struct tagMixedFileHeader
{
    U32 u32Magic;
    U8  u8Rsvd[2];
    U8  u8CurFileNum;   /* offset */
    U8  u8FileType;
    SUB_FILE_INFO_S acSubfile[MAX_SUB_FILE_NUM];
}MIXED_FILE_S;

class CFile
{
public:
    explicit CFile(std::string& fileName ):m_fileName(fileName)
    {
　　   
    }

    bool open()
    {
        if (m_fileStream.is_open())
        {
            return true;
        }
        
        m_fileStream.open(m_fileName,ios::binary|ios::in,0);

        if (m_fileStream.eof())
        {
            cout << "file is empty."<<endl;
            return 0;
         }
    }

    /* Not allowed when get stream, because of the lseek offset */
    int getFileLen()
    {
        m_fileStream.seekg (0, m_fileStream.end);
        int length = m_fileStream.tellg();
        m_fileStream.seekg (0, m_fileStream.beg);
    }

    int readBuf(char* buf, int len)
    {
        m_fileStream.read(buf, len);

        /*
         while (!m_fileStream.eof())
        {
            fin.get(ch);
            cout << ch;
        }    
        */

        if (m_fileStream)
            std::cout << "all characters read successfully.";
        else
            std::cout << "error: only " << m_fileStream.gcount() << " could be read";
    }

    void close()
    {
        m_fileStream.close();
    }

    /* stream based operation */
    //m_fileStream.read((unsigned char*)n,sizeof(n));

    //1.get file size 
    //2.read every 4M 
    //3.
private:
    std::string m_fileName;
    fstream m_fileStream;
};

S32 InterConnect(CFile& , CFile& b, std::string& targetName);
{

}

S32 addPackdeFile(CFile& file, CFile& toAddFile, std::string& targetName);
{

}

/* 文件加密：可逆，查找 */

/* 将输入的一个组合文件拆分成一个个独立的文件 */
S32 extractFile(CFile& );

//无论如何，我要把自己想做的事条分缕析，清清楚楚
/*
 * 1. 写伪代码：关注类和接口，关注接口相关的数据，简单来说就是关注的类的public函数
 * 2. 具体的类之后再实现
 * 3. 这就是自顶向下的方法，不同层次的内容有逻辑、有顺序的进行，
 */

//学习库的方法论
/* 1. 类图关系，http://www.cplusplus.com/reference/ios/； 描述什么过程，解决什么问题
 * 2. 重视public接口，重视运用，重视实现 --->循序渐进
 * 3. 关注其设计，职责划分、解耦的方法，提升性能的方法
 * 4. c++ api设计，以继承、聚合的形式使用 + 静态检测 + 
 * /
 * 
 * 在特殊的情景下可以实现自己的streambuf类，自己实现的类必须继承streambuf 类，
 * 自定义的streambuf 必须实现overflow,underflow,uflow 等方法，其中overflow在输出缓冲区不够用时调用，
 * underflow和uflow在输入缓冲区无数据时调用,区别是uflow 会让读取位置前进一位，而underflow不会。
 * streambuf 内部维护着六个指针 eback,gptr,egptr,pbase,pptr,epptr.分别指向读取缓冲区的头，
 * 当前读取位置，尾，写缓冲区的头，当前写位置，尾（实际上这几个指针指向同一段缓冲区）
 *自定义实现方式要注意要在该返回EOF的时候，返回EOF,underflow和uflow都有可能返回EOF,一旦返回了EOF则标志着流结束，之后对流的操作无效。
