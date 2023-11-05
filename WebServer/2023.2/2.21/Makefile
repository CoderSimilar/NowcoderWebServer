# 第一个版本

# test:add.c sub.c mult.c div.c main.c
# 	gcc add.c sub.c mult.c div.c main.c -o test

# 第二个版本

# test:add.o sub.o mult.o div.o main.o
# 	gcc add.o sub.o mult.o div.o main.o -o test

# add.o:add.c
# 	gcc -c add.c

# sub.o:sub.c
# 	gcc -c sub.c
	
# mult.o:mult.c
# 	gcc -c mult.c

# div.o:div.c
# 	gcc -c div.c

# main.o:main.c
# 	gcc -c main.c

# 第三个版本
# 定义变量+模式匹配
# src=add.o sub.o mult.o div.o main.o
# target=test
# $(target):$(src)
# 	$(CC) $^ -o $(target)

# %.o:%.c
# 	$(CC) -c $< -o $@

# 第四个版本
# 函数
# src=$(wildcard ./*.c)
# objs=$(patsubst %.c, %.o, $(src))
# target=test
# $(target):$(objs)
# 	$(CC) $(objs) -o $(target)

# %.o:%.c
# 	$(CC) -c $< -o $@


# 第五个版本
# 伪目标
src=$(wildcard ./*.c)
objs=$(patsubst %.c, %.o, $(src))
target=test
$(target):$(objs)
	$(CC) $(objs) -o $(target)

%.o:%.c
	$(CC) -c $< -o $@

.PHONY:clean
clean:
	rm -f $(objs) $(target)


