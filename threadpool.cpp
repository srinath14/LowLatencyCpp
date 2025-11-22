#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>
#include <chrono>
#include <vector>
#include <future>
#include <utility>
#include <memory>
using namespace std;

class Threadpool
{
    queue<function<void()>> threadpoolQueue;
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
                    function<void()> fd;
                {
                    unique_lock<mutex>lock(mtx);
                    cv.wait(lock,[this]{return !threadpoolQueue.empty()|| stop;});
                    if(stop && threadpoolQueue.empty())return ;
                    fd = move(threadpoolQueue.front());
                    threadpoolQueue.pop();
                }
                    fd();
                } });
        }
    }
    template<typename F, typename... Args>

    auto enqueue(F&& f, Args&&... args)->future<decltype(f(args...))>
    {
        using return_type = decltype(f(args...));
        auto task = make_shared<packaged_task<return_type()>>(
            bind(forward<F>(f), forward<Args>(args)...)
        );
        auto fut = task->get_future();

        {
            lock_guard<mutex> lock(mtx);
            cout<<"size of the queue"<<threadpoolQueue.size()<<endl;
            threadpoolQueue.push([task]{
                (*task)();
            });
        }
        cv.notify_one();
        return fut;
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

void func()
{
    this_thread::sleep_for(chrono::milliseconds(2));

    cout << "this is some time taking function" << endl;
}

int func1(int a, int b){
    return a+b;
}



int main()
{

    Threadpool pool(8);

    while (true)
    {
        auto fut=pool.enqueue(func1, 2,3);
        decltype(2) c=fut.get();
        cout<<"vaue"<<c<<endl;

    }

    return 0;
}