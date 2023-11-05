SRCS := $(wildcard *.cpp) # SRCS表示获取当前目录下的所有.cpp文件
ELFS := $(patsubst %.cpp, %, $(SRCS))	# ELFS将当前目录下的.cpp文件去除掉.cpp后缀，表示编译后的二进制文件
CC := g++ # 编译方式设置为g++
CFLAGS := -Wall -std=c++11 # 设置编译选项
all : $(ELFS) # all目标表示构建ELFS中的所有二进制文件

% : %.cpp 
	$(CC) $(CFLAGS) $< -o $@ 
clean:
	rm -rf $(ELFS)
