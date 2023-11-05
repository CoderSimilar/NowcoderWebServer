#include "http_conn.h"

// 定义HTTP响应的一些状态信息
const char* ok_200_title = "OK";
const char* error_400_title = "Bad Request";
const char* error_400_form = "Your request has bad syntax or is inherently impossible to satisfy.\n";
const char* error_403_title = "Forbidden";
const char* error_403_form = "You do not have permission to get file fron this server.\n";
const char* error_404_title = "Not Found";
const char* error_404_form = "The request file was not found on this server.\n";
const char* error_500_title = "Internal Error";
const char* error_500_form = "There was an unusual problem serving the request file.\n";

// 网站的根目录
const char* doc_root = "/home/admin/cppCode/WebServer/resources";

int http_conn :: m_epollfd = -1; //所有的socket上的事件都被注册到同一个epoll对象中
int http_conn :: m_user_count = 0; //统计用户的数量

//设置文件描述符非阻塞
void setnonblocking(int fd) {
    int old_flag = fcntl(fd, F_GETFL);
    int new_flag = old_flag | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_flag);
}

// 向epoll中添加需要监听的文件描述符
void addfd(int epollfd, int fd, bool one_shot) {

    epoll_event event;  // 定义epoll事件
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLRDHUP; // 使用水平触发模式，也可以设置成既可以水平触发，也可以边缘触发

    if(one_shot) {
        // 防止同一个通信被不同线程处理
        event.events |= EPOLLONESHOT;
    }
    // 通过epoll_ctl添加事件
    int ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl")
    //设置文件描述符非阻塞
    setnonblocking(fd);

}

//从epoll中移除监听的文件描述符
void removefd(int epollfd, int fd) {

    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);

    close(fd);

}

// 修改文件描述符，重置socket上的EPOLLONESHOT事件，
// 以确保下一次可读时，EPOLLIN事件被触发
void modfd(int epollfd, int fd, int ev) {
    epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLONESHOT | EPOLLRDHUP;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

//初始化连接，外部调用初始化套接字地址
void http_conn::init(int sockfd, const sockaddr_in & addr){
    
    m_sockfd = sockfd;
    m_address = addr;

    //设置端口复用
    int reuse = 1;
    setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    //添加到epoll对象中
    addfd(m_epollfd, m_sockfd, true);
    m_user_count++;//总用户数++

    //初始化连接的其他信息，有时候初始化链接其他信息的时候不需要传递参数以及让用户数+1，所以封装成另外一个函数让此函数调用
    init();

}

//初始化连接的其他信息
void http_conn::init() {
    
    this->m_check_state = CHECK_STATE_REQUESTLINE;  // 主状态机初始化状态为解析请求首行
    this->m_checked_index = 0;  // 当前正在分析的字符在读缓冲区中的位置
    this->m_start_line = 0; // 当前正在解析的行的起始位置
    this->m_read_index = 0;
    this->m_method = GET;   // 默认请求方式为GET
    this->m_url = 0;
    this->m_version = 0;
    this->m_linger = false; // 默认不保持连接 Connection ： keep-alive保持连接

    bzero(m_read_buf, READ_BUFFER_SIZE);
    bzero(m_write_buf, READ_BUFFER_SIZE);
    // bzero(m_real_file, FILENAME_LEN);

}

//关闭连接
void http_conn::close_conn() {
    if(m_sockfd != -1) {
        removefd(m_epollfd, m_sockfd);
        m_sockfd = -1;
        m_user_count--;//关闭一个连接，客户总数-1
    }
}

//读取接收到的数据
bool http_conn::read() {
    //如果读缓冲区已经满了
    if(m_read_index >= READ_BUFFER_SIZE) {
        return false;
    }
    //读取到的字节
    int bytes_read = 0;
    while(true) {
        // 从my_read_buf + m_read_idx索引处开始保存数据，大小是READ_BUFFER_SIZE - m_read_idx
        bytes_read = recv(m_sockfd, m_read_buf + m_read_index, 
        READ_BUFFER_SIZE - m_read_index, 0);
        //如果出错
        if(bytes_read < 0) {
            if(errno == EAGAIN || errno == EWOULDBLOCK) {
                //没有数据
                break;
            }
            return false;
        }else if(bytes_read == 0) {
            //对方断开连接
            return false;
        }
        //更新读取字节的下标
        m_read_index += bytes_read;

    }
    printf("read data: %s\n", m_read_buf);

    return true;
}

bool http_conn::write() {
    printf("一次性写完数据\n");
    return true;
}

//解析HTTP请求

//主状态机，从大的范围解析请求
//先执行process_read
http_conn:: HTTP_CODE http_conn::process_read() {
    //定义初始状态
    http_conn::LINE_STATE line_status = http_conn::LINE_STATE_OK;
    http_conn::HTTP_CODE ret = http_conn::NO_REQUEST;

    //要获取的一行数据
    char *text = NULL;
    //一行一行地解析数据，parse_line表示获取状态，如果是LINE_STATE_OK表示状态正常
    //while循环一行一行地解析请求
    while(( ( (http_conn::m_check_state == http_conn::CHECK_STATE_CONTENT) && (line_status == http_conn::LINE_STATE_OK) )
             || (line_status = http_conn::parse_line()) == http_conn::LINE_STATE_OK)) {
            //解析到了一行完整的数据，或者解析到了请求体，也是完整的数据
                
            //获取一行数据
            text = get_line();

            m_start_line += m_checked_index;
            printf("got 1 http line : %s\n", text);

            switch(m_check_state) {
                //解析请求行
                case CHECK_STATE_REQUESTLINE :
                {
                    ret = parse_request_line(text);
                    if(ret == BAD_REQUEST) {
                        return BAD_REQUEST;
                    }
                    break;
                }
                //解析请求头部
                case CHECK_STATE_HEADER :
                {
                    ret = parse_headers(text);
                    if(ret == BAD_REQUEST) {
                        return BAD_REQUEST;
                    }else if(ret == GET_REQUEST) {
                        return do_request();//解析具体的请求信息
                    }
                    
                }
                //解析请求体
                case CHECK_STATE_CONTENT :
                {
                    ret = parse_content(text);
                    if(ret == BAD_REQUEST) {
                        return BAD_REQUEST;
                    }else if(ret == GET_REQUEST) {
                        return do_request();
                    }
                    line_status = LINE_STATE_OPEN;
                    break;

                }
                //其他情况
                default :
                {
                    return INTERNAL_ERROR;
                }
            }
            return NO_REQUEST;//请求信息不完整            
    }

    return http_conn::NO_REQUEST;
}

// 当得到一个完整、正确的HTTP请求的时候，我们需要分析目标文件的属性
// 如果目标文件存在，对所有用户可读，且不是目录，则使用mmap将其映射到内存地址m_file_address处，并告诉调用者获取文件成功
http_conn::HTTP_CODE http_conn::do_request() {

    // "/home/nowcoder/webserver/resources"
    strcpy( m_real_file, doc_root );
    int len = strlen(doc_root);
    strncpy( m_real_file + len, m_url, FILENAME_LEN - len - 1 );
    // 获取m_real_life文件的相关的状态信息 -1 ： 失败， 0 ： 成功
    if( stat( m_real_file, &m_file_stat ) < 0 ) {
        return NO_RESOURCE;
    }

    // 判断访问权限
    if( !(m_file_stat.st.mode & S_IROTH) ) {
        return FORBIDDEN_REQUEST;
    }

    // 判断是否是目录
    if( S_ISDIR(m_file_stat.st_mode) ) {
        return BAD_REQUEST;
    }

    // 以只读的方式打开文件
    int fd = open( m_real_file, O_RDONLY );
    ERROR_CHECK(fd, -1, "open");
    // 将文件映射到内存地址m_file_address处
    

    return NO_REQUEST; 

}


//解析HTTP请求行，获取请求方法，目标URL，HTTP版本
http_conn::HTTP_CODE http_conn::parse_request_line(char* text) {

    // GET /index.html HTTP/1.1
    m_url = strpbrk(text, " \t");

    // GET\0/index.html HTTP/1.1
    *m_url ++= '\0';

    char* method = text;
    if(strcasecmp(method, "GET") == 0) {
        m_method = GET;
    }else {
        return BAD_REQUEST;
    }
    // /index.html HTTP/1.1
    m_version = strpbrk(m_url, " \t");
    if(!m_version) {
        return BAD_REQUEST;
    }
    *m_version ++= '\0';

    if(strcasecmp(m_version, "HTTP/1.1") != 0) {
        return BAD_REQUEST;
    }

    if(strncasecmp(m_url, "http://", 7) == 0) {
        m_url += 7;                     //192.168.1.1:10000/index.html
        m_url = strchr(m_url, '/');     // /index/html
    }

    if(!m_url || m_url[0] != '/') {
        return BAD_REQUEST;
    }

    m_check_state = CHECK_STATE_HEADER; //主状态机检查状态变成检查请求头

    return http_conn::NO_REQUEST;
} 

//解析请求头
http_conn::HTTP_CODE http_conn::parse_headers(char* text) {
    
    //遇到空行，表示头部字段解析完毕
    if(text[0] == '\0') {
        //如果HTTP请求有消息体，则还需读取m_content_length字节的消息体
        //状态机转移到CHECK_STATE_CONTENT状态
        if(m_content_length > 0) {
            m_check_state = CHECK_STATE_CONTENT;
            return NO_REQUEST;
        }
        //否则说明我们已经读取完整的HTTP请求
        return GET_REQUEST;
    }else if(strncasecmp(text, "Connection:", 11) == 0) {
        // 处理Connection头部字段 Connection: keep-alive
        text += 11;
        text += strspn(text, " \t");
        if(strcasecmp(text, "keep-alive") == 0) {
            m_linger = true;
        }
    }else if(strncasecmp(text, "Content-Length:", 15) == 0) {
        //处理Content-Length头部字段
        text += 15;
        text += strspn(text, " \t");
        m_content_length = atol(text);
    }else if(strncasecmp(text, "Host:", 5) == 0) {
        //处理Host头部字段
        text += 5;
        text += strspn(text, " \t");
        m_host = text;
    }else {
        printf("oop ! unknown header : %s\n", text);
    }
    
    return http_conn::NO_REQUEST;
} 

//解析请求体
http_conn::HTTP_CODE http_conn:: parse_content(char* text) {
    //并没有真正地解析请求体，只是判断它是否被完整读取了
    if(m_read_index >= (m_content_length + m_checked_index)) {
        text[m_content_length] = '\0';
        return GET_REQUEST;
    }
    return http_conn::NO_REQUEST;
}

//解析一行的数据，判断依据是请求的每一行结尾都会有/r/n
http_conn:: LINE_STATE http_conn::parse_line() {

    char temp;

    for( ; m_checked_index < m_read_index; ++m_checked_index) {
        temp = m_read_buf[m_checked_index];
        if(temp == '\r') {
            if(m_checked_index + 1 == m_read_index) {
                return LINE_STATE_OPEN;//没有读取到完整
            }else if(m_read_buf[m_checked_index + 1] == '\n') {
                m_read_buf[m_checked_index++] = '\0';
                m_read_buf[m_checked_index++] = '\0';
                return LINE_STATE_OK;
            }
            return LINE_STATE_BAD;
        }else if(temp == '\n') {
            if(m_checked_index > 1 && m_read_buf[m_checked_index - 1] == '\r') {
                m_read_buf[m_checked_index - 1] = '\0';
                m_read_buf[m_checked_index ++] = '\0';
                return LINE_STATE_OK;
            }
            return LINE_STATE_BAD;
        }

        return LINE_STATE_OPEN;

    }



    return http_conn:: LINE_STATE_OK;
}



//由线程池中的工作线程调用，这是处理HTTP请求的入口函数
void http_conn:: process() {
    
    //解析http请求
    HTTP_CODE read_ret = process_read();
    if( read_ret == NO_REQUEST ) {
        //请求不完整
        modfd(m_epollfd, m_sockfd, EPOLLIN);
        return;
    }

    

    //生成响应



}
