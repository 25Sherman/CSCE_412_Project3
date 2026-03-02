CXX = g++
CXXFLAGS = -Wall -std=c++17
TARGET = loadbalancer.exe

# link it all together
$(TARGET): main.o Webserver.o LoadBalancer.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o Webserver.o LoadBalancer.o

# compile the individual pieces
main.o: main.cpp Request.h Webserver.h LoadBalancer.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Webserver.o: Webserver.cpp Webserver.h Request.h
	$(CXX) $(CXXFLAGS) -c Webserver.cpp

LoadBalancer.o: LoadBalancer.cpp LoadBalancer.h Webserver.h Request.h
	$(CXX) $(CXXFLAGS) -c LoadBalancer.cpp

# clean up old files
clean:
	rm -f *.o $(TARGET)