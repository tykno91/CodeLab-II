#include <iostream>
#include <iomanip>
#include <limits>

// Enumeration for supported fuel types
enum class FuelType {
    Diesel = 1,
    Petrol = 2
};

// Prompts the user for travel distance with input validation
double getDistanceTravelled() {
    double distance = 0.0;
    while (true) {
        std::cout << "Enter distance travelled (in miles): ";
        if (std::cin >> distance && distance >= 0.0) {
            return distance;
        }
        std::cout << "Invalid input. Please enter a non-negative number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// Prompts the user to select the fuel type
FuelType getFuelType() {
    int choice = 0;
    while (true) {
        std::cout << "\nSelect Fuel Type:\n";
        std::cout << "1. Diesel\n";
        std::cout << "2. Petrol\n";
        std::cout << "Enter your choice (1 or 2): ";
        if (std::cin >> choice && (choice == 1 || choice == 2)) {
            return static_cast<FuelType>(choice);
        }
        std::cout << "Invalid selection. Please choose 1 or 2.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// Calculates total CO2 emissions based on journey distance and fuel type
double calculateEmissions(double distance, FuelType fuel) {
    // 9.66 litres per 100 miles = 0.0966 litres per mile
    const double litresPerMile = 9.66 / 100.0;
    const double dieselEmissionFactor = 2.68; // kg CO2 per litre
    const double petrolEmissionFactor = 2.31; // kg CO2 per litre

    double totalLitres = distance * litresPerMile;
    double emissionFactor = (fuel == FuelType::Diesel) ? dieselEmissionFactor : petrolEmissionFactor;

    return totalLitres * emissionFactor;
}

int main() {
    std::cout << "=======================================\n";
    std::cout << "        Car Journey CO2 Calculator     \n";
    std::cout << "=======================================\n";

    double distance = getDistanceTravelled();
    FuelType fuel = getFuelType();
    double co2Emissions = calculateEmissions(distance, fuel);

    std::cout << "\n------------- Summary -----------------\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Distance:       " << distance << " miles\n";
    std::cout << "Fuel Type:      " << (fuel == FuelType::Diesel ? "Diesel" : "Petrol") << "\n";
    std::cout << "Total CO2:      " << co2Emissions << " kg CO2\n";
    std::cout << "=======================================\n";

    return 0;
}