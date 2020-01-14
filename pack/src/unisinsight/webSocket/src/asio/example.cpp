using boost::asio;
io_service service;
ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 2001);
ip::tcp::socket sock(service);
sock.connect(ep);

typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;
io_service service;
ip::tcp::endpoint ep( ip::tcp::v4(), 2001)); // listen on 2001
ip::tcp::acceptor acc(service, ep);
while ( true) {
    socket_ptr sock(new ip::tcp::socket(service));
    acc.accept(*sock);
    boost::thread( boost::bind(client_session, sock));
}
void client_session(socket_ptr sock) {
    while ( true) {
        char data[512];
        size_t len = sock->read_some(buffer(data));
        if ( len > 0)
            write(*sock, buffer("ok", 2));
    }
}

using boost::asio;
io_service service;
ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 2001);
ip::tcp::socket sock(service);
sock.async_connect(ep, connect_handler);
service.run();
void connect_handler(const boost::system::error_code & ec) {
    // 如果ec返回成功我们就可以知道连接成功了
}


using boost::asio;
typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;
io_service service;
ip::tcp::endpoint ep( ip::tcp::v4(), 2001)); // 监听端口2001
ip::tcp::acceptor acc(service, ep);
socket_ptr sock(new ip::tcp::socket(service));
start_accept(sock);
service.run();
void start_accept(socket_ptr sock) {
    acc.async_accept(*sock, boost::bind( handle_accept, sock, _1) );
}
void handle_accept(socket_ptr sock, const boost::system::error_code &
err) {
    if ( err) return;
    // 从这里开始, 你可以从socket读取或者写入
    socket_ptr sock(new ip::tcp::socket(service));
    start_accept(sock);
}

using boost::asio;
ip::tcp::endpoint ep;
ip::tcp::socket sock(service);
sock.connect(ep); // 第一行
boost::system::error_code err;
sock.connect(ep, err); // 第二行


try {
    sock.connect(ep);
} catch(boost::system::system_error e) {
    std::cout << e.code() << std::endl;
}

boost::system::error_code err;
sock.connect(ep, err);
if ( err)
    std::cout << err << std::endl;



void client_session(socket_ptr sock) {
    try {
        ...
    } catch ( boost::system::system_error e) {
        // 处理错误
    }
}


char data[512];
boost::system::error_code error;
size_t length = sock.read_some(buffer(data), error);
if (error == error::eof)
    return; // 连接关闭



io_service service;
serial_port sp(service, "COM7");
打开端口后，你就可以使用下面的代码设置一些端口选项，比如端口的波特率、奇偶校验和停止位。

serial_port::baud_rate rate(9600);
sp.set_option(rate);
打开端口后，你可以把这个串行端口看做一个流，然后基于它使用自由函数对串行端口进行读/写操作。比如async_read(), write, async_write(), 就像下面的代码片段：

char data[512];
read(sp, buffer(data, 512));
Boost.Asio也可以连接到Windows的文件，然后同样使用自由函数，比如read(), asyn_read()等等，就像下面的代码片段：

HANDLE h = ::OpenFile(...);
windows::stream_handle sh(service, h);
char data[512];
read(h, buffer(data, 512));
对于POXIS文件描述符，比如管道，标准I/O和各种设备（但不包括普通文件）你也可以这样做，就像下面的代码所做的一样：

posix::stream_descriptor sd_in(service, ::dup(STDIN_FILENO));
char data[512];
read(sd_in, buffer(data, 512));




bool read = false;
void deadline_handler(const boost::system::error_code &) {
    std::cout << (read ? "read successfully" : "read failed") << std::endl;
}
void read_handler(const boost::system::error_code &) {
    read = true;
}
ip::tcp::socket sock(service);
…
read = false;
char data[512];
sock.async_read_some(buffer(data, 512));
deadline_timer t(service, boost::posix_time::milliseconds(100));
t.async_wait(&deadline_handler);
service.run();