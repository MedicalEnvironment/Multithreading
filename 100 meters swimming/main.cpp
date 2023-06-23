#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <cctype>

// Structure to store swimmer information
struct Swimmer {
    std::string name;
    double speed; // meters per second
    double position; // current position in meters

    Swimmer(const std::string& n, double s) : name(n), speed(s), position(0) {}
};

// Function to simulate the swim race for a single swimmer
void swim(Swimmer& swimmer, int& count) {
    while (swimmer.position < 100) {
        // Update swimmer's position
        swimmer.position += swimmer.speed;

        // Increment the count atomically
        ++count;

        // Print swimmer's progress
        std::cout << swimmer.name << " has swum " << swimmer.position << " meters.\n";

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    std::vector<Swimmer> swimmers;
    std::vector<std::thread> threads;

    // Get swimmers' names and speeds from user
    for (int i = 0; i < 6; ++i) {
        std::string name;
        std::string speedInput; // Store speed as string initially
        double speed;

        std::cout << "Enter swimmer " << i + 1 << "'s name: ";
        std::getline(std::cin >> std::ws, name);
        std::cout << "Enter swimmer " << i + 1 << "'s speed (m/s): ";
        std::getline(std::cin >> std::ws, speedInput);

        bool containsLetters = false;
        for (char c : speedInput) {
            if (!std::isdigit(c)) {
                containsLetters = true;
                break;
            }
        }

        if (containsLetters) {
            std::cout << "Speed cannot contain letters. Please try again.\n";
            --i; // Retry input for the current swimmer
            continue;
        }

        speed = std::stod(speedInput); // Convert the valid speed string to double

        swimmers.emplace_back(name, speed);
    }

    // Variable to keep track of the number of swimmers who have reached the mark
    int count = 0;

    // Create threads for each swimmer
    for (auto& swimmer : swimmers) {
        threads.emplace_back(swim, std::ref(swimmer), std::ref(count));
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    // Sort swimmers based on their final swim time
    std::sort(swimmers.begin(), swimmers.end(), [](const Swimmer& a, const Swimmer& b) {
        return a.position < b.position;
    });

    // Display the final results
    std::cout << "\nFinal Results:\n";
    std::cout << "-----------------------------------\n";
    std::cout << "Swimmer\t\tDistance (meters)\n";
    std::cout << "-----------------------------------\n";
    for (const auto& swimmer : swimmers) {
        std::cout << swimmer.name << "\t\t" << swimmer.position << "\n";
    }

    return 0;
}
