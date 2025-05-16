/******************************************************************************************
 * MODULE NAME    : Main Application
 * FILE           : main.cpp
 * DESCRIPTION    : Entry point for the Airline Reservation and Management System.
 *                  Handles user role selection, authentication, and dispatches to
 *                  Administrator, BookingAgent, or Passenger menus.
 * AUTHOR         : Hassan Darwish
 * DATE CREATED   : May 2025
 ******************************************************************************************/

#include <iostream>        // Console I/O
#include <fstream>         // File streams for JSON persistence
#include <iomanip>         // setw for JSON formatting
#include <cstdlib>         // srand, rand
#include <ctime>           // time
#include "../header/json.hpp"
#include "../header/Administrator.hpp"
#include "../header/Passenger.hpp"
#include "../header/BookingAgent.hpp"

using namespace std;
using json = nlohmann::json;

#define PATH_OF_USERS_DATA_BASE "data_base/users.json"


/*
* Checks if the user operating system 
*/
#if defined(_WIN32) || defined(_WIN64)
  #include <conio.h>
  static char getch_wrap() {
      return _getch();
  }
#else
  #include <termios.h>
  #include <unistd.h>
  static char getch_wrap() {
      struct termios oldt, newt;
      tcgetattr(STDIN_FILENO, &oldt);
      newt = oldt;
      newt.c_lflag &= ~(ECHO | ICANON);
      tcsetattr(STDIN_FILENO, TCSANOW, &newt);
      char c = static_cast<char>(std::getchar());
      tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
      return c;
  }
#endif

/**
 * Loads all users from the specified JSON file.
 * @param filename Path to the users JSON file.
 * @return JSON object containing user credentials and roles.
 */
json loadUsers(const string& filename)
{
    ifstream file(filename);
    json users;
    if (file.is_open()) {
        file >> users;
    }
    return users;
}

/**
 * Saves the provided users JSON object to the specified file.
 * @param filename Path to the users JSON file.
 * @param users    JSON object of user data to save.
 */
void saveUsers(const string& filename, const json& users)
{
    ofstream file(filename);
    file << setw(4) << users << endl;
}

/**
 * Reads a password from console input, masking characters with '*'.
 * Supports backspace to delete characters.
 * @return The entered password string.
 */
string getMaskedPassword(void)
{
    std::string password;
    char ch;

    // Use getch_wrap() instead of _getch()
    while ((ch = getch_wrap()) != '\r' && ch != '\n') {
        if ((ch == '\b' || ch == 127) && !password.empty()) {
            password.pop_back();
            std::cout << "\b \b";
        }
        else if (ch != '\b' && ch != 127) {
            password += ch;
            std::cout << '*';
        }
    }

    std::cout << std::endl;
    return password;
}

/**
 * Main function: prompts role selection, handles login/registration,
 * and dispatches to the appropriate user menu.
 */
int main(void)
{
    srand(static_cast<unsigned>(time(nullptr)));

    cout << "\t\t\t  Welcome to Airline Reservation and Management System\n\n"
         << "Please select your role:\n"
         << "1. Administrator\n"
         << "2. Booking Agent\n"
         << "3. Passenger\n"
         << "Enter choice: ";

    int choice;
    cin >> choice;

    string role;
    switch (choice) {
        case 1: role = "Administrator"; break;
        case 2: role = "Booking Agent";  break;
        case 3: role = "Passenger";      break;
        default:
            cout << "Invalid choice.\n";
            return 0;
    }

    auto usersJson = loadUsers(PATH_OF_USERS_DATA_BASE);

    cout << "\n--- " << role << " Login ---\n"
         << "Username: ";
    string username;
    cin >> username;

    cout << "Password: ";
    string password = getMaskedPassword();

    // Authenticate or register new user
    if (usersJson.contains(username)) {
        if (usersJson[username]["password"] == password &&
            usersJson[username]["role"]     == role)
        {
            cout << "\nLogin successful!\n";

            if (role == "Administrator") {
                Administrator admin(username, password);
                admin.displayMenu();
            }
            else if (role == "Passenger") {
                Passenger passenger(username, password);
                passenger.displayMenu();
            }
            else {  // Booking Agent
                BookingAgent agent(username, password);
                agent.displayMenu();
            }
        }
        else {
            cout << "\nInvalid credentials or role mismatch!\n";
        }
    }
    else {
        cout << "\nCreating new user account...\n";
        usersJson[username] = {
            {"password", password},
            {"role",     role}
        };
        saveUsers(PATH_OF_USERS_DATA_BASE, usersJson);
        cout << "Account created successfully!\n";

        if (role == "Administrator") {
            Administrator admin(username, password);
            admin.displayMenu();
        }
        else if (role == "Passenger") {
            Passenger passenger(username, password);
            passenger.displayMenu();
        }
        else {  // Booking Agent
            BookingAgent agent(username, password);
            agent.displayMenu();
        }
    }

    return 0;
}

/******************************************************************************************
 * END OF FILE
 ******************************************************************************************/
