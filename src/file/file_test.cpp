
#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct tagBlockHdr
{
    int iMagic;
    int iBlockId;
    int iRecordCnt;
    int iLeftSize;
}BLOCK_HDR_S;

typedef struct tagRecordHdr
{
    int iMagic;
    int iDrop;
    int iKeySize;
    int iValueSize;
}RECORD_HDR_S;

#define U32 unsigned int

constexpr auto block_size = 256;

class CTestDb
{
public:
    CTestDb(std::string filename)
    {
        loadFile(filename);
    }

    ~CTestDb()
    {
        fsync(m_file);
    }

    int loadFile(std::string fileName)
    {
        int fd = open(fileName.c_str(), O_CREAT | O_RDWR | O_TRUNC);
        if( -1 == fd )
        {
            std::cout << "open file failed." << std::endl;
            return 0;
        }
        m_file = fd;

        //test 先申请一个200*4k大小的文件
        int i = 200;
        while( i > 0)
        {
            char fileBuf[block_size] = {0};
            initBlock(fileBuf, 200 - i);
            write(m_file, fileBuf, block_size);
            i--;
        }

        //fsync(m_file);

        return 0 ;
    }

    int initRootBlock()
    {
        return 0 ;
    }

    void initBlock(char* fileBuf, int index)
    {
        BLOCK_HDR_S* pstBlockHdr = (BLOCK_HDR_S*)fileBuf;

        pstBlockHdr->iMagic = 0x5A5A5A5A;
        pstBlockHdr->iBlockId = index;
        pstBlockHdr->iRecordCnt = 0;
        pstBlockHdr->iLeftSize = block_size - sizeof(BLOCK_HDR_S);
    }

public:
    int insert(std::string key, std::string value)
    {
        static int cnt = 0;
        std::cout << "insert id:" << cnt << std::endl;
        cnt++;

        unsigned int offset = block_size;
        int iRet;
        int iKeySize = key.length();
        int iValueSize = value.length();
        //seek insert positon
        while(true)
        {
            char buf[block_size];
            iRet = readBlock(buf, offset);
            if( 0 != iRet )
            {
                std::cout << "read block failed!" <<std::endl;
                break;
            }

            BLOCK_HDR_S* pstBlockHdr = (BLOCK_HDR_S*)buf;

            if( 0x5A5A5A5A != pstBlockHdr->iMagic )
            {
                std::cout<< "bad block magic when insert:" << std::hex << pstBlockHdr->iMagic << std::endl;
                break;
            }

            if( pstBlockHdr->iLeftSize < iKeySize + iValueSize + sizeof(RECORD_HDR_S))
            {
                std::cout << "---------next block insert!" <<std::endl;
                offset += block_size ;
                continue;
            }
            pstBlockHdr->iRecordCnt++;

            char* bufferInsert = (char*)(buf + block_size - pstBlockHdr->iLeftSize);
            RECORD_HDR_S* pstRecordHdr = (RECORD_HDR_S*)(buf + block_size - pstBlockHdr->iLeftSize);
            pstRecordHdr->iKeySize = iKeySize;
            pstRecordHdr->iMagic = 0x4A4A4A4A;
            pstRecordHdr->iValueSize = iValueSize;
            pstRecordHdr->iDrop = false;
            pstBlockHdr->iLeftSize -= (sizeof(RECORD_HDR_S) + iKeySize + iValueSize);

            //std::cout << "key size:" << iKeySize << std::endl;
            //std::cout << "value size:" << iValueSize << std::endl;
            //std::cout << "left size:" << pstBlockHdr->iLeftSize << std::endl;

            memcpy(bufferInsert + sizeof(RECORD_HDR_S), key.c_str(), iKeySize);
            memcpy(bufferInsert + sizeof(RECORD_HDR_S) + iKeySize , value.c_str(), iValueSize);

            writeBlock(buf, offset);
            offset += block_size ;
            break;
        }
    }

    int update(std::string keyParam, std::string valueParam)
    {
        unsigned int offset = block_size;
        int iRet = 0;

        while(true)
        {
            std::cout << "offset = " << offset << std::endl;
            char buf[block_size]= {0};
            iRet = readBlock(buf, offset);
            if( 0 != iRet )
            {
                std::cout << "read block failed! when show." <<std::endl;
                break;
            }

            BLOCK_HDR_S* pstBlockHdr = (BLOCK_HDR_S*)buf;
            if( 0 == pstBlockHdr->iRecordCnt )
            {
                std::cout << "block record = 0" <<std::endl;
                break;
            }

            if( 0x5A5A5A5A != pstBlockHdr->iMagic )
            {
                std::cout<< "bad block magic when show:" << std::hex << pstBlockHdr->iMagic << std::endl;
                break;
            }

            int iCnt = pstBlockHdr->iRecordCnt;
            std::cout << "recordCnt = " << iCnt << std::endl;
            int recordOffset = 0;
            for( int i = 0 ; i < iCnt; i++ )
            {
                char* bufferReader = (char*)(buf + sizeof(BLOCK_HDR_S) + recordOffset);
                RECORD_HDR_S* pstRecordHdr = (RECORD_HDR_S*)(buf + sizeof(BLOCK_HDR_S) + recordOffset);
                int iKeySize = pstRecordHdr->iKeySize;
                int iValueSize = pstRecordHdr->iValueSize;
                std::string key((char*)(bufferReader+sizeof(RECORD_HDR_S)), iKeySize);
                std::string value((char*)(bufferReader+sizeof(RECORD_HDR_S) +iKeySize), iValueSize);
                //std::cout<< key << ":" << value << std::endl;

                if( keyParam == key )
                {
                    pstRecordHdr->iDrop = true;
                    writeBlock(buf, offset);
                    return insert(keyParam, valueParam);
                }

                recordOffset+=(sizeof(RECORD_HDR_S) + iKeySize + iValueSize);
            }

            offset += block_size; 
        }

        return iRet;
    }
    
    int deleteItem()
    {
        return 0;
    }

    int show()
    {
        unsigned int offset = block_size;
        int iRet;

        while(true)
        {
            std::cout << "offset = " << offset << std::endl;
            char buf[block_size]= {0};
            iRet = readBlock(buf, offset);
            if( 0 != iRet )
            {
                std::cout << "read block failed! when show." <<std::endl;
                break;
            }

            BLOCK_HDR_S* pstBlockHdr = (BLOCK_HDR_S*)buf;
            if( 0 == pstBlockHdr->iRecordCnt )
            {
                std::cout << "block record = 0" <<std::endl;
                break;
            }

            if( 0x5A5A5A5A != pstBlockHdr->iMagic )
            {
                std::cout<< "bad block magic when show:" << std::hex << pstBlockHdr->iMagic << std::endl;
                break;
            }

            int iCnt = pstBlockHdr->iRecordCnt;
            std::cout << "recordCnt = " << iCnt << std::endl;
            int recordOffset = 0;
            for( int i = 0 ; i < iCnt; i++ )
            {
                char* bufferReader = (char*)(buf + sizeof(BLOCK_HDR_S) + recordOffset);
                RECORD_HDR_S* pstRecordHdr = (RECORD_HDR_S*)(buf + sizeof(BLOCK_HDR_S) + recordOffset);
                int iKeySize = pstRecordHdr->iKeySize;
                int iValueSize = pstRecordHdr->iValueSize;
                if( pstRecordHdr->iDrop == false )
                {     
                    std::string key((char*)(bufferReader+sizeof(RECORD_HDR_S)), iKeySize);
                    std::string value((char*)(bufferReader+sizeof(RECORD_HDR_S) +iKeySize), iValueSize);
                    std::cout<< key << ":" << value << std::endl;
                }

                recordOffset+=(sizeof(RECORD_HDR_S) + iKeySize + iValueSize);
            }

            offset += block_size; 
        }
    }
private:
    int readBlock(char* block, unsigned int u32Offset)
    {
        //std::cout << "read u32Offset = " << u32Offset << std::endl;
        int iRet = lseek(m_file, u32Offset, SEEK_SET);//文件头开始
        if( u32Offset != iRet)
        {
            std::cout << "lseek failed!" << iRet <<std::endl;
            return iRet;
        }
        iRet = read(m_file, block, block_size);
        if( block_size != iRet)
        {
            std::cout << "fread failed!" << iRet <<std::endl;
            return 1;
        }

        return 0;
    }

    int writeBlock(char* block, unsigned int u32Offset)
    {
        int iRet ;
        iRet = lseek(m_file, u32Offset, SEEK_SET);//文件头开始
        if( u32Offset != iRet)
        {
            return iRet;
        }
        iRet = write(m_file, block, block_size);
        if( 0 !=  iRet)
        {
            return iRet;
        }

        //fsync(m_file);

        return iRet;
    }

    int getRootBlock(char* rootBlock)
    {
        return readBlock(rootBlock, 0);
    }
private:
    int m_file; 
};

int main()
{ 
    CTestDb db("test.db");

    std::string lixi_name = "lixi";
    std::string lixi_words = "I am lixi.";

    std::string gby_name = "gby";
    std::string gby_words = "I am guboyuan.";

    std::string cwc_name = "cwc";
    std::string cwc_words = "I am chenwencai.";

    std::cout << "insert lixi" << std::endl;
    db.insert(lixi_name, lixi_words);
    std::cout << "insert guboyuan" << std::endl;
    db.insert(gby_name, gby_words);
    std::cout << "insert cwc" << std::endl;
    db.insert(cwc_name, cwc_words);
#if 0
    std::cout << "insert lixi" << std::endl;
    db.insert(lixi_name, lixi_words);
    std::cout << "insert guboyuan" << std::endl;
    db.insert(gby_name, gby_words);
    std::cout << "insert cwc" << std::endl;
    db.insert(cwc_name, cwc_words);
      std::cout << "insert lixi" << std::endl;
    db.insert(lixi_name, lixi_words);
    std::cout << "insert guboyuan" << std::endl;
    db.insert(gby_name, gby_words);
    std::cout << "insert cwc" << std::endl;
    db.insert(cwc_name, cwc_words);
    std::cout << "------------------------insert 12:----------------" << std::endl;
      std::cout << "insert lixi" << std::endl;
    db.insert(lixi_name, lixi_words);
    std::cout << "insert guboyuan" << std::endl;
    db.insert(gby_name, gby_words);
    std::cout << "insert cwc" << std::endl;
    db.insert(cwc_name, cwc_words);
      std::cout << "insert lixi" << std::endl;
    db.insert(lixi_name, lixi_words);
    std::cout << "insert guboyuan" << std::endl;
    db.insert(gby_name, gby_words);
    std::cout << "insert cwc" << std::endl;
    db.insert(cwc_name, cwc_words);
      std::cout << "insert lixi" << std::endl;
    db.insert(lixi_name, lixi_words);
    std::cout << "insert guboyuan" << std::endl;
    db.insert(gby_name, gby_words);
    std::cout << "insert cwc" << std::endl;
    db.insert(cwc_name, cwc_words);
    std::cout << "------------------------insert 2:----------------" << std::endl;
   std::cout << "insert lixi" << std::endl;
    db.insert(lixi_name, lixi_words);
    std::cout << "insert guboyuan" << std::endl;
    db.insert(gby_name, gby_words);
    std::cout << "insert cwc" << std::endl;
    db.insert(cwc_name, cwc_words);
        std::cout << "------------------------insert 3:----------------" << std::endl;
    std::cout << "insert lixi" << std::endl;
    db.insert(lixi_name, lixi_words);
    std::cout << "insert guboyuan" << std::endl;
    db.insert(gby_name, gby_words);
    std::cout << "insert cwc" << std::endl;
    db.insert(cwc_name, cwc_words);
#endif
    std::cout << "------------------------show:----------------" << std::endl;

    db.show();

    std::cout << "------------------------update cwc:----------------" << std::endl;

    db.update("cwc", "I am chenwencai, I am a girl.");

    std::cout << "------------------------show:----------------" << std::endl;
    db.show();

    //getchar();
} 