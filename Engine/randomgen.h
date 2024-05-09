#ifndef RANDOMGEN_H
#define RANDOMGEN_H
#include <random>

class Random {
public:
    // Static function to generate a random integer between min and max (inclusive)
    static int generateRandomInt(int min, int max) {
        // Create a random number engine
        static std::random_device rd;
        static std::mt19937 gen(rd()); // Mersenne Twister pseudo-random number generator

        // Define a distribution
        std::uniform_int_distribution<int> distribution(min, max);

        // Generate a random integer within the range
        return distribution(gen);
    }

    // Static function to generate a random float between min and max (inclusive)
    static float generateRandomFloat(float min, float max) {
        // Create a random number engine
        static std::random_device rd;
        static std::mt19937 gen(rd()); // Mersenne Twister pseudo-random number generator

        // Define a distribution
        std::uniform_real_distribution<float> distribution(min, max);

        // Generate a random float within the range
        return distribution(gen);
    }
};

#endif // RANDOMGEN_H
