/**
 * @file RFAsrv.cpp
 * @author Jordan396, leechongyan, seaerchin
 * @date 13 April 2020
 * @brief Implementation code for RFAsrv.cpp
 */

#include "RFAsrv.h"

// Global variables
int RMI_SCHEME; // at most once if 1, at lease once if 0
int freshnessInterval = 100;
int timeOut = 300;
int bufferSize = 1024;
int udpDatagramSize = 4096;
int sel;
int lossRate;
int debugMode = 1;

// Variables to handle data transfer
int inboundSockFD, outboundSockFD;
sockaddr_in destAddr, sourceAddr;
string serverPortNo;
string clientPortNo;

// Filehandler class
FileHandler fh;

/* Variables to handle transfer of data */
std::string request;
std::string response;

/* Variables for commands */
char filepath[200];

/* Variables to store request and response messages */
std::map<std::string, size_t> requestMap;
std::map<std::string, std::string> responseMap;

/* Variables to store clients to monitor */
std::map<std::string, std::list<struct RegisteredClient>> monitorMap;

int main(int argc, char *argv[])
{
  char serverBuffer[udpDatagramSize]; // Buffer for echo string
  int recvMsgSize;                    // Size of received message
  string sourceAddress;               // Address of datagram source
  unsigned short sourcePort;          // Port of datagram source

  // Initialization of cli arguments
  if (argc < 9)
  {
    perror("Insufficient arguments entered.");
    exit(EXIT_FAILURE);
  }
  else
  {
    for (int i = 1; i < argc; i += 2)
    {
      string s1(argv[i]);
      if (s1 == "-rmi")
      {
        RMI_SCHEME = atoi(argv[i + 1]);
      }
      else if (s1 == "-lr")
      {
        lossRate = atoi(argv[i + 1]);
      }
      else if (s1 == "-sp")
      {
        string s2(argv[i + 1]);
        serverPortNo = s2;
      }
      else if (s1 == "-debug")
      {
        debugMode = atoi(argv[i + 1]);
      }
    }
  }

  // Set up separate thread to monitor expired clients
  int i;
  pthread_t child_thread;
  pthread_create(&child_thread, NULL, monitor_registered_clients, &i);

  // Initialize sockets
  init_sockets();
  int len = sizeof(destAddr);

  for (;;)
  { // Run forever
    // Block until receive message from a client
    Debug::msg("Listening...");
    int n = recvfrom(inboundSockFD, serverBuffer, udpDatagramSize, MSG_WAITALL,
                     (struct sockaddr *)&destAddr, (socklen_t *)&len);
    serverBuffer[n] = '\0';

    sourceAddress = inet_ntoa(destAddr.sin_addr);
    sourcePort = ntohs(destAddr.sin_port);

    Debug::msg("Received packet from " + sourceAddress + ":" + to_string(sourcePort));
    request = serverBuffer;

    // Process only if true
    if (utils::loss(lossRate))
    {
        Debug::msg("Packet loss simulation: Accepting packet.");
      process_request(request);
    }
    else
    {
        Debug::msg("Packet loss simulation: Dropping packet.");
    }
  }
  return 0;
}

void init_sockets()
{
  // Creating socket file descriptor
  if ((inboundSockFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }
  if ((outboundSockFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&destAddr, 0, sizeof(destAddr));
  memset(&sourceAddr, 0, sizeof(sourceAddr));

  // Filling source information
  sourceAddr.sin_family = AF_INET; // IPv4
  sourceAddr.sin_addr.s_addr = INADDR_ANY;
  sourceAddr.sin_port =
      htons((unsigned short)strtoul(serverPortNo.c_str(), NULL, 0));
  // Filling destination information
  destAddr.sin_family = AF_INET; // IPv4

  // Bind the socket with the source address
  socklen_t len = sizeof(sourceAddr);
  if (bind(inboundSockFD, (const struct sockaddr *)&sourceAddr, len) < 0)
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  if (getsockname(inboundSockFD, (struct sockaddr *)&sourceAddr, &len) == -1)
  {
    perror("getsockname");
  }
  else
  {
    serverPortNo = std::to_string(ntohs(sourceAddr.sin_port));
  }
}

void send_message(string destAddress, string destPort, string message)
{
  destAddr.sin_family = AF_INET; // IPv4
  destAddr.sin_addr.s_addr = inet_addr(destAddress.c_str());
  destAddr.sin_port = htons((unsigned short)strtoul(destPort.c_str(), NULL, 0));
  sendto(outboundSockFD, message.c_str(), strlen(message.c_str()), 0,
         (const struct sockaddr *)&destAddr, sizeof(destAddr));
  // cout << "Sending message: " + message + " : to " +
  //             (char *)inet_ntoa((struct in_addr)destAddr.sin_addr)
  //      << endl;
  store_response(destAddress, destPort, message);
}

void process_request(string request)
{
  // Parse request message
  cJSON *jobjReceived;
  unsigned short sourcePort; // Port of datagram source
  string sourceAddress;      // Address of datagram source
  string destPort;

  jobjReceived = cJSON_CreateObject();
  jobjReceived = cJSON_Parse(request.c_str());
  sourceAddress = inet_ntoa(destAddr.sin_addr);
  sourcePort = ntohs(destAddr.sin_port);
  destPort = get_dest_port(jobjReceived);

  // Check RMI scheme
  if ((RMI_SCHEME == 1) &&
      is_request_exists(sourceAddress, destPort,
                        request))
  { // at most once - check if request exists
    response = retrieve_response(sourceAddress, destPort);
    send_message(sourceAddress, destPort, response);
  }
  else
  {
    store_request(sourceAddress, destPort, request);
    cJSON_AddItemToObject(jobjReceived, "RESPONSE_ID",
                          cJSON_CreateNumber(request.size()));
    // Handle request accordingly
    switch (get_request_code(jobjReceived))
    {
    case FETCH_LAST_MODIFIED_TIME_CMD:
        Debug::msg("Executing get_last_modified_time command...");
      execute_fetch_last_modified_time_command(sourceAddress, destPort,
                                               jobjReceived);
      break;
    case READ_CMD:
        Debug::msg("Executing read command...");
      execute_read_command(sourceAddress, destPort, jobjReceived);
      break;
    case WRITE_CMD:
        Debug::msg("Executing write command...");
      execute_write_command(sourceAddress, destPort, jobjReceived);
      break;
    case REGISTER_CMD:
        Debug::msg("Executing register command...");
      execute_register_command(sourceAddress, destPort, jobjReceived);
      break;
    case CLEAR_FILE_CMD:
        Debug::msg("Executing clear file command...");
      execute_clear_file_command(sourceAddress, destPort, jobjReceived);
      break;
    }
  }
  cJSON_Delete(jobjReceived);
}

void execute_fetch_last_modified_time_command(string destAddress,
                                              string destPort,
                                              cJSON *jobjReceived)
{
  string pseudo_filepath;
  string actual_filepath;
  time_t last_modified_time;
  char last_modified_time_string[80];

  // Extract parameters from message
  pseudo_filepath = get_filepath(jobjReceived);
  actual_filepath = translate_filepath(pseudo_filepath);
  if (actual_filepath != "")
  {
    if (std::experimental::filesystem::exists(actual_filepath))
    {
      // time_t manipulation
      last_modified_time = get_last_modified_time(actual_filepath.c_str());
      strftime(last_modified_time_string, 20, "%Y-%m-%d %H:%M:%S",
               gmtime(&last_modified_time));

      // Send response
      cJSON *jobjToSend;
      jobjToSend = cJSON_CreateObject();
      cJSON_AddItemToObject(
          jobjToSend, "RESPONSE_CODE",
          cJSON_CreateNumber(FETCH_LAST_MODIFIED_TIME_SUCCESS));
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_ID",
                            cJSON_CreateNumber(get_response_id(jobjReceived)));
      cJSON_AddItemToObject(jobjToSend, "LAST_MODIFIED",
                            cJSON_CreateString(last_modified_time_string));
      send_message(destAddress, destPort, cJSON_Print(jobjToSend));
      cJSON_Delete(jobjToSend);
    }
    else
    {
      cJSON *jobjToSend;
      jobjToSend = cJSON_CreateObject();
      cJSON_AddItemToObject(
          jobjToSend, "RESPONSE_CODE",
          cJSON_CreateNumber(FETCH_LAST_MODIFIED_TIME_FAILURE));
      cJSON_AddItemToObject(jobjToSend, "LAST_MODIFIED",
                            cJSON_CreateString("Error reading file."));
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_ID",
                            cJSON_CreateNumber(get_response_id(jobjReceived)));
      send_message(destAddress, destPort, cJSON_Print(jobjToSend));
      cJSON_Delete(jobjToSend);
    }
  }
}

void execute_read_command(string destAddress, string destPort,
                          cJSON *jobjReceived)
{
  int offset;
  int nBytes;
  int readResult;
  int readStatus;
  char readBuffer[bufferSize];
  string pseudo_filepath;
  string actual_filepath;
  string last_modified_time;
  string content;

  // Extract parameters from message
  pseudo_filepath = get_filepath(jobjReceived);
  offset = get_offset(jobjReceived);
  nBytes = get_nBytes(jobjReceived);
  actual_filepath = translate_filepath(pseudo_filepath);

  if (actual_filepath != "")
  {
    if (std::experimental::filesystem::exists(actual_filepath))
    {
      // Empty buffer
      memset(readBuffer, 0, bufferSize * (sizeof readBuffer[0]));

      // ReadFile
      readResult =
          fh.ReadFile(actual_filepath.c_str(), readBuffer, nBytes, offset);
      readStatus = fh_read_status(readResult);
      content = readBuffer;

      if (readStatus == READ_SUCCESS)
      {
        // Send response
        cJSON *jobjToSend;
        jobjToSend = cJSON_CreateObject();
        cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE",
                              cJSON_CreateNumber(readStatus));
        cJSON_AddItemToObject(jobjToSend, "CONTENT",
                              cJSON_CreateString(content.c_str()));
        cJSON_AddItemToObject(jobjToSend, "N_BYTES",
                              cJSON_CreateNumber(readResult));
        cJSON_AddItemToObject(
            jobjToSend, "RESPONSE_ID",
            cJSON_CreateNumber(get_response_id(jobjReceived)));
        send_message(destAddress, destPort, cJSON_Print(jobjToSend));
        cJSON_Delete(jobjToSend);
        return;
      }
    }
  }
  cJSON *jobjToSend;
  jobjToSend = cJSON_CreateObject();
  cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE",
                        cJSON_CreateNumber(READ_FAILURE));
  cJSON_AddItemToObject(jobjToSend, "RESPONSE_ID",
                        cJSON_CreateNumber(get_response_id(jobjReceived)));
  send_message(destAddress, destPort, cJSON_Print(jobjToSend));
  cJSON_Delete(jobjToSend);
  return;
}

void execute_write_command(string destAddress, string destPort,
                           cJSON *jobjReceived)
{
  int offset;
  int nBytes;
  int writeResult;
  int writeStatus;
  string content;
  string pseudo_filepath;
  string actual_filepath;

  // Extract parameters from message
  pseudo_filepath = get_filepath(jobjReceived);
  offset = get_offset(jobjReceived);
  actual_filepath = translate_filepath(pseudo_filepath);
  content = get_content(jobjReceived);

  if (actual_filepath != "")
  {
    if (std::experimental::filesystem::exists(actual_filepath))
    {
      // WriteFile
      writeResult =
          fh.WriteFile(actual_filepath.c_str(), content.c_str(), offset);
      writeStatus = fh_write_status(writeResult);

      if (writeStatus == WRITE_SUCCESS)
      {
        // Send response
        cJSON *jobjToSend;
        jobjToSend = cJSON_CreateObject();
        cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE",
                              cJSON_CreateNumber(writeStatus));
        cJSON_AddItemToObject(
            jobjToSend, "RESPONSE_ID",
            cJSON_CreateNumber(get_response_id(jobjReceived)));
        send_message(destAddress, destPort, cJSON_Print(jobjToSend));
        cJSON_Delete(jobjToSend);
        return;
      }
    }
  }
  cJSON *jobjToSend;
  jobjToSend = cJSON_CreateObject();
  cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE",
                        cJSON_CreateNumber(WRITE_FAILURE));
  cJSON_AddItemToObject(jobjToSend, "RESPONSE_ID",
                        cJSON_CreateNumber(get_response_id(jobjReceived)));
  send_message(destAddress, destPort, cJSON_Print(jobjToSend));
  cJSON_Delete(jobjToSend);
  return;
}

void execute_register_command(string destAddress, string destPort,
                              cJSON *jobjReceived)
{
  string pseudo_filepath;
  string actual_filepath;
  string monitor_duration;
  struct RegisteredClient registeredClient;

  // Extract parameters from message
  pseudo_filepath = get_filepath(jobjReceived);
  monitor_duration = get_monitor_duration(jobjReceived);
  actual_filepath = translate_filepath(pseudo_filepath);

  // Get current and expiration time
  time_t time_now =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  time_t expiration_time = time_now + stoi(monitor_duration);

  registeredClient.address = destAddress;
  registeredClient.port = destPort;
  registeredClient.registerTime = time_now;
  registeredClient.expirationTime = expiration_time;

  if (actual_filepath != "")
  {
    if (std::experimental::filesystem::exists(actual_filepath))
    {
      (monitorMap[actual_filepath]).push_back(registeredClient);

      cJSON *jobjToSend;
      jobjToSend = cJSON_CreateObject();
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE",
                            cJSON_CreateNumber(MONITOR_SUCCESS));
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_ID",
                            cJSON_CreateNumber(get_response_id(jobjReceived)));
      send_message(destAddress, destPort, cJSON_Print(jobjToSend));
      cJSON_Delete(jobjToSend);
      return;
    }
  }

  // Send response
  cJSON *jobjToSend;
  jobjToSend = cJSON_CreateObject();
  cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE",
                        cJSON_CreateNumber(MONITOR_FAILURE));
  cJSON_AddItemToObject(jobjToSend, "RESPONSE_ID",
                        cJSON_CreateNumber(get_response_id(jobjReceived)));
  send_message(destAddress, destPort, cJSON_Print(jobjToSend));
  cJSON_Delete(jobjToSend);
  return;
}

void execute_clear_file_command(string destAddress, string destPort,
                                cJSON *jobjReceived)
{
  string pseudo_filepath;
  string actual_filepath;

  // Extract parameters from message
  pseudo_filepath = get_filepath(jobjReceived);
  actual_filepath = translate_filepath(pseudo_filepath);

  if (actual_filepath != "")
  {
    if (std::experimental::filesystem::exists(actual_filepath))
    {
      fh.ClearFile(actual_filepath.c_str());

      cJSON *jobjToSend;
      jobjToSend = cJSON_CreateObject();
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE",
                            cJSON_CreateNumber(CLEAR_FILE_SUCCESS));
      cJSON_AddItemToObject(jobjToSend, "RESPONSE_ID",
                            cJSON_CreateNumber(get_response_id(jobjReceived)));
      send_message(destAddress, destPort, cJSON_Print(jobjToSend));
      cJSON_Delete(jobjToSend);
      return;
    }
  }

  // Send response
  cJSON *jobjToSend;
  jobjToSend = cJSON_CreateObject();
  cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE",
                        cJSON_CreateNumber(CLEAR_FILE_FAILURE));
  cJSON_AddItemToObject(jobjToSend, "RESPONSE_ID",
                        cJSON_CreateNumber(get_response_id(jobjReceived)));
  send_message(destAddress, destPort, cJSON_Print(jobjToSend));
  cJSON_Delete(jobjToSend);
  return;
}

void *monitor_registered_clients(void *ptr)
{
  while (true)
  {
    // Get current time
    auto time_now = std::chrono::system_clock::now();
    time_t currentTime = std::chrono::system_clock::to_time_t(time_now);

    // Stack to track expired clients to remove
    std::stack<int> s;

    int debug_counter = 0;

    // Iterate over the map using Iterator till end.
    for (std::map<std::string, std::list<struct RegisteredClient>>::iterator
             filepathIterator = monitorMap.begin();
         filepathIterator != monitorMap.end(); ++filepathIterator)
    {
      // Get last modified time of filepath, which is the key of the iterator
      time_t last_modified_time =
          get_last_modified_time((filepathIterator->first).c_str());

      // Iterate over all registered clients for file
      for (std::list<struct RegisteredClient>::iterator it =
               (filepathIterator->second).begin();
           it != (filepathIterator->second).end(); ++it)
      {

        // Monitor duration expired
        if (comparetime(currentTime, it->expirationTime) == 1)
        {
            Debug::msg(it->address + ":" + it->port + " monitor duration expired.");
          // expire_registered_client(it->address, it->port);
          s.push(std::distance((filepathIterator->second).begin(), it));
        }
        else
        { // Monitor duration not expired
          // Last modified time greater that register time
          if (comparetime(last_modified_time, it->registerTime) == 1)
          {
            // Update registered client
            update_registered_client(it->address, it->port);
            (it->registerTime) = last_modified_time + 1;
          }
        }
      }
      // Remove expired clients from list of registered clients
      std::list<struct RegisteredClient>::iterator it_to_erase = (filepathIterator->second).begin();
      while (!s.empty())
      {
        std::advance(it_to_erase, s.top());
        (filepathIterator->second).erase(it_to_erase);
        s.pop();
      }
    }
    // Sleep for 1,000 milliseconds
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}

void update_registered_client(string sourceAddress, string destPort)
{
  cJSON *jobjToSend;
  jobjToSend = cJSON_CreateObject();
  cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE",
                        cJSON_CreateNumber(MONITOR_UPDATE));
  send_message(sourceAddress, destPort, cJSON_Print(jobjToSend));
  cJSON_Delete(jobjToSend);
  return;
}

void expire_registered_client(string sourceAddress, string destPort)
{
  cJSON *jobjToSend;
  jobjToSend = cJSON_CreateObject();
  cJSON_AddItemToObject(jobjToSend, "RESPONSE_CODE",
                        cJSON_CreateNumber(MONITOR_EXPIRED));
  send_message(sourceAddress, destPort, cJSON_Print(jobjToSend));
  cJSON_Delete(jobjToSend);
  return;
}

bool is_request_exists(string sourceAddress, string destPort, string message)
{
  std::hash<std::string> str_hash;
  string requestMapKey = sourceAddress + ":" + destPort;
  size_t requestMapValue = str_hash(message);
  return (requestMapValue == requestMap[requestMapKey]);
}

void store_request(string sourceAddress, string destPort, string message)
{
    Debug::msg("Storing request...");
  std::hash<std::string> str_hash;
  string requestMapKey = sourceAddress + ":" + destPort;
  size_t requestMapValue = str_hash(message);

  Debug::msg("requestMapKey: " + requestMapKey);
  Debug::msg("requestMapValue: " + requestMapValue);

  requestMap[requestMapKey] = requestMapValue;
}

void store_response(string sourceAddress, string destPort, string message)
{
    Debug::msg("Storing response...");
  string responseMapKey = sourceAddress + ":" + destPort;

  Debug::msg("responseMapKey: " + responseMapKey);
  Debug::msg("responseMapValue: " + message);

  responseMap[responseMapKey] = message;
}

string retrieve_response(string sourceAddress, string destPort)
{
    Debug::msg("Retrieving response...");
  string responseMapKey = sourceAddress + ":" + destPort;

  // Debugging
  Debug::msg("responseMapKey: " + responseMapKey);
  Debug::msg("responseMapValue: " + responseMap[responseMapKey]);
  return responseMap[responseMapKey];
}

int get_request_code(cJSON *jobjReceived)
{
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "REQUEST_CODE")
      ->valueint;
}

int get_response_id(cJSON *jobjReceived)
{
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "RESPONSE_ID")
      ->valueint;
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
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "RFA_PATH")
      ->valuestring;
}

string get_monitor_duration(cJSON *jobjReceived)
{
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "MONITOR_DURATION")
      ->valuestring;
}

string get_dest_port(cJSON *jobjReceived)
{
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "PORT")->valuestring;
}

string get_content(cJSON *jobjReceived)
{
  return cJSON_GetObjectItem(jobjReceived, "CONTENT")->valuestring;
}

int fh_read_status(int result)
{
  if (result >= 0)
  {
    return READ_SUCCESS;
  }
  return READ_FAILURE;
}

int fh_write_status(int result)
{
  if (result >= 0)
  {
    return WRITE_SUCCESS;
  }
  return WRITE_FAILURE;
}

int comparetime(time_t time1, time_t time2)
{
  return difftime(time1, time2) > 0.0 ? 1 : -1;
}

string translate_filepath(string pseudo_filepath)
{
  char rfa_prefix[6];
  string current_path = std::experimental::filesystem::current_path();

  // assign here
  string actual_filepath = pseudo_filepath;
  strncpy(rfa_prefix, pseudo_filepath.c_str(),
          6); // Copy just the "RFA://" portion
  if (strcmp(rfa_prefix, "RFA://") == 0)
  {
    actual_filepath =
        current_path + "/ServerRemoteFileAccess/" + actual_filepath.substr(6);
    Debug::msg("actual_filepath: " + actual_filepath);
    return actual_filepath;
  }
  Debug::msg("pseudo_filepath: " + pseudo_filepath);
  return "";
}

time_t get_last_modified_time(const char *path)
{
  struct stat attr;
  stat(path, &attr);
  return attr.st_mtime;
}