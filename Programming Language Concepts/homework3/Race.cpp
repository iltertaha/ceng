#include "Race.h"
#include "Utilizer.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Race::Race(std::string race_name):race_name(race_name),head(nullptr),average_laptime(Utilizer::generateAverageLaptime()){
}

Race::Race(const Race& rhs){
    Car* ptr1=this->head;
    Car* ptr2=rhs.head;
    while (ptr2) {
       Car* newCar=new Car(*ptr2);
       newCar->setLapHead(nullptr);
       
       ptr1->addCar(ptr2);
       ptr1 =ptr1->getNext();             
       ptr2 =ptr2->getNext();             
    }
  }

    



Race::~Race(){}

std::string Race::getRaceName() const{return this->race_name;}

void Race::addCartoRace(){
    Car* ptr = head;
    if(!ptr){
        head = new Car("random");
    }
    else{
    Car* newCar = new Car("random");
    while(ptr->getNext()){
    

        ptr=ptr->getNext();
    }
    ptr->setNext(newCar); // added end of the list
    }
}



void Race::addCartoRace(Car& car){
    Car* ptr = head;
    if(!ptr){
        head = new Car(car);
    }

    else{
        Car* newCar = new Car(car);
        while(ptr->getNext()){
            ptr=ptr->getNext();
        }
        ptr->setNext(newCar);
    }

}


int Race::getNumberOfCarsinRace(){
    Car* ptr = head;
    int howmuch= 0;
    while(ptr){
        howmuch+=1;
        ptr=ptr->getNext();
    }
    return howmuch;
}
void Race::goBacktoLap(int lap){
    Car* arabagez= this->head;
    while(arabagez){
        Laptime* select=arabagez->getLapHead();
        int i =0;
        while(select){
            if(i==lap){
                break;
            }
            select=select->getNext();
        }



        arabagez->removeafter(select);
        arabagez=arabagez->getNext();
    }
    sortall();
}
void Race::operator++(){
Car* arabagez = this->head;
while(arabagez){
    arabagez->Lap(average_laptime);
    arabagez=arabagez->getNext();
}
sortall();
}
void Race::operator--(){
    Car* arabagez= this->head;
    while(arabagez){
        Laptime* ptr = arabagez->getLapHead();

  
        if (ptr!=nullptr && ptr->getNext() == NULL) 
        { 
        delete ptr;
        head=nullptr; 
        }
        else{
  
        // Find the second last node 
        Laptime* second_last = arabagez->getLapHead(); 
        while (second_last->getNext()->getNext() != NULL) 
            second_last = second_last->getNext(); 
    
        // Delete last node 
        delete (second_last->getNext()); 
    
        // Change next of second last 
        second_last->setNext(nullptr);


        }

        arabagez=arabagez->getNext();
        }

        
      sortall();  
    }

Car Race::operator[](const int car_in_position){
    Car* ptr = this->head;
    while(ptr){
        int i=1;
        if(i==car_in_position){
            break;
        }
        ptr=ptr->getNext();
    }
    return *ptr;
}

Car Race::operator[](std::string driver_name){
    Car* ptr = this->head;
    while(ptr){
        int i=1;
        if(ptr->getDriverName()==driver_name){
            break;
        }
        ptr=ptr->getNext();
    }
    return *ptr;
}

Race& Race::operator=(const Race& rhs){
    if (this != &rhs) {
        Car* ptr= head;
        while(ptr!= NULL)
        {
                if(ptr == NULL)
                        break;
                else
                {
                        Car* temp = ptr;
                        ptr = ptr->getNext();
                        delete temp; //car destructor leaksiz olmali
                }
        }
        this->head=nullptr;
        
    this->average_laptime=rhs.average_laptime;
    this->race_name = rhs.race_name;
    Car* r = rhs.head; // r = the zeroth of rhs
    Car* p = this->head;    
  
   while (r) {         
       this->addCartoRace(*head); //implement et
       r = r->getNext();              // move r forward
       p = p->getNext();              // move p forward
    }
 

     return *this;   
  }
  return *this;     // return lhs for casca


}
std::ostream& operator<<(std::ostream& os, const Race& race){
Car* ptr = race.head;
int i =1;

while(ptr){
    os << race.kacinci(ptr) << "-"<< ptr->kisalt(*ptr) << "--" << race.rankingpoint(ptr) <<"--"<< race.fastestoflap(ptr) << std::endl;
    ptr=ptr->getNext();
}

}

void Race::setAverageLapTime(int val){average_laptime = val;}




void Race::swap(int index1, int index2){
  
  int pos=-1;
  Car* prev1=NULL;
  Car* prev2=NULL;
  Car* node1=NULL;
  Car* node2=NULL;
  Car* p=head;
  Car* temp=NULL;

  Car* sayac=head;
  int length=0;
  while(sayac){
      length++;
      sayac=sayac->getNext();}
/***********************************************/
  if(index1<length && index2<length){
  
  while(p){
 
    
    if(pos==index1-1)
    {
      prev1=p;
      break;
    }
    p=p->getNext();
    pos++;
  }

  if(prev1->getNext()){
    node1=prev1->getNext();
  }
  /******************************************/
  p=head;
  int pos2=-1;
  while(p){
    
   
    if(pos2==index2-1)
    {
      prev2=p;
      break;
    }
     p=p->getNext();
    pos2++;
  }
  
  if(prev2->getNext()){
    node2=prev2->getNext();
  }

  prev1->setNext(node2);
  prev2->setNext(node1);

  temp= node1->getNext();
  node1->setNext(node2->getNext());
  node2->setNext(temp);


  }
}


void Race::sortall(){
    /* LinkedList<Video*>*kullanici=users.findNode(userName)->getDataPtr()->getSubscriptions();
    */
   Car* gez=head;
	 


    Car* ptr;
    Car* ptr2;
    Car* cur;
    Car* sonraki;
    
    ptr=head;

    
    Car* counter;
    int boyut=0;
    
    Car* min;
	Car* p;
	Car* q;
    
    
int say=0;
while(gez){
    gez=gez->getNext();
    say++;
    }
    
/* int n=kullanici->getLength(); */
    int n=say;

        

bool sorted = false ;
for (int i = 0; (i < n-1) && !sorted; i++) {
    sorted = true;
    ptr=head;
    for (int j=1; j <= n-i-1; j++){
        if (ptr->getTotal() < ptr->getNext()->getTotal()) { //
            
            swap(j-1, j); // Swap these two  
            
            sorted = false ; // Mark exchange
            }
        else{
            ptr=ptr->getNext();
        }
        }

    }
}



int Race::fastestoflap(Car * car)const{
    Car * arac = car;
    Car * arabagez = head;
    int birincibitirdi=0;
    Laptime min;
    int i=0;
    int j=0;
    int lapCount=0;
    Laptime* say = head->getLapHead();
    while(say){
        lapCount++;
        say=say->getNext();}

    while(j<lapCount){
        arabagez=head;
        min=*(arabagez->getLapHead());
        
        while(arabagez){
            if(arabagez->operator[](j+1).getLaptime()< min.getLaptime()){
                min.setLaptime(arabagez->operator[](j+1).getLaptime());
            }
            arabagez=arabagez->getNext();
        }

        if(car->operator[](j+1).getLaptime() == min.getLaptime()){
            birincibitirdi++;
        }
        j++;
    }
        return birincibitirdi;

}


int Race::rankingpoint(Car * car)const{
    
    int i=0;
    Car* ptr =head;
    while(ptr){
        if(ptr->getDriverName()==car->getDriverName()){
            break;
        }
        ptr=ptr->getNext();
        i++;
    }
   
    if(i==1){return 25;}
    else if(i==2){return 18;}
    else if(i==3){return 15;}
    else if(i==4){return 12;}
    else if(i==5){return 10;}
    else if(i==6){return 8;}
    else if(i==7){return 6;}
    else if(i==8){return 4;}
    else if(i==9){return 2;}
    else if(i==10){return 1;}
    else {return 0;}
}

std::string Race::kacinci(Car * car)const{
    Car * ptr = head;
    int sira=0;
    while(ptr){
        if(ptr->getDriverName()==car->getDriverName()){
            break;
        }
        ptr=ptr->getNext();
        sira++;
    }
    int kacinci=sira;
    int basamaksayisi=0;

    /* Sayinin Kac basamaklı oldugunu buluyoruz **/
    while(sira>0)
    {
    basamaksayisi++;  // her bolme isleminden once sayac değerini arttır.
    sira/=10;         // devamlı ona bol.
    }
    

    int toplambas=0;
    Car* ptr = head;
    int howmuch= 0;
    while(ptr){
        howmuch+=1;
        ptr=ptr->getNext();
    }
    

    int toplambasamak=howmuch;
    while(sira>0)
    {
    toplambas++;  // her bolme isleminden once sayac değerini arttır.
    toplambasamak/=10;         // devamlı ona bol.
    }

    return std::string(toplambas-basamaksayisi, '0') + std::to_string(sira);
    



}

Car* Race::getCarHead() const {return head;}