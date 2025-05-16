/******************************************************************************************
* MODULE NAME    : Passenger Module
* FILE           : Passenger.hpp
* DESCRIPTION    : Declares the Passenger class responsible for managing passenger-related
*                  activities such as booking, check-in, payment, and viewing reservations.
* AUTHOR         : Hassan Darwish
* DATE CREATED   : May 2025
******************************************************************************************/

#pragma once

#include "User.hpp"
#include "Flight.hpp"
#include "Reservation.hpp"
#include "Payment.hpp"
#include <vector>
#include <memory>

/******************************************************************************************
* CLASS NAME     : Passenger
* DESCRIPTION    : Inherits from User and manages the interaction of a passenger with
*                  the system including flight booking, reservation, and payment.
******************************************************************************************/
class Passenger : public User 
{
public:
    /*
    * Constructor: Initializes a Passenger object with username and password.
    */
    Passenger(std::string username, std::string password);

    /*
    * Displays the passenger's interactive menu options.
    */
    void displayMenu(void);

    /*
    * Allows the passenger to search for available flights.
    */
    void searchFlights(void);

    /*
    * Handles the booking process of a selected flight.
    */
    void bookFlight(void);

    /*
    * Displays all reservations associated with the passenger.
    */
    void viewAllReservations(void);

    /*
    * Handles check-in for a booked flight and prints the boarding pass.
    */
    void checkIn(void);

    /*
    * Cancels a specific reservation for the passenger.
    */
    void cancelReservation(void);

    /*
    * Displays a specific reservation in detail.
    */
    void viewSpecificReservations(void);

    /*
    * Confirms that a cash payment has been made at the airport.
    */
    void confirmCashPayment(void);

private:
    std::vector<std::shared_ptr<Reservation>> reservations;  // Stores passenger reservations
};

/******************************************************************************************
* END OF FILE
******************************************************************************************/
