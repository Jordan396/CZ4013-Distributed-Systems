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
- f: Freshness interval in milliseconds
- s: IPv4 address of server to send requests to
- sp: Port of server to send requests to
- cp: Port of client to receive responses from
- debug: Option to print out debugging output (1 for yes, 0 for no) 

To run the program:
```
./bin/ConsoleApp -f xxx -s xxx.xxx.xxx.xxx -sp xxxx -cp xxxx -debug x
```

#### Server-side
The server program requires several flags:
- rmi: Remote method invocation scheme (0 for at least once, 1 for at most once)
- sp: Port of server to receive requests from
- slr: Packet loss rate at sending end (out of 100)
- rlr: Packet loss rate at receiving end (out of 100)

To run the program:
```
./bin/RFAsrv -rmi x -slr x -rlr x -sp xxxx
```

## Authors
- Tan Chye Hong, Jordan (U1722016G)
- Er Jia Chin (U1722575L) 
- Lee Chong Yan (U1721687G) 
