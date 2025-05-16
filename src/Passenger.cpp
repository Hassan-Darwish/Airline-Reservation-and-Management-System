/******************************************************************************************
 * MODULE NAME    : Passenger Module
 * FILE           : Passenger.cpp
 * DESCRIPTION    : Implements the Passenger class methods for searching flights,
 *                  booking, viewing/canceling reservations, check-in, and payment.
 * AUTHOR         : Hassan Darwish
 * DATE CREATED   : May 2025
 ******************************************************************************************/

#include "../header/Passenger.hpp"
#include "../header/Payment.hpp"
#include "../header/json.hpp"
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;
using json = nlohmann::json;

/**
 * Constructor: Initializes Passenger and loads existing reservations.
 */
Passenger::Passenger(string username, string password)
    : User(username, password, "Passenger")
{
    reservations = Reservation::loadReservations();
}

/**
 * Displays the main menu and dispatches user choices.
 */
void Passenger::displayMenu(void)
{
    int choice;
    do
    {
        cout << "\n--- Passenger Menu ---\n"
             << "1. Search Flights\n"
             << "2. View My Reservations\n"
             << "3. Check-In\n"
             << "4. Cancel Reservation\n"
             << "5. Confirm Cash Payment\n"
             << "6. Logout\n"
             << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1: searchFlights();          break;
            case 2: viewSpecificReservations(); break;
            case 3: checkIn();                break;
            case 4: cancelReservation();      break;
            case 5: confirmCashPayment();     break;
            case 6: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice! Please try again.\n"; break;
        }
    }
    while (choice != 6);
}

/**
 * Prompts for origin/destination, lists matching flights, then books.
 */
void Passenger::searchFlights(void)
{
    string origin, destination;
    cout << "\n--- Search Flights ---\n"
         << "Enter Origin: ";
    cin.ignore();
    getline(cin, origin);

    cout << "Enter Destination: ";
    getline(cin, destination);

    auto flights = Flight::loadFlights(PATH_OF_FLIGHTS_DATA_BASE);
    vector<Flight> matches;

    for (auto const& fl : flights)
    {
        if (fl.getOrigin() == origin && fl.getDestination() == destination)
            matches.push_back(fl);
    }

    if (matches.empty())
    {
        cout << "No flights found for the given criteria.\n";
        return;
    }

    cout << "\nAvailable Flights:\n";
    for (size_t i = 0; i < matches.size(); ++i)
    {
        cout << i + 1 << ". Flight Number: "
             << matches[i].getFlightNumber() << "\n";
        matches[i].displayFlightInfo();
    }

    bookFlight();
}

/**
 * Generates a random reservation ID: Letter + 3-digit number.
 */
static string generateRandomReservationID(void)
{
    char letter = 'A' + rand() % 26;
    int  num    = rand() % 900 + 100;
    return string(1, letter) + to_string(num);
}

/**
 * Books a flight: select flight, seat, process payment, save reservation.
 */
void Passenger::bookFlight(void)
{
    string flightNumber;
    cout << "\nEnter Flight Number to book (or '0' to cancel): ";
    cin >> flightNumber;
    if (flightNumber == "0") return;

    auto flightsVec = Flight::loadFlights(PATH_OF_FLIGHTS_DATA_BASE);
    shared_ptr<Flight> selected;

    for (auto const& fl : flightsVec)
        if (fl.getFlightNumber() == flightNumber)
            selected = make_shared<Flight>(fl);

    if (!selected)
    {
        cout << "Invalid flight number. Booking cancelled.\n";
        return;
    }

    cout << "Enter Seat Number (e.g., 14): ";
    string seat;
    cin >> seat;

    if (stoi(seat) > selected->getTotalSeats())
    {
        cout << "Invalid seat number. Must be ≤ "
             << selected->getTotalSeats() << ".\n";
        return;
    }

    bool occupied = false;
    auto allRes = Reservation::loadReservations();
    for (auto const& res : allRes)
    {
        if (res->getFlight()->getFlightNumber() == flightNumber &&
            res->getSeatNumber() == seat)
        {
            occupied = true;
            break;
        }
    }

    if (occupied)
    {
        cout << "Seat " << seat << " already reserved. Choose another.\n";
        return;
    }

    srand(static_cast<unsigned>(time(nullptr)));
    string resID = generateRandomReservationID();
    auto newRes  = make_shared<Reservation>(
        resID, username, selected, seat, "", ""
    );

    bool paid = Payment::processPayment(newRes);
    if (paid && newRes->getIsPaid())
    {
        allRes.push_back(newRes);
        Reservation::saveReservations(allRes);
        cout << "Reservation successful!\n";
        newRes->displayReservation();
    }
    else if (!newRes->getIsPaid())
    {
        cout << "Reservation on hold. Confirm payment at airport.\n";
        allRes.push_back(newRes);
        Reservation::saveReservations(allRes);
        newRes->displayHoldingReservation();
    }
    else
    {
        cout << "Payment failed. Booking aborted.\n";
    }

    reservations = Reservation::loadReservations();
}

/**
 * Displays reservations filtered by current username.
 */
void Passenger::viewSpecificReservations(void)
{
    if (reservations.empty())
    {
        cout << "\nNo reservations found.\n";
        return;
    }

    cout << "\n--- My Reservations ---\n";
    bool found = false;
    for (auto const& res : reservations)
    {
        if (res->getPassengerName() == username)
        {
            res->displayReservation();
            found = true;
        }
    }
    if (!found)
        cout << "No reservations found for " << username << ".\n";
}

/**
 * Checks in using a Reservation ID; prints boarding pass if paid.
 */
void Passenger::checkIn(void)
{
    cout << "\n--- Check-In ---\n"
         << "Enter Reservation ID: ";
    string resID;
    cin >> resID;

    for (auto const& res : reservations)
    {
        if (res->getReservationID() == resID)
        {
            if (res->getIsPaid())
            {
                cout << "\n✅ Check-In Successful!\n";
                res->printBoardingPass();
            }
            else
            {
                cout << "Payment pending; cannot check in.\n";
            }
            return;
        }
    }
    cout << "Invalid Reservation ID.\n";
}

/**
 * Cancels a reservation by ID and reloads reservation list.
 */
void Passenger::cancelReservation(void)
{
    cout << "\n--- Cancel Reservation ---\n"
         << "Enter Reservation ID: ";
    string resID;
    cin >> resID;

    Reservation::removeReservation(resID);
    reservations = Reservation::loadReservations();
}

/**
 * Confirms a cash payment at airport and prints boarding pass.
 */
void Passenger::confirmCashPayment(void)
{
    cout << "\nEnter Reservation ID to confirm payment: ";
    string resID;
    cin >> resID;

    for (auto& res : reservations)
    {
        if (res->getReservationID() == resID && !res->getIsPaid())
        {
            cout << "Payment confirmed at airport!\n";
            res->setIsPaid(true);
            Reservation::saveReservations(reservations);
            cout << "Reservation confirmed.\n";
            res->printBoardingPass();
            return;
        }
    }
    cout << "Reservation ID not found or already paid.\n";
}

/******************************************************************************************
 * END OF FILE
 ******************************************************************************************/
