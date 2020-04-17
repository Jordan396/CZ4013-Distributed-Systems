# CZ4013-Distributed-Systems

This is our team's source code submission for CZ4013 Distributed Systems. It is written in C++, and developed on `Ubuntu 18.04.4 LTS` virtual machines provided courtesy of SCSE's IT office.

## Getting Started
```
.
├── README.md                   // Describes the contents in this folder
├── Makefile                    // Commands to compile and link program
├── bin                         // Executables for this program
├── docs                        // Documentation of this program
├── include                     // Header files
├── obj                         // Compiled objects
├── src                         // Source code files
├── ClientCache                 // Files will be stored here temporarily on client side
└── ServerRemoteFileAccess      // Mounted directory on server side for remote file access
```

### Prerequisites
As this is a project on distributed systems, we will require one machine for the server and at least one machine for the clients.

The build tools `make` and `g++` must be installed to compile this program. Note that these tools are specific to Linux distributions.

To install these tools:
```
sudo apt-get install build-essential
```


### Compilation
To compile this program, run:
```
make
```
To recompile the program, first clear the existing objects and executables by running:
```
make clean
```
And then compiling:
```
make
```

### Execution
After compilation, two executables `ConsoleApp` and `RFAsrv` will be produced, and they are to be executed on the client and server side respectively.

#### Client-side
The client program requires several flags:
- f: Freshness interval in seconds
- s: IPv4 address of server to send requests to
- sp: Port of server to send requests to
- cp: Port of client to receive responses from
- l: Packet loss rate

To run the program:
```
./bin/ConsoleApp -f xxx -s xxx.xxx.xxx.xxx -sp xxxx -cp xxxx -l x.x
```

#### Server-side
The server program requires several flags:
- rmi: Remote method invocation scheme (0 for at least once, 1 for at most once)
- sp: Port of server to receive requests from
- l: Packet loss rate

To run the program:
```
./bin/RFAsrv -rmi x -lr x -sp xxxx
```

## Authors
- Tan Chye Hong, Jordan (U1722016G)
- Er Jia Chin (U1722575L) 
- Lee Chong Yan (U1721687G) 

---

## SECTION BELOW IS FOR INTERNAL REFERENCE ONLY - TO BE DELETED BEFORE SUBMISSION!
Please update the Gantt chart [here](https://entuedu-my.sharepoint.com/:x:/g/personal/jtan396_e_ntu_edu_sg/EYqmrwXfK7NOitmOkLr2iU4BdR_3Xr_MVr1pC5doaXm1Pg?e=htumpf) after assigning yourself a task and upon completion of the task.

https://entuedu-my.sharepoint.com/personal/jtan396_e_ntu_edu_sg/_layouts/15/onedrive.aspx?id=%2Fpersonal%2Fjtan396%5Fe%5Fntu%5Fedu%5Fsg%2FDocuments%2FCZ4013%2DDistributed%2DSystems

Commands
- ./bin/ConsoleApp -f 300 -s 172.21.148.168 -sp 2222 -cp 2221 -l 0.5
- ./bin/RFAsrv -rmi 0 -lr 0 -sp 2222
- scp -r ./bin/* VMuser@172.21.148.168:/home/VMuser/CZ4013-Distributed-Systems/bin/
- scp -r ./bin/* VMuser@172.21.146.188:/home/VMuser/CZ4013-Distributed-Systems/bin/


