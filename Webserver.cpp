/**
 * @file Webserver.cpp
 * @author Milan Singh Grewal
 * @brief Implementation of the Webserver class.
 */
#include "Webserver.h"

// starts completely free
Webserver::Webserver() {
    busy = false;
    timeLeft = 0;
}

// check if it's doing something
bool Webserver::isBusy() const {
    return busy;
}

// load up a new request and start the timer
void Webserver::addRequest(Request req) {
    currReq = req;
    timeLeft = req.time;
    busy = true;
}

// simulate a single clock tick
void Webserver::processCycle() {
    if (busy) {
        timeLeft--;
        
        // free up the server if the job is done
        if (timeLeft <= 0) {
            busy = false;
            timeLeft = 0; 
        }
    }
}