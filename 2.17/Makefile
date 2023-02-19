# 获取当前目录下所有的.cpp文件
SRC = $(wildcard ./*.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SRC))
# 指定编译方式为g++
CC = g++
# 目标文件命名为test
TARGET = test
# 编译选项
CFLAGS = -Wall -g

# 构建目标依赖
$(TARGET) : $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o : %.c
	$(CC) $^ -c $@

clean:
	rm $(TARGET) $(OBJS)

.PHONY : clean