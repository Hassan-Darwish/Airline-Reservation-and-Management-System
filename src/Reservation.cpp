/******************************************************************************************
 * MODULE NAME    : Reservation Module
 * FILE           : Reservation.cpp
 * DESCRIPTION    : Implements the Reservation class methods for creating, displaying,
 *                  serializing, and managing reservations, including file persistence
 *                  and payment status handling.
 * AUTHOR         : Hassan Darwish
 * DATE CREATED   : May 2025
 ******************************************************************************************/

#include "../header/Reservation.hpp"
#include "../header/json.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <memory>

using namespace std;
using json = nlohmann::json;

/**
 * Constructor: Initializes a Reservation with full details (unpaid by default).
 */
Reservation::Reservation(string reservationID,
                         string passengerName,
                         shared_ptr<Flight> flight,
                         string seatNumber,
                         string paymentMethod,
                         string paymentDetails)
    : reservationID(reservationID),
      passengerName(passengerName),
      flight(flight),
      seatNumber(seatNumber),
      paymentMethod(paymentMethod),
      paymentDetails(paymentDetails),
      isPaid(false)
{}

/**
 * Default Constructor: Initializes an empty Reservation (unpaid).
 */
Reservation::Reservation(void)
    : reservationID(""),
      passengerName(""),
      flight(nullptr),
      seatNumber(""),
      paymentMethod(""),
      paymentDetails(""),
      isPaid(false)
{}

/**
 * Getter: Returns the reservation ID.
 */
string Reservation::getReservationID(void) const
{
    return reservationID;
}

/**
 * Getter: Returns the passenger name.
 */
string Reservation::getPassengerName(void) const
{
    return passengerName;
}

/**
 * Getter: Returns the associated Flight pointer.
 */
shared_ptr<Flight> Reservation::getFlight(void) const
{
    return flight;
}

/**
 * Getter: Returns the seat number.
 */
string Reservation::getSeatNumber(void) const
{
    return seatNumber;
}

/**
 * Getter: Returns the payment method.
 */
string Reservation::getPaymentMethod(void) const
{
    return paymentMethod;
}

/**
 * Getter: Returns the payment details.
 */
string Reservation::getPaymentDetails(void) const
{
    return paymentDetails;
}

/**
 * Serializes this Reservation into JSON.
 */
json Reservation::toJson(void) const
{
    return json{
        {"reservationID",   reservationID},
        {"passengerName",   passengerName},
        {"flight",          flight->toJson()},
        {"seatNumber",      seatNumber},
        {"paymentMethod",   paymentMethod},
        {"paymentDetails",  paymentDetails},
        {"isPaid",          isPaid}
    };
}

/**
 * Deserializes a JSON object into a Reservation.
 */
Reservation Reservation::fromJson(const json& j)
{
    auto flightPtr = make_shared<Flight>(Flight::fromJson(j.at("flight")));
    Reservation res(
        j.at("reservationID").get<string>(),
        j.at("passengerName").get<string>(),
        flightPtr,
        j.at("seatNumber").get<string>(),
        j.at("paymentMethod").get<string>(),
        j.at("paymentDetails").get<string>()
    );

    if (j.contains("isPaid"))
        res.setIsPaid(j.at("isPaid").get<bool>());

    return res;
}

/**
 * Displays reservation details.
 */
void Reservation::displayReservation(void) const
{
    cout << "\n--- Reservation Details ---\n"
         << "Reservation ID : " << reservationID << '\n'
         << "Passenger Name : " << passengerName << '\n'
         << "Seat Number    : " << seatNumber   << '\n'
         << "Payment Method : " << paymentMethod << '\n'
         << "Flight Details :\n";
    flight->displayFlightInfo();
}

/**
 * Displays reservation with payment-on-hold notice.
 */
void Reservation::displayHoldingReservation(void) const
{
    cout << "\n--- Reservation Details (On Hold) ---\n"
         << "Reservation ID : " << reservationID << '\n'
         << "Passenger Name : " << passengerName << '\n'
         << "Seat Number    : " << seatNumber   << '\n'
         << "Payment Method : " << paymentMethod << '\n'
         << "Payment on hold. Please complete payment at airport.\n"
         << "Flight Details :\n";
    flight->displayFlightInfo();
}

/**
 * Prints a formatted boarding pass for this reservation.
 */
void Reservation::printBoardingPass(void) const
{
    cout << "\nBoarding Pass:\n"
         << "-----------------------------\n"
         << "Reservation ID: " << reservationID   << '\n'
         << "Passenger:      " << passengerName   << '\n'
         << "Flight:         " << flight->getFlightNumber() << '\n'
         << "Origin:         " << flight->getOrigin()       << '\n'
         << "Destination:    " << flight->getDestination()  << '\n'
         << "Departure:      " << flight->getDepartureTime() << '\n'
         << "Seat:           " << seatNumber << '\n'
         << "-----------------------------\n";
}

/**
 * Adds a new reservation and saves to file.
 */
void Reservation::addReservation(const Reservation& newReservation)
{
    auto resList = loadReservations();
    resList.push_back(make_shared<Reservation>(newReservation));
    saveReservations(resList);
    cout << "Reservation added successfully!\n";
}

/**
 * Adds a booking-agent reservation and saves to agent file.
 */
void Reservation::addBookingAgentReservation(const Reservation& newReservation)
{
    auto resList = loadBookingAgentReservations();
    resList.push_back(make_shared<Reservation>(newReservation));
    saveBookingAgentReservations(resList);
    cout << "Reservation added successfully!\n";
}

/**
 * Removes a reservation by ID and saves updated list.
 */
void Reservation::removeReservation(const string& reservationID)
{
    auto resList = loadReservations();
    auto it = find_if(resList.begin(), resList.end(),
                      [&](auto const& r){ return r->getReservationID() == reservationID; });

    if (it != resList.end())
    {
        resList.erase(it);
        saveReservations(resList);
        cout << "Reservation " << reservationID << " removed successfully!\n";
    }
    else
    {
        cout << "Reservation ID " << reservationID << " not found!\n";
    }
}

/**
 * Saves a list of reservations to the main file, avoiding duplicates.
 */
void Reservation::saveReservations(const vector<shared_ptr<Reservation>>& reservations)
{
    json allRes;
    ifstream inFile(PATH_OF_RESERVATION_DATA_BASE);
    if (inFile.is_open())
    {
        inFile >> allRes;
        inFile.close();
    }

    for (auto const& res : reservations)
    {
        auto& userArr = allRes[res->getPassengerName()];
        bool exists = false;
        for (auto const& e : userArr)
        {
            if (e["reservationID"] == res->getReservationID())
            {
                exists = true;
                break;
            }
        }
        if (!exists)
            userArr.push_back(res->toJson());
    }

    ofstream outFile(PATH_OF_RESERVATION_DATA_BASE);
    if (!outFile.is_open())
    {
        cerr << "Error: Unable to save reservations.\n";
        return;
    }
    outFile << setw(4) << allRes << endl;
}

/**
 * Saves a list of reservations to the booking agent file, avoiding duplicates.
 */
void Reservation::saveBookingAgentReservations(const vector<shared_ptr<Reservation>>& reservations)
{
    json allRes;
    ifstream inFile(PATH_OF_BOOKING_AGENT_RESERVATION_DATA_BASE);
    if (inFile.is_open())
    {
        inFile >> allRes;
        inFile.close();
    }

    for (auto const& res : reservations)
    {
        auto& userArr = allRes[res->getPassengerName()];
        bool exists = false;
        for (auto const& e : userArr)
        {
            if (e["reservationID"] == res->getReservationID())
            {
                exists = true;
                break;
            }
        }
        if (!exists)
            userArr.push_back(res->toJson());
    }

    ofstream outFile(PATH_OF_BOOKING_AGENT_RESERVATION_DATA_BASE);
    if (!outFile.is_open())
    {
        cerr << "Error: Unable to save booking agent reservations.\n";
        return;
    }
    outFile << setw(4) << allRes << endl;
}

/**
 * Loads all reservations from the main file into a vector.
 */
vector<shared_ptr<Reservation>> Reservation::loadReservations(void)
{
    vector<shared_ptr<Reservation>> list;
    ifstream file(PATH_OF_RESERVATION_DATA_BASE);
    if (!file.is_open())
    {
        cerr << "No reservation data found. Starting fresh.\n";
        return list;
    }

    json j; file >> j; file.close();

    for (auto const& user : j.items())
    {
        for (auto const& rj : user.value())
        {
            auto res = make_shared<Reservation>(fromJson(rj));
            if (rj.contains("isPaid"))
                res->setIsPaid(rj["isPaid"].get<bool>());
            list.push_back(res);
        }
    }

    return list;
}

/**
 * Loads all reservations from the booking agent file into a vector.
 */
vector<shared_ptr<Reservation>> Reservation::loadBookingAgentReservations(void)
{
    vector<shared_ptr<Reservation>> list;
    ifstream file(PATH_OF_BOOKING_AGENT_RESERVATION_DATA_BASE);
    if (!file.is_open())
    {
        cerr << "No booking agent reservation data found. Starting fresh.\n";
        return list;
    }

    json j; file >> j; file.close();

    for (auto const& user : j.items())
    {
        for (auto const& rj : user.value())
        {
            auto res = make_shared<Reservation>(fromJson(rj));
            if (rj.contains("isPaid"))
                res->setIsPaid(rj["isPaid"].get<bool>());
            list.push_back(res);
        }
    }

    return list;
}

/**
 * Setter: Updates the payment status.
 */
void Reservation::setIsPaid(bool status)
{
    isPaid = status;
}

/**
 * Getter: Returns the payment status.
 */
bool Reservation::getIsPaid(void) const
{
    return isPaid;
}

/**
 * Setter: Updates the payment method.
 */
void Reservation::setPaymentMethod(const string& method)
{
    paymentMethod = method;
}

/**
 * Setter: Updates the payment details.
 */
void Reservation::setPaymentDetails(const string& details)
{
    paymentDetails = details;
}

/******************************************************************************************
 * END OF FILE
 ******************************************************************************************/
