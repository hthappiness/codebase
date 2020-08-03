

all:cv rvalue

cv:cv.cpp
	g++ --std=c++11 $^ -o $@ -lpthread

rvalue:rvalue.cpp
	g++ --std=c++11 $^ -o $@ -fno-elide-constructors

clean:
	rm -f cv
	rm -f rvalue