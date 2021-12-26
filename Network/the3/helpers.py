
# necessary libraries

import struct
import hashlib
import time
import os




# SERVER CONFIGS


# tcp message header size
header_config = dict(
    size = 12
)

file_config = dict(
    tcp_file_in_name = "./transfer_file_TCP.txt",
    udp_file_in_name = "./transfer_file_UDP.txt",
    tcp_file_out_name = "./transfer_file_TCP.txt",
    udp_file_out_name = "./transfer_file_UDP.txt"
)








# OTHER CONFIGS


# if you want to see all unnecessary prints used in debugging 
# set this to 1
printExtra = 0



# calculates the checksum over given data
# returns 16 bit checksum
def checksum(chunk):
    chunk_md = hashlib.md5()
    chunk_md.update(chunk)
    return chunk_md.digest()


## open file in the same directory and return file descriptor
def safe_open_file(file_name,opentype):
    file = None
    try:
        file = open(file_name,opentype)
    except IOError:
        print("Open operation failed for ",file_name)
        
    return file



# split the given data into chunks
# notice that this function is written according to homework statement
# if there is a larger file it is not logical to load whole file to memory 

def split_data(file,chunksize):
    chunks = []
    current_seq = 0
    while True:
        data = file.read(chunksize)
        if not data:
            break
        chunks.append([current_seq,data])
        current_seq = current_seq +1

    # add final message 
    # states end of transmission
    final_message =b'thanks_for_all_fish'
    final_message+= b"/"*(512-len(final_message))
    finalpack = create_finalizer(current_seq,final_message)
    seq_fin,_,_,chunk_fin =  open_packet(finalpack)
    chunks.append([seq_fin,chunk_fin])

    return (current_seq,len(chunks),chunks)


## creates package for each read data chunk
def create_packet(sequence,chunk_data=b''):

    # measure time
    # take 5 significant figure after comma
    # convert to integer
    time_in_int = time.time()
    time_in_int = int(time_in_int*100000)

    start_time = time_in_int

    # convert sequence number to byte
    sequence_byte = sequence.to_bytes(4,byteorder = 'little',signed=True)
    # convert time to byte
    time_byte = struct.pack("l",start_time)
    # calculate ch
    checksum_byte = checksum(sequence_byte + time_byte + chunk_data)
    
    return sequence_byte + time_byte + checksum_byte +  chunk_data


# create last package which contains thanks_for_all_fish string with padding to complete the size to 512 bytes
def create_finalizer(sequence,chunk_data=b''):
    #chunk_data = os.urandom(16)
    time_in_int = time.time()
    time_in_int = int(time_in_int*100000)

    start_time = time_in_int
    sequence_byte = sequence.to_bytes(4,byteorder = 'little',signed=True)
    time_byte = struct.pack("q",start_time)
    checksum_byte = checksum(sequence_byte + time_byte + chunk_data)
    
    return sequence_byte + time_byte + checksum_byte +  chunk_data



# returns the contents of the package as a tuple 
# sequence , time (as an integer), checksum value , data
def open_packet(packet):
    sequence_num = int.from_bytes(packet[0:4], byteorder = 'little', signed = True)
    time_val = struct.unpack("q",packet[4:12])[0]
    
    checksum = packet[12:28]
    data = packet[28:]

    return sequence_num,time_val,checksum,data


# slides the window
def config_window(first_in_window,chunk_num,windowlength):
    reorder = chunk_num - first_in_window
    if(reorder < windowlength):
        return reorder
    else:
        return windowlength
    #return min(reorder, windowlength)
    

# gets tcp time 
def extract_tcp_transmission_time(inc_data):
    timestamp= struct.unpack("q",inc_data[4:12])[0]
    transmission_time = (time.time()-timestamp)* 1000.0
    return transmission_time 

# as mentioned in README
# check if the data coming over TCP is greater or equal to the expected length
def bigger_or_equal_to_length(complete_data,length_of_package,headersize):
    if(len(complete_data)-headersize >= length_of_package):
        return True
    else:
        return False
        


# as mentioned in README
# check if the data coming over TCP is greater than the expected length
def bigger_than_length(complete_data,length_of_package,headersize):
    if(len(complete_data)-headersize > length_of_package):
        return True
    else:
        return False
