#include <iostream>

template<typename Derived>
class PayementInterface{

    public:
        void pay(){
            std::cout<<"payement is done by Base"<<std::endl;
            static_cast<Derived*>(this)->pay();
        }

};


class UpiPayment:public PayementInterface<UpiPayment>{

    public:
    void pay(){
        std::cout<<"payement is done by UPI"<<std::endl;

    }
};

class CashPayment:public PayementInterface<CashPayment>{

    public:
    void pay(){
        std::cout<<"payement is done by Cash"<<std::endl;

    }
};
template<typename T>
void processPayement(PayementInterface<T> *p){
    p->pay();
}

int main(){

    UpiPayment *upi= new UpiPayment();

   

    processPayement(upi);

    return 0;
}