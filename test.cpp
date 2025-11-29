#include <iostream>
#include <thread>

using namespace std;




class Uptr{
    int* res;

    public:
    Uptr(int* ptr){
        res = ptr;
    }

    ~Uptr(){
        cout<<"destructor is called for UPtr and deleteing res"<<endl;
        delete res;
    }
    
    int& operator*(){
        return *res;
    }

    friend ostream& operator<<(ostream &s, const Uptr& a){
        return s<<*(a.res)<<"hello";
    
    }
   
};

#include <iostream>
#include <cstdlib>
using namespace std;

void* operator new(size_t size) {
    cout << "Custom new, allocating " << size << " bytes\n";
    void* ptr = malloc(size);
    if (!ptr) throw bad_alloc();
    return ptr;
}

class Test {
    int x;
    double y;
};






int main(){

    thread t([&]{
        Uptr a(new int(10));
        Uptr b(new int(20));
        //cout<<a<<b<<endl;
    });

    //int num_cores=thread::hardware_concurrency();

    //cout<<num_cores<<endl;

    t.join();

    Test* t1 = new Test();   // triggers custom operator new

    
    delete t1;



    //cout<<"the vaue of a "<<*a<<endl;
    ///*a<
    //

    
    

    //int* x = new int();
    //delete x;
    return 0;
}