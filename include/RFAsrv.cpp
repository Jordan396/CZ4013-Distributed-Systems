  
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
// int get_offset(cJSON *jobjReceived);
// int get_nBytes(cJSON *jobjReceived);
// void get_filepath(cJSON *jobjReceived, char *filepath);
// char* get_toWrite(cJSON *jobjReceived);
// int ReadFile(char* fileName, char echoBuffer[], int nBytes, int startPos);
// int WriteFile(char* filepath, char* toWrite, char *responseContent, int offset); 


// Global variables
int freshnessInterval = 100;
int lossRate = 0;
int timeOut = 300;
int bufferSize = 1024;
int udpDatagramSize = 4096;
int sel;
string serverIP = "172.21.148.168";
string serverPortNo = "2222";
string clientPortNo = "";
bool RMI_SCHEME = true; // at most once if true
int inboundSockFD, outboundSockFD;
sockaddr_in destAddr, sourceAddr;
FileHandler fh;

/* Variables to handle transfer of data */
// std::string request;        /* String response received */
std::string response;

/* Variables for commands */
char filepath[200];

/* Variables to store request and response messages */
std::map<std::string, size_t> requestMap;
std::map<std::string, std::string> responseMap;

/* Variables to store registered clients */
struct RegisteredClient
{
  string address;
  string port;
  string expiration;
} RegisteredClient;

std::map<std::string, std::list<struct RegisteredClient> > monitorMap;

int main(int argc, char *argv[]) {
  string request; 
  char serverBuffer[udpDatagramSize];   // Buffer for echo string
  int recvMsgSize;                  // Size of received message
  string sourceAddress;             // Address of datagram source
  unsigned short sourcePort;        // Port of datagram source

  // testing utils function 
  // TODO: figure out why no binary value @ chin
  cout << utils::loss(50) << endl; 

  // Set up separate thread to monitor expired clients
  pthread_t child_thread;
  int i;
  pthread_create(&child_thread, NULL, monitor_registered_clients, (void*) i);

  // Initialize sockets
  init_sockets();
  int len = sizeof(destAddr);

  for (;;) {  // Run forever
    // Block until receive message from a client
    cout << "Listening..." << endl;
    int n = recvfrom(inboundSockFD, serverBuffer, udpDatagramSize, MSG_WAITALL, ( struct sockaddr *) &destAddr, (socklen_t*)&len); 
    serverBuffer[n] = '\0'; 

    sourceAddress = inet_ntoa(destAddr.sin_addr);
    sourcePort = ntohs(destAddr.sin_port);
    cout << "Received packet from " << sourceAddress << ":" << sourcePort << endl;

    request = serverBuffer;
    cout << "Received message:\n" + request << endl;
    process_request(request);
  }
  return 0;
}

void init_sockets()
{
  // Creating socket file descriptor 
  if ( (inboundSockFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
      perror("socket creation failed"); 
      exit(EXIT_FAILURE); 
  } 
  if ( (outboundSockFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
      perror("socket creation failed"); 
      exit(EXIT_FAILURE); 
  } 

  memset(&destAddr, 0, sizeof(destAddr)); 
  memset(&sourceAddr, 0, sizeof(sourceAddr)); 

  // Filling source information 
  sourceAddr.sin_family    = AF_INET; // IPv4 
  sourceAddr.sin_addr.s_addr = INADDR_ANY; 
  sourceAddr.sin_port = htons((unsigned short) strtoul(serverPortNo.c_str(), NULL, 0)); 
  // Filling destination information 
  destAddr.sin_family    = AF_INET; // IPv4 

  // Bind the socket with the source address 
  socklen_t len = sizeof(sourceAddr);
  if (bind(inboundSockFD, (const struct sockaddr *)&sourceAddr, len) < 0 ) 
  { 
      perror("bind failed"); 
      exit(EXIT_FAILURE); 
  }
  if (getsockname(inboundSockFD, (struct sockaddr *)&sourceAddr, &len) == -1){
    perror("getsockname");
  }
  else {
    serverPortNo = std::to_string(ntohs(sourceAddr.sin_port));
    cout << "Source port number: " + serverPortNo << endl;
  }
}

void *monitor_registered_clients( void *ptr ){
  while (true){
    cout << "Checking for expired clients..." << endl;
    // Get current time
    auto time_now = std::chrono::system_clock::now();
    time_t currentTime = std::chrono::system_clock::to_time_t(time_now);

    // Stack to track expired clients to remove
    std::stack <int> s;

    // Iterate over the map using Iterator till end.
    for (std::map<std::string, std::list <struct RegisteredClient>>::iterator filepathIterator = monitorMap.begin(); filepathIterator != monitorMap.end(); ++filepathIterator){
      // Iterate over all registered clients for file
      for (std::list<struct RegisteredClient>::iterator it = (filepathIterator->second).begin(); it != (filepathIterator->second).end(); ++it){
        struct tm tm;
        strptime((it->expiration).c_str(), "%H:%M:%S", &tm);
        time_t expirationTime = mktime(&tm);
        if (comparetime(expirationTime, currentTime) == -1)
        {
          s.push(std::distance((filepathIterator->second).begin(), it));
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
        std::list<struct RegisteredClient>::iterator it_to_erase;
        std::advance( it_to_erase, s.top() );
        (filepathIterator->second).erase(it_to_erase);
        s.pop();
      }
    }
    // Sleep for 10,000 milliseconds
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  }
}

int comparetime(time_t time1,time_t time2){
 return difftime(time1,time2) > 0.0 ? 1 : -1; 
} 

int send_message(string destAddress, string destPort, string message){
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  destAddr.sin_family    = AF_INET; // IPv4 
  destAddr.sin_addr.s_addr = inet_addr(destAddress.c_str()); 
  destAddr.sin_port = htons((unsigned short) strtoul(destPort.c_str(), NULL, 0));
  sendto(outboundSockFD, message.c_str(), strlen(message.c_str()), 0, (const struct sockaddr *) &destAddr,  sizeof(destAddr)); 
  cout << "Sending message: " + message + " : to " + (char*)inet_ntoa((struct in_addr)destAddr.sin_addr) << endl;
  
  store_response(destAddress, destPort, message);
}

void process_request(string request){
  // Parse request message
  cJSON *jobjReceived;
  string sourceAddress;             // Address of datagram source
  unsigned short sourcePort;        // Port of datagram source
  string destPort;

  jobjReceived = cJSON_CreateObject();
  jobjReceived = cJSON_Parse(request.c_str());
  sourceAddress = inet_ntoa(destAddr.sin_addr);
  sourcePort = ntohs(destAddr.sin_port);
  destPort = get_dest_port(jobjReceived);

  cout << "Processing request..." << endl;
  // Check RMI scheme
  if (RMI_SCHEME && is_request_exists(sourceAddress, destPort, request)){ // at most once - check if request exists
    response = retrieve_response(sourceAddress, destPort);
    send_message(sourceAddress, destPort, response);
  }
  else {
    store_request(sourceAddress, destPort,request);
    // Handle request accordingly
    switch (get_request_code(jobjReceived)){
      case FETCH_LAST_MODIFIED_TIME_CMD:
        cout << "Executing get_last_modified_time command..." << endl;
        execute_fetch_last_modified_time_command(sourceAddress, destPort, jobjReceived);
        break;
      case READ_CMD:
        cout << "Executing read command..." << endl;
        execute_read_command(sourceAddress, destPort, jobjReceived);
        break;
      case WRITE_CMD:
        cout << "Executing write command..." << endl;
        execute_write_command(sourceAddress, destPort, jobjReceived);
        break;
      case REGISTER_CMD:
        cout << "Executing register command..." << endl;
        execute_register_command(sourceAddress, destPort, jobjReceived);
        break;
    }
  }
  cJSON_Delete(jobjReceived);
}

void execute_fetch_last_modified_time_command(string destAddress, string destPort, cJSON *jobjReceived) {
  string pseudo_filepath;
  string actual_filepath;
  time_t last_modified_time;
  char last_modified_time_string[80];

  // Extract parameters from message
  pseudo_filepath = get_filepath(jobjReceived);
  actual_filepath = translate_filepath(pseudo_filepath);
  if (actual_filepath != ""){
    // Debugging
    cout << "Reference to file at: " << actual_filepath << endl;
    if (std::experimental::filesystem::exists(actual_filepath)){
      cout << "File exists." << endl;

      // time_t manipulation
      last_modified_time = get_last_modified_time(actual_filepath.c_str());
      strftime(last_modified_time_string, 20, "%Y-%m-%d %H:%M:%S", gmtime(&last_modified_time));

      // TODO: Integrate with Jia Chin
      cJSON *jobjToSend;
      jobjToSend = cJSON_CreateObject();
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(FETCH_LAST_MODIFIED_TIME_SUCCESS)); 
      cJSON_AddItemToObject(jobjToSend, "LAST_MODIFIED", cJSON_CreateString(last_modified_time_string)); 
      send_message(destAddress, destPort, cJSON_Print(jobjToSend));
      cJSON_Delete(jobjToSend);
    }
    else {
      cJSON *jobjToSend;
      jobjToSend = cJSON_CreateObject();
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(FETCH_LAST_MODIFIED_TIME_FAILURE)); 
      cJSON_AddItemToObject(jobjToSend, "LAST_MODIFIED", cJSON_CreateString("Error reading file.")); 
      send_message(destAddress, destPort, cJSON_Print(jobjToSend));
      cJSON_Delete(jobjToSend);
    }
  }
}

void execute_read_command(string destAddress, string destPort, cJSON *jobjReceived){
  int offset;
  int nBytes;
  int readResult;
  int readStatus;
  string pseudo_filepath;
  string actual_filepath;
  string last_modified_time;
  char readBuffer[bufferSize];
  string content;

  // Extract parameters from message
  pseudo_filepath = get_filepath(jobjReceived);
  offset = get_offset(jobjReceived);
  nBytes = get_nBytes(jobjReceived);
  actual_filepath = translate_filepath(pseudo_filepath); 

  if (actual_filepath != "") {
    // Debugging
    cout << "Reference to file at: " << actual_filepath << endl;
    if (std::experimental::filesystem::exists(actual_filepath)){
      cout << "File exists." << endl;

      // Empty buffer
      memset(readBuffer, 0, bufferSize * (sizeof readBuffer[0]) );

      // ReadFile
      readResult = fh.ReadFile(actual_filepath.c_str(), readBuffer, nBytes, offset);
      readStatus = filehandler_result_to_response_code(readResult);
      content = readBuffer;

      cout << "readResult: " + std::to_string(readResult) << endl;

      if (readStatus == READ_SUCCESS){
        // Send response
        cJSON *jobjToSend;
        jobjToSend = cJSON_CreateObject();
        cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(readStatus)); 
        cJSON_AddItemToObject(jobjToSend, "CONTENT", cJSON_CreateString(content.c_str())); 
        cJSON_AddItemToObject(jobjToSend, "N_BYTES", cJSON_CreateNumber(readResult)); 
        send_message(destAddress, destPort, cJSON_Print(jobjToSend));
        cJSON_Delete(jobjToSend);
        return;
      }
    }
  }
  cJSON *jobjToSend;
  jobjToSend = cJSON_CreateObject();
  cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(READ_FAILURE)); 
  send_message(destAddress, destPort, cJSON_Print(jobjToSend));
  cJSON_Delete(jobjToSend);
  return;
}

void execute_write_command(string destAddress, string destPort, cJSON *jobjReceived){
  int offset;
  int nBytes;
  string pseudo_filepath;
  string actual_filepath;
  string last_modified_time;

  // Extract parameters from message
  pseudo_filepath = get_filepath(jobjReceived);
  offset = get_offset(jobjReceived);
  nBytes = get_nBytes(jobjReceived);
  actual_filepath = translate_filepath(pseudo_filepath); 
  if (actual_filepath != ""){
    // Debugging
    cout << "Reference to file at: " << actual_filepath << endl;
    if (std::experimental::filesystem::exists(actual_filepath)){
      // TODO: Integrate with Jia Chin
      // WriteFile()
      cout << "File exists." << endl;

      // Send response
      cJSON *jobjToSend;
      jobjToSend = cJSON_CreateObject();
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(100)); 
      cJSON_AddItemToObject(jobjToSend, "CONTENT", cJSON_CreateString("Content written!")); 
      send_message(destAddress, destPort, cJSON_Print(jobjToSend));
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
      send_message(destAddress, destPort, cJSON_Print(jobjToSend));
      cJSON_Delete(jobjToSend);
    }
  }
}

void execute_register_command(string destAddress, string destPort, cJSON *jobjReceived){
  string pseudo_filepath;
  string actual_filepath;
  string monitor_duration;
  struct RegisteredClient registeredClient;

  // Extract parameters from message
  pseudo_filepath = get_filepath(jobjReceived);
  int offset = get_offset(jobjReceived);
  int nBytes = get_nBytes(jobjReceived);
  monitor_duration = get_monitor_duration(jobjReceived);

  // Assign registeredClient attributes
  registeredClient.address = destAddress;
  registeredClient.port = destPort;
  registeredClient.expiration = monitor_duration;

  // Extract parameters from message
  actual_filepath = get_filepath(jobjReceived);
  if (actual_filepath != "") {
    // Debugging
    cout << "Reference to file at: " << actual_filepath << endl;
    (monitorMap[actual_filepath]).push_back(registeredClient);

    // TODO: Integrate with Jia Chin
      cJSON *jobjToSend;
      jobjToSend = cJSON_CreateObject();
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(100)); 
      cJSON_AddItemToObject(jobjToSend, "CONTENT", cJSON_CreateString("Client registered")); 
      send_message(destAddress, destPort, cJSON_Print(jobjToSend));
      cJSON_Delete(jobjToSend);
  }
  else {
      // TODO: Integrate with Jia Chin
      cJSON *jobjToSend;
      jobjToSend = cJSON_CreateObject();
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(0)); 
      cJSON_AddItemToObject(jobjToSend, "LAST_MODIFIED", cJSON_CreateString("ERROR: Cannot register client.")); 
      send_message(destAddress, destPort, cJSON_Print(jobjToSend));
      cJSON_Delete(jobjToSend);
    }
}

void update_registered_clients(string filepath){
  std::list <struct RegisteredClient> registeredClientList = monitorMap[filepath];
  for (std::list<struct RegisteredClient>::iterator it = registeredClientList.begin(); it != registeredClientList.end(); ++it){
    // Update registered client
    cJSON *jobjToSend;
    jobjToSend = cJSON_CreateObject();
    cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(100)); 
    send_message(it->address, it->port, cJSON_Print(jobjToSend));
    cJSON_Delete(jobjToSend);
  }
}

bool is_request_exists(string sourceAddress, string destPort, string message){
  std::hash<std::string> str_hash;
  string requestMapKey = sourceAddress + ":" + destPort;
  size_t requestMapValue = str_hash(message);
  return (requestMapValue == requestMap[requestMapKey]);
}

void store_request(string sourceAddress, string destPort, string message){
  cout << "Storing request..." << endl;
  std::hash<std::string> str_hash;
  string requestMapKey = sourceAddress + ":" + destPort;
  size_t requestMapValue = str_hash(message);

  // Debugging
  cout << "requestMapKey: " << requestMapKey << endl;
  cout << "requestMapValue: " << requestMapValue << endl;

  requestMap[requestMapKey] = requestMapValue;
}

void store_response(string sourceAddress, string destPort, string message){
  cout << "Storing response..." << endl;
  string responseMapKey = sourceAddress + ":" + destPort;

  // Debugging
  cout << "responseMapKey: " << responseMapKey << endl;
  cout << "responseMapValue: " << message << endl;

  responseMap[responseMapKey] = message;
}

string retrieve_response(string sourceAddress, string destPort){
  cout << "Retrieving response..." << endl;
  string responseMapKey = sourceAddress + ":" + destPort;

  // Debugging
  cout << "responseMapKey: " << responseMapKey << endl;
  cout << "responseMapValue: " << responseMap[responseMapKey] << endl;
  return responseMap[responseMapKey];
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

string get_filepath(cJSON *jobjReceived)
{
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "RFA_PATH")->valuestring;
}

string get_monitor_duration(cJSON *jobjReceived)
{
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "MONITOR_DURATION")->valuestring;
}

string get_dest_port(cJSON *jobjReceived)
{
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "PORT")->valuestring;
}

char* get_toWrite(cJSON *jobjReceived) {
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "toWrite")->valuestring;
}

string translate_filepath(string pseudo_filepath){
  char rfa_prefix[6];
  string current_path = std::experimental::filesystem::current_path();

  // assign here 
  string actual_filepath = pseudo_filepath;
  strncpy(rfa_prefix, pseudo_filepath.c_str(), 6); // Copy just the "RFA://" portion
  if (strcmp(rfa_prefix, "RFA://") == 0){ 
    // actual_filepath = actual_filepath.replace(0, 6, "../RemoteFileAccess/");
    actual_filepath = current_path + "/ServerRemoteFileAccess/" + actual_filepath.substr(6);
    cout << "actual_filepath: " + actual_filepath << endl;
    return actual_filepath;
  }
  cout << "pseudo_filepath: " + pseudo_filepath << endl;
  return "";
}

time_t get_last_modified_time(const char *path) {
    struct stat attr;
    stat(path, &attr);
    printf("Last modified time: %s", ctime(&attr.st_mtime));
    return attr.st_mtime;
}

int filehandler_result_to_response_code(int result){
  if (result >= 0){
    return READ_SUCCESS;
  }
  return READ_FAILURE;
}

void testing(string s) { 
  cout << s << endl; 
}