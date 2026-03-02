/**
 * @file main.cpp
 * @author Milan Singh Grewal
 * @brief Driver program to simulate a cloud computing load balancer.
 */
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "LoadBalancer.h"

// ANSI Colors for terminal output
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string CYAN = "\033[36m";

// function prototypes
std::string getRandomIP();
Request createRandomRequest(int minTime, int maxTime);

int main() {
    srand(time(0)); // seed random generator

    // default config values just in case
    int serverCount = 10;
    int cycleCount = 10000;
    int minTask = 5;
    int maxTask = 50;

    // try to load configs.txt
    std::ifstream configFile("configs.txt");
    if (configFile.is_open()) {
        std::string key;
        // read pairs of keys and values
        while (configFile >> key) {
            if (key == "NUM_SERVERS") configFile >> serverCount;
            if (key == "NUM_CYCLES") configFile >> cycleCount;
            if (key == "MIN_TASK_TIME") configFile >> minTask;
            if (key == "MAX_TASK_TIME") configFile >> maxTask;
        }
        configFile.close();
        std::cout << GREEN << "Configs loaded.\n" << RESET;
    } else {
        std::cout << RED << "Warning: Couldn't open configs.txt. Using defaults.\n" << RESET;
    }

    // setup load balancer and block a subnet
    std::string badIP = "192.168."; 
    LoadBalancer lb(serverCount, badIP);
    std::cout << CYAN << "Starting Load Balancer with " << serverCount << " servers...\n" << RESET;

    // fill up the starting queue
    int startQueueSize = serverCount * 100;
    for (int i = 0; i < startQueueSize; i++) {
        lb.addRequest(createRandomRequest(minTask, maxTask));
    }
    
    lb.setStartingQueueSize();
    std::cout << GREEN << "Created initial queue of " << startQueueSize << " requests.\n\n" << RESET;
    std::cout << YELLOW << "--- Running Simulation for " << cycleCount << " Cycles ---\n\n" << RESET;

    // main simulation loop
    for (int cycle = 0; cycle < cycleCount; cycle++) {
        
        // 20% chance to add new requests
        if (rand() % 100 < 20) {
            int newReqs = (rand() % 3) + 1;
            for (int r = 0; r < newReqs; r++) {
                lb.addRequest(createRandomRequest(minTask, maxTask));
            }
        }

        lb.processTimeStep();

        // print an update every 1000 cycles
        if (cycle > 0 && cycle % 1000 == 0) {
            std::cout << CYAN << "[Cycle " << cycle << "] " << RESET 
                      << "Queue: " << lb.getQueueSize() 
                      << " | Active Servers: " << lb.getServerCount() << "\n";
        }
    }

    std::cout << "\n" << YELLOW << "--- Simulation Done ---\n\n" << RESET;

    // generate the summary output
    std::ofstream logFile("log.txt");
    
    std::string summary = 
        "=== Load Balancer Simulation Summary ===\n"
        "Initial Servers Configured: " + std::to_string(serverCount) + "\n"
        "Total Clock Cycles Run: " + std::to_string(cycleCount) + "\n"
        "Task Time Range: " + std::to_string(minTask) + " to " + std::to_string(maxTask) + " cycles\n"
        "----------------------------------------\n"
        "Starting Queue Size: " + std::to_string(lb.getStartingQueueSize()) + "\n"
        "Ending Queue Size: " + std::to_string(lb.getQueueSize()) + "\n"
        "Total Requests Processed: " + std::to_string(lb.getTotalProcessed()) + "\n"
        "Total Requests Blocked (Firewall): " + std::to_string(lb.getTotalBlocked()) + "\n"
        "Minimum Servers Used: " + std::to_string(lb.getMinServers()) + "\n"
        "Maximum Servers Used: " + std::to_string(lb.getMaxServers()) + "\n"
        "========================================\n";

    // print to console
    std::cout << GREEN << summary << RESET;

    // save to log.txt
    if (logFile.is_open()) {
        logFile << summary;
        logFile.close();
        std::cout << CYAN << "Summary written to log.txt\n" << RESET;
    }

    return 0;
}

/**
 * @brief Generates a random IPv4 address.
 * @return A string representing a random IP address (e.g., "192.168.1.1").
 */
std::string getRandomIP() {
    std::string ip = "";
    // Loop to build the first 3 octets
    for(int i = 0; i < 3; i++) {
        ip += std::to_string(rand() % 256) + ".";
    }
    // add the final octet
    ip += std::to_string(rand() % 256);
    return ip;
}

/**
 * @brief Creates a randomized network request.
 * @param minTime Minimum clock cycles the job takes.
 * @param maxTime Maximum clock cycles the job takes.
 * @return A newly generated Request object.
 */
Request createRandomRequest(int minTime, int maxTime) {
    Request req;
    req.ip_in = getRandomIP();
    req.ip_out = getRandomIP();
    
    int timeRange = maxTime - minTime + 1;
    req.time = minTime + (rand() % timeRange);
    
    // randomly assign job type
    if (rand() % 2 == 0) {
        req.job_type = 'P';
    } else {
        req.job_type = 'S';
    }
    
    return req;
}