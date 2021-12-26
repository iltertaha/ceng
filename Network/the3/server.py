


#### DESCRIPTION ###


####################

# Ilter Taha AKTOLGA   2236891

# This file contains the server side of the TCP and UDP connections 
# There are two threads namely: udp_controller and tcp_controller.
# Each thread functions are explained with comments.


################### USED LIBRARIES ####################

# For TCP and UDP 
import socket

import sys
import os.path
import operator

import struct

## Calculation of time difference
import time

import pickle

## IMPORTANT : YOU NEED helper.py in the same directory with server.py to run 
from helpers import safe_open_file,open_packet,checksum,create_packet,header_config,file_config,printExtra,extract_tcp_transmission_time,bigger_or_equal_to_length,bigger_than_length


## TCP and UDP parts are running concurrently 
import threading

import select
#######################################################




def udp_controller(filename,udp_socket,recv_addr):

    # Bind to address and ip of client
    udp_socket.bind(recv_addr)
    
    ## counts the successfully received packets
    packet_counter = 0
    avg_counter = 0

    ## open given file with function from helpers.py
    f = safe_open_file(filename,"wb")

    ## timing functions
    udp_average_time = 0.0
    udp_total_time = 0.0
    

    while True:
        
        # retrieve 540 byte data from udp socket
        # 540 is calculated according:
        #     file read: 512 byte
        #     time data: 8 byte
        #     checksum: 16 byte
        #     sequence number: 4 byte
        #     total 540 byte

        chunk = 0
        try:
            r, _, _ = select.select([udp_socket], [], [])
        except:
            pass


        if r:
            chunk, addresss = udp_socket.recvfrom(540)

        #chunk, addresss= udp_socket.recvfrom(540)
        
        
        ## control flags for incoming data
        seqbit =0         
        checkbit =0
        #timebit =0  

         
        ## if data available
        if(chunk):
            # divide incoming data into sequence,time,checksum and data
            sequence_num,time_val,received_checksum,data =open_packet(chunk)
            

            # make checksum control
            if(checksum(chunk[0:4]+chunk[4:12]+chunk[28:]) != received_checksum):
                
                # if data is corrupted 
                # send acknowledge for previous valid highest packet index
                prev_ack = create_packet(packet_counter-1)

                # verbose
                if(printExtra):
                    print('CORRUPTED', sequence_num - 1)
                

                # pass the rest of the loop if not valid ack
                udp_socket.sendto(prev_ack,addresss)
                continue


            

            # print("packet")
            # print("seqnum",sequence_num)
            # print("time_val",time_val)
            # print("received_checksum",received_checksum)
            # print("data",data)
            

            #validation controls
            
           

            # check whether the incoming ack is the expected one 
            # if not set the flag 
            if(sequence_num != packet_counter):
                seqbit = 1
            
            
            

            if(seqbit == 0 and (checkbit ==0)):
                # if the data is valid and the expected one
                
                #verbose
                if(printExtra):
                    print("data",data[:19])
                
                # if this data is the last package
                # Last package is always contains the following string message encoded in bytes
                # This package is always come with a highest sequence number 
                # in order to guarantee the all file is transmitted correctly
                if(data[:19].decode() == "thanks_for_all_fish"):
                    # stop waiting packages from client it is finished
                    break

                
                #verbose
                if(printExtra):
                    print("correctly get")
                
         
                # create and send ACK
                ack_chunk = create_packet(packet_counter)
                udp_socket.sendto(ack_chunk,addresss)


                # if the packet is not the final package:
                #    use its contents to write to file
                #    calculate time difference and add to total 
                packet_counter = packet_counter +1 
                

                #print("current_moment",current_moment)
                #print("time_val",time_val)
                
                # add time diff 
                timestamp= time_val
                

                current_timee = int(time.time() *100000)
                
                udp_transmission_time = (current_timee-timestamp) / 100.0
                
                udp_total_time += udp_transmission_time
                avg_counter+=1
                

                

                #verbose
                if(printExtra):
                    print('Got expected and valid packet')
                    print('Sending ack for seq num', sequence_num)
                    
                # write to file 
                f.write(data)

            else:
                # data is not valid or not the expected one
                # send ack for previous highest valid data

                prev_ack = create_packet(packet_counter-1)
                
                #verbose
                if(printExtra):
                    print('Sending ack for seq num', sequence_num - 1)
                
                
                udp_socket.sendto(prev_ack,addresss)

        else:
            break # dont wait since no data coming

    
    # finished sending then close socket and file
    f.close()
    udp_socket.close()

    #verbose
    if(printExtra):
        print("server_finished")


    # calculate average time
    udp_average_time = udp_total_time/packet_counter

    #verbose
    if(printExtra):
        print("packet count",packet_counter)
    
    # print expected results for udp
    print("UDP Packets Average Transmission Time: ",str(udp_average_time),"ms")
    print("UDP Packets Total Transmission Time: ",str(udp_total_time),"ms")




def tcp_controller(serversoc,tcp_file_name,headersize):
    
    # assigns socket to an address and  given tcp port
    serversoc.bind((socket.gethostname(),tcp_port))


    # num of connections that server will allow
    serversoc.listen(5)

    if(printExtra):
        print("started listening")

    # when the client connects, server accepts to complete the connection
    connectionsoc,_ = serversoc.accept()
    
    # variables needed for printing at the end
    total_time = 0.0
    packet_count = 0

    # create a file to write binary data
    f = safe_open_file(tcp_file_name, "wb")
    complete_data = b''

    # variable for get complete chunk
    waiting_chunk = True

    # verbose
    if(printExtra):
        print('Receiving file from client..')

    
    # get first message 
    partial_data = connectionsoc.recv(524)

    # add this to complete data
    if(partial_data):
        complete_data+=partial_data

    # loop for incoming tcp data
    while True:
        
        # partial data is available
        if waiting_chunk:
            waiting_chunk = False
            # since the TCP is bytestream
            # 524 byte chunks doesnt arrive at the same time
            # i.e. header will arrive earlier than rest of the message
            
            # to correctly get the full chunk get length of chunk
            length_of_package = struct.unpack("i",complete_data[:4])[0]

        
        # for incoming data there will be 2 cases:
        #     1) data is greater than expected
        #     2) data is equal to the specified length in header

        # we have to get data up to message length (otherwise, extracted information will be invalid)
        # rest of the data will be merged with next incoming byte stream

        if bigger_or_equal_to_length(complete_data,length_of_package,headersize):

            #header= struct.unpack("i",complete_data[:4])[0]
                      
            #print(header)

            packet_count += 1

            

            if(bigger_than_length(complete_data,length_of_package,headersize)):

                # data is bigger than expected. 

                # get needed length
                message = complete_data[headersize:length_of_package+headersize]

                # write to file
                f.write(message)
                
                # calculate time diff
                timestamp= struct.unpack("q",complete_data[4:12])[0]
                current_timee = int(time.time() *100000)
                transmission_time = (current_timee-timestamp) / 100.0
                
                total_time += transmission_time
                
                
                # pass remaining data for next use
                complete_data = complete_data[(headersize + length_of_package):]
            else:

                # data length is equal to the length 

                # get all data
                message = complete_data[headersize:]
                timestamp= struct.unpack("q",complete_data[4:12])[0]
                current_timee = int(time.time() *100000)
                transmission_time = (current_timee-timestamp) / 100.0
                total_time += transmission_time  


                # no remaining data for next use
                complete_data = b''

                #write to file
                f.write(message)

                # calculate time diff
                
        
            # next cycle we need to check message length again
            waiting_chunk = True
            
        # get next chunk of data
        partial_data = connectionsoc.recv(524)
        complete_data += partial_data

        # break condition 
        if(not complete_data):
            #print("breaked")
            break

    # close socket and file
    serversoc.close()
    f.close()

    if(printExtra):
        print("packet count ",packet_count)
    
    # calc average
    average_time = total_time/packet_count
    
    # necessary prints
    print("TCP Packets Average Transmission Time: ",str(average_time),"ms")
    print("TCP Packets Total Transmission Time: ",str(total_time),"ms")



###################################################### MAIN SECTION ########################################################

### Parse Command Line Arguments ## 

udp_port = int(sys.argv[1])
tcp_port = int(sys.argv[2])

## create ip,port pair for udp ##
recv_addr = ('', udp_port)


##  Given Input Files ## 

tcp_file_name = "./transfer_file_TCP.txt"
udp_file_name = "./transfer_file_UDP.txt"

##  adjustable header size ##
headersize = header_config["size"]


##  Datagram socket creation ##
serversoc = socket.socket(socket.AF_INET,socket.SOCK_STREAM)


#print("connection established")


udp_socket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)


############## THREAD INITALIZATION AND START #####################

tcp_threader = threading.Thread(target = tcp_controller, args = (serversoc,tcp_file_name,headersize))
tcp_threader.start()


udp_threader = threading.Thread(target = udp_controller, args = (udp_file_name,udp_socket,recv_addr))
udp_threader.start()


##### WAIT THREADS TO FINISH #########

tcp_threader.join()
udp_threader.join()






############## END OF IMPLEMENTATION ##################













## UDP server

#print("server finished tcp")




    #udp_socket.close()

#udp_socket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
#udp_socket.bind(RECEIVER_ADDR)


#udp_controller(udp_file_name,udp_socket)
#udp_controller.join() 

