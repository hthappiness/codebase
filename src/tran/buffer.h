
class CBaseBuf
{
public:
    CBaseBuf();
    ~CBaseBuf();

    int PutData(char* pBuf, int iLen);
    int GetData(char* pBuf, int& iLen);
    int GetDataLen();
    int ResizeBuf(int);
    int ResetBuf();
    bool IsBufEmpty();
    bool IsBufValid();
private:
    int m_BufMaxLen;
    int m_RdIndex;
    int m_WrIndex;
};

/* for user transfer */
class CSenderBuf: public CBaseBuf
{
public:
    bool IsBufEmpty();
    bool IsBufValid();
private:
    U64  u64VirtAddr;
    std::mutex m_mutex;
};

class CPcivBuf: public CBaseBuf
{

};
/* for pciv transfer */
class CPcivSenderBuf: public CBaseBuf
{
public:
    int initLocalBuf();
    bool IsRemoteInit();
    int setRemotePhyAddr(U64);
    U64 getLocalPhyAddr();
    U64 getLocalVirtAddr();
    bool IsBufEmpty();
    bool IsBufValid();
private:
    bool bBufInit;
    U64 u64LocalPhyAddr;
    U64 u64LocalVirtAddr;
    U64 u64RemotePhyAddr;

    std::mutex m_mutex;
};

class CPcivReceiverBuf: public CBaseBuf
{
public:
    int initLocalBuf();
    bool IsRemoteInit();
    int setRemotePhyAddr(U64);
    U64 getLocalPhyAddr();
    U64 getLocalVirtAddr();
private:
    bool bBufInit;
    
    U64 u64LocalPhyAddr;
    U64 u64LocalVirtAddr;
    U64 u64RemotePhyAddr;

    std::mutex m_mutex;
};

class CSender
{
public:
    CPcivSender();
    ~CPcivSender();

    int pcivSendData(char* pBuf, int ilen);
private:
    int pcivDmaMoveData();
private:
    CSenderBuf             m_UserBuf;
    CPcivSenderBuf         m_PcivBuf;
    std::thread            m_thread;
    std::mutex             m_mutex;
    std:condition_variable m_cv;
};

class CReceiver
{
    /* for callback:
     * 1.绑定类静态成员函数，需要添加this
     * 2.绑定类非静态成员函数
     * 3.C形式的函数
     */
    using DataReceiverCb = std::function<void (void* , char* , int)>; /* 这里无需添加类名，以免强耦合某个类;这里只要符合参数即可绑定 */
public:
    CReceiver();
    ~CReceiver();

    int pcivGetData();
private:
    CPcivReceiverBuf m_ReceiverBuf;
    DataReceiverCb   m_DataReceiverCb;
    void* pUsrData;
};

class ITranEventCb
{
    virtual int connectionMessage() = 0;
    virtual int connectionTimeout() = 0;
};

class CMccMsgPoller : public IPollerItem
{
public:
    virtual int readData()  ;
    virtual int writeData() ;
    virtual int ErrData()   ;

    CPcivConnector* m_pstConn;
};

class CMccMsgWrapper
{
public:
    CMccMsgWrapper(int iPort);
    int mccSendMsg();
    int mccGetRdHndl(int& );
private:
    int m_iPort;
    int m_iSendFd;
    int m_iRecvFd;
};

/* 类比tcp */
enum EConnectionState
{
    CONN_WAITING,
    CONN_ESTABLISHED,
    CONN_FIN_WAIT,
    CONN_CLOSED
};

class CPcivConnector ：public IPollerItem
{
public:
    CPcivConnector();
    ~CPcivConnector();

    int pcivInit(int iTargetId, int iPort);
    int pcivConnect();
    int pcivSendData(char* pBuf, int ilen);

    /* 清理buffer之前需要告知对端停止发送 */
    int pcivCleanBuf();
    int pcivSetEventCb(std::unique_ptr<ITranEventCb>&& pstEventCb);
private:
    int pcivWaitConnect();

    virtual int readData() ;
    virtual int writeData();
    virtual int ErrData()  ;
private:
    CSender    m_Sender;
    CReceiver  m_Receiver;

    //CMccMsgPoller  m_MccMsgHandler;
    CMccMsgWrapper m_MccMsgWrapper;
    std::unique_ptr<ITranEventCb> m_EventCb;
    CIoThread m_IoPoller;
};

