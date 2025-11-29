#include <iostream>

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


};

int main(){

    Uptr a(new int(10));

    


    cout<<"the vaue of a "<<*a<<endl;
    ///*a<
    //

    
    

    //int* x = new int();
    //delete x;
    return 0;
}