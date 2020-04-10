#include "FileCLI.h"

using namespace std;

FileCLI::FileCLI() { cv.restoreHashMap(); }

/** readfile reads the user inputs for offset and num of bytes
 * @param void
 * @return void
 **/
void FileCLI::readFile() {
  int fileID;

  listFile();
  cin >> fileID;

  // check if the fileID input is within bounds
  if (checkValidity(fileID)) {
    int offSet;
    int numBytes;

    cout << "Offset: ";
    cin >> offSet;
    cout << "Number of bytes: ";
    cin >> numBytes;

    // translate fileID to real remote file path
    map<int, string>::iterator it;
    it = cacheReference.find(fileID);
    string remoteFilePath = it->second;

    cout << cv.readFile(remoteFilePath, offSet, numBytes) << endl;

    // this one is just a blocking call to display the text until something is
    // keyed by user
    cin >> offSet;
  } else {
    cout << "That is not a valid file ID" << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  return;
}


void FileCLI::monitorFile(){
    int fileID;

    listFile();
    cin >> fileID;

    // check if the fileID input is within bounds
    if (checkValidity(fileID)) {
        // translate fileID to real remote file path
        map<int, string>::iterator it;
        it = cacheReference.find(fileID);
        string remoteFilePath = it->second;

        // input the monitoring duration
        int monitorDuration;
        cout << "Input the monitoring duration: ";
        cin >> monitorDuration;
        if (cv.monitorFile(remoteFilePath, monitorDuration)) {
            cout << "Monitoring has ended with success" << endl;
        }
        else {
            cout << "Monitoring has ended with failure" << endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return;
}
void FileCLI::clearFile() {
  int fileID;

  listFile();
  cin >> fileID;

  // check if the fileID input is within bounds
  if (checkValidity(fileID)) {

    // translate fileID to real remote file path
    map<int, string>::iterator it;
    it = cacheReference.find(fileID);
    string remoteFilePath = it->second;

    if (cv.clearFile(remoteFilePath)) {
      cout << "File is removed" << endl;
    } else {
      cout << "Failed to remove file" << endl;
    }
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  return;
}

void FileCLI::clearCache() {
  if (cv.clearCache()) {
    cout << "Cache is cleared successfully" << endl;
  } else {
    cout << "Failed to clear cache" << endl;
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}
void FileCLI::writeFile() {
  int fileID;

  listFile();
  cin >> fileID;

  // check if the fileID input is within bounds
  if (checkValidity(fileID)) {
    int offSet;
    string textAppend;
    string dumb;
    cout << "Offset: ";
    cin >> offSet;
    cout << "Input the text to be appended: ";
    getchar();
    getline(cin, textAppend);

    // translate fileID to real remote file path
    map<int, string>::iterator it;
    it = cacheReference.find(fileID);
    string remoteFilePath = it->second;

    if (cv.writeFile(remoteFilePath, textAppend, offSet)) {
      cout << "Written into File Successfully" << endl;
    } else {
      cout << "Failed to write to file" << endl;
    }
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  return;
}

void FileCLI::fetchFile() {
  string filePath;
  cout << "Please input a file path: RFA://"; // line hit
  cin >> filePath;
  string filePathModified = "RFA://" + filePath;
  cout << "filepath is: " + filePathModified
       << endl; // filepath = RFA://documents/test.txt
  if (cv.checkValidityFetch(filePathModified)) {
    cout << "File is fetched and cached" << endl; // line is never hit
  } else {
    cout << "Failed to fetch and cache file" << endl;
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  return;
}

void FileCLI::clearContent() {
  int fileID;

  listFile();
  cin >> fileID;

  // check if the fileID input is within bounds
  if (checkValidity(fileID)) {
    int offSet;
    string textAppend;

    // translate fileID to real remote file path
    map<int, string>::iterator it;
    it = cacheReference.find(fileID);
    string remoteFilePath = it->second;

    if (cv.clearContent(remoteFilePath)) {
      cout << "Clear File content successfully" << endl;
    } else {
      cout << "Failed to clear file content" << endl;
    }
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  return;
}
// table to show all the available files
void FileCLI::listFile() {
  int j;
  cacheKeys = cv.listCache();
  cacheSize = cacheKeys.size();
  cacheReference.clear();
  for (j = 0; j < cacheKeys.size(); j++) {
    cacheReference.insert({j + 1, cacheKeys[j]});
  }

  // get all the files available
  cout << "\u250f";
  for (int i = 0; i < 40; i++) {
    cout << "\u2501";
  }
  cout << "\u2513" << endl;
  cout << "\u2503"
       << "Key in the FileID or -1 for manual input"
       << "\u2503" << endl;

  // no entries
  if (j == 0) {
    cout << "\u2517";
    for (int i = 0; i < 40; i++) {
      cout << "\u2501";
    }
    cout << "\u251b" << endl;
    return;
  } else {
    cout << "\u2523";
    for (int i = 0; i < 4; i++) {
      cout << "\u2501";
    }
    cout << "\u2533";
    for (int i = 0; i < 35; i++) {
      cout << "\u2501";
    }
    cout << "\u252b" << endl;
  }

  for (int k = 0; k < cacheKeys.size(); k++) {
    cout << "\u2503" << k + 1;
    for (int i = 0; i < 4 - to_string(i + 1).length(); i++) {
      cout << " ";
    }
    cout << "\u2503" << cacheKeys[k];
    for (int i = 0; i < (35 - cacheKeys[k].length()); i++) {
      cout << " ";
    }
    cout << "\u2503" << endl;

    if (k != cacheKeys.size() - 1) {
      cout << "\u2523";
      for (int i = 0; i < 4; i++) {
        cout << "\u2501";
      }
      cout << "\u254b";
      for (int i = 0; i < 35; i++) {
        cout << "\u2501";
      }
      cout << "\u252b" << endl;
    } else {
      cout << "\u2517";
      for (int i = 0; i < 4; i++) {
        cout << "\u2501";
      }
      cout << "\u253b";
      for (int i = 0; i < 35; i++) {
        cout << "\u2501";
      }
      cout << "\u251b" << endl;
    }
  }

  return;
}

bool FileCLI::checkValidity(int fileID) {
  if (fileID >= 1 && fileID <= cacheSize) {
    return true;
  } else {
    return false;
  }
}

FileCLI::~FileCLI() { cv.saveHashMap(); }
