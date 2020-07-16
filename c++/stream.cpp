#include <iostream>
#include <streambuf>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <memory>

using namespace std;

// physical device（io device:uart keyboard disk net tty） ----- stream （buffer payload）------ data
//关于stream的input和output
//1. I/O, input和output的方向是什么, io设备和内存之间的数据走向， io device到内存，则为input，比如网卡、硬盘、串口；
//   内存到io device，则为output
//2. cin和cout, cin >> a, 意味着从键盘到内存，即为input；cout << a ，意味着从内存到tty，即为output
//   input device --------> inputstream  -----> mem buffer  |  when operating in the way of stream interface, input buffer, read
//   output device <--------- outputstream  <----- mem buffer
//3. 

//https://www.cnblogs.com/luzhlon/p/7055385.html

//stringbuf
//filebuf
//


// 在特殊的情景下可以实现自己的streambuf类，自己实现的类必须继承streambuf 类，自定义的streambuf 必须实现overflow,underflow,uflow 等方法，
// 其中overflow在输出缓冲区不够用时调用，underflow和uflow在输入缓冲区无数据时调用,区别是uflow 会让读取位置前进一位，而underflow不会。
// streambuf 内部维护着六个指针 eback,gptr,egptr | pbase,pptr,epptr.
// 分别指向读取缓冲区的头，当前读取位置，尾，写缓冲区的头，当前写位置，尾（实际上这几个指针指向同一段缓冲区）
// 自定义实现方式要注意要在该返回EOF的时候，返回EOF,underflow和uflow都有可能返回EOF,一旦返回了EOF则标志着流结束，之后对流的操作无效。
class mybuf : public streambuf {
public:
	enum{ SIZE = 100};
	mybuf() {
		memset(buffer, 'j', SIZE);
		buffer[3] = 'c';
		setbuf(buffer, SIZE);
	}
	void log() {
		cout << "log:" << hex << gptr() << endl;
	}
protected:
	// 溢出
	int_type overflow( int_type c) {
		cout << "overflow" << endl;
		return c;
	}
	streambuf* setbuf(char* s, streamsize n) {
		// Set output sequence pointers
		setp(s, s + n);

		// Set input sequence pointers
		setg(s, s, s + n);
		return this;
	}

	// underflow和uflow在输入缓冲区无数据时调用
	int_type underflow() override{
		cout << "underflow"<<endl;
		memset(buffer, 'w', 10);

		// Set input sequence pointers. device提供的数据比较多，或者mem端读取数据
		setg(buffer, buffer, buffer+10);
		return ' ';
	}

	int_type uflow() override{
		cout << "uflow" << endl;
		memset(buffer, 'x', 10);

		// Set input sequence pointers. device提供的数据比较多，或者mem端读取数据
		setg(buffer, buffer, buffer + 10);
		return EOF;
	}
private:
	char buffer[SIZE];
};

class resultHeader
{
public:
	void setId(unsigned char id){id_ = id;}
	void setLen(unsigned char len){len_ = len;}
	unsigned char getId(){return id_;}
	unsigned char getLen(){return len_;}
private:
	friend istream& operator>>(istream& istream, resultHeader& header);
	friend ostream& operator<<(ostream& istream, resultHeader& header);
	unsigned char id_;
	unsigned char len_;
};

istream& operator>>(istream& istream, resultHeader& header)
{
	istream >> header.id_;
	istream >> header.len_;
	return istream;
}

ostream& operator<<(ostream& ostream, resultHeader& header)
{
	ostream << header.id_;
	ostream << header.len_;
	return ostream;
}

class resultbuf : public streambuf {
public:
	enum{ SIZE = 10};
	resultbuf() {
		//memcpy(buffer, &header, sizeof(resultHeader));
		setbuf(buffer, SIZE);
	}
	void log() {
		cout << "log:" << hex << gptr() << endl;
	}
protected:
	
	streambuf* setbuf(char* s, streamsize n) {
		
		// Set output sequence pointers
		setp(s, s + n);

		// Set input sequence pointers
		setg(s, s, s + n);
		return this;
	}
	
	// 溢出;输出缓冲区不够用时调用
	int_type overflow( int_type c) {
		cout << "overflow" << endl;
		return c;
	}

	// underflow和uflow在输入缓冲区无数据时调用
	int_type underflow() override{
		cout << "underflow"<<endl;
		memset(buffer, 'w', 10);

		// Set input sequence pointers. device提供的数据比较多，或者mem端读取数据
		setg(buffer, buffer, buffer+10);
		return ' ';
	}

	int_type uflow() override{
		cout << "uflow" << endl;
		memset(buffer, 'x', 10);

		// Set input sequence pointers. device提供的数据比较多，或者mem端读取数据
		setg(buffer, buffer, buffer + 10);
		return EOF;
	}
private:
	char buffer[SIZE];
};

int testMyBuf() 
{
	mybuf buf;

	char test[200];
	memset(test, 'a', 200);
	//buf.pubsetbuf(test, 200);

	string hh;

	// 这两个流使用了同一个buffer
	istream in(&buf);
	ostream tt(&buf);
	in >> hh;  //读取streamBuffer中的数据到hh中
    //in.write(hh.c_str());
	cout << "hh:" << hh << endl;
	//tt.write(test, 9);
	buf.log();

	string xx("test write data.");
	tt << xx; //读取xx的数据到streamBuffer中
	buf.log();
	in.read(test, xx.size());
	
	buf.log();
    cout<< "test: "<< test << endl;

	cout << "end" << endl;

	return 0;
}

// 字节流
int testResultBuf()
{
	resultbuf buffer;

	resultHeader writeHeader;
	writeHeader.setId(0x12);
	writeHeader.setLen(0x38);
	ostream out(&buffer);
	out << writeHeader;   //输出缓冲区溢出，调用overflow

	buffer.log();

	std::cout << "-------start input. reader-------\r\n" ;

	resultHeader readHeader;
	istream in(&buffer);
	in >> readHeader;   //输入缓冲区没有数据，调用underflow

	std::cout << std::hex << static_cast<int>(readHeader.getId()) << std::endl;
	std::cout << std::hex << static_cast<int>(readHeader.getLen())<< std::endl;

	buffer.log();

	return 0;
}

int main()
{
	testResultBuf();

	return 0;
}