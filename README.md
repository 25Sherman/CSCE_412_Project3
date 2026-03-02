# CSCE 412: Cloud Computing Load Balancer

**Author:** Milan Singh Grewal

## Project Overview
This project is a C++ simulation of a network load balancer. It manages a dynamic pool of web servers, implements a firewall to block specific subnets, and uses a scaling algorithm to adjust server capacity based on queue traffic.

## File Structure
* **main.cpp**: Driver program that runs the simulation loop.
* **LoadBalancer.h/cpp**: Manages the request queue and server scaling logic.
* **Webserver.h/cpp**: Individual server logic that processes requests.
* **Request.h**: Struct representing a network job (IPs, time, type).
* **configs.txt**: Configuration file for simulation parameters.

## How to Run
1. **Compile**: Run the command `make` in your terminal.
2. **Execute**: Run `./loadbalancer.exe`.
3. **Configure**: You can change the number of servers or cycles by editing `configs.txt`.

## Understanding the Output
* **Real-time Status**: Every 1,000 cycles, the terminal displays the current queue size and the number of active servers.
* **Dynamic Scaling**: You will see the server count increase if the queue gets too long (80x capacity) and decrease as servers become idle.
* **Final Summary**: At the end, a summary is printed and saved to `log.txt` showing total requests processed, requests blocked by the firewall, and the min/max server usage.

## Documentation
Technical documentation is generated via Doxygen and can be viewed at:  
**URL:** https://people.it.tamu.edu/~msgrewal/