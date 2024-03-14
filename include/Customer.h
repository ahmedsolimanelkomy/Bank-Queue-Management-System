#ifndef CUSTOMER_H
#define CUSTOMER_H
class Customer
{

    public:

        Customer(int ID,int arrivalTime,int serviceTime,bool priority) {
            setId(ID);
            setArrivalTime(arrivalTime);
            setServiceTime(serviceTime);
            setPriority(priority);
        }

        void setId(int customerId) { this->customerId = customerId ; }
        void setArrivalTime(int arrivalTime) { this->arrivalTime = arrivalTime; }
        void setServiceTime(int serviceTime) { this->serviceTime = serviceTime; }
        void setPriority(bool priority) { this->priority = priority; }
        void setWaitingTime(int waitingTime) { this->waitingTime = waitingTime; }

        int getId() const { return customerId; }
        int getArrivalTime() const { return arrivalTime; }
        int getServiceTime() const { return serviceTime; }
        bool getPriority() const { return priority; }
        int getWaitingTime() const { return waitingTime; }


    protected:

    private:

        int customerId;
        int arrivalTime;
        int serviceTime;
        int waitingTime = 0;
        bool priority;
};

#endif // CUSTOMER_H
