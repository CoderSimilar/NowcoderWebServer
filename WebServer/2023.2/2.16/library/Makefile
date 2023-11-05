# 如何使用：
#	1，将此Makefile文件保存在与源代码同级的目录下
#	2，在源代码目录中创建一个名为src的子目录，并将源代码文件放入其中。
#	3，在头文件目录中创建一个名为include的子目录，并将头文件放入其中。
#	4，如果使用静态库链接，则将生成的库文件保存在lib目录下的libmylib.a中。如果使用动态库链接，则将生成的库文件保存在.lib目录下的libmylib_dynamic.so中。
#	5，执行make命令以生成目标程序。执行make clean命令以清除生成的目标文件和库文件。可以使用make LINK=dynamic命令以选择动态链接库链接。
#指定源代码，头文件，库文件的路径
SRC_DIR=./src
INC_DIR=./include
LIB_DIR=./lib

#源代码文件
SRCS = $(wildcard $(SRC_DIR)/*.c)

#头文件路径
INCS = $(wildcard $(INC_DIR)/*.h)

#库文件路径和库名,-L指定库文件所在的目录，-mylib指定库的名称
LIBS = -L$(LIB_DIR) -lmylib

#编译选项
CFLAGS = -Wall -g

#目标文件
TARGET = myapp

#默认链接选项为静态库
LINK = static

# 如果连接选项为dynamic，则动态链接库名字为mylib.so
ifeq ($(LINK), dynamic)
	LIBS = -L $(LIB_DIR) -l mylib_dynamic
	TARGET = myapp_dynamic
endif

all: $(TARGET)

#静态库编译规则：
lib:$(LIB_DIR)/libmylib.a
$(LIB_DIR)/libmylib.a: $(SRCS) $(INCS)
	$(CC) $(CFLAGS) -c $(SRCS) -I $(INC_DIR)
	ar rcs $(LIB_DIR)/libmylib.a *.o
	ls -l $(LIB_DIR)/libmylib.a

#动态库编译规则：
$(LIB_DIR)/libmylib_dynamic.so: $(SRCS) $(INCS)
	$(CC) -shared -fPIC $(CFLAGS) $(SRCS) -I $(INC_DIR) -o $(LIB_DIR)/libmylib_dynamic.so

#应用程序编译规则：
$(TARGET):$(SRCS) $(INCS) 
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) -I $(INC_DIR)  $(LIBS)

clean:
	rm -f $(LIB_DIR)/*.a $(LIB_DIR)/*.so *.o $(TARGET)

.PHONY:clean all

