#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstdlib>
#include <ctime>

std::mutex kitchenMutex;
std::mutex courierMutex;
int successfulDeliveries = 0;

void cookOrder() {
    std::this_thread::sleep_for(std::chrono::seconds((std::rand() % 11) + 5));
}

void waiterThread() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    while (successfulDeliveries < 10) {
        std::this_thread::sleep_for(std::chrono::seconds((std::rand() % 6) + 5));

        // Generate a random dish
        std::string dish;
        int dishNumber = std::rand() % 5;
        switch (dishNumber) {
            case 0:
                dish = "Pizza";
                break;
            case 1:
                dish = "Soup";
                break;
            case 2:
                dish = "Steak";
                break;
            case 3:
                dish = "Salad";
                break;
            case 4:
                dish = "Sushi";
                break;
        }

        kitchenMutex.lock();
        std::cout << "Order received: " << dish << std::endl;
        kitchenMutex.unlock();

        // Cook the order
        std::thread cookThread(cookOrder);
        cookThread.detach();

        std::this_thread::sleep_for(std::chrono::seconds((std::rand() % 11) + 5));

        kitchenMutex.lock();
        std::cout << "Order ready: " << dish << std::endl;
        kitchenMutex.unlock();
    }
}

void courierThread() {
    while (successfulDeliveries < 10) {
        std::this_thread::sleep_for(std::chrono::seconds(30));

        courierMutex.lock();
        kitchenMutex.lock();

        std::cout << "Courier arrived and picked up the order." << std::endl;
        successfulDeliveries++;

        kitchenMutex.unlock();
        courierMutex.unlock();
    }
}

int main() {
    std::thread waiter(waiterThread);
    std::thread courier(courierThread);

    waiter.join();
    courier.join();

    return 0;
}
