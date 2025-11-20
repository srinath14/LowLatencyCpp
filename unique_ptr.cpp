#include <iostream>
#include<memory>

using namespace std;

enum class vehicle{
    BIKE,
    CAR
};

class Vehicle {
    public:
        virtual void getName() = 0;
        // CRITICAL FIX: Add a virtual destructor
        virtual ~Vehicle()=default; // Or add a print statement here if you like
    };
    

class Bike:public Vehicle{
    public:
    Bike(){
        cout<<"Bike is created"<<endl;
    }
    void getName(){
        cout<<"this is a bike"<<endl;
    }
    ~Bike(){
        cout<<"desturctor called for bike"<<endl;
    }

};
class Car:public Vehicle{
    public:
    Car(){
        cout<<"Car is created"<<endl;
    }
    void getName(){
        cout<<"this is a Car"<<endl;
    }
};
class VehicleFactory{
    public:
    /*unique_ptr<Vehicle>*/Vehicle* createVehile(vehicle v){
        switch(v){
            case vehicle::BIKE:
            {
                return new Bike();//make_unique<Bike>();
            }
            case vehicle::CAR:
            {
                return new Car();//make_unique<Car>();
            }
        }
        return nullptr;
    }
    ~VehicleFactory(){
        cout<<"desturctor called for vehcilefactory class"<<endl;
    }

};

int main(){

        VehicleFactory f;
       //unique_ptr<Vehicle>v = f.createVehile(vehicle::BIKE);
       Vehicle* v = f.createVehile(vehicle::BIKE);
        delete v;
    //nique_ptr<Vehicle>p=
    //stream ss;
    return 0;
}