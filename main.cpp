#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

// function to analyse of heap usage
std::unordered_map<std::string, long> analyzeMemoryUsage() {
    std::unordered_map<std::string, long> memoryUsage;

    // open the file "/proc/meminfo", for information
    std::ifstream meminfoFile("/proc/meminfo");
    std::string line;
    while (std::getline(meminfoFile, line)) {
        std::istringstream iss(line);
        std::string key;
        long value;
        if (iss >> key >> value) {
          // more analysing for future keys
          if (key == "MemTotal:"){
              std::cout << "Gesamtspeicher; " << value << " KB " << std::endl;
          } else if (key == "MemFree") {
              std::cout << "Freier Speicher: " << value << " KB " << std::endl;

          }
        }
    }
    // open file "/proc/[pid]/status" for open processes
    std::ifstream statusFile("/proc/self/status");
    std::string pid;
    while (std::getline(statusFile, line)) {
        std::istringstream iss(line);
        std::string key;
        if (iss >> key >> pid && key == "Pid:") {
            std::ifstream procStatusFile("/proc/" + pid + "/status");
            std::string pname;
            long vmsize = 0;
            while (std::getline(procStatusFile, line)) {
                std::istringstream iss(line);
                std::string token;
                if (iss >> token) {
                    if (token == "Name:") {
                        iss >> pname;
                    } else if (token == "VmSize:") {
                        iss >> vmsize;
                    }
                }
            }
            memoryUsage[pname] = vmsize;
        }
    }

    return memoryUsage;
}

int main() {

    // analyse memory usage
    auto memoryUsage = analyzeMemoryUsage();

    // print out the result
    std::cout << "\nSpeichernutzung der Prozesse:\n";
    for (const auto& entry : memoryUsage) {
        std::cout << entry.first << ": " << entry.second << " KB\n";
    }
    return 0;
}
