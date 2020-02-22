  
/****************************************************************************
 * @author:                                                                 *
 * - Jordan396 <https://github.com/Jordan396/>                              *
 * - leechongyan <https://github.com/leechongyan/>                          *
 * - seaerchin <https://github.com/seaerchin/>                              *
 *                                                                          *
 * This file was heavily adapted from the source code in:                   *
 *   "TCP/IP Sockets in C: Practical Guide for Programmers"                 *
 *   by Michael J. Donahoo and Kenneth L. Calvert                           *
 *   <http://cs.baylor.edu/~donahoo/practical/CSockets/textcode.html>       *
 *                                                                          *
 ****************************************************************************/

 /**
  * @file server.cpp
  * @author Jordan396, leechongyan, seaerchin
  * @date xxx 2020
  * @brief .
  *
  * 
  * TODO:
  * - Integrate readfile with cJSON
  * 
  */

#include "./RFAsrv.h"

/* Function declarations */
int get_client_command_code(cJSON *jobjReceived);
// void readFile(char* filepath, int offset, int nBytes, char* responseContent);
// void writeFile(char* filepath, int offset, int nBytes, char *responseContent);
int get_client_command_code(cJSON *jobjReceived);
int get_offset(cJSON *jobjReceived);
int get_nBytes(cJSON *jobjReceived);
void get_filepath(cJSON *jobjReceived, char *filepath);

// TODO: Integrate the hardcoded variables below with command prompt parser 
// Variables relating to server addressing
char *servAddressHardcode = "172.21.148.168";
unsigned short servPortHardcode = Socket::resolveService("2222", "udp");
bool RMI_SCHEME = true; // at most once if true


/* Variables to handle transfer of data */
std::string request;        /* String response received */
std::string response;

/* Variables for commands */
char filepath[200];
const int BUFFER_SIZE = 255;     // Longest string to echo


/* Variables to store request and response messages */
std::map<std::string, size_t> requestMap;
std::map<std::string, std::string> responseMap;

/* Variables to store registered clients */
struct RegisteredClient
{
  string address;
  unsigned short port;
  string expiration;
} RegisteredClient;
std::map<std::string, std::list <RegisteredClient>> monitorMap;

int main(int argc, char *argv[]) {

  try {
    UDPSocket sock(servPortHardcode);                
  
    char serverBuffer[BUFFER_SIZE];         // Buffer for echo string
    int recvMsgSize;                  // Size of received message
    string sourceAddress;             // Address of datagram source
    unsigned short sourcePort;        // Port of datagram source

    for (;;) {  // Run forever
      // Block until receive message from a client
      cout << "Listening..." << endl;
      recvMsgSize = sock.recvFrom(serverBuffer, BUFFER_SIZE, sourceAddress, sourcePort);
      cout << "Received packet from " << sourceAddress << ":" << sourcePort << endl;
      strncpy(request, serverBuffer, sizeof(serverBuffer));
      
      // sock.sendTo(echoBuffer, recvMsgSize, sourceAddress, sourcePort);

      // Check RMI scheme
      if (RMI_SCHEME){ // at most once - check if request exists
        if (is_request_exists(sourceAddress, sourcePort, request)){ // request already exists
          retrieve_response(sourceAddress, sourcePort, response);
          send_message(sourceAddress, sourcePort, response);
        }
        else { // new request
          process_request(sourceAddress, sourcePort, request);
        }
      }
      else {
        process_request(sourceAddress, sourcePort, request);
      }
    }
  }
  catch (SocketException &e) {
    cerr << e.what() << endl;
    exit(1);
  }
  return 0;
}

int send_message(string destAddress, unsigned short destPort, string message){
  try {
    UDPSocket sock;
    sock.sendTo(message, strlen(message), destAddress, destPort);

  } catch (SocketException &e) {
    cerr << e.what() << endl;
    exit(1);
  }
  return 0;
}

void process_request(string sourceAddress, unsigned short sourcePort, string request){
  // Parse request message
  cJSON *jobjReceived;
  jobjReceived = cJSON_CreateObject();
  jobjReceived = cJSON_Parse(request);
  
  // Handle request accordingly
  switch (get_client_command(sourceAddress, sourcePort, jobjReceived)){
    case READ_CMD:
      cout << "Executing read command..." << endl;
      execute_read_command(jobjReceived);
      break;
    case WRITE_CMD:
      cout << "Executing write command..." << endl;
      execute_write_command(sourceAddress, sourcePort, jobjReceived);
      break;
    case REGISTER_CMD:
      cout << "Executing register command..." << endl;
      execute_register_command(sourceAddress, sourcePort, jobjReceived);
      break;
  }

  cJSON_Delete(jobjReceived);
}

void execute_read_command(string sourceAddress, unsigned short sourcePort, cJSON *jobjReceived){
  int offset;
  int nBytes;
  string filepath;

  // Extract parameters from message
  get_filepath(jobjReceived, filepath);
  offset = get_offset(jobjReceived);
  nBytes = get_nBytes(jobjReceived);

  // TODO: Integrate with Jia Chin
  cJSON *jobjToSend;
  jobjToSend = cJSON_CreateObject();
  cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(0)); 
  cJSON_AddItemToObject(jobjToSend, "CONTENT", cJSON_CreateString("Content read!")); 
  cJSON_AddItemToObject(jobjToSend, "LAST_MODIFIED", cJSON_CreateString("Tiger time.")); 
  send_message(sourceAddress, sourcePort, cJSON_Print(jobjToSend));
  cJSON_Delete(jobjToSend);
}

void execute_write_command( string sourceAddress, unsigned short sourcePort, cJSON *jobjReceived){
  int offset;
  int nBytes;
  string filepath;

  // Extract parameters from message
  get_filepath(jobjReceived, filepath);
  offset = get_offset(jobjReceived);
  nBytes = get_nBytes(jobjReceived);

  // TODO: Write to file
  // WriteFile()

  // Send acknowledgement
  cJSON *jobjToSend;
  jobjToSend = cJSON_CreateObject();
  cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(0)); 
  cJSON_AddItemToObject(jobjToSend, "CONTENT", cJSON_CreateString("Content written!")); 
  send_message(sourceAddress, sourcePort, cJSON_Print(jobjToSend));
  cJSON_Delete(jobjToSend);

  // TODO: Update registered clients in monitorMap
  // update_registered_clients(filepath);
}

// void update_registered_clients(string filepath){
//   // Stack to track expired clients to remove
//   std::stack <int> s;

//   std::list <RegisteredClient> registeredClientList = monitorMap[filepath];
//   for (std::list<RegisteredClient>::iterator it = registeredClientList.begin(); it != registeredClientList.end(); ++it){
//     if(it->expiration < current_time_stamp){
//       s.push(std::distance(registeredClientList.begin(), it));
//     }
//     else{
//       // Update registered client
//       send_message();
//     }
//   }
//   // Remove expired clients
//   while (!s.empty())
//   {
//     registeredClientList.erase( s.pop());
//   }
// }

bool is_request_exists(string sourceAddress, unsigned short sourcePort, string message){
  std::hash<std::string> str_hash;
  string requestMapKey = sourceAddress + ":" + std::to_string(sourcePort);
  size_t requestMapValue = str_hash(message);
  return (requestMapValue == requestMap[requestMapKey])
}

void store_request(string sourceAddress, unsigned short sourcePort, string message){
  std::hash<std::string> str_hash;
  string requestMapKey = sourceAddress + ":" + std::to_string(sourcePort);
  size_t requestMapValue = str_hash(message);

  // Debugging
  cout << "requestMapKey: " << requestMapKey << endl;
  cout << "requestMapValue: " << requestMapValue << endl;

  requestMap[requestMapKey] = requestMapValue;
}

void store_response(string sourceAddress, unsigned short sourcePort, string message){
  string responseMapKey = sourceAddress + ":" + std::to_string(sourcePort);

  // Debugging
  cout << "responseMapKey: " << responseMapKey << endl;
  cout << "responseMapValue: " << message << endl;

  responseMap[responseMapKey] = message;
}

void retrieve_response(string sourceAddress, unsigned short sourcePort, string message){
  string responseMapKey = sourceAddress + ":" + std::to_string(sourcePort);

  strcpy(message, responseMap[responseMapKey]);

  // Debugging
  cout << "responseMapKey: " << responseMapKey << endl;
  cout << "responseMapValue: " << message << endl;
}

/** \copydoc get_client_command_code */
int get_client_command(cJSON *jobjReceived)
{
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "REQUEST_CODE")->valueint;
}

int get_offset(cJSON *jobjReceived)
{
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "offset")->valueint;
}

int get_nBytes(cJSON *jobjReceived)
{
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "nBytes")->valueint;
}

void get_filepath(cJSON *jobjReceived, string filepath)
{
  strcpy(filepath ,cJSON_GetObjectItemCaseSensitive(jobjReceived, "rfaPath")->valuestring);
}

// void readFile(char* filepath, int offset, int nBytes, char *responseContent){
//   cout << "filepath: " << filepath << endl;
//   cout << "offset: " << offset << endl;
//   cout << "nBytes: " << nBytes << endl;
//   strcpy(responseContent,"It's all yours, Jia Chin! Jiayou!");
//   cout << "responseContent: " << responseContent << endl;
// }

// void writeFile(char* filepath, int offset, int nBytes, char *responseContent){
//   cout << "filepath: " << filepath << endl;
//   cout << "offset: " << offset << endl;
//   cout << "nBytes: " << nBytes << endl;
//   strcpy(responseContent,"It's all yours, Jia Chin! Jiayou!");
//   cout << "responseContent: " << responseContent << endl;
// }