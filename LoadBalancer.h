/**
 * @file LoadBalancer.h
 * @author Milan Singh Grewal
 * @brief Defines the LoadBalancer class that manages the request queue and server scaling.
 */
#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include "Request.h"
#include "Webserver.h"
#include <queue>
#include <vector>
#include <string>

/**
 * @class LoadBalancer
 * @brief Manages a dynamic pool of web servers and distributes incoming network requests.
 */
class LoadBalancer {
private:
    std::queue<Request> reqQueue;  /**< Queue of incoming network requests */
    std::vector<Webserver> servers; /**< Dynamic list of active web servers */
    
    int sysTime;               /**< Current system clock cycle */
    std::string firewallIP;    /**< IP subnet range to block */
    int scaleCooldown;         /**< Wait timer before allowing another scale operation */

    // stats for the log file
    int startQueueSize;        /**< Size of the queue at the start of simulation */
    int processedCount;        /**< Total number of requests successfully processed */
    int blockedCount;          /**< Total number of requests dropped by the firewall */
    int maxServers;            /**< Highest number of concurrent servers used */
    int minServers;            /**< Lowest number of concurrent servers used */

public:
    /**
     * @brief Constructs a LoadBalancer with an initial server count and a blocked IP range.
     * @param numServers Initial number of web servers to spin up.
     * @param blockedRange The IP prefix to block (e.g., "192.168.").
     */
    LoadBalancer(int numServers, std::string blockedRange);

    /**
     * @brief Locks in the starting queue size for final log reporting.
     */
    void setStartingQueueSize(); 

    /**
     * @brief Adds a request to the queue if it passes the firewall check.
     * @param req The Request to add.
     */
    void addRequest(Request req);

    /**
     * @brief Executes one clock cycle, updating servers, distributing jobs, and managing scale.
     */
    void processTimeStep();

    /** @brief Gets the current number of requests waiting in the queue. */
    int getQueueSize() const;
    /** @brief Gets the current number of active servers. */
    int getServerCount() const;
    /** @brief Gets the initial queue size recorded at startup. */
    int getStartingQueueSize() const;
    /** @brief Gets the total number of requests processed during the simulation. */
    int getTotalProcessed() const;
    /** @brief Gets the total number of requests blocked by the firewall. */
    int getTotalBlocked() const;
    /** @brief Gets the maximum number of servers active at any point. */
    int getMaxServers() const;
    /** @brief Gets the minimum number of servers active at any point. */
    int getMinServers() const;

private:
    /**
     * @brief Internal helper to distribute queued requests to idle servers.
     */
    void distributeRequests();

    /**
     * @brief Internal helper to dynamically scale the server pool based on queue size.
     */
    void manageScale();
};

#endif