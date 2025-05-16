/******************************************************************************************
 * MODULE NAME    : BookingAgent Module
 * FILE           : BookingAgent.cpp
 * DESCRIPTION    : Implements the BookingAgent class, providing menu-driven methods
 *                  for searching flights, booking, viewing/canceling reservations,
 *                  check-in, and handling payments.
 * AUTHOR         : Hassan Darwish
 * DATE CREATED   : May 2025
 ******************************************************************************************/

#include "../header/BookingAgent.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;
using json = nlohmann::json;

/**
 * Constructor: Initializes BookingAgent and loads existing reservations.
 */
BookingAgent::BookingAgent(string username, string password)
    : User(username, password, "BookingAgent")
{
    reservations = Reservation::loadReservations();
}

/**
 * Displays the main menu and dispatches user choices.
 */
void BookingAgent::displayMenu(void)
{
    int choice;
    do
    {
        cout << "\n--- Booking Agent Menu ---\n"
             << "1. Search Flights\n"
             << "2. Book a Flight\n"
             << "3. View My Reservations\n"
             << "4. Modify Reservation\n"
             << "5. Cancel Reservation\n"
             << "6. Logout\n"
             << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1: searchFlights();            break;
            case 2: bookFlight();               break;
            case 3: viewSpecificReservations(); break;
            case 5: cancelReservation();        break;
            case 6: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice! Please try again.\n"; break;
        }
    }
    while (choice != 6);
}

/**
 * Prompts for origin/destination, lists matching flights.
 */
void BookingAgent::searchFlights(void)
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
}

/**
 * Generates a random reservation ID: Letter + 3-digit number.
 */
string BookingAgent::generateRandomReservationID(void)
{
    char letter = 'A' + rand() % 26;
    int  num    = rand() % 900 + 100;
    return string(1, letter) + to_string(num);
}

/**
 * Allows booking a flight: select flight, seat, process payment, save reservation.
 */
void BookingAgent::bookFlight(void)
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
    auto agentRes = Reservation::loadBookingAgentReservations();
    for (auto const& res : agentRes)
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

    srand(time(nullptr));
    string resID = generateRandomReservationID();
    auto newRes  = make_shared<Reservation>(
        resID, username, selected, seat, "", ""
    );

    bool paid = Payment::processPayment(newRes);
    if (paid && newRes->getIsPaid())
    {
        agentRes.push_back(newRes);
        Reservation::saveReservations(agentRes);
        Reservation::saveBookingAgentReservations(agentRes);
        cout << "Reservation successful!\n";
        newRes->displayReservation();
    }
    else if (!newRes->getIsPaid())
    {
        cout << "Reservation on hold. Confirm payment at airport.\n";
        agentRes.push_back(newRes);
        Reservation::saveReservations(agentRes);
        Reservation::saveBookingAgentReservations(agentRes);
        newRes->displayHoldingReservation();
    }
    else
    {
        cout << "Payment failed. Booking aborted.\n";
    }
}

/**
 * Displays all reservations for the logged-in user.
 */
void BookingAgent::viewAllReservations(void)
{
    if (reservations.empty())
    {
        cout << "\nNo reservations found.\n";
        return;
    }

    cout << "\n--- My Reservations ---\n";
    for (auto const& res : reservations)
        res->displayReservation();
}

/**
 * Displays reservations filtered by current username.
 */
void BookingAgent::viewSpecificReservations(void)
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
void BookingAgent::checkIn(void)
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
void BookingAgent::cancelReservation(void)
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
void BookingAgent::confirmCashPayment(void)
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
            Reservation::saveBookingAgentReservations(reservations);
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
