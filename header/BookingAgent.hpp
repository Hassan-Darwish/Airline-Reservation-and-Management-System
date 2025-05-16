/******************************************************************************************
* MODULE NAME    : Booking Agent Module
* FILE           : BookingAgent.hpp
* DESCRIPTION    : This file defines the BookingAgent class responsible for handling
*                  reservations, flight search, check-ins, and booking management.
* AUTHOR         : Hassan Darwish
* DATE CREATED   : May 2025
******************************************************************************************/

#pragma once

#include "User.hpp"
#include "Flight.hpp"
#include "Reservation.hpp"
#include "Payment.hpp"
#include "json.hpp"
#include <vector>
#include <memory>
#include <string>
#include <fstream>

using namespace std;

/******************************************************************************************
* CLASS NAME     : BookingAgent
* DESCRIPTION    : Inherits from User. Manages booking operations, check-ins, payments,
*                  and flight-related services for customers.
******************************************************************************************/
class BookingAgent : public User 
{
public:
    /*
    * Constructor: Initializes BookingAgent with username and password.
    */
    BookingAgent(string username, string password);

    /*
    * Description: Displays the booking agent menu for operations.
    */
    void displayMenu(void) override;

    /*
    * Description: Allows searching available flights.
    */
    void searchFlights(void);

    /*
    * Description: Facilitates booking a flight for a customer.
    */
    void bookFlight(void);

    /*
    * Description: Displays all reservations made by the user.
    */
    void viewAllReservations(void);

    /*
    * Description: Allows the user to check in and prints the boarding pass.
    */
    void checkIn(void);

    /*
    * Description: Cancels an existing reservation.
    */
    void cancelReservation(void);

    /*
    * Description: Views a specific reservation by ID or filter.
    */
    void viewSpecificReservations(void);

    /*
    * Description: Confirms cash payment made at the airport.
    */
    void confirmCashPayment(void);

private:
    /*
    * Description: Stores reservations associated with the booking agent.
    */
    vector<shared_ptr<Reservation>> reservations;

    /*
    * Description: Generates a random reservation ID.
    */
    string generateRandomReservationID(void);
};

/******************************************************************************************
* END OF FILE
******************************************************************************************/
