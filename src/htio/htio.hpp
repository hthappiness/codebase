#include <uv.h>

class CServer
{
public:
    static void onConnection(uv_stream_t* server, int status);
    int runServer(char* ip, int iPort);
    int stopServer();
private:
    uv_tcp_t   m_server;
    uv_loop_t* m_loop;

    //会话管理
    //线程池
};

class CSession
{
public:
    static void onClose();
    static void onCreate();
private:
    uv_tcp_t   m_session;
    uv_async_t m_mutex;
};

class CHttpServer
{
private:
    virtual int httpGetMsgProcess() = 0;
    virtual int httpPostMsgProcess() = 0;
    virtual int httpDeleteMsgMsgProcess() = 0;
    virtual int httpPutMsgProcess() = 0;

    int httpMsgProcess();
public:

}

void CServer::onConnection(uv_stream_t* server, int status)
{
    //auto session = std::make_shared<CSession>();

    if(status == 0) {
		uv_tcp_t* client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
		uv_tcp_init(m_loop, client);
		uv_accept((uv_stream_t*)&m_server, (uv_stream_t*)client);

        /* 异步接口 */
		write_uv_data((uv_stream_t*)client, http_respone, -1, 0);
		//close client after uv_write, and free it in after_uv_close()
        // uv_tcp_t and uv_udp_t could be regarded as the sub class of uv_stream_t
	}
}

int CServer::runServer(char* ip, int iPort)
{
    m_loop = uv_default_loop();

    uv_tcp_init(m_loop, &m_server);
	uv_tcp_bind(&m_server, uv_ip4_addr(ip&&ip[0]?ip:"0.0.0.0", iPort));
	uv_listen((uv_stream_t*)&m_server, 8, CServer::onConnection);

    uv_run(m_loop, UV_RUN_DEFAULT);
}

int CServer::stopServer()
{
    uv_close((uv_handle_t*)w->handle, after_uv_close);
}