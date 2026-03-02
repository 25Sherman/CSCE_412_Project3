/**
 * @file Webserver.h
 * @author Milan Singh Grewal
 * @brief Defines the Webserver class to handle individual requests.
 */
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

/**
 * @class Webserver
 * @brief Simulates a single web server that processes requests over time.
 */
class Webserver {
private:
    bool busy;        /**< Flag indicating if the server is currently processing a job */
    int timeLeft;     /**< Clock cycles remaining for the current job */
    Request currReq;  /**< The request currently being processed */

public:
    /**
     * @brief Initializes a free web server.
     */
    Webserver();

    /**
     * @brief Checks if the server is currently handling a request.
     * @return true if the server is busy, false otherwise.
     */
    bool isBusy() const;

    /**
     * @brief Assigns a new request to this server and starts the timer.
     * @param req The request object to process.
     */
    void addRequest(Request req);

    /**
     * @brief Simulates one clock cycle, decrementing the time remaining on the current task.
     */
    void processCycle();
};

#endif