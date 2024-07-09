run_simulation: simulation.o shopQueue.o
	@g++ -g -Wall simulation.o shopQueue.o -o run_simulation -lm
simulation.o: simulation.cpp shopQueue.h
	@g++ -g -Wall -c simulation.cpp
shopQueue.o: shopQueue.cpp shopQueue.h 
	@g++ -g -Wall -c shopQueue.cpp
clean:
	@rm -f run_simulation simulation.o shopQueue.o
