/*
1. Problem Statement

You have a shared resource (like a database or a file).

There are readers and writers:

Type	Behavior
Reader	Only reads the data; multiple readers can read simultaneously.
Writer	Writes/modifies the data; must have exclusive access.

Goal:

Allow multiple readers at the same time.

Ensure writers have exclusive access.

Avoid deadlocks and starvation (no thread waits forever).

*/
#include <iostream>
#include <shared_mutex>
#include <thread>
#include <queue>
#include <condition_variable>
#include <future>
using namespace std;
mutex mtx;
queue<int>q;
condition_variable cv;


void write(int a){
    {
        lock_guard<mutex>lock(mtx);
        q.push(a);
        cout<<"writing to thread"<<endl;
    }
    cv.notify_one();
    
}

int read()
{
    
        unique_lock<mutex>lock(mtx);
        cv.wait(lock, []{
            return !q.empty();
        });
        auto fut = q.front();
        q.pop();
        return fut;
    

}


int main(){
    

    thread writer(write, 10);
    auto fut = async(read);
    int ans = fut.get();
    cout<<"answer is "<<ans<<endl;
    writer.join();
    
    return 0;
}