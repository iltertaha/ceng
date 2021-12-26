#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <errno.h>
#include <limits.h>

#include<vector>
#include<iostream>
#include<string>

/*special libs*/
#include "logging.h"
#include "message.h"

/* define pipe */
#define PIPE(fd) socketpair(AF_UNIX,SOCK_STREAM,PF_UNIX,fd)

int aliveBidders;

typedef enum { BIDDER_END, SERVER_END } pipeEnd;

/* set verbose 1 to debug*/

int firstbidderconnected = 0;


/* id generator func*/
int generate_id() 
{  
    // static variable 
    static int count = -1; 
    //cout << count << " "; 
  
    count++;
    return count;
} 


int main(){
    
/* GET INPUT FROM TERMINAL */
int starting_bid;
int minimum_increment;
int number_of_bidders;


std::cin >> starting_bid;
std::cin >> minimum_increment;
std::cin >> number_of_bidders;


std::vector<int> bidderDelays(number_of_bidders,INT_MAX);
int minimum_delay = 0;

struct timeval tv;


std::vector<std::vector<std::string> > bid_vec; /*keep bidder infos*/

std::string bid_path;
int num_of_args; /*keeps argument count for each bidder*/


for(int i=0;i<number_of_bidders;i++){
    std::cin>> bid_path;
    std::cin>> num_of_args;
    std::vector<std::string> temp_vec;

    temp_vec.push_back(bid_path); /*first item is always bid_path*/


    for(int j=0;j<num_of_args;j++){
        std::string temp_arg;
        std::cin >> temp_arg;

        

        temp_vec.push_back(temp_arg);    
    }

    bid_vec.push_back(temp_vec); /*add temp_vec to general vec*/
}


/*alive bidders bitmap*/
std::vector<int> bidderArray(number_of_bidders,1);



int bidderFds[number_of_bidders][2];
pid_t bidderPidArray[number_of_bidders]; /*bidder pid array*/
int bidderIDs[number_of_bidders]; /*bidder unique IDs*/




    

    /*create pipes*/
    for (int i = 0; i < number_of_bidders; i++) {
		PIPE(bidderFds[i]);
	}


    /*fork and exec bidders*/
	for (int i = 0; i < number_of_bidders; i++) {
        /*fflush(stdout);*/
        /*pid_t pid = fork();*/

		if ( (bidderPidArray[i]=fork()) == 0) {
            /*bidder will write to pipe and read from pipe*/
			dup2(bidderFds[i][0], 0);
			dup2(bidderFds[i][0], 1);

            /*close unnecessary fds*/
            close(bidderFds[i][0]);

            /*argument size is size() -1 (pathname) + 1 (nullptr at the end) */
			int argsize = bid_vec[i].size();
            
            
            
            char *yeni_args[argsize+1];
			
            int j;
            
            /**/
                
                
                
                
              
                 const size_t array_size = bid_vec[i].size();

                 char** arg_array = new char*[array_size + 1]; // extra space for terminating NULL

                 for (size_t j = 0; j < array_size; ++j)
                 {
                 arg_array[j] = strdup(bid_vec[i][j].c_str());
                 }
                 arg_array[array_size] = NULL;

               

                execv(arg_array[0], arg_array);

		}
        else{
           
            //std::cout<<getppid()<<std::endl;
           
            //close(bidderFds[i][0]); 
            
            //std::cout<<"parent\n";
            //bidderPidArray[i] = pid;
            //close(bidderFds[i][1]);
           
        }

        
	}


    /*generate id for each bidder*/
    for(int i=0;i<number_of_bidders;i++){
        int cli_id = generate_id();
        bidderIDs[i] = cli_id; 
    }

    /*set readset and max*/
    fd_set readset;

    
    int max = 0;
	for (int i = 0; i < number_of_bidders; i++) {
		if (bidderFds[i][1] > max) {
			max = bidderFds[i][1];
		}
	}
	//fprintf(stderr, "%d\n", max);
	max += 1;
	//fprintf(stderr, "%d\n", max);

    
    
    
    

    /************ server variables***********************************/
    int bidder_count = number_of_bidders;

    int current_highest_bid = 0;

    int finished_bidders= 0;

    int current_highest_bidder;

    int ilter;

    int bidderStatus[number_of_bidders];
    /****************************************************************/

    
    /*round robin*/
    while(1){
        
        if(finished_bidders == number_of_bidders ){
            server_message final_response;

            /*print server finished before killing bidders*/
            print_server_finished(current_highest_bidder,current_highest_bid);

            /*send winner info to bidders then kill em all*/
            for(int i=0;i<number_of_bidders;i++){
                final_response.message_id = SERVER_AUCTION_FINISHED;
                final_response.params.winner_info.winner_id   = current_highest_bidder;
                final_response.params.winner_info.winning_bid = current_highest_bid;

                /*send winner info*/
                write(bidderFds[i][1],&final_response,sizeof(server_message));
                
                

                /*output message send*/
                oi outt;
                
                outt.type = SERVER_AUCTION_FINISHED;
                outt.pid = bidderPidArray[i];
                
                outt.info.winner_info.winner_id = current_highest_bidder;
                outt.info.winner_info.winning_bid = current_highest_bid;
        
                print_output(&outt,bidderIDs[i]);

               
                /*close all fds of all bidders*/
                close(bidderFds[i][0]);
                close(bidderFds[i][1]);                

                
            }

            for(int i=0;i<number_of_bidders;i++){
                pid_t finished_pid = waitpid(-1,&ilter,0);


                int found_index=0;
                for(int j=0;j<number_of_bidders;j++){
                    if(bidderPidArray[j] == finished_pid){break;}
                    found_index++;
                }
                
                /*std::cout<<"finished_pid = "<<finished_pid<<std::endl;
                std::cout<<"ilter = "<<ilter<<std::endl;*/

                print_client_finished(bidderIDs[found_index],ilter,(bidderStatus[found_index]==ilter));
                    
                
            }

         break;   
        }


    FD_ZERO(&readset);

    for(int i=0; i < number_of_bidders; i++){
        if(bidderArray[i]){
            FD_SET(bidderFds[i][1],&readset);
        }
    }

    

    tv.tv_sec = minimum_delay / 1000;
    tv.tv_usec = (minimum_delay % 1000) * 1000; 
    /*std::cout<<"check tv.tv_sec : "<<tv.tv_sec<<std::endl;*/
    /*std::cout<<"check tv.tv_usec : "<<tv.tv_usec<<std::endl;*/
    /*fflush(stdout);*/
    select(max,&readset,NULL,NULL,&tv);

    for(int i=0;i<number_of_bidders;i++){
        if(FD_ISSET(bidderFds[i][1],&readset)){
            client_message request;
            //std::cout<<"test : "<<request.params.delay<<std::endl; 
            if(read(bidderFds[i][1],&request,sizeof(client_message))>=0){
        
                
                /*print the incoming message to the standart output*/
                
                //fflush(stdout);
               ii inputt;
               inputt.type = request.message_id;
               inputt.pid = bidderPidArray[i];
               inputt.info.delay =  request.params.delay;

               print_input(&inputt,bidderIDs[i]);

                

                /*set server message response*/
                sm reply;
                
                
                if(inputt.type == CLIENT_CONNECT){
                    
                    /*prepare reply for client connect*/
                    reply.message_id = SERVER_CONNECTION_ESTABLISHED;
                    reply.params.start_info.client_id    = bidderIDs[i];
                    reply.params.start_info.starting_bid = starting_bid;
                    reply.params.start_info.current_bid  = current_highest_bid;
                    reply.params.start_info.minimum_increment = minimum_increment;


                    write(bidderFds[i][1],&reply,sizeof(server_message));

                    /*prepare output to print*/
                    oi outt;
                    outt.pid = bidderPidArray[i];
                    outt.type = request.message_id;
                    outt.info.start_info.client_id = bidderIDs[i];
                    outt.info.start_info.starting_bid=starting_bid;;
                    outt.info.start_info.current_bid=current_highest_bid;;
                    outt.info.start_info.minimum_increment=  minimum_increment;


                    print_output(&outt,bidderIDs[i]);

                    
                    if(firstbidderconnected==0){firstbidderconnected=1;
                                                minimum_delay = request.params.delay;
                                            
                                                bidderDelays[i] = request.params.delay;
                                                } 

                    else{
                        if(minimum_delay > request.params.delay){
                            minimum_delay = request.params.delay;
                        }
                        bidderDelays[i] = request.params.delay;

                    }


                }

                if(inputt.type == CLIENT_BID){
                    
                    /*prepare reply for client bid*/
                    int new_bid = request.params.bid;
    
                    reply.message_id = SERVER_BID_RESULT;
                    
                    

                    if(new_bid < starting_bid){
                        reply.params.result_info.result = BID_LOWER_THAN_STARTING_BID;
                    }
                    else if(new_bid < current_highest_bid){
                        reply.params.result_info.result = BID_LOWER_THAN_CURRENT;
                    }

                    else if((new_bid - current_highest_bid) < minimum_increment){
                        reply.params.result_info.result = BID_INCREMENT_LOWER_THAN_MINIMUM;
                    }
                    else{
                        reply.params.result_info.result = BID_ACCEPTED;

                    
                        current_highest_bid    = new_bid;
                        current_highest_bidder =  bidderIDs[i];
                    }
                        
                    

                    reply.params.result_info.current_bid = current_highest_bid;
                    
                    write(bidderFds[i][1],&reply,sizeof(server_message));

                    /*prepare output to print*/
                    oi outt;
                    outt.pid = bidderPidArray[i];
                    outt.type = request.message_id;
                    outt.info.result_info.current_bid= current_highest_bid;
                    outt.info.result_info.result=reply.params.result_info.result;



                    print_output(&outt,bidderIDs[i]);
                    
                }
                
                
                if(inputt.type == CLIENT_FINISHED){

                    /*count bidder*/
                    finished_bidders++;

                    /*save status of bidder*/
                    bidderStatus[i] = request.params.status;

                    /*skip that bidder during select*/
                    bidderArray[i] = 0;
                
                     bidderDelays[i] = INT_MAX;

                     minimum_delay = INT_MAX;
                    for(int m=0;m<number_of_bidders;m++){
                        if(bidderDelays[m]< minimum_delay){
                            minimum_delay = bidderDelays[m];
                        }
                    }


                }

                

            }



        }
       
       
    }
    
    


        

}

    



}







