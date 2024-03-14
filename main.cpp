#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include "Teller.h"
#include "Customer.h"
#include "BankQueue.h"
#include <windows.h>


using namespace std;

// Function to read customer data from an external file
Customer **readCustomersFromFile(const string &filename, int &numCustomers)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Unable to open file: " << filename << endl
             << endl;
        return nullptr;
    }

    string line;
    numCustomers = 0;

    while (getline(file, line))
    {
        ++numCustomers;
    }

    file.clear();
    file.seekg(0, ios::beg);

    Customer **customers = new Customer *[numCustomers];

    int i = 0;
    while (getline(file, line))
    {
        istringstream iss(line);
        int id, arrival, service;
        string priorityStr;

        iss >> id >> arrival >> service >> priorityStr;

        bool isPriority = (priorityStr == "true");

        customers[i++] = new Customer(id, arrival, service, isPriority);
    }

    file.close();

    return customers;
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

// Function to generate random customer data
Customer **generateRandomCustomers(int numCustomers)
{
    Customer **customers = new Customer *[numCustomers];

    for (int i = 0; i < numCustomers; ++i)
    {
        int id = i + 1;
        int arrival = rand() % 50 + 1;
        int service = rand() % 10 + 1;
        bool priority = rand() % 2;

        customers[i] = new Customer(id, arrival, service, priority);
    }

    return customers;
}

// Function to display the menu and get user choice
int displayMenu()
{
    SetColor(18);
    cout <<endl;
    cout << "   Bank Queue Management System" << endl;
    cout << "-----------------------------" << endl;
    cout << "   1. Read customer data from file" << endl;
    cout << "   2. Generate random customer data" << endl;
    cout << "   3. Exit" << endl;
    cout << "------------------------------" << endl;
    cout << "   Please Enter your choice: ";
    int choice;
    cin >> choice;
    return choice;
}

/*--------------------------------------------------------------------------------------------------*/
int main()
{
    int numCustomers;
    string filename;

    int choice;
    bool validChoice;
    do
    {
        validChoice = true;

        choice = displayMenu();

        switch (choice)
        {
        case 1:
        {
            cout << "   Please Enter Customer File Name : " << endl;
            cin >> filename;
            cout << "------------------------------" << endl;

            Customer **customers = readCustomersFromFile(filename, numCustomers);
            if (customers != nullptr)
            {
                BankQueue *myQueue = new BankQueue(numCustomers, customers);
                for (int i = 0; i < numCustomers; ++i)
                {
                    myQueue->enqueue(customers[i]);
                }
                myQueue->runSimulation();
                delete myQueue;
                for (int i = 0; i < numCustomers; ++i)
                {
                    delete customers[i];
                }
                delete[] customers;
            }
            else
            {
                cout << "   Failed to read customer data from file." << endl;
            }
            break;
        }
        case 2:
        {
            cout << "   Enter the number of random customers to generate: ";
            int numRandomCustomers;
            cin >> numRandomCustomers;
            if (numRandomCustomers <= 0) {
                cout << "   Invalid input. Number of random customers must be greater than 0." << endl;
                validChoice = false;
                break;
            }
            Customer **customers = generateRandomCustomers(numRandomCustomers);
            if (customers != nullptr)
            {
                BankQueue *myQueue = new BankQueue(numRandomCustomers, customers);
                for (int i = 0; i < numRandomCustomers; ++i)
                {
                    myQueue->enqueue(customers[i]);
                }
                myQueue->runSimulation();
                delete myQueue;
                for (int i = 0; i < numRandomCustomers; ++i)
                {
                    delete customers[i];
                }
                delete[] customers;
            }
            else
            {
                cout << "   Failed to generate random customer data." << endl;
            }
            break;
        }
        case 3:
            cout << "   Exiting program." << endl;
            break;
        default:
            cout << "   Invalid choice. Please enter a valid option (1, 2, or 3)." << endl;
            validChoice = false;
            break;
        }

        if (!validChoice)
        {
            break;
        }
    } while (choice != 3);

    return 0;
}


