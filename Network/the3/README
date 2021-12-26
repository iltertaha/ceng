## NETWORK THE2 README 

Name: Ilter Taha AKTOLGA
Student ID : 2236891

### HOW TO TEST?
- Extract zip 
- Put client.py and server.py into different directories
- Copy helpers.py into the same directory for both client.py and server.py
*Both of the client.py and server.py includes functions and config variables from the helpers.py*

- IMPORTANT: This code assumes that data will be transferred is not smaller than 1MB as stated in odtuclass

- Moreover error checking is not implemented for some scenarios, since it is not mentioned that the codes will be tested with errorneous data.

- Timeout for client is set to 0.5 

- While sending chunk time from client to server:
  I have converted the time.time() s float data to integer by multiplying with 100000. Since the integers are more reliable while converting to bytes and extracting from bytes. 

- TCP and UDP implementations are running concurrently. So the TCP outputs will appear first and UDP outputs will be appear next when the code is executed.

- **Note that since the timeout is 0.5 higher delays creates problem but with the example commands below, you should get the requested file appropriately.**


- Put files in the same directory with client

- In other words: 
.
├── **folder1**
│   ├── client.py
│   └── helpers.py
│   └── transfer_file_TCP.txt
│   └── transfer_file_UDP.txt
    
|
├── **folder2**
│   ├── server.py
│   └── helpers.py

helpers.py is always used.

**Running order: **
1. Run simulator.py with your arguments
2. Run server.py with your arguments.
3. Run client.py with your arguments.

It will create transfer_file_TCP.txt and transfer_file_UDP.txt under the same directory as server.py

**Example testing setup with the ports reserved for me** (id: 2236891):
* I have used inek13, inek14 and inek17 for testing.
* **inek13:** run the command in the terminal from the directory where client.py and helpers.py  resides.
   - `python3 client.py 144.122.238.37 15466 15467 15468 15469`  
   
* **inek14:** run the command in the terminal from the directory where server.py and helpers.py resides.  
   - `python3 server.py 15464 15465`  
   
* **inek17:** run the command in the terminal from the directory where simulator.py resides.
   -   `./simulator 144.122.238.33 144.122.238.34 15466 15467 15468 15464 15465 15470 15471 5 3 4
`  

#### What I have done in this study:
I have implemented a TCP socket which sends data from client to server. In my implementation, it reads 512 bytes from the given file and generates the files sent by using TCP and UDP. I have used concurrency while implementing. This allowed me to send data by TCP and UDP at the same time.
* In TCP part, since the TCP is a **reliable** protocol, I have only checked whether the chunks of data arrived completely before extracting the chunks to get data and header information from them. (There is no corruption case)
* Since the UDP socket itself is **not reliable** protocol, I have implemented a **RDT protocol** namely **Go Back N** to handle cases such as: missing packages, corrupted packages, and packages which couldn't received due to early timeout, corrupted acknowledges, missing acknowledges. 


#### The part I started with:
I have first started with dividing the data into chunks. For both TCP and UDP, I divided the data into 512 byte chunks and concatenated with header information. I have studied whether I decode header correctly and extract necessary information correctly. This was the most important part of the project, since the wrong implementation may lead to catastrophic changes during the later modifications. Therefore I decided which variables to include in header, how much byte they occupy, and how to parse them when they reached to the server.


####  How I planned what to do:
I have reviewed transformation layer chapter from the book and the slide materials of 435.
I have examined the state machines in the textbook. Although they were high level, they allowed me to create a working approach to sockets. As homework text stated, I have divided homework into parts as:
1. Divide data and add header
2. Implement TCP socket
	2.1 Test TCP only
3. Implement UDP socket
	3.1 Test UDP only
4. Merge TCP and UDP
5. Create threads for TCP and UDP
	5.1 Check thread joins
6. Add reliable transfer protocol to UDP
7. Measure time and resent packages
8. Debug and test 


#### Problems that I have faced:
I have faced with several problems during the implementation.
* **Adding header information:** I have first used string encode method to convert string to byte object. However then I realized that in this way, sending variables(objects) is hard since I need to include the size of the objects. Therefore I have switched to struct library which allows converting several type of data into bytes object and vice versa.  Using structs I have packed my variables into header in the client, and unpacked in the server.

* ** Stopping the server after sending: **  After implementing UDP, I saw that server is not stopping after sending the data. Then I have decided to break server when the client indicates that all messages are sent from client side. I implemented this as adding one extra chunk at the end, which will send after the successful transmission of the data, I have managed to stop server. In my implementation client sends a package with highest sequence number. If the message in this package is *"thanks_for_all_fish" *then server understands that it stops.


* ** TCP bytestream problem: ** This was the most interesting problem that I have faced. Since the TCP is reliable, I was not checking whether the chunk received completely. However, because the TCP is **byte stream**.  For each 512 byte data with 12 byte header, I need to be able to understand how much of the chunk is read and how much of the chunk is left. Without this, I have got wrong time information at the server side.  


* ** Time miscalculation: **  I was sending double values to client, which I have calculated using time.time() function. However this leads to floating point precision loss and resulted in negative values during the comparison at the server. Then I have switched to use integer, packed in the struct. In this way I have managed to satisfy consistency of the sent and received information.

#### What I learned after the study:
I have learned that while implementing,  TCP is easier than UDP since it doesn't need much configuration to send data. However in UDP, RDT is must. Although RDT is challenging, I liked the idea that sending and receving data as a package in UDP. I have also learned to use threads in python. Before this homework, I have used threads only in c++. In the python, in order to kill one thread when the another one finishes, I need to set `daemon` property of the thread which will be killed to `True`. This was necessary for client. Client uses threads in it: 
One thread to send data and another to receive acknowledges. So, when the sender thread finishes, other thread stops listening acknowledges.

#### How many days did it take:
It took nearly 6 days to implement the homework. But before coding, I have reviewed the course material and understood the reliable data transfer protocols. Then I have decided which RDT to implement. After choosing method namely Go Back N, I have studied automata's in the textbook. 
Then I have started coding, my plan can be seen above, under the title of *How I planned what to do:*
#### RDT protocol you have used
I have used Go Back N protocol. I selected this method since it is easier for receiver side. There is no need for buffer. Server only sends acknowledges when the correct data is received. And repeats the highest valid acknowledge when the wrong data received. Besides, in the client, client prepares a window, and sets a timeout value. Each time when the data with lowest sequence number in the window is received by the server. Window slides. If one of the data in the window is not received, client waits as until the timeout occurs. Then it resends all the packages in the window, starting from the missing data.
