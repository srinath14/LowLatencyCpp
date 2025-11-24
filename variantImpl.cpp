#include <iostream>
#include <variant>
#include <vector>
using namespace std;
template<typename... Types>

class VariantImpl{

    public:
    VariantImpl(Types... types):vec{types...}{


    }

};



int main(){

    //variant<int, string>var;

    //var=10;
    //cout<<get<int>(var)<<endl;

    VariantImpl<int, string>var;

    return 0;
}