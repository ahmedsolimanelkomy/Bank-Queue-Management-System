#include <iostream>
#include <queue>
#include <list>
#include <algorithm>
#include "Customer.h"
#include <windows.h>
#include <thread>
using namespace std;

class BankQueue {
private:
    struct CompareCustomer {
        bool operator()(const Customer* c1, const Customer* c2) const {
            return c1->getArrivalTime() > c2->getArrivalTime();
        }
    };

    priority_queue<Customer*, vector<Customer*>, CompareCustomer> customerQueue;
    list<Teller *> tellers;
    Customer** customers;
    int numCustomers;

    // Metrics variables
    int totalCustomersServed = 0;
    int totalWaitingTime = 0;
    int totalServiceTime = 0;
    double avgWaitingTime = 0;
    double avgServiceTime = 0;
    double utilizationRate = 0;
    int currentTime = 0;

public:

    BankQueue(int numCustomers,Customer** customers):numCustomers(numCustomers) , customers(customers) {}

    void enqueue(Customer* c) {

        customerQueue.push(c);
        //cout << "   NEW CUSTOMER ARRIVES ........."<<endl;
        //cout << "   ------------------------------" << endl;

    }

    void runSimulation(){

        allocateTellers();
        sortQueue();
        SetColor(14);
        cout<< endl;
        cout<<  "   SIMULATION :"<<endl;
        assignToTellers();
    }

private :

   void assignToTellers() {

        std::list<Teller*>::iterator teller = tellers.begin();

        while (!customerQueue.empty()) {

            for (teller = tellers.begin(); teller != tellers.end(); ++teller) {

                if ((*teller)->isAvailable()) {

                    Customer* customer = dequeue();

                    if (customer != nullptr) {

                        (*teller)->assignCustomer(customer);

                        if ((*teller)->getLastCurrentTime() > customer->getArrivalTime()) {

                            customer->setWaitingTime((*teller)->getLastCurrentTime() - customer->getArrivalTime());
                            (*teller)->setCurrentTime(customer->getArrivalTime() + customer->getServiceTime() + customer->getWaitingTime());

                        }else {
                            (*teller)->setCurrentTime(customer->getArrivalTime() + customer->getServiceTime());
                        }

                        currentTime =(*teller)->getCurrentTime();
                        (*teller)->setLastCurrentTime((*teller)->getCurrentTime());

                        updateMetrics(customer);

                        // Output serving details
                        SetColor(14);
                        std::this_thread::sleep_for(std::chrono::milliseconds(customer->getServiceTime()*10));

                        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
                        cout << "   TELLER " << (*teller)->getTellerID() << endl << "    SERVING CUSTOMER : " << customer->getId() <<" | "
                             << "   ARRIVAL TIME : " << customer->getArrivalTime() << " | "
                             << "   SERVICE TIME : " << customer->getServiceTime() << " | "
                             << "   CURRENT TIME : " << customer->getArrivalTime() + customer->getServiceTime() + customer->getWaitingTime()
                             << " | PRIORITY : " << customer->getPriority() << endl << endl;

                        (*teller)->setLastCurrentTime((*teller)->getCurrentTime());

                    } else {

                        cout << "   No customers to serve at Teller " << (*teller)->getTellerID() << endl;
                        (*teller)->setCurrentTime((*teller)->getCurrentTime() + 1);
                    }
                }
            }
        }
        cout << "-----------------------------------" << endl;

        calculateMetrics();
        exportData();
    }

    void sortQueue() {

        vector<Customer*> tempVector;

        while (!customerQueue.empty()) {
            tempVector.push_back(customerQueue.top());
            customerQueue.pop();
        }
        sort(tempVector.begin(), tempVector.end(), CompareCustomer());

        for (Customer* customer : tempVector) {
            customerQueue.push(customer);
        }
    }


     void updateMetrics(Customer* customer) {

        totalCustomersServed++;
        totalWaitingTime += customer->getWaitingTime();
        totalServiceTime += customer->getServiceTime();
    }
    void calculateMetrics() {

        avgWaitingTime = totalCustomersServed > 0 ? static_cast<double>(totalWaitingTime) / totalCustomersServed : 0;
        avgServiceTime = totalCustomersServed > 0 ? static_cast<double>(totalServiceTime) / totalCustomersServed : 0;
        utilizationRate = totalCustomersServed > 0 ? static_cast<double>(totalServiceTime) / (currentTime * tellers.size()) : 0;

        SetColor(13);
        cout << "   Metrics:" << endl;
        cout << "   Total customers served : " << totalCustomersServed << endl;
        cout << "   totalServiceTime : " << totalServiceTime << endl;
        cout << "   totalWaitingTime : " << totalWaitingTime << endl;
        cout << "   Average waiting time : " << avgWaitingTime << endl;
        cout << "   Average service time : " << avgServiceTime << endl;
        cout << "   Teller utilization rate : " << utilizationRate << endl <<endl;
    }

    Customer* dequeue() {
        if (!customerQueue.empty()) {

            Customer *customer = customerQueue.top();
            customerQueue.pop();
            return customer;

        } else {

            return NULL;
        }
    }



    void exportData() {

        const string filename = "DATA STATISTICS.txt";

        std::ofstream outFile(filename);

        if (!outFile.is_open()) {
            std::cerr << "  Error: Unable to open file for writing." << std::endl;
            return;
        }

        outFile << "\nMetrics :\n";
        outFile << "Total customers served : " << totalCustomersServed << std::endl;
        outFile << "total Service Time : " << totalServiceTime << std::endl;
        outFile << "total Waiting Time : " << totalWaitingTime << std::endl;
        outFile << "Average waiting time : " << avgWaitingTime << std::endl;
        outFile << "Average service time : " << avgServiceTime << std::endl;
        outFile << "Teller utilization rate : " << utilizationRate << std::endl;

        outFile.close();

        cout<<"-------------------------------"<<endl;
        cout << "   Data exported to " << filename << " successfully...." << std::endl;
        cout<<"-------------------------------"<<endl;
    }

    void allocateTellers()
    {
        int numPriorityCustomers = 0;
        int numTellers = 0;

            for (int i = 0; i < numCustomers; ++i)
            {
                if (customers[i]->getPriority())
                {
                    numPriorityCustomers++;
                }
            }

            if (numCustomers < 5 ){

                numTellers = 1;

            }else{

                if (numCustomers <= 50 ){

                    numTellers = (numPriorityCustomers / 3) + 1;

                }else{

                    numTellers = 10;
                }
            }

            for (int i = 0; i < numTellers; ++i)
            {
                tellers.push_back(new Teller(i + 1));
            }
    }

    void SetColor(int ForgC)
    {
        WORD wColor;
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;

        if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
        {
            wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
            SetConsoleTextAttribute(hStdOut, wColor);
        }
    }


};
