#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>

// Global constants for file name
const std::string DATABASE_FILE = "atm_db.txt";

// Function to create a new account
void createAccount() {
    std::string user_id, password;
    std::cout << "Please enter your user id: ";
    std::cin >> user_id;
    std::cout << "Please enter your password: ";
    std::cin >> password;

    std::ofstream file(DATABASE_FILE, std::ios::app);
    if (file.is_open()) {
        file << user_id << ":" << password << ":0" << std::endl;
        file.close();
        std::cout << "Thank You! Your account has been created!" << std::endl;
    } else {
        std::cout << "Error: Unable to open file for writing." << std::endl;
    }
}

// Function to check login credentials
bool login(std::string& user_id, std::string& password, double& balance) {
    std::ifstream file(DATABASE_FILE);
    std::string line, stored_id, stored_pass;
    double stored_balance;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            size_t pos = line.find(':');
            if (pos != std::string::npos) {
                stored_id = line.substr(0, pos);
                line = line.substr(pos + 1);
                pos = line.find(':');
                stored_pass = line.substr(0, pos);
                stored_balance = std::stod(line.substr(pos + 1));
                if (user_id == stored_id && password == stored_pass) {
                    balance = stored_balance;
                    file.close();
                    return true;
                }
            }
        }
        file.close();
    } else {
        std::cout << "Error: Unable to open file for reading." << std::endl;
    }
    return false;
}

// Function to update balance in the database
void updateBalance(const std::string& user_id, double new_balance) {
    std::ifstream file(DATABASE_FILE);
    std::ofstream temp("temp.txt");

    std::string line, stored_id, stored_pass;
    double stored_balance;

    if (file.is_open() && temp.is_open()) {
        while (std::getline(file, line)) {
            size_t pos = line.find(':');
            if (pos != std::string::npos) {
                stored_id = line.substr(0, pos);
                line = line.substr(pos + 1);
                pos = line.find(':');
                stored_pass = line.substr(0, pos);
                stored_balance = std::stod(line.substr(pos + 1));
                if (user_id == stored_id) {
                    temp << stored_id << ":" << stored_pass << ":" << new_balance << std::endl;
                } else {
                    temp << line << std::endl;
                }
            }
        }
        file.close();
        temp.close();
        remove(DATABASE_FILE.c_str());
        rename("temp.txt", DATABASE_FILE.c_str());
    } else {
        std::cout << "Error: Unable to open file for updating." << std::endl;
    }
}

// Main menu for transactions
void mainMenu(std::string user_id, double& balance) {
    char choice;
    double amount;
    do {
        std::cout << "d -> Deposit Money" << std::endl;
        std::cout << "w -> Withdraw Money" << std::endl;
        std::cout << "r -> Request Balance" << std::endl;
        std::cout << "q -> Quit" << std::endl;
        std::cout << "> ";
        std::cin >> choice;

        switch (choice) {
            case 'd':
                std::cout << "Amount of deposit: $";
                std::cin >> amount;
                balance += amount;
                updateBalance(user_id, balance);
                break;
            case 'w':
                std::cout << "Amount of withdrawal: $";
                std::cin >> amount;
                if (amount > balance) {
                    std::cout << "Insufficient funds!" << std::endl;
                } else {
                    balance -= amount;
                    updateBalance(user_id, balance);
                }
                break;
            case 'r':
                std::cout << "Your balance is $" << std::fixed << std::setprecision(2) << balance << "." << std::endl;
                break;
            case 'q':
                std::cout << "Thank you for using our ATM!" << std::endl;
                break;
            default:
                std::cout << "Invalid option, please try again." << std::endl;
        }
    } while (choice != 'q');
}

// Introduction menu
void introMenu() {
    char choice;
    std::string user_id, password;
    double balance;

    do {
        std::cout << "l -> Login" << std::endl;
        std::cout << "c -> Create New Account" << std::endl;
        std::cout << "q -> Quit" << std::endl;
        std::cout << "> ";
        std::cin >> choice;

        switch (choice) {
            case 'l':
                std::cout << "Please enter your user id: ";
                std::cin >> user_id;
                std::cout << "Please enter your password: ";
                std::cin >> password;
                if (login(user_id, password, balance)) {
                    std::cout << "Access Granted!" << std::endl;
                    mainMenu(user_id, balance);
                } else {
                    std::cout << "*** LOGIN FAILED! ***" << std::endl;
                }
                break;
            case 'c':
                createAccount();
                break;
            case 'q':
                std::cout << "Thank you for using our ATM!" << std::endl;
                break;
            default:
                std::cout << "Invalid option, please try again." << std::endl;
        }
    } while (choice != 'q');
}

int main() {
    introMenu();
    return 0;
}
