all:
	clang++ -std=c++11 \
	        -ggdb \
	        -Wall \
	        -Wextra \
	        -fmessage-length=0 \
	        -pthread \
	        -Isrc \
	        -o test \
	        src/test/main.cpp \
	        src/test/test.cpp
