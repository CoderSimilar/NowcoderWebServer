#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H
#include <func.h>
#include "locker.h"

class http_conn {

public:

    static int m_epollfd; //所有的socket上的事件都被注册到同一个epoll对象中
    static int m_user_count; //统计用户的数量
    
    static const int READ_BUFFER_SIZE = 2048; //读缓冲区大小
    static const int WRITE_BUFFER_SIZE = 1024; //写缓冲区大小

    //HTTP请求方法，目前只支持GET
    enum METHOD { GET = 0, POST, HEAD, PUT, DELETE, TRACE, OPTIONS, CONNECT };

    /*
        解析客户端请求时，主状态机的状态
        CHECK_STATE_REQUESTLINE : 当前正在分析请求行
        CHECK_STATE_HEADER : 当前正在分析头部字段
        CHECK_STATE_CONTENT : 当前正在解析请求体
    */
    enum CHECK_STATE {
        CHECK_STATE_REQUESTLINE = 0,
        CHECK_STATE_HEADER,
        CHECK_STATE_CONTENT
    };

    //从状态机的三种可能状态，即行的读取状态，分别表示：
    //1,读取到一个完整的行；2，行出错；3，行数据尚且不完整
    enum LINE_STATE {
        LINE_STATE_OK = 0,
        LINE_STATE_BAD,
        LINE_STATE_OPEN
    };


    /*
        服务器处理HTTP请求的可能结果，报文解析的结果
        NO_REQUEST          : 请求不完整，需要继续读取客户数据
        GET_REQUEST         : 表示获得了一个完整的客户请求
        BAD_REQUEST         : 表示客户请求出现语法错误
        NO_RESOURCE         : 表示服务器没有资源
        FORBIDDEN_REQUEST   : 表示客户对资源没有足够的访问权限
        FILE_REQUEST        : 文件请求，获取文件成功
        INTERNAL_ERROR      : 表示服务器内部错误
        CLOSED_CONNCTION    : 表示客户端已经关闭连接了
    */

    enum HTTP_CODE { NO_REQUEST, GET_REQUEST, BAD_REQUEST, NO_RESOURCE, FORBIDDEN_REQUEST, FILE_REQUEST, INTERNAL_ERROR, CLOSED_CONNECTION };


    // 构造函数
    http_conn() {}
    // 析构函数
    ~http_conn() {}
    
    void process();// 处理客户端的请求（解析HTTP请求） + 响应 

    void init(int sockfd, const sockaddr_in & addr);//初始化新接受的连接

    void close_conn();//关闭连接

    bool read();//非阻塞读

    bool write();//非阻塞写

    HTTP_CODE process_read();//解析HTTP请求
    HTTP_CODE parse_request_line(char* text); //解析请求首行
    HTTP_CODE parse_headers(char* text); //解析请求头
    HTTP_CODE parse_content(char* text); //解析请求体

    LINE_STATE parse_line();
    //内联函数，获取一行数据
    char* get_line() {return m_read_buf + m_start_line; }
    HTTP_CODE do_request();


private:

    int m_sockfd; // HTTP连接的socket
    sockaddr_in m_address; //通信的socket地址
    
    char m_read_buf[READ_BUFFER_SIZE]; //读缓冲区
    int m_read_index; //标识读缓冲区中已经读取的客户端数据的最后一个字节的下一个位置
    char m_write_buf[WRITE_BUFFER_SIZE]; //写缓冲区

    int m_checked_index; //当前正在分析的字符在读缓冲区中的位置
    int m_start_line; //当前正在解析的行的起始位置

    char* m_url;        //请求目标文件的文件名
    char* m_version;    //协议版本，目前仅支持HTTP1.1
    METHOD m_method;    //请求方法，目前仅支持GET
    char* m_host;         //主机名
    bool m_linger;      //判断HTTP请求是否要保持链接
    int m_content_length; //HTTP请求的总长度
    CHECK_STATE m_check_state; // 主状态机当前所处的状态

    void init();//初始化连接其余的信息，即上面三个变量

};



#endif