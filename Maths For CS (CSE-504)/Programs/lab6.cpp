#include <iostream>
#include <cmath>
using namespace std;

class Die {
private:
    int sides;

public:
    // Constructor
    Die(int sides) : sides(sides) {}

    // Function to calculate expectation E[X]
    double expectation() const {
        return (1.0 + sides) / 2.0;
    }

    // Function to calculate expectation E[X^2]
    double expectationSquared() const {
        double sum = 0.0;
        for (int i = 1; i <= sides; ++i) {
            sum += pow(i, 2);
        }
        return sum / sides;
    }

    // Function to calculate variance of X
    double variance() const {
        double ex = expectation();
        double exSquared = expectationSquared();
        return exSquared - pow(ex, 2);
    }
};

int main() {
    int sides = 10; // Number of sides on the die

    // Create a Die object
    Die die(sides);

    // Calculate and print expectation E[X]
    cout << "Expectation E[X]: " << die.expectation() << endl;

    // Calculate and print expectation E[X^2]
    cout << "Expectation E[X^2]: " << die.expectationSquared() << endl;

    // Calculate and print variance of X
    cout << "Variance of X: " << die.variance() << endl;

    return 0;
}
