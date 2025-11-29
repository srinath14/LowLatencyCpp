#include <iostream>
#include <variant>
#include <vector>
#include <string>
union U{
    int a;
    char c;
};


int main(){

    //variant<int, string>var;

    //var=10;
    //cout<<get<int>(var)<<endl;

    //ariantImpl<int, string>var;
    std::variant<int, std::string, float>var;
    var= "srinath";
    var=1.00f;

    //U x;
    //x.a=10;

    //std::cout<<"hello "<<x.c<<std::endl;

    //td::cout<< std::get_if<std::string>(&var) <<std::endl;

    //std::cout<< std::get<std::string>(var)<< std::endl;


    try{
        std::cout<< std::get<int>(var)<< std::endl;
    }catch(const std::bad_variant_access &e){
        std::cout <<"error"<<e.what()<<std::endl;
    }
    std::cout<<var.index()<<std::endl;

    return 0;
}