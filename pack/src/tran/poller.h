
class IPollerItem
{
public:
    virtual int readData()  = 0;
    virtual int writeData() = 0;
    virtual int ErrData()   = 0;
};

class CIoThread
{
public:
    int AddPollerItem( int fd, IPollerItem* pstPollerItem);
private:
    poller m_poller;
    std::thread m_thread;
};

