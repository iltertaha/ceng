#include "Laptime.h"
#include <iostream>
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Laptime::Laptime(){
    this->laptime = 0;
    this->next = nullptr;
}

Laptime::Laptime(int laptime){
    this->laptime = laptime;
    this->next = nullptr;
}
Laptime::Laptime(const Laptime& rhs){
    this->laptime = rhs.laptime;
    this->next = rhs.next;
}

Laptime::~Laptime(){}

void Laptime::addLaptime(Laptime *next){
    this->next = next;
}

bool Laptime::operator<(const Laptime& rhs) const{
    if(this->laptime < rhs.laptime){
        return true;
    }
    else{
        return false;
    }
}

bool Laptime::operator>(const Laptime& rhs) const{
    if(this->laptime > rhs.laptime){
        return true;
    }
    else {
        return false;
    }
}
Laptime Laptime::operator[](const int lap) const{
    
    const Laptime *ptr= this; //check
    
    while(ptr){
        if(ptr->laptime == lap){
            return *ptr;
        }
        ptr=ptr->next;
    }
    return Laptime(0);

}
Laptime& Laptime::operator+(const Laptime& rhs){
    
    this->laptime = this->laptime + rhs.laptime;
    return *this;
}
std::ostream& operator<<(std::ostream& os, const Laptime& laptime){

    int total=laptime.laptime;
    
    int ms=total;
    int min=ms/60000;
    ms=ms-min*60000;
    int sec=ms/1000;
    ms=ms-sec*1000;
    total=ms;
    /* os << min <<":" << sec << "." << total;
    return os; */
    if(total<10){
        if(sec<10){
         os << min <<":" << 0<< sec << "." <<00<< total;
         return os;
        }
        os << min <<":" << sec << "." <<00<< total;
        return os;
    }
    else if(total<100){
        if(sec<10){
         os << min <<":" << 0<< sec << "." <<0<< total;
         return os;
        }

        os << min <<":" << sec << "." <<0<< total;
        return os;
    }
    else{
        if(sec<10){
         os << min <<":" << 0<< sec << "." << total;
         return os;
        }

        os << min <<":" << sec << "." << total;
        return os;
    }

    
    

}

Laptime* Laptime::getNext() const {return next;}
int Laptime::getLaptime() const {return laptime;}
void Laptime::setLaptime(int val) {laptime = val;}
void Laptime::setNext(Laptime *newNext) {next = newNext;}
Laptime& Laptime::operator=(const Laptime& lapt){
    laptime=lapt.laptime;
    next=lapt.next;

    return *this;
}



/* 
 int main(){
    using namespace std;
    Laptime A(60000);
    std::cout << A;

}  */