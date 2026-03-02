/**
 * @file LoadBalancer.cpp
 * @author Milan Singh Grewal
 * @brief Implementation of the LoadBalancer class.
 */
#include "LoadBalancer.h"

// setup the load balancer with initial servers
LoadBalancer::LoadBalancer(int numServers, std::string blockedRange) {
    sysTime = 0;
    firewallIP = blockedRange;
    scaleCooldown = 0;
    
    startQueueSize = 0;
    processedCount = 0;
    blockedCount = 0;
    maxServers = numServers;
    minServers = numServers;

    // spin up starting servers
    for (int i = 0; i < numServers; ++i) {
        servers.push_back(Webserver());
    }
}

// locks in the starting size for the log
void LoadBalancer::setStartingQueueSize() {
    startQueueSize = reqQueue.size();
}

// add request if it passes the firewall check
void LoadBalancer::addRequest(Request req) {
    if (req.ip_in.find(firewallIP) == 0) {
        blockedCount++;
        return; 
    }
    reqQueue.push(req);
}

// pass out jobs to free servers
void LoadBalancer::distributeRequests() {
    for (std::size_t i = 0; i < servers.size(); ++i) {
        if (!servers[i].isBusy() && !reqQueue.empty()) {
            servers[i].addRequest(reqQueue.front());
            reqQueue.pop();
            processedCount++;
        }
    }
}

// 50x to 80x scaling logic
void LoadBalancer::manageScale() {
    // wait n cycles if we just scaled
    if (scaleCooldown > 0) {
        scaleCooldown--;
        return;
    }

    int qSize = reqQueue.size();
    int numServers = servers.size();

    if (numServers == 0) numServers = 1; // avoid divide by zero

    // queue too big -> add server
    if (qSize > 80 * numServers) {
        servers.push_back(Webserver());
        scaleCooldown = 10; 
        
        if ((int)servers.size() > maxServers) {
            maxServers = servers.size();
        }
    } 
    // queue too small -> remove server
    else if (qSize < 50 * numServers && numServers > 1) {
        // only pop if the server isn't actively working on a job
        if (!servers.back().isBusy()) {
            servers.pop_back(); 
            scaleCooldown = 10; 
            
            if ((int)servers.size() < minServers) {
                minServers = servers.size();
            }
        } 
    }
}

// main clock cycle execution
void LoadBalancer::processTimeStep() {
    sysTime++;
    
    // 1. tick down active server timers
    for (std::size_t i = 0; i < servers.size(); ++i) {
        servers[i].processCycle();
    }
    
    // 2. hand out new jobs
    distributeRequests();
    
    // 3. check scaling
    manageScale();
}

// --- getters ---
int LoadBalancer::getQueueSize() const { return reqQueue.size(); }
int LoadBalancer::getServerCount() const { return servers.size(); }
int LoadBalancer::getStartingQueueSize() const { return startQueueSize; }
int LoadBalancer::getTotalProcessed() const { return processedCount; }
int LoadBalancer::getTotalBlocked() const { return blockedCount; }
int LoadBalancer::getMaxServers() const { return maxServers; }
int LoadBalancer::getMinServers() const { return minServers; }