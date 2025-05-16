/******************************************************************************************
* MODULE NAME    : Crew Management Module
* FILE           : Crew.hpp
* DESCRIPTION    : This file defines the Crew class responsible for managing crew member
*                  details, flight assignments, and file storage operations.
* AUTHOR         : Hassan Darwish
* DATE CREATED   : May 2025
******************************************************************************************/

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Flight.hpp"
#include "json.hpp"

#define PATH_OF_CREW_DATA_BASE "data_base/crewData.json"

using namespace std;

/******************************************************************************************
* CLASS NAME     : Crew
* DESCRIPTION    : Represents a crew member, their role, and their assigned flights. 
*                  Includes functionality for managing flight assignments and 
*                  persistence to/from JSON files.
******************************************************************************************/
class Crew 
{
public:
    /*
    * Constructor: Initializes a crew member with a name and role.
    */
    Crew(string name, string role);

    /*
    * Description: Returns the crew member's name.
    */
    string getName(void) const;

    /*
    * Description: Returns the role of the crew member (e.g., pilot, co-pilot, attendant).
    */
    string getRole(void) const;

    /*
    * Description: Retrieves all flights assigned to the crew member.
    */
    vector<shared_ptr<Flight>> getAssignedFlights(void) const;

    /*
    * Description: Assigns a new flight to the crew member.
    */
    void assignFlight(shared_ptr<Flight> flight);

    /*
    * Description: Removes a flight from the crew member's assignments based on flight number.
    */
    void removeFlight(const string& flightNumber);

    /*
    * Description: Displays the crew member's basic information and assigned flights.
    */
    void displayCrewInfo(void) const;

    /*
    * Description: Saves the crew member's data to the specified JSON file.
    */
    void saveCrewToFile(const string& filename);

    /*
    * Description: Loads all crew data from the specified JSON file.
    */
    static void loadCrewFromFile(const string& filename);

    /*
    * Description: Removes a crew member from the JSON file by name.
    */
    static bool removeCrewFromFile(const string& crewName, const string& filename);

    /*
    * Description: Updates the assigned flights of a crew member in the JSON file.
    */
    static void updateCrewInFile(const string& crewName, const shared_ptr<Flight>& flight, const string& filename);

    /*
    * Description: Retrieves a pointer to a Crew object based on the crew member's name.
    */
    static Crew* getCrewByName(const string& crewName);

private:
    /*
    * Description: The name of the crew member.
    */
    string name;

    /*
    * Description: The role of the crew member (e.g., pilot, flight attendant).
    */
    string role;

    /*
    * Description: List of flights assigned to the crew member.
    */
    vector<shared_ptr<Flight>> assignedFlights;
};

/******************************************************************************************
* END OF FILE
******************************************************************************************/
