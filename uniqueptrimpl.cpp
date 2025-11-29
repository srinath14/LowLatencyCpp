#include <iostream>

template<typename T>
class uniqueptr{
    private:
    T* res;
    public:
    uniqueptr(T* ptr=nullptr):res(ptr){

    }

    uniqueptr(const uniqueptr& ptr)=delete;//copy constructor
    uniqueptr& operator=(const uniqueptr& ptr)=delete;//copy assignment operator

    uniqueptr& operator=(uniqueptr&& ptr){//move constructor
        if(this!=&ptr){
            delete this->res;
            this->res = ptr.res;
            ptr.res = nullptr;
        }
        return *this;
    }

    uniqueptr(uniqueptr&& ptr){
        this->res = ptr.res;
        ptr.res = nullptr;

    }

    T& operator*(){
        return *res;
    }
};
int main(){

   



    
    return 0;
}