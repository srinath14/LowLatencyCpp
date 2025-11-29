#include <iostream>
#include <future>


int add(int a, Args...&& args){


}

int main(){

    std::packaged_task<int>task(bind(add, 1,2,3));
    future<int>fut =task.get_future();
    cout<<fut.get()<<endl;
    return 0;
}