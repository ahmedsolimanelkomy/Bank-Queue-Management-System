#ifndef TELLER_H
#define TELLER_H
#include <Customer.h>
using namespace std;
class Teller
{
    private:

        int tellerID;
        bool available;
        int availableTime;
        Customer *currentCustomer;
        int currentTime;
        int lastCurrentTime;

    public:

        Teller(int ID) : available (true), tellerID(ID), availableTime(0),
        currentCustomer(nullptr), currentTime(0), lastCurrentTime(0) {}

        void assignCustomer(Customer* customer) {

            currentCustomer = customer;

            this->available = false;

            int serviceTime = customer->getServiceTime();

            this->available = true;
        }

        void setTellerID(int id) { tellerID = id; }
        void setAvailable(bool avail) { available = avail; }
        void setAvailableTime(int time) { availableTime = time; }
        void setCurrentCustomer(Customer *customer) { currentCustomer = customer; }
        void setCurrentTime(int time) { currentTime = time; }
        void setLastCurrentTime(int time) { lastCurrentTime = time; }

        int getTellerID() const { return tellerID; }
        bool isAvailable() const { return available; }
        int getAvailableTime() const { return availableTime; }
        Customer* getCurrentCustomer() const { return currentCustomer; }
        int getCurrentTime() const { return currentTime; }
        int getLastCurrentTime() const { return lastCurrentTime; }

};

#endif // TELLER_H
