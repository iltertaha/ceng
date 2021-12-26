#include "Championship.h"
#include "Utilizer.h"
#include <algorithm>
#include <utility>
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Championship::Championship(){}
Championship::Championship(const Championship& rhs){
    for ( auto &i : rhs.races ) {
        races.push_back(i);
}
}
Championship::~Championship(){}
void Championship::addNewRace(std::string race_name){
   Race ornek = races.front();
   Race* newRace = new Race(ornek);
   newRace->setAverageLapTime(Utilizer::generateAverageLaptime());
    races.push_back(*newRace);
}
void Championship::removeRace(std::string race_name){
    int i=0;
    for ( auto &race : races ) {
        
        if(race.getRaceName==race_name){
        
            break;
        }
        i++;
}
    races.erase (races.begin() + i);

}
Race Championship::operator[](std::string race_name){
    for ( auto &race : races ) {
        
        if(race.getRaceName()==race_name){
            return race;
        }
    }
}
std::ostream& operator<<(std::ostream& os, const Championship& car){
    int temp=0;
    int total=0;
    std::vector< std::pair<std::string,int> > status;
    Car* ptr=car.races.front().getCarHead();
    while(ptr){
        total=0;
        for ( auto &race : car.races ) {
            
            total+=race.rankingpoint(ptr) + race.fastestoflap(ptr);
            
            }
            status.push_back(std::make_pair(ptr->getDriverName(),total));


        ptr=ptr->getNext();
        }

        std::sort(status.begin(), status.end(), sortbysec); 

    for ( auto &i : status ) {
        std::string name=status.back().first;
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
        os <<listesira() <<initials << status.back().first << status.back().second;
        status.pop_back();
     /* 01-RAI--194 */
        }
}
    
    bool sortbysec(const std::pair<std::string,int>& a,const std::pair<std::string, int>& b){ 
    return (a.second > b.second); 
} 
    liste


    

