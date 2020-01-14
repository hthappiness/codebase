

class CRaftServer
{
public:
    CRaftServer();//初始化timer，处理心跳消息
private:

    void init();
    void run();

    //http消息处理函数

    //定时器处理函数

    //

private:
    boost::asio::io_context m_ioCtx;
    boost::asio::timer m_timer;
    boost::asio::ip::tcp m_socket;
};

void CRaftServer::init()
{
    //注册定时器回调函数

    //注册http处理函数
}

void CRaftServer::run()
{
    m_ioCtx.run();
}