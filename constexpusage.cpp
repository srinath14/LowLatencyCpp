#include <iostream>
template<int N>
struct fibo{
    static constexpr int value = fibo<N-1>::value+fibo<N-2>::value;
};
template<>
struct fibo<0>{
    static constexpr int value=0;
};
template<>
struct fibo<1>{
    static constexpr int value=1;
};


/*Write a constexpr Fibonacci that works at compile time.*/
int main(){

    constexpr int val = fibo<6>::value;

    std::cout<<"the value is "<<val<<std::endl;
    
//fibo(5)=fibo(4)+fibo(3);
    // print fibo upto 5;

    if constexpr (std::is_integral_v<decltype(val)>){
        std::cout<<"this is integer"<<std::endl;
    }else{
        std::cout<<"this is not integer"<<std::endl;
    }

    return 0;
}