#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <fcntl.h>
#include <sys/epoll.h>
#include <future>
using namespace std;
/*
create socket
bind to socket to the IP
listen
accept
*/

void chatWithClient(int client_fd);


class Threadpool
{
    queue<int> threadpoolQueue;
    mutex mtx;
    condition_variable cv;
    vector<thread> threads;
    int size;
    bool stop;

public:
    Threadpool(int size) : size(size)
    {
        stop = false;
        for (int i = 0; i < size; i++)
        {
            threads.emplace_back([this]
                                 {
                while(true){
                    int fd;
                {
                    unique_lock<mutex>lock(mtx);
                    cv.wait(lock,[this]{return !threadpoolQueue.empty()|| stop;});
                    if(stop && threadpoolQueue.empty())return ;
                    fd = threadpoolQueue.front();
                    threadpoolQueue.pop();
                }
                    chatWithClient(fd);
                } });
        }
    }
    void enqueue(int id)
    {
        {
            lock_guard<mutex> lock(mtx);
            threadpoolQueue.push(id);
        }
        cv.notify_one();
    }
    ~Threadpool()
    {
        {
            lock_guard<mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all();
        for (int i = 0; i < size; i++)
        {
            threads[i].join();
        }
    }
};
Threadpool t(8);
void epollEventThread(int epfd)
{
    struct epoll_event events[64];

    while (true)
    {
        int n = epoll_wait(epfd, events, 64, -1);

        for (int i = 0; i < n; i++)
        {
            int fd = events[i].data.fd;
            if (events[i].events & EPOLLIN)
            {
                t.enqueue(fd);
            }
        }
    }
}


void chatWithClient(int client_fd)
{
    char buffer[1024];
    while (true)
    {

        int bytes_read = read(client_fd, buffer, 1023);
        if (bytes_read > 0)
        {
            buffer[bytes_read] = '\0'; // terminate properly
            cout << "Client: " << buffer << endl;
            const char *msg = "hello from server";
            write(client_fd, msg, strlen(msg));
        }
        else if (bytes_read = 0)
        {
            cout << "Client disconnected\n";
            close(client_fd);
            return; // return so thread can handle next client
        }
        else
        { // bytes_read<0
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                return;
            }
            else
            {
                cout << "read err from client" << endl;
                close(client_fd);
                return;
            }
        }
    }
}
int main()
{
    int epfd = epoll_create1(0);
    promise<int>p;
    auto fut = p.get_future();

    thread epollthread(epollEventThread, epfd);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    cout << sockfd << endl;
    sockaddr_in s, c;
    memset(&s, 0, sizeof(s));
    memset(&c, 0, sizeof(c));

    s.sin_family = AF_INET;
    s.sin_addr.s_addr = INADDR_ANY;
    s.sin_port = htons(8008);
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (bind(sockfd, (sockaddr *)&s, sizeof(s)) != 0)
    {
        cout << "socket bind is failed" << endl;
    }
    if (listen(sockfd, 5) != 0)
    {
        cout << "socket is not listening" << endl;
    }

    while (true)
    {
        socklen_t len = sizeof(c);
        int client_fd = accept(sockfd, (sockaddr *)&c, &len);

        if (client_fd < 0)
        {
            cout << "accept is failed" << endl;
            close(sockfd);
        }
        int flags = fcntl(client_fd, F_GETFL, 0);
        // thread t(chatWithClient, client_fd);
        // t.detach();
        // threadpoolQueue.push(client_fd);
        fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

        struct epoll_event ev;
        ev.data.fd = client_fd;
        ev.events = EPOLLIN | EPOLLET;
        epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);
        // t.enqueue(client_fd);

        // chatWithClient(client_fd);

        // close(client_fd);
    }

    return 0;
}