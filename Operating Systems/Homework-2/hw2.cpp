#include <algorithm>
#include <queue>
#include <iostream>
#include "monitor.h"
#include <vector>
#include <pthread.h>
#include <deque> 
#include <bits/stdc++.h> 
#include <string>
#include <unistd.h>

#include <chrono>
#include<fstream>


std::vector<std::string> hp_lp={"hp","lp"};

std::vector<std::string> up_down = {"Moving-up","Moving-down"};

int num_floors;
int num_people;
int weight_capacity;
int person_capacity;
int TRAVEL_TIME;
int IDLE_TIME;
int IN_OUT_TIME;



std::vector<int> already_requested;

std::vector<int> drop_at_floor;

std::vector<int> waiting_highs;

std::vector<int> in_elev;

struct passenger {
    int person_id;
    int weight;
    int initial;
    int dest;
    int priority;

    
};


class ElevatorMonitor;

struct PParam {
        ElevatorMonitor *el;
        int person_id;
        int weight;
        int initial;
        int dest;
        int priority;
    };

class ElevatorMonitor:public Monitor {

    int elev_capacity;
    int elev_weight;
    int elev_floor;

    int current_capacity;
    int current_weight;
    int current_floor;

    std::vector<int> destinations;
    int elevator_status;

    std::vector<passenger> request_topla;

    int finished;


    /*elevator allow requests*/
    Condition allow_requests;

    Condition newcond;

    Condition elev_waiting;

    std::vector< std::vector<Condition> > apart;



public:
    ElevatorMonitor(int enum_floors,int enum_people,int eweight_capacity,int eperson_capacity,int eTRAVEL_TIME,int eIDLE,int eIN_OUT_TIME):allow_requests(this),newcond(this),elev_waiting(this) {
        elev_capacity = eperson_capacity;
        elev_weight = eweight_capacity;
        elev_floor = enum_floors;


        current_capacity = 0;
        current_weight = 0;
        current_floor = 0;
        

        finished = 0;

        elevator_status = 0;

        for(int i=0;i<num_floors;i++){
            
            
            std::vector<Condition> temp;
            temp.push_back(newcond);    
            temp.push_back(newcond);    
            temp.push_back(newcond);
            

            apart.push_back(temp);
        }

    }


    ~ElevatorMonitor(){}

    void elevHandler(){
        __synchronized__;

        while(1){
            
            if(finished == num_people){break;}
            else{
                
                while(destinations.size() == 0 && finished != num_people){
                    //if no request idle
                    //elevator_status = 0;
                    
                    //wait for request
                    elevator_status = 0;
                    elev_waiting.notifyAll();
                    allow_requests.timedwait(IDLE_TIME);

                    
                    //printElevator();std::cout<<"1\n";
                    if(destinations.size() == 0 && finished == num_people){break;}  
                    
                }

                //wait for travel
                allow_requests.timedwait(TRAVEL_TIME);

                if(finished == num_people){break;}
                //change floor
                if(elevator_status == 1){current_floor++; }
                else if(elevator_status == -1){current_floor--; }
               
                

                //remove current floor from dest queue
                if(current_floor == destinations[0]){
                    destinations.erase(destinations.begin());
                }
                if(destinations.size() == 0){elevator_status = 0;}
                 printElevator();

                

                //call out
                apart[current_floor][0].notifyAll();
                // call high ins
                usleep(IN_OUT_TIME/3);
                apart[current_floor][1].notifyAll();
                //call low ins
                usleep(IN_OUT_TIME/3);

                apart[current_floor][2].notifyAll();
                usleep(IN_OUT_TIME/3);
                

                



            }
        }


    }

    void personHandler(int id,int weight,int initt, int destt,int pri){
        __synchronized__;

        int flag = 0;

        while(1){
            if(flag == 1){
                // bu kattakiler ciksin dendiyse

                //std::cout<<"id"<<id<<" burada\n";
                apart[destt][0].wait();
                //std::cout<<"id"<<id<<" also burada\n";

                exitt(id,weight,initt,destt,pri);
                break;
                //
            }

            else{
                make_request(id,weight,initt,destt,pri);

                if(pri == 1){
                    apart[initt][1].wait();
                    flag = enter(id,weight,initt,destt,pri);
                    
                    
                }
                
                else if(pri == 2){
                    apart[initt][2].wait();
                    flag = enter(id,weight,initt,destt,pri);
                }

            }

        }
    }


    // print functions
    void printElevator(){

        //std::cout<<"Elevator ("<<"Moving-up, "<< current_weight<<", " << current_capacity<<", "<< current_floor<<" -> "<<wholedest<<")"<<std::endl;
        std::string stat;
        if(elevator_status == 0){
            stat = "Idle, ";
        } 
        else if(elevator_status == 1){
            stat = "Moving-up, ";
        }
        else if(elevator_status == -1){
            stat = "Moving-down, ";

        }


                        std::string wholedest;
                        std::string temp;
                    
                    if(destinations.size() == 0){
                        std::cout<<"Elevator ("<<stat<<current_weight<<", "<<current_capacity<<", "<<current_floor<<" ->"<<wholedest<<")"<<std::endl;
                    }
                    else{
                        
                        for (int i = 0; i < destinations.size(); i++) {

                        temp = std::to_string(destinations[i]);

                        wholedest += temp;
                        if(i != destinations.size()-1){
                             wholedest += ",";
                        }

                }

                     std::cout<<"Elevator ("<<stat<<current_weight<<", "<<current_capacity<<", "<<current_floor<<" -> "<<wholedest<<")"<<std::endl;

                    }

                }


    void printReqeust(int id,int weight,int initt, int destt,int pri){
        std::cout<<"Person ("<<id<<", "<<hp_lp[pri-1]<<", "<<initt<<" -> "<<destt<<", "<<weight<<") made a request"<<std::endl;}
    void printEntered(int id,int weight,int initt, int destt,int pri){
        std::cout<<"Person ("<<id<<", "<<hp_lp[pri-1]<<", "<<initt<<" -> "<<destt<<", "<<weight<<") entered the elevator"<<std::endl;}
    void printExited(int id,int weight,int initt, int destt,int pri){
        std::cout<<"Person ("<<id<<", "<<hp_lp[pri-1]<<", "<<initt<<" -> "<<destt<<", "<<weight<<") has left the elevator"<<std::endl;}



    // enter exit and request

    //enter if eligible

    int enter(int id,int weight,int initt, int destt,int pri){
        // if elevator have capacity
        int flag =0;

        //std::cout<<"enter cagirildi "<<id<<"\n";

        //capacity fit ise
        if(current_capacity < elev_capacity){
            //std::cout<<"enter kapasite tamam "<<id<<"\n";
            // weight fit ise
            if(current_weight + weight <= weight_capacity){
                //std::cout<<"enter weight tamam "<<id<<"\n";

                //extra check
                if(correct_direction_front(initt,destt)){
                    flag = 1;
                    //std::cout<<"enter kabul edildi "<<id<<"\n";
                    
                    current_capacity++;
                    current_weight+= weight;
                    
                    if(elevator_status==0){
                        if(destt > current_floor){elevator_status = 1;}
                        else if(destt< current_floor){elevator_status = -1;}
                    }
                    
                    add_to_dest_and_sort(destt);
                    printEntered(id,weight,initt,destt,pri);
                    printElevator();



                }

            }
        }
        return flag;

    }

    void exitt(int id,int weight,int initt, int destt,int pri){
        
        //std::cout<<"exit cagirildi "<<id<<"\n";


        current_capacity--;
        current_weight -= weight;

        finished++;

        printExited(id,weight,initt,destt,pri);
        printElevator();

    }



    void make_request(int id,int weight,int initt, int destt,int pri){
        
        //eligible ise request yap
        // correct way
        // front
        

        if(correct_direction_front(initt,destt)){
            //std::cout<<"request yapmayı bekliyor"<<id<<"\n";
            elev_waiting.wait();
            //std::cout<<"request yaptı: "<<id<<"\n";
            if(current_floor < initt){
                //std::cout<<"request kabul edildi: "<<id<<"\n";
                add_to_dest_and_sort(initt);
                elevator_status = 1;
                printReqeust(id,weight,initt,destt,pri);
                printElevator();
                
            }
            else if(current_floor > initt){
                //std::cout<<"request kabul edildi: "<<id<<"\n";
                
                add_to_dest_and_sort(initt);
                elevator_status = -1;
                printReqeust(id,weight,initt,destt,pri);
                printElevator();


            }
            else{
                if(current_floor < destt){
                    //std::cout<<"request kabul edildi: "<<id<<"\n";
                    
                    add_to_dest_and_sort(destt);
                    elevator_status = 1;
                    printReqeust(id,weight,initt,destt,pri);
                    printElevator();
                    

                }
                else if(current_floor > destt){
                    //std::cout<<"request kabul edildi: "<<id<<"\n";
                    
                    add_to_dest_and_sort(destt);
                    elevator_status = -1;
                    printReqeust(id,weight,initt,destt,pri);
                    printElevator();
                    

                }
            }
        }






       
    }

    void add_to_dest_and_sort(int newFloor){
        if ( !(std::find(destinations.begin(), destinations.end(), newFloor) != destinations.end()) ){
                        destinations.push_back(newFloor);
                        }
        
                /*moving up kucukten buyuge*/
                if(elevator_status == 1){
                    std::sort(destinations.begin(), destinations.end()); 
                }
                /*moving down sort buyukten kucuge */
                else if(elevator_status == -1){
                    std::sort(destinations.begin(), destinations.end(), std::greater<int>());  
                }
    }

    bool correct_direction_front(int initt,int destt){
        if(elevator_status ==0){
            return true;
        }
        else if(elevator_status==1){
            if((destt > initt) && (initt >= current_floor)){
                return true;
            }
            else{return false;}
        }
        else if(elevator_status == -1){
            if((destt < initt ) && (initt <= current_floor)){
                return true;
            }
            else{return false;}
        }
        else{
            return false;
        }
    }

};


void *elev(void *p){
    ElevatorMonitor *em = (ElevatorMonitor *) p;
    
    em->elevHandler();
}

void *person(void *p){

    PParam *pp = (PParam *) p;
    ElevatorMonitor *em =  pp->el;
    int id = pp->person_id;


    em->personHandler(pp->person_id,pp->weight,pp->initial,pp->dest,pp->priority);
    

}



int main(int argc, char** argv){
    /* get input */

    std::ifstream infile(argv[1]);


    infile >> num_floors >> num_people >> weight_capacity >> person_capacity >> TRAVEL_TIME >> IDLE_TIME >> IN_OUT_TIME ;

    already_requested.resize(num_people);
    in_elev.resize(num_people);

    for(int i=0;i<num_people;i++){
        already_requested[i] = 0;
        in_elev[i]= 0;
    }

    drop_at_floor.resize(num_floors);
    waiting_highs.resize(num_floors);

    for(int i=0;i<num_floors;i++){
        drop_at_floor[i] = 0;
        waiting_highs[i] = 0;
    }

    





    pthread_t *people , elevator;

    PParam *pparams = new PParam[num_people];

    ElevatorMonitor em(num_floors,num_people,weight_capacity,person_capacity,TRAVEL_TIME,IDLE_TIME,IN_OUT_TIME);

    people = new pthread_t[num_people];

 
    //std::cout<<num_people;
    std::vector<std::vector<int> > temp_vec(num_people);

    pthread_create(&elevator,NULL,elev,(void *) &em);

    //if(VERBOSE){std::cout<<"Elevator thread started.\n";}
    
    int temp_weight;
    int temp_initial;
    int temp_dest;
    int temp_pri;



    for(int i=0;i<num_people;i++){
        //std::cout<<" people "<< i<<"\n";
        
        infile >> temp_weight >> temp_initial >> temp_dest >> temp_pri;

        //std::cout<< temp_weight <<" "<<temp_initial<<" "<<temp_dest<<" "<<temp_pri<<"\n";
        temp_vec[i].push_back(temp_weight);
        temp_vec[i].push_back(temp_initial);
        temp_vec[i].push_back(temp_dest);
        temp_vec[i].push_back(temp_pri); 

        
        /*high low sayisi */
        

        
    }
    

    for(int i=0;i<num_people;i++){
        pparams[i].person_id = i;
        pparams[i].weight = temp_vec[i][0];
        pparams[i].initial = temp_vec[i][1];
        pparams[i].dest = temp_vec[i][2];
        pparams[i].priority = temp_vec[i][3];
        pparams[i].el = &em; 

        pthread_create(&people[i],NULL,person,(void *)(pparams+i));

        
    }

    //std::cout<<"People started. \n";
    


   for(int i =0;i<num_people;i++){
        pthread_join(people[i],NULL);
    }



    

    pthread_join(elevator,NULL);

    delete [] pparams;
    delete [] people;

    return 0;
     





}