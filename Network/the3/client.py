

##### tcp part ####

########## USED LIBRARIES #############
import select
import socket
import sys
import time
import pickle
import struct

import threading
import time
from helpers import hashlib,checksum,safe_open_file,split_data,create_packet,open_packet,config_window,create_finalizer,header_config,file_config, printExtra
########################################################


#### GET ARGUMENTS FROM TERMINAL ######
server = str(sys.argv[1])
udp_listen = int(sys.argv[2])
tcp_listen = int(sys.argv[3])

udp_sender = int(sys.argv[4])
tcp_sender = int(sys.argv[5])

########################################


## GET HEADER CONFIGURATIONS FROM THE HELPER MODULE


headerSize = header_config["size"]

tcp_file_name = file_config["tcp_file_in_name"]
udp_file_name = file_config["udp_file_in_name"]

################################################



### NECESSARY COUNTERS FOR PRINTS
packet_count = 0
total_time = 0
average_time = 0
##################################


## Create TCP socket and connect to server ip and port
clientsoc = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
clientsoc.connect((server,tcp_listen))

#verbose
if(printExtra):
    print("server connection successful")


## open the file which will be transferred
f = open(tcp_file_name,"rb")

#verbose
if(printExtra):
    print("opening file")


# read first data 
chunk = f.read(512)


count = 0

## tcp sender loop
while(chunk):

    ##### prepare the chunk #####

    # each chunk contains it's header, how long the chunk is
    # this handles the last package of the TCP which may be less than the fixed package size
    header = struct.pack("i",len(chunk))
    
    #### include sending time of the chunk####
    # as mentioned in README, I sent time information as a long integer.
    # Therefore I have took 5 significant figure after the comma.
    # and packed into long in struct

    time_in_int = time.time()
    time_in_int = int(time_in_int*100000)
    
    timee = struct.pack("q",time_in_int)
    
    # concatenate the length, time and data
    total = header + timee + chunk
    
    # send data over socket
    clientsoc.send(total)

    # read next chunk
    chunk = f.read(512)
    count+= 1


    ################################################
    
## tcp operation is completed
## close file and socket ##
f.close()
clientsoc.close()

#verbose
if(printExtra):
    print("client finished tcp")



######################################################## udp part ##########################



#### timeout control for udp ### 
############################################
## the value when the timer is not active
notime = -1

# start time of the timer
timer_start = notime

# timeout 
total_time_count = 0.5
############################################

#verbose
if(printExtra):
    print("udp starting")


############## USED VARIABLES ##############

# read 512 byte from file 
chunksize = 512

# sliding window start index
first_in_window = 0


### server and client ip and ports for udp
INCOMING_ADDRESS = (server, udp_listen)
OUTGOING_ADDR = ('', udp_sender)
##########################################

# window size
w_size = 10

# global flag to stop 
kill_me = 0

# number of packages resent
resend_packages = 0

# lock to avoid race condition when listening acknowledge and sending data
# during modification of the global or shared data
lock = threading.Lock()

############################################

def send_thread(inputfile,udp_socket,w_size,receiver_addresss):

    # use shared global variables #
    global notime
    global timer_start
    global total_time_count
    global first_in_window
    global lock
    INCOMING_ADDRESS = (receiver_addresss,udp_listen)
    global kill_me
    ###########################
    timeout = 0.5

    # index of the data that will be send next time
    
    # local variables to count or terminate the loop
    cnt = 0
    next_ind = 0
    kill_outsider = 0
    ################################################
    
    # open file to write
    f = safe_open_file(inputfile,"rb")


    # divide file into chunks of data
    # see helpers file to examine de split_data func implementation
    _, num_chunks, chunks = split_data(f,chunksize)


    #verbose
    if(printExtra):
        print("number of chunks : ", num_chunks)
    

    # rearrange the window sliding
    sliding_window_length = config_window(first_in_window,num_chunks,w_size)


    
    # this part sets the total_acks globally
    # so that receiver_thread will check this variable and stop
    global total_acks
    total_acks = num_chunks

    # verbose
    if(printExtra):
        print("total acks",total_acks)
    ###########################################################

    # init and start another thread to listen acknowledges
    #recv_thread = threading.Thread(target = receive_thread, args = (udp_socket,num_chunks ))

    #recv_thread.start()


    ############################################################
    
    

    # data sending loop #
    while(first_in_window < num_chunks):

        # take the lock
        lock.acquire()
        
        cnt += 1

        # if the next data is in our sliding window and if we didnt exceed total data size
        while( next_ind < num_chunks and next_ind < first_in_window+ sliding_window_length):
            

            # if flag set terminate the loop
            if(kill_me):
                kill_outsider = 1
                break
            
            # verbose
            if(printExtra):
                print('Sending packet',next_ind)
            
            # create a packet
            # see helpers file imported for implementation

            new_chunk = create_packet(chunks[next_ind][0],chunks[next_ind][1])

            # send the prepared packet
            udp_socket.sendto(new_chunk,INCOMING_ADDRESS)
            next_ind +=1

        # check flags to terminate outer loop
        if(kill_me or kill_outsider):
            break
         

            
        #timer is not running
        if (timer_start == notime ):
            # run the timer
            timer_start = time.time()

        
        # if timer is running and when timeout occurs 
        while((timer_start != notime) and not (time.time()- timer_start >= total_time_count)):
            lock.release()
            time.sleep(timeout)
            lock.acquire()


        # stop the timer 
        if time.time() - timer_start >= total_time_count:
            if(timer_start != notime):
                timer_start = notime

            next_ind = first_in_window

        # reconfigure the window
        else:
            sliding_window_length = config_window(first_in_window,num_chunks,w_size)

        # end of racing part
        lock.release()
        

        
    # finished sending udp packages 
    f.close()

    # verbose
    if(printExtra):
        print("client finished sending udp packages")
    

    # print resend packages from the global variable
    # which is set by the acknowledge thread
    global resend_packages
    print("UDP Transmission Re-transferred Packets:",resend_packages)

    


# listening the acknowledges
def receive_thread(udp_socket):

    # use global variables
    global lock
    global first_in_window
    
    global notime
    global timer_start
    global total_time_count
    global kill_me

    counter =1

    global resend_packages
    
    # get first data from the server side
    chunk, _ = udp_socket.recvfrom(540)

    # ack listening loop
    while True:
        #chunk, _ = udp_socket.recvfrom(512)
        

        # parse the received acknowledge
        acknow,_,received_checksum,_ = open_packet(chunk)

        # checksum the package
        if(checksum(chunk[0:4]+chunk[4:12]+chunk[28:]) == received_checksum):
            
            #verbose
            if(printExtra):
                print('Got valid ACK', acknow)
                print("acknow",acknow)
                print("total_acks",total_acks)
            
            # last ack is received
            # set the flag and terminate the client
            if(acknow+1 == total_acks-1):
                kill_me = 1
                break    
            
            # sequence number of received acknowledge is in the sliding window
            if(acknow >= first_in_window):
                lock.acquire()

                # slide the window
                first_in_window = acknow+1
                
                counter+=1
                
                # stop the timer to avoid timeout
                if(timer_start != notime):
                    timer_start = notime

                lock.release()

            else:
                # acknowledge is not valid
                # package will be resent
                resend_packages += 1

        else:
            # ack is corrupted
            # package will be resent
            resend_packages += 1


        ########################################
        # if socket has data
        # get data from socket
        # this part avoids hanging at recvfrom
        r = 0
        try:
            r, _, _ = select.select([udp_socket], [], [])
        except:
            pass
        
        if r:
            chunk, _ = udp_socket.recvfrom(540)
        
        ########################################



## create udp socket
udp_socket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
udp_socket.bind(OUTGOING_ADDR)
filename = "./transfer_file_UDP.txt"

#send_thread(filename,udp_socket,10)

sender = threading.Thread(target = send_thread, args = (filename,udp_socket, w_size,server))
recv_thread = threading.Thread(target = receive_thread, args = (udp_socket, ))
sender.start()
recv_thread.daemon = True
#recv_thread.daemon = True
recv_thread.start()


sender.join()

udp_socket.close()











########################## end of the implementation











    






