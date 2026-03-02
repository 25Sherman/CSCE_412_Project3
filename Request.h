/**
 * @file Request.h
 * @author Milan Singh Grewal
 * @brief Defines the Request struct representing a network job.
 */
#ifndef REQUEST_H
#define REQUEST_H

#include <string>

/**
 * @struct Request
 * @brief Holds information about an incoming network request.
 */
struct Request {
    std::string ip_in;     /**< The source IP address of the request */
    std::string ip_out;    /**< The destination IP address */
    int time;              /**< The number of clock cycles required to process the request */
    char job_type;         /**< 'P' for processing, 'S' for streaming */

    /**
     * @brief Default constructor for a Request.
     * @param in Source IP address (default empty).
     * @param out Destination IP address (default empty).
     * @param t Time required to process (default 0).
     */
    Request(std::string in = "", std::string out = "", int t = 0) 
        : ip_in(in), ip_out(out), time(t), job_type('P') {}
};

#endif