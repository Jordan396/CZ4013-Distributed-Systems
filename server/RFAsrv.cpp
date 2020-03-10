  
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
int ReadFile(char* fileName, char echoBuffer[], int nBytes, int startPos);
int WriteFile(char* filepath, char* toWrite, char *responseContent, int offset); 
// TODO: Integrate the hardcoded variables below with command prompt parser 
// Variables relating to server addressing
char *servAddressHardcode = "172.21.148.168";
unsigned short servPortHardcode = Socket::resolveService("2222", "udp");
bool RMI_SCHEME = true; // at most once if true

<<<<<<< HEAD
const int ERR_FILE_NOT_EXIST = -1;
const int ERR_MEMORY_INSUFFICIENT = -2; 
const int ERR_READ = -3;
const int ERR_OFFSET_OVERFLOW = -4; 
const int ERR_NEGATIVE_OFFSET = -5; 
const int ERR_WRITE = -6;
const int BUFFER_SIZE = 255;     // Longest string to echo
=======
>>>>>>> 42790c57c7b123ae9cafe6e27080c9827c97d350

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
    cout << "Checking for expired clients..."
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
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(100); 
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
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(100); 
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
  offset = get_offset(jobjReceived);
  nBytes = get_nBytes(jobjReceived);
  get_monitor_duration(jobjReceived, monitor_duration)

  // Assign registeredClient attributes
  registeredClient.address = sourceAddress;
  registeredClient.port = sourcePort;
  registeredClient.expiration = monitor_duration;

  // Extract parameters from message
  get_filepath(jobjReceived, filepath);
  if (translate_filepath(pseudo_filepath, actual_filepath)){
    // Debugging
    cout << "Reference to file at: " << actual_filepath << endl;
    (monitorMap[actual_filepath]).push_back(registeredClient)

    // TODO: Integrate with Jia Chin
      cJSON *jobjToSend;
      jobjToSend = cJSON_CreateObject();
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE", cJSON_CreateNumber(100); 
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
}

// readfile is for use by the server, reads from a given file to a standard writer and returns number of bytes read 
// assumption made is that we either specify FULL file path or it exists in current directory where server is executing 
int ReadFile(char* fileName, char echoBuffer[], int nBytes, int startPos = 0) { // we write to a buffer 
  // file opening logic can be abstracted away for reuse
  // first we check if file exists 
  FILE * pFile; 

  /*
  int length = fileName.length(); 
  char char_array[length + 1];
  strcpy(char_array, fileName.c_str()); 
  pFile = fopen(char_array, "rb");
  */
  
  pFile = fopen(fileName, "rb");
  if (pFile == NULL) { // file requested does not exist, we return error back to client 
      sprintf (echoBuffer, "%s", "File does not exist"); 
      return ERR_FILE_NOT_EXIST; // server calling this function has to check err code 
  }

  long lsize;
  fseek (pFile, 0, SEEK_END);   
  lsize = ftell (pFile); // get size of the file 

  if (lsize >  BUFFER_SIZE) { // no memory to allocate buffer: return error code to client 
      sprintf (echoBuffer, "%s", "Memory error"); 
      return ERR_MEMORY_INSUFFICIENT; 
    }

  // check whether >= 0
  if (startPos < 0) { 
    sprintf(echoBuffer, "%s", "The required starting position is less than 0");
    return ERR_NEGATIVE_OFFSET;
  } 

  // check whether startPos > maxlength 
  if (startPos > lsize) { 
    sprintf(echoBuffer, "%s", "The specified offset is greater than the length of the file");
    return ERR_OFFSET_OVERFLOW;
  }
  
  fseek(pFile, startPos, SEEK_SET);
  size_t result;
  
  // no checking of nBytes
  result = fread (echoBuffer, 1, nBytes, pFile); // pFile advanced to startPos 
  if (result != lsize) {
    sprintf (echoBuffer, "%s", "Reading error");  
    return ERR_READ; 
    }
  fclose (pFile);
  return result; 
}

// writefile will write to a file pointed at by filepath at offset 0 
int WriteFile(char* filepath, char* toWrite, char *responseContent, int offset = 0) {
  // first we check if file exists 
  FILE * pFile; 

  pFile = fopen(filepath, "r+");
  if (pFile == NULL) { // file requested does not exist, we return error back to client 
      strcpy(responseContent, "File does not exist"); 
      return ERR_FILE_NOT_EXIST; // server calling this function has to check err code 
  }

  long lsize;
  fseek (pFile, 0, SEEK_END);   
  lsize = ftell (pFile); // get size of the file 
  cout << "Size of the file is " << lsize << endl; 

  // check whether startPos > maxlength 
  if (offset > lsize) { 
    strcpy(responseContent, "Starting offset exceeded file size! Please use negative indexing to start writing from the end!");
    return ERR_OFFSET_OVERFLOW; 
  }

  // check whether >= 0
  if (offset < 0) { 
    offset = offset + lsize + 1; 
  } 

  size_t length = strlen(toWrite); // get number of elements from array pointer 
  cout << "length is " << length << endl; 
  // overwrite with toWrite + [offset:] of original file 
  char originalFile [lsize + length]; // allocate length of string to be written + filesize to ensure capacity
  fseek(pFile, offset, SEEK_SET);
  strcpy(originalFile, toWrite);
  char temp[lsize - offset]; // holds off set till end 
  fread(temp, 1, lsize - offset, pFile); 
  strcat(originalFile, temp);
  fseek(pFile, offset, SEEK_SET);
  int written = fwrite(originalFile, 1, (lsize-offset)+length, pFile);
  if (written != (lsize-offset)+length) {
    strcpy(responseContent, "Writing error");
    return ERR_WRITE;
  } else {
    strcpy(responseContent, originalFile);
  }

  fclose (pFile);
<<<<<<< HEAD
  return written; 
}

// check if file exists - if yes, overwrite else return error 
int ClearFile(char* filepath, char *responseContent) {
  FILE * pFile; 

  pFile = fopen(filepath, "r");
  if (pFile == NULL) { // file requested does not exist, we return error back to client 
      strcpy(responseContent, "File does not exist"); 
      return ERR_FILE_NOT_EXIST; // server calling this function has to check err code 
  }
  fclose(pFile);

  // file exists, write 
  pFile = fopen(filepath, "w");
  fclose(pFile);
  return 1; 
}
=======
  return 0; 
}
>>>>>>> 42790c57c7b123ae9cafe6e27080c9827c97d350
