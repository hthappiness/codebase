#include <iostream>
#include <streambuf>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <memory>

using namespace std;

//https://www.cnblogs.com/luzhlon/p/7055385.html

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
	int_type overflow( int_type c) {
		cout << "overflow" << endl;
		return c;
	}
	streambuf* setbuf(char* s, streamsize n) {
		setp(s, s + n);
		setg(s, s, s + n);
		return this;
	}
	int_type underflow() override{
		cout << "underflow"<<endl;
		memset(buffer, 'w', 10);
		setg(buffer, buffer, buffer+10);
		return ' ';
	}
	int_type uflow() override{
		cout << "uflow" << endl;
		memset(buffer, 'x', 10);
		setg(buffer, buffer, buffer + 10);
		return EOF;
	}
private:
	char buffer[SIZE];
};
int main() {
	mybuf buf;
	char test[2000];
	memset(test, 'a', 2000);
	//buf.pubsetbuf(test, 1000);
	string hh;
	string xx("test write data.");
	istream in(&buf);
	ostream tt(&buf);
	in >> hh;
    //in.write(hh.c_str());
	cout << "hh:" << hh << endl;
	//tt.write(test, 9);
	tt << xx;
	in.read(test, xx.size());
    cout<< "test: "<< test << endl;
	//cout<< xx << endl;
	cout << "end" << endl;
	return 0;
}