#include "Car.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

#include "Utilizer.h"
#include <iostream>
Car::Car(std::string driver_name){
    this->driver_name = driver_name;
    this->performance = Utilizer::generatePerformance();
    this->head = nullptr;
    this->next = nullptr;

}
Car::Car(const Car& rhs){
    driver_name = rhs.driver_name;
    head = rhs.head;
    next = rhs.next;
}
Car::~Car(){
     while(head)
    {
        Laptime *temp = head;
        head = temp->getNext();
        delete temp;
    } 
}
std::string Car::getDriverName() const{return driver_name;}
double Car::getPerformance() const{return performance;}
void Car::setPerformance(int val){performance=val;}
void Car::addCar(Car *next){this->next = next;}
bool Car::operator<(const Car& rhs) const{
    int total_lap_of_this=0;
    int total_lap_of_rhs =0;
    Laptime* ptr = head;
    while(ptr){
        total_lap_of_this += ptr->getLaptime();
        ptr=ptr->getNext();}

    Laptime* ptr2 = rhs.head;
    while(ptr2){
        total_lap_of_rhs += ptr2->getLaptime();
        ptr2=ptr2->getNext();}

    if(total_lap_of_this < total_lap_of_rhs){
        return true;
    }
    else {return false;}
}

bool Car::operator>(const Car& rhs) const{
    int total_lap_of_this=0;
    int total_lap_of_rhs =0;
    Laptime* ptr = head;
    while(ptr){
        total_lap_of_this += ptr->getLaptime();
        ptr=ptr->getNext();}

    Laptime* ptr2 = rhs.head;
    while(ptr2){
        total_lap_of_rhs += ptr2->getLaptime();
        ptr2=ptr2->getNext();}

    if(total_lap_of_this > total_lap_of_rhs){
        return true;
    }
    else {return false;}
}

Laptime Car::operator[](const int lap) const{
    return this->head->operator[](lap);
}

void Car::Lap(const Laptime& average_laptime){
    Laptime* ptr = head;
    if(head==nullptr){

    
        Laptime* yeni =new Laptime(average_laptime.getLaptime() +  Utilizer::generateLaptimeVariance(performance));

        head=yeni;
    }
    else{
        while(ptr->getNext()!=nullptr){
        ptr=ptr->getNext();
    }
    
    Laptime* yeni =new Laptime(average_laptime.getLaptime() + Utilizer::generateLaptimeVariance(performance));
    ptr->setNext(yeni);

    }
    }
    

    Laptime Car::getLatest() const{
        Laptime* ptr = head;
        while(ptr->getNext()){
            ptr=ptr->getNext();
        }
        return *ptr;
    }
    Laptime Car::getFastest() const{
        Laptime min= *head;
        Laptime* ptr = head;
        while(ptr->getNext()){
            if(ptr->operator<(min)){
                min=*ptr;
            }

            ptr=ptr->getNext();
        }

    return min; 
    }
    Laptime Car::getTotal() const{
    int total_lap=0;
    Laptime* ptr = head;
    while(ptr){
        total_lap += ptr->getLaptime();
        ptr=ptr->getNext();}
        
        Laptime yeni(total_lap);
        return  yeni;

    }


std::ostream& operator<<(std::ostream& os, const Car& car){

    os << car.kisalt(car) << "--" << car.getLatest() << "--" <<car.getFastest() << "--" << car.getTotal();
     

    return os;
}

Car* Car::getNext() const {return next;}
void Car::setNext(Car *newNext) {next = newNext;}
void Car::removeone(Laptime* prevNode){
    if (prevNode) {  // if p is not NULL ? keeps the address of the prev.   
    Laptime* tmp = prevNode->getNext();  // address of the node to delete
    prevNode->setNext(tmp->getNext()); // pâ€™s next will be next of to-be-deleted
    delete tmp;
}
}

void Car::removeafter(Laptime* ptr){
     while(ptr->getNext() != NULL)    // while list is not empty
        removeone(ptr); 
}


int main(){
	 
    
    Car ilter("ilter");
    Car taha("taha");
    Car aktolga("aktolga");
    Laptime l1(1000);
    Laptime l2(1000);
    Laptime l3(1000);
    ilter.Lap(l1);ilter.Lap(l2);ilter.Lap(l3);
    taha.Lap(l1);taha.Lap(l2);taha.Lap(l3);
    aktolga.Lap(l1);aktolga.Lap(l2);aktolga.Lap(l3);
    std::cout<<ilter;

	
}

Laptime* Car::getLapHead() const {return head;}
void Car::setLapHead(Laptime* lapp) {head=lapp;}


std::string Car::kisalt(const Car& car)const{
    std::string name=car.getDriverName();
    std::vector<std::string> name_vector;
    std::string k;
    for (char t : name)
    {		
	if (t==' '&&k.length() > 1)
	{
		name_vector.push_back(k);
		k="";
	}
	else if (t!=' ')
		k += t;
    }
    if (k!="")
    name_vector.push_back(k);
    std::string lastname=name_vector.back();
  

    
  char arr[3] ={(char)toupper(lastname[0]),(char)toupper(lastname[1]),(char)toupper(lastname[2])};

    std::string initials(arr);
    return initials;
}

   
 /* g++ -Wall --pedantic-errors -std=c++11 Car.cpp Laptime.cpp */
