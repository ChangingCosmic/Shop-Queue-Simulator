#include "shopQueue.h"

int main(int argc, char *argv[]){
    //check the number of command line arguments
    if(argc != 7){
        std::cout << "Error: Wrong number of arguments!\nUsage: ./run_simulation <seed> <total_min> <arrival_prob> <order_prob> <num_queues> <outputFile>" << endl;
        return -1;
    }

    //initialize variables
    int seed = std::stoi(argv[1]);          // converts to a num; int for random num seed
    int total_min = std::stoi(argv[2]);     // converts to num; int for total time
    float arrive_prob = std::stof(argv[3]); // converts to float; probability of a customer arriving (0 to 1)
    float order_prob = std::stof(argv[4]);  // converts to float; probabilty a customer finishes their order in each time step
    int queues = std::stoi(argv[5]);        // converts to num; number of queues to model
    float randomProb = 0;                   // initalize random probability
    const char *file = argv[6];             // the file
    int ID = 0;                             // ID
    int shortestQueueIndex = 0;             // variable to keep track of the shortest queue index
    int shortestQueueLength = 0;            // variable to compare the queue lengths

    vector<ShopQueue> queueVectors(queues);             // queue for customers
    vector<ShopQueue> cashierCompletedQueues(queues);   // the queue for the cashiers' completed customers
    vector<shared_ptr<Customer>> completed;             // completed customers queue only

    // sets the seed
    std::srand(seed);

    //print the welcome msg
    std::cout << "Welcome to Gompei Coffee Shop!\n-- # of Checkout Queues: " << queues << " --" << endl;

    //adds the first customer!
    queueVectors[0].addNewCustomer(0, 0, 0, ID);
    ID++;
    std::cout << "New customer at t = 0" << endl;

    // loop through the time we have
    for (int currentMin = 0; currentMin < total_min; currentMin++) {
        // find the shortest queue to know where to put the customer
        shortestQueueIndex = 0;
        shortestQueueLength = queueVectors[0].getLength();

        // loop through all the queues
        for (int j = 0; j < queues; j++) {
            if(queueVectors[j].getLength() < shortestQueueLength) {
                shortestQueueLength = queueVectors[j].getLength();
                shortestQueueIndex = j;
            }
        }

        randomProb = (float)std::rand() / (float) RAND_MAX;     // randomProb to see if a customer arrived
        if (randomProb < arrive_prob) { // new customer!
            queueVectors[shortestQueueIndex].addNewCustomer(currentMin, currentMin, 0, ID);
            ID++;
            std::cout << "New customer at t = " << currentMin << endl;
        }

        // now checking if a customer finished their order
        for(int j = 0; j < queues; j++){        // for loop to loop through the queues to check the front customer
            if (!queueVectors[j].isEmpty()){    // if it's not empty
                randomProb = (float)std::rand() / (float)RAND_MAX;
                if(randomProb < order_prob){
                    // removing front customer and changing the completed time
                    auto frontCust = queueVectors[j].removeFrontCustomer();
                    frontCust->completedTime = currentMin;
                    completed.push_back(frontCust);

                    std::cout << "Customer #" << frontCust->ID << " was served from t = " << frontCust->startTime << " to " << frontCust->completedTime << "." << endl;

                    // adds to the cashier queue!
                    cashierCompletedQueues[j].addNewCustomer(frontCust->arrivalTime, frontCust->startTime, frontCust->completedTime, frontCust->ID);

                    // modify the start time of the next front customer
                    if(!queueVectors[j].isEmpty()){
                        auto nextFrontCust = queueVectors[j].peekFrontCustomer();
                        nextFrontCust->startTime = currentMin;
                    }
                } // end of randomProb < order_prob
            } // end of checking if the queue is empty
        } // end of queue loop for customer order
    } // end of looping through the time

    //opens the file & checks if it can be opened
    ofstream outfile(file);
    if(!outfile.is_open()){
        std::cout << "Error: cannot open file :(" << endl;
        return -2;
    }

    //print to simulation_out.txt the complete customer vector
    outfile << right << std::setw(8) << "ID" << std::setw(8) << "Arrive" << std::setw(8) << "Start" << std::setw(8) << "Leave" << endl;
    for (const auto& customerPtr : completed) { // loop through to get the customer
        outfile << right << std::setw(8) << customerPtr->ID << right << std::setw(8) << customerPtr->arrivalTime << right << std::setw(8) << customerPtr->startTime << right << std::setw(8) << customerPtr->completedTime << endl;
    }
    outfile.close();  

    // printing out how many customers remain in the queues
    for (int i = 0; i < queues; i++){
        std::cout << "After " << total_min << " minutes, " << queueVectors[i].getLength() << " customers remain in queue #" << i << endl;
    }

    // printing out how many customer each cashier served
    for(int i = 0; i < queues; i++){
        std::cout << "Cashier " << i << " served " << cashierCompletedQueues[i].getLength() << " customers." << endl;
    }
}