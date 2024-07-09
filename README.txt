Project 3
Phuong Tran
March 1, 2024

This program simulates a shopping queue in C++.

To compile the program, run the command:
	@g++ -g -Wall simulation.o shopQueue.o -o run_simulation -lm
or:
    @make

To run the program, run the command:
	./run_simulation seed total_min arrive_prob order_prob queues outputFile
where:
    seed: An integer for the random number seed
    total_min: The total “time” to run the simulation
    arrive_prob: The probability of a customer arriving (a float between 0 and 1)
    order_prob: The probability of a customer arriving (a float between 0 and 1)
    queues: The number of queues to simulate
    outputFile: The name of the output file

Sources: Kaveh Eskandari Miandoab helped me debug my code and go thorugh the errors.