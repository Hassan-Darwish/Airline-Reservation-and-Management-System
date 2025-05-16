/******************************************************************************************
* MODULE NAME    : Administrator Module
* FILE           : Administrator.hpp
* DESCRIPTION    : This file defines the Administrator class which inherits from User. 
*                  It provides administrative control for managing flights, aircrafts, 
*                  crew assignments, and generating reports in the airline system.
* AUTHOR         : Hassan Darwish
* DATE CREATED   : May 2025
******************************************************************************************/

#pragma once

#include "User.hpp"
#include "Flight.hpp"
#include "Reservation.hpp"
#include "Aircraft.hpp"
#include "Maintenance.hpp"
#include "Crew.hpp"
#include <vector>
#include <string>
#include "json.hpp"

/******************************************************************************************
* CLASS NAME     : Administrator
* DESCRIPTION    : Administrator class extends the User class and contains high-level 
*                  administrative operations on system resources like flights, aircraft, 
*                  and crew management.
******************************************************************************************/
class Administrator : public User {
public:
    /*
    * Constructor: Initializes the Administrator object with a username and password.
    */
    Administrator(std::string username, std::string password);

    /*
    * Description: Displays the menu interface for the Administrator role.
    */
    void displayMenu() override;

    /*
    * Description: Entry point to access the flight management submenu.
    */
    void manageFlights();

    /*
    * Description: Adds a new flight to the system's schedule.
    */
    void addNewFlight();

    /*
    * Description: Updates the details of an existing flight.
    */
    void updateFlight();

    /*
    * Description: Removes a flight from the schedule.
    */
    void removeFlight();

    /*
    * Description: Displays a list of all available flights.
    */
    void viewAllFlights();

    /*
    * Description: Generates a comprehensive operational report for analysis.
    */
    void generateOperationalReport();

    /*
    * Description: Assigns available crew members to a flight.
    */
    void assignCrew();

    /*
    * Description: Deletes crew records or their association from flights.
    */
    void deleteCrew();

    /*
    * Description: Modifies the crew assignments for a given flight.
    */
    void changeCrewAssignment();

    /*
    * Description: Assigns a new crew to a flight from scratch.
    */
    void assignNewCrew();

    /*
    * Description: Entry point to access the aircraft management submenu.
    */
    void manageAircraft();

    /*
    * Description: Adds a new aircraft to the fleet.
    */
    void addAircraft();

    /*
    * Description: Edits information of an existing aircraft.
    */
    void editAircraft();

    /*
    * Description: Deletes an aircraft from the system.
    */
    void deleteAircraft();
};

/******************************************************************************************
* END OF FILE
******************************************************************************************/
