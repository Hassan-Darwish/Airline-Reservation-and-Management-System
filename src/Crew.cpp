/******************************************************************************************
 * MODULE NAME    : Crew Management Module
 * FILE           : Crew.cpp
 * DESCRIPTION    : Implements the Crew class methods for managing crew member details,
 *                  flight assignments, and JSON file persistence.
 * AUTHOR         : Hassan Darwish
 * DATE CREATED   : May 2025
 ******************************************************************************************/

#include "../header/Crew.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>

using json = nlohmann::json;

// Global storage of all crew members loaded from file
std::vector<Crew> allCrewMembers;

/**
 * Constructor: Initializes a Crew member with name and role.
 */
Crew::Crew(std::string name, std::string role)
    : name(name), role(role)
{}

/**
 * Getter: Returns the crew member's name.
 */
std::string Crew::getName(void) const
{
    return name;
}

/**
 * Getter: Returns the crew member's role.
 */
std::string Crew::getRole(void) const
{
    return role;
}

/**
 * Getter: Returns the list of flights assigned to this crew member.
 */
std::vector<std::shared_ptr<Flight>> Crew::getAssignedFlights(void) const
{
    return assignedFlights;
}

/**
 * Assigns a flight to this crew member.
 */
void Crew::assignFlight(std::shared_ptr<Flight> flight)
{
    assignedFlights.push_back(flight);
}

/**
 * Removes a flight assignment by flight number.
 */
void Crew::removeFlight(const std::string& flightNumber)
{
    assignedFlights.erase(
        std::remove_if(
            assignedFlights.begin(),
            assignedFlights.end(),
            [&](const std::shared_ptr<Flight>& flight)
            {
                return flight->getFlightNumber() == flightNumber;
            }
        ),
        assignedFlights.end()
    );
}

/**
 * Displays the crew member's information and assigned flights.
 */
void Crew::displayCrewInfo(void) const
{
    std::cout << "Crew Name:       " << name << "\n"
              << "Role:            " << role << "\n"
              << "Assigned Flights: ";

    for (auto const& flight : assignedFlights)
        std::cout << flight->getFlightNumber() << " ";

    std::cout << "\n";
}

/**
 * Saves this crew member's assigned flights to the JSON file (overwrites entry).
 */
void Crew::saveCrewToFile(const std::string& filename)
{
    json crewJson;
    std::ifstream inFile(filename);

    if (inFile.is_open())
    {
        inFile >> crewJson;
        inFile.close();
    }

    json flightsArray = json::array();
    for (auto const& flight : assignedFlights)
        flightsArray.push_back(flight->toJson());

    crewJson[name] = flightsArray;

    std::ofstream outFile(filename);
    if (outFile.is_open())
    {
        outFile << std::setw(4) << crewJson << std::endl;
    }
    else
    {
        std::cerr << "Error: Unable to save crew data.\n";
    }
}

/**
 * Loads all crew members and their assigned flights from the JSON file.
 */
void Crew::loadCrewFromFile(const std::string& filename)
{
    std::ifstream inFile(filename);
    if (!inFile.is_open())
    {
        std::cerr << "Error: Unable to open crew data file.\n";
        return;
    }

    json crewJson;
    inFile >> crewJson;
    inFile.close();

    for (auto& element : crewJson.items())
    {
        std::string crewName       = element.key();
        auto        flightsData    = element.value();

        Crew crewMember(crewName, "Role Placeholder");

        for (auto& flightJson : flightsData)
        {
            std::shared_ptr<Flight> flightPtr =
                std::make_shared<Flight>(Flight::fromJson(flightJson));

            crewMember.assignFlight(flightPtr);
        }

        allCrewMembers.push_back(crewMember);
    }
}

/**
 * Finds and returns a pointer to a Crew by name, loading from file if needed.
 */
Crew* Crew::getCrewByName(const std::string& crewName)
{
    if (allCrewMembers.empty())
    {
        loadCrewFromFile(PATH_OF_CREW_DATA_BASE);
    }

    for (auto& crew : allCrewMembers)
    {
        if (crew.getName() == crewName)
            return &crew;
    }

    std::cerr << "Crew member " << crewName << " not found.\n";
    return nullptr;
}

/**
 * Removes a crew member entry from the file.
 */
bool Crew::removeCrewFromFile(const std::string& crewName,
                              const std::string& filename)
{
    std::ifstream inFile(filename);
    if (!inFile.is_open())
    {
        std::cerr << "Error: Unable to open crew data file.\n";
        return false;
    }

    json crewJson;
    inFile >> crewJson;
    inFile.close();

    if (!crewJson.contains(crewName))
    {
        std::cout << "Crew member " << crewName << " not found.\n";
        return false;
    }

    crewJson.erase(crewName);

    std::ofstream outFile(filename);
    if (outFile.is_open())
    {
        outFile << std::setw(4) << crewJson << std::endl;
        return true;
    }
    else
    {
        std::cerr << "Error: Unable to save updated crew data.\n";
        return false;
    }
}

/**
 * Updates the assigned flights array for a crew member in the file.
 */
void Crew::updateCrewInFile(const std::string& crewName,
                            const std::shared_ptr<Flight>& flight,
                            const std::string& filename)
{
    std::ifstream inFile(filename);
    if (!inFile.is_open())
    {
        std::cerr << "Error: Unable to open crew data file.\n";
        return;
    }

    json crewJson;
    inFile >> crewJson;
    inFile.close();

    if (crewJson.contains(crewName))
    {
        crewJson[crewName] = json::array();
        crewJson[crewName].push_back(flight->toJson());
    }

    std::ofstream outFile(filename);
    if (outFile.is_open())
    {
        outFile << std::setw(4) << crewJson << std::endl;
    }
    else
    {
        std::cerr << "Error: Unable to save updated crew data.\n";
    }
}

/******************************************************************************************
 * END OF FILE
 ******************************************************************************************/
