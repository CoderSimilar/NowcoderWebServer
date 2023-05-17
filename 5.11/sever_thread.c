#include <func.h>

struct sockInfo{

    int fd;//通信的文件描述符
    pthread_t pid;//线程号
    struct sockaddr_in addr;

};

//为了防止过多客户端连接服务器端，设置一个数组限制最大连接数
struct sockInfo sockInfos[128];

//每个线程的执行函数
void* working(void* arg) {

    //获取客户端的信息，将参数强转为sockInfo的指针类型
    struct sockInfo* pinfo = (struct sockInfo*) arg;

    //获取客户端的IP和端口
    char clientIp[16];
    //获取客户端IP
    inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, clientIp, sizeof(clientIp));
    //获取客户端端口
    unsigned short cport = ntohs(pinfo->addr.sin_port);

    printf("%s : %d\n", clientIp, cport);

    while(1) {
        
        //接收对面发过来的数据
        char bufRecv[1024] = {0};
        int len = read(pinfo->fd, bufRecv, sizeof(bufRecv));

        if(len > 0) {
            printf("Recv Data : %s\n", bufRecv);
        }else if(len == -1) {
            perror("read");
            exit(-1);
        }else if(len == 0) {
            printf("client closed\n");
            break;
        }

        //向对面发送数据
        write(pinfo->fd, bufRecv, strlen(bufRecv) + 1);

    }

    close(pinfo->fd);
    return NULL;

}


int main() {

    //创建socket
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");

    //绑定IP和端口
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;//指定协议为IPv4协议
    saddr.sin_addr.s_addr = INADDR_ANY;//监听所有的网卡
    saddr.sin_port = htons(9999);//监听9999的端口，将其转化为网络字节序

    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    int ret = bind(sockfd, (struct sockaddr*)&saddr, sizeof(saddr));
    ERROR_CHECK(ret, -1, "bind");

    //监听
    ret = listen(sockfd, 10);
    ERROR_CHECK(ret, -1, "listen");



    //不断循环等待客户端连接
    while(1) {
        
        struct sockaddr_in caddr;
        socklen_t len = sizeof(caddr);

        int clientfd = accept(sockfd, (struct sockaddr*)&caddr, &len);
        ERROR_CHECK(clientfd, -1, "accept");

        //创建子线程
        pthread_t pid;

        int size = sizeof(sockInfos) / sizeof(sockInfos[0]);//获取最多能够同时连接服务器端的数量
        struct sockInfo *pinfo;//定义一个sockInfo的指针，用于向子线程传递参数
        bzero(pinfo, 0);
        for(int i = 0; i < size; i++) {
            sockInfos[i].fd = -1;//将sockInfo结构体数组的每一个成员的线程号以及socket文件描述符设置成-1，表示未被使用
            sockInfos[i].pid = -1;
        }
        

        for(int i = 0; i < size; i++) {
            //如果结构体数组的某个成员未被使用的话，那么就将其取地址赋值给pinfo,并结束循环
            if(-1 == sockInfos[i].pid) {
                pinfo = &sockInfos[i];
                break;
            }
            //如果达到了连接服务器的最大限制
            if(i == size - 1) {
                sleep(1);
                i--;
            }
        }   
        pinfo->fd = clientfd;
        memcpy(&pinfo->addr, &caddr, len);

        //创建子线程
        int ret = pthread_create(&pinfo->pid, NULL, working, pinfo);
        //分离子线程，从而让其结束的时候可以自己回收资源
        pthread_detach(pinfo->pid);

    }

    close(sockfd);

    return 0;


}

