#include <iostream>
#include <thread>
#include <chrono>
#include <cctype>

std::mutex stationMutex;
bool stationOccupied = false;

void trainArrival(char symbol, int travelTime) {
    std::this_thread::sleep_for(std::chrono::seconds(travelTime));

    std::unique_lock<std::mutex> lock(stationMutex);
    std::cout << "Train " << symbol << " has arrived at the station.\n";

    while (stationOccupied) {
        std::cout << "Train " << symbol << " is waiting for a free seat.\n";
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        lock.lock();
    }

    stationOccupied = true;
    std::cout << "Train " << symbol << " is in the station.\n";
    lock.unlock();

    // Wait for the departure signal
    std::string command;
    while (true) {
        std::cin >> command;
        if (command == "depart") {
            std::unique_lock<std::mutex> lock(stationMutex);
            stationOccupied = false;
            std::cout << "Train " << symbol << " has departed from the station.\n";
            break;
        }
    }
}

int getTimeInSeconds() {
    while (true) {
        std::string input;
        std::cin >> input;

        bool isNumeric = true;
        for (char c : input) {
            if (!std::isdigit(c)) {
                isNumeric = false;
                break;
            }
        }

        if (isNumeric) {
            return std::stoi(input);
        } else {
            std::cout << "Invalid input! Please enter a valid time in seconds: ";
        }
    }
}

int main() {
    int travelTimeA, travelTimeB, travelTimeC;
    std::cout << "Enter travel time for Train A (in seconds): ";
    travelTimeA = getTimeInSeconds();
    std::cout << "Enter travel time for Train B (in seconds): ";
    travelTimeB = getTimeInSeconds();
    std::cout << "Enter travel time for Train C (in seconds): ";
    travelTimeC = getTimeInSeconds();

    std::thread trainThreadA(trainArrival, 'A', travelTimeA);
    std::thread trainThreadB(trainArrival, 'B', travelTimeB);
    std::thread trainThreadC(trainArrival, 'C', travelTimeC);

    trainThreadA.join();
    trainThreadB.join();
    trainThreadC.join();

    return 0;
}
