  
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
  */

#include "./RFAsrv.h"

/* Function declarations */
// int get_client_command_code(cJSON *jobjReceived);
// void readFile(char* filepath, int offset, int nBytes, char* responseContent);
// void writeFile(char* filepath, int offset, int nBytes, char *responseContent);
// int get_client_command_code(cJSON *jobjReceived);
int get_offset(cJSON *jobjReceived);
int get_nBytes(cJSON *jobjReceived);
void get_filepath(cJSON *jobjReceived, char *filepath);
char* get_toWrite(cJSON *jobjReceived);
// int ReadFile(char* fileName, char echoBuffer[], int nBytes, int startPos);
// int WriteFile(char* filepath, char* toWrite, char *responseContent, int offset); 
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
  
    char serverBuffer[BUFFER_SIZE];   // Buffer for echo string
    int recvMsgSize;                  // Size of received message
    string sourceAddress;             // Address of datagram source
    unsigned short sourcePort;        // Port of datagram source

    pthread_t child_thread;
    int i;

    pthread_create(&child_thread, NULL, monitor_registered_clients, (void*) i);

    for (;;) {  // Run forever
      // Block until receive message from a client
      cout << "Listening..." << endl;
      recvMsgSize = sock.recvFrom(serverBuffer, BUFFER_SIZE, sourceAddress, sourcePort);
      cout << "Received packet from " << sourceAddress << ":" << sourcePort << endl;
      strncpy(request, serverBuffer, sizeof(serverBuffer));

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

void *monitor_registered_clients( void *ptr )(){
  while (true){
    cout << "Checking for expired clients...";
    // Get current time
    auto currentTime = std::chrono::system_clock::now();

    // Stack to track expired clients to remove
    std::stack <int> s;

    // Create a map iterator and point to beginning of map
    std::map<std::string, std::list <RegisteredClient>>::iterator filepathIterator = monitorMap.begin();

    // Iterate over the map using Iterator till end.
    while (filepathIterator != monitorMap.end()) {
      // Iterate over all registered clients for file
      for (std::list<RegisteredClient>::iterator it = filepathIterator.begin(); it != filepathIterator.end(); ++it){
        if(it->expiration < currentTime){
          s.push(std::distance(registeredClientList.begin(), it));
        }
        else{
          // Update registered client
          cJSON *jobjToSend;
          jobjToSend = cJSON_CreateObject();
          cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(100)); 
          send_message(it->address, it->port, cJSON_Print(jobjToSend));
          cJSON_Delete(jobjToSend);
        }
      }
      // Remove expired clients
      while (!s.empty())
      {
        registeredClientList.erase(s.pop());
      }
    }
    // Sleep for 10,000 milliseconds
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  }
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
  switch (get_client_command(jobjReceived)){
    case GET_LAST_MODIFIED_TIME:
      cout << "Executing get_last_modified_time command..." << endl;
      execute_get_last_modified_time_command(sourceAddress, sourcePort, jobjReceived);
      break;
    case READ_CMD:
      cout << "Executing read command..." << endl;
      execute_read_command(sourceAddress, sourcePort, jobjReceived);
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

void execute_get_last_modified_time_command(string sourceAddress, unsigned short sourcePort, cJSON *jobjReceived) {
  string pseudo_filepath;
  string actual_filepath;
  string last_modified_time;

  // Extract parameters from message
  get_filepath(jobjReceived, pseudo_filepath);
  if (translate_filepath(pseudo_filepath, actual_filepath)){
    // Debugging
    cout << "Reference to file at: " << actual_filepath << endl;
    if (std::experimental::filesystem::exists(actual_filepath)){
      get_last_modified_time(actual_filepath, last_modified_time);

      // TODO: Integrate with Jia Chin
      cJSON *jobjToSend;
      jobjToSend = cJSON_CreateObject();
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(0)); 
      cJSON_AddItemToObject(jobjToSend, "LAST_MODIFIED", cJSON_CreateString(last_modified_time)); 
      send_message(sourceAddress, sourcePort, cJSON_Print(jobjToSend));
      cJSON_Delete(jobjToSend);
    }
    else {
      // TODO: Integrate with Jia Chin
      cJSON *jobjToSend;
      jobjToSend = cJSON_CreateObject();
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(0)); 
      cJSON_AddItemToObject(jobjToSend, "LAST_MODIFIED", cJSON_CreateString("Error reading file.")); 
      send_message(sourceAddress, sourcePort, cJSON_Print(jobjToSend));
      cJSON_Delete(jobjToSend);
    }
  }
}

void execute_read_command(string sourceAddress, unsigned short sourcePort, cJSON *jobjReceived){
  int offset;
  int nBytes;
  string pseudo_filepath;
  string actual_filepath;
  string last_modified_time;

  // Extract parameters from message
  get_filepath(jobjReceived, pseudo_filepath);
  offset = get_offset(jobjReceived);
  nBytes = get_nBytes(jobjReceived);
  if (translate_filepath(pseudo_filepath, actual_filepath)){
    // Debugging
    cout << "Reference to file at: " << actual_filepath << endl;
    if (std::experimental::filesystem::exists(actual_filepath)){
      // TODO: Integrate with Jia Chin
      // ReadFile()

      // Send response
      cJSON *jobjToSend;
      jobjToSend = cJSON_CreateObject();
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(100)); 
      cJSON_AddItemToObject(jobjToSend, "CONTENT", cJSON_CreateString("Content read!")); 
      send_message(sourceAddress, sourcePort, cJSON_Print(jobjToSend));
      cJSON_Delete(jobjToSend);
    }
    else {
      // TODO: Integrate with Jia Chin
      cJSON *jobjToSend;
      jobjToSend = cJSON_CreateObject();
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(0)); 
      cJSON_AddItemToObject(jobjToSend, "LAST_MODIFIED", cJSON_CreateString("Error reading file.")); 
      send_message(sourceAddress, sourcePort, cJSON_Print(jobjToSend));
      cJSON_Delete(jobjToSend);
    }
  }
}

void execute_write_command(string sourceAddress, unsigned short sourcePort, cJSON *jobjReceived){
  int offset;
  int nBytes;
  string pseudo_filepath;
  string actual_filepath;
  string last_modified_time;

  // Extract parameters from message
  get_filepath(jobjReceived, pseudo_filepath);
  offset = get_offset(jobjReceived);
  nBytes = get_nBytes(jobjReceived);
  if (translate_filepath(pseudo_filepath, actual_filepath)){
    // Debugging
    cout << "Reference to file at: " << actual_filepath << endl;
    if (std::experimental::filesystem::exists(actual_filepath)){
      // TODO: Integrate with Jia Chin
      // WriteFile()

      // Send response
      cJSON *jobjToSend;
      jobjToSend = cJSON_CreateObject();
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(100)); 
      cJSON_AddItemToObject(jobjToSend, "CONTENT", cJSON_CreateString("Content written!")); 
      send_message(sourceAddress, sourcePort, cJSON_Print(jobjToSend));
      cJSON_Delete(jobjToSend);

      // TODO: Update registered clients in monitorMap
      update_registered_clients(pseudo_filepath);
    }
    else {
      // TODO: Integrate with Jia Chin
      cJSON *jobjToSend;
      jobjToSend = cJSON_CreateObject();
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(0)); 
      cJSON_AddItemToObject(jobjToSend, "LAST_MODIFIED", cJSON_CreateString("Error reading file.")); 
      send_message(sourceAddress, sourcePort, cJSON_Print(jobjToSend));
      cJSON_Delete(jobjToSend);
    }
  }
}

void execute_register_command(string sourceAddress, unsigned short sourcePort, cJSON *jobjReceived){
  string pseudo_filepath;
  string actual_filepath;
  string monitor_duration;
  RegisteredClient registeredClient;

  // Extract parameters from message
  get_filepath(jobjReceived, pseudo_filepath);
  int offset = get_offset(jobjReceived);
  int nBytes = get_nBytes(jobjReceived);
  get_monitor_duration(jobjReceived, monitor_duration);

  // Assign registeredClient attributes
  registeredClient.address = sourceAddress;
  registeredClient.port = sourcePort;
  registeredClient.expiration = monitor_duration;

  // Extract parameters from message
  get_filepath(jobjReceived, filepath);
  if (translate_filepath(pseudo_filepath, actual_filepath)){
    // Debugging
    cout << "Reference to file at: " << actual_filepath << endl;
    (monitorMap[actual_filepath]).push_back(registeredClient);

    // TODO: Integrate with Jia Chin
      cJSON *jobjToSend;
      jobjToSend = cJSON_CreateObject();
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(100)); 
      cJSON_AddItemToObject(jobjToSend, "CONTENT", cJSON_CreateString("Client registered")); 
      send_message(sourceAddress, sourcePort, cJSON_Print(jobjToSend));
      cJSON_Delete(jobjToSend);
  }
  else {
      // TODO: Integrate with Jia Chin
      cJSON *jobjToSend;
      jobjToSend = cJSON_CreateObject();
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(0)); 
      cJSON_AddItemToObject(jobjToSend, "LAST_MODIFIED", cJSON_CreateString("ERROR: Cannot register client.")); 
      send_message(sourceAddress, sourcePort, cJSON_Print(jobjToSend));
      cJSON_Delete(jobjToSend);
    }
}

void update_registered_clients(string filepath){
  std::list <RegisteredClient> registeredClientList = monitorMap[filepath];
  for (std::list<RegisteredClient>::iterator it = registeredClientList.begin(); it != registeredClientList.end(); ++it){
    // Update registered client
    cJSON *jobjToSend;
    jobjToSend = cJSON_CreateObject();
    cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(100)); 
    send_message(it->address, it->port, cJSON_Print(jobjToSend));
    cJSON_Delete(jobjToSend);
  }
}

bool is_request_exists(string sourceAddress, unsigned short sourcePort, string message){
  std::hash<std::string> str_hash;
  string requestMapKey = sourceAddress + ":" + std::to_string(sourcePort);
  size_t requestMapValue = str_hash(message);
  return (requestMapValue == requestMap[requestMapKey]);
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

/** \copydoc get_request_code */
int get_request_code(cJSON *jobjReceived)
{
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "REQUEST_CODE")->valueint;
}

int get_offset(cJSON *jobjReceived)
{
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "OFFSET")->valueint;
}

int get_nBytes(cJSON *jobjReceived)
{
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "N_BYTES")->valueint;
}

void get_filepath(cJSON *jobjReceived, string filepath)
{
  strcpy(filepath ,cJSON_GetObjectItemCaseSensitive(jobjReceived, "RFA_PATH")->valuestring);
}

void get_monitor_duration(cJSON *jobjReceived, string monitor_duration)
{
  strcpy(monitor_duration ,cJSON_GetObjectItemCaseSensitive(jobjReceived, "MONITOR_DURATION")->valuestring);
}

char* get_toWrite(cJSON *jobjReceived) {
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "toWrite")->valuestring;
}

bool translate_filepath(string pseudo_filepath, string actual_filepath){
  string rfa_prefix;

  strcpy(actual_filepath, pseudo_filepath);
  strncpy(rfa_prefix, pseudo_filepath, 6); // Copy just the "RFA://" portion
  if (strcmp(rfa_prefix, "RFA://") == 0){
    actual_filepath.replace(0, 6, "../RemoteFileAccess/");
    return true;
  }
  return false;
}

void get_last_modified_time(char *path, string last_modified_time) {
    struct stat attr;
    stat(path, &attr);
    printf("Last modified time: %s", ctime(&attr.st_mtime));
    strcpy(last_modified_time, ctime(&attr.st_mtime));
}

