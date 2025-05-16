/******************************************************************************************
 * MODULE NAME    : Administrator Module
 * FILE           : Administrator.cpp
 * DESCRIPTION    : Implements the Administrator class, providing menu-driven methods
 *                  for managing flights, aircraft, crew, users, and generating reports.
 * AUTHOR         : Hassan Darwish
 * DATE CREATED   : May 2025
 ******************************************************************************************/

#include "../header/Administrator.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <memory>
#include <iomanip>
#include <sstream>

std::vector<Flight> flights;  // Store flights in memory (can later sync with file)

/**
 * Constructor: Initializes Administrator and loads existing flights.
 */
Administrator::Administrator(std::string username, std::string password)
    : User(username, password, "Administrator")
{
    flights = Flight::loadFlights(PATH_OF_FLIGHTS_DATA_BASE);
}

/**
 * Displays the main Administrator menu and dispatches user choices.
 */
void Administrator::displayMenu(void)
{
    int choice;

    do
    {
        std::cout << "\n\t\t\t-------- Administrator Menu --------\n"
                  << "1. Manage Flights\n"
                  << "2. Manage Aircraft\n"
                  << "3. Manage Users\n"
                  << "4. Generate Reports\n"
                  << "5. Logout\n"
                  << "Enter choice: ";
        std::cin >> choice;

        switch (choice)
        {
            case 1: manageFlights();               break;
            case 2: manageAircraft();              break;
            case 4: generateOperationalReport();   break;
            case 5: std::cout << "Logging out...\n"; break;
            default: std::cout << "Invalid choice! Please try again.\n"; break;
        }
    }
    while (choice != 5);
}

/**
 * Displays the Manage Flights submenu and handles flight operations.
 */
void Administrator::manageFlights(void)
{
    int choice;

    do
    {
        std::cout << "\n\t\t\t-------- Manage Flights --------\n"
                  << "1. Add New Flight\n"
                  << "2. Update Existing Flight\n"
                  << "3. Remove Flight\n"
                  << "4. View All Flights\n"
                  << "5. Assign Crew Team\n"
                  << "6. Back to Main Menu\n"
                  << "Enter choice: ";
        std::cin >> choice;

        switch (choice)
        {
            case 1: addNewFlight();      break;
            case 2: updateFlight();      break;
            case 3: removeFlight();      break;
            case 4: viewAllFlights();    break;
            case 5: assignCrew();        break;
            case 6: return;
            default: std::cout << "Invalid choice! Please try again.\n"; break;
        }
    }
    while (choice != 6);
}

/**
 * Prompts for flight details, then adds and persists a new flight.
 */
void Administrator::addNewFlight(void)
{
    std::string flightNumber, origin, destination;
    std::string departureTime, arrivalTime, aircraftType, status, flightPrice;
    int totalSeats;

    std::cout << "\n\t\t\t-------- Add New Flight --------\n"
              << "Enter Flight Number: ";
    std::cin >> flightNumber;

    std::cout << "Enter Origin: ";
    std::cin.ignore();
    std::getline(std::cin, origin);

    std::cout << "Enter Destination: ";
    std::getline(std::cin, destination);

    std::cout << "Enter Departure Date and Time (YYYY-MM-DD HH:MM): ";
    std::getline(std::cin, departureTime);

    std::cout << "Enter Arrival Date and Time (YYYY-MM-DD HH:MM): ";
    std::getline(std::cin, arrivalTime);

    // Load and display available aircraft
    std::map<std::string, Aircraft> aircraftRecords;
    Aircraft::loadAircraftData(PATH_OF_AIR_CRAFT_DATA_BASE, aircraftRecords);

    if (aircraftRecords.empty())
    {
        std::cout << "No available aircrafts in the database.\n";
        return;
    }

    std::cout << "\n\t\t\t-------- Available Aircrafts --------\n";
    int counter = 1;
    for (auto const& pair : aircraftRecords)
    {
        std::cout << counter++ << ". ";
        pair.second.displayAircraftInfo();
        std::cout << "------------------------------------\n";
    }

    std::cout << "Enter the number of the aircraft you want to use: ";
    size_t choice;
    std::cin >> choice;

    if (choice < 1 || choice > aircraftRecords.size())
    {
        std::cout << "Invalid choice. Please select a valid aircraft number.\n";
        return;
    }

    auto it = std::next(aircraftRecords.begin(), choice - 1);
    aircraftType = it->second.getAircraftType();

    std::cout << "Enter Total Seats: ";
    std::cin >> totalSeats;

    std::cout << "Enter Flight Price in $ (e.g. 20$): ";
    std::cin.ignore();
    std::getline(std::cin, flightPrice);

    std::cout << "Enter Status (1-Scheduled / 2-Delayed / 3-Canceled): ";
    int statusInput;
    std::cin >> statusInput;

    if      (statusInput == 2) status = "Delayed";
    else if (statusInput == 3) status = "Canceled";
    else                       status = "Scheduled";

    Flight newFlight(
        flightNumber, origin, destination,
        departureTime, arrivalTime,
        aircraftType, totalSeats, status, flightPrice
    );

    Flight::addFlightToFile(newFlight, PATH_OF_FLIGHTS_DATA_BASE);
    std::cout << "\nFlight " << flightNumber
              << " [" << origin << " → " << destination
              << " at " << departureTime << "] added successfully.\n";
}

/**
 * Loads and displays all existing flights.
 */
void Administrator::viewAllFlights(void)
{
    flights = Flight::loadFlights(PATH_OF_FLIGHTS_DATA_BASE);

    std::cout << "\n\t\t\t-------- All Flights --------\n";

    if (flights.empty())
    {
        std::cout << "No flights available.\n";
        return;
    }

    for (auto const& flight : flights)
    {
        flight.displayFlightInfo();
    }
}

/**
 * Prompts for a flight number and field, then updates the flight record.
 */
void Administrator::updateFlight(void)
{
    flights = Flight::loadFlights(PATH_OF_FLIGHTS_DATA_BASE);

    if (flights.empty())
    {
        std::cout << "\nNo flights available to update.\n";
        return;
    }

    std::cout << "\n\t\t\t-------- Available Flights --------\n";
    for (auto const& fl : flights)
    {
        fl.displayFlightInfo();
    }

    std::string flightNumberToUpdate;
    std::cout << "\nEnter the flight number to update: ";
    std::cin >> flightNumberToUpdate;

    std::cout << "\nWhich field to update? (1-8)\n"
              << "1.Departure Time 2.Arrival Time 3.Status 4.Origin\n"
              << "5.Destination    6.Aircraft Type   7.Total Seats 8.Flight Price\n"
              << "Enter choice: ";
    int fieldChoice;
    std::cin >> fieldChoice;

    std::string fieldName;
    switch (fieldChoice)
    {
        case 1: fieldName = "departureTime"; break;
        case 2: fieldName = "arrivalTime";   break;
        case 3: fieldName = "status";        break;
        case 4: fieldName = "origin";        break;
        case 5: fieldName = "destination";   break;
        case 6: fieldName = "aircraftType";  break;
        case 7: fieldName = "totalSeats";    break;
        case 8: fieldName = "flightPrice";   break;
        default:
            std::cout << "Invalid selection!\n";
            return;
    }

    std::string newValue;
    if (fieldName == "totalSeats")
    {
        int seats;
        std::cout << "Enter new Total Seats: ";
        std::cin >> seats;
        newValue = std::to_string(seats);
    }
    else
    {
        std::cout << "Enter new value for " << fieldName << ": ";
        std::cin.ignore();
        std::getline(std::cin, newValue);
    }

    Flight::updateFlightInFile(
        flightNumberToUpdate,
        fieldName,
        newValue,
        PATH_OF_FLIGHTS_DATA_BASE
    );

    std::cout << "\nFlight " << flightNumberToUpdate << " updated successfully!\n";
}

/**
 * Prompts for a flight number then removes that flight record.
 */
void Administrator::removeFlight(void)
{
    flights = Flight::loadFlights(PATH_OF_FLIGHTS_DATA_BASE);

    if (flights.empty())
    {
        std::cout << "\nNo flights available to remove.\n";
        return;
    }

    std::cout << "\n\t\t\t-------- Available Flights --------\n";
    for (auto const& fl : flights)
    {
        fl.displayFlightInfo();
    }

    std::string flightNumberToRemove;
    std::cout << "\nEnter the flight number to remove: ";
    std::cin >> flightNumberToRemove;

    Flight::removeFlightFromFile(
        flightNumberToRemove,
        PATH_OF_FLIGHTS_DATA_BASE
    );
}

/**
 * Generates an operational report for a given month-year.
 */
void Administrator::generateOperationalReport(void)
{
    std::cout << "\nEnter Month and Year for Report (MM-YYYY): ";
    std::string monthYear;
    std::cin >> monthYear;

    std::stringstream ss(monthYear);
    std::string month, year;
    std::getline(ss, month, '-');
    std::getline(ss, year);

    int totalFlightsScheduled = 0,
        flightsCompleted      = 0,
        flightsDelayed        = 0,
        flightsCanceled       = 0,
        totalReservationsMade = 0;
    double totalRevenue = 0.0;

    std::map<std::string, double> flightRevenue;
    std::map<std::string, int>    flightReservations;

    for (auto const& fl : flights)
    {
        ++totalFlightsScheduled;

        auto status = fl.getStatus();
        if      (status == "Scheduled") ++flightsCompleted;
        else if (status == "Delayed")   ++flightsDelayed;
        else if (status == "Canceled")  ++flightsCanceled;

        auto reservations = Reservation::loadReservations();
        int countForFlight = 0;

        for (auto const& resptr : reservations)
        {
            if (resptr->getFlight()->getFlightNumber() == fl.getFlightNumber())
            {
                ++countForFlight;
            }
        }

        double price   = std::stod(fl.getflightPrice());
        double revenue = countForFlight * price;

        totalRevenue += revenue;
        flightRevenue[fl.getFlightNumber()]   = revenue;
        flightReservations[fl.getFlightNumber()] = countForFlight;
        totalReservationsMade += countForFlight;
    }

    std::cout << "\nGenerating Operational Report for " << monthYear << "...\n\n"
              << "Report Summary:\n"
              << "- Total Flights Scheduled: " << totalFlightsScheduled << '\n'
              << "- Flights Completed:        " << flightsCompleted      << '\n'
              << "- Flights Delayed:          " << flightsDelayed        << '\n'
              << "- Flights Canceled:         " << flightsCanceled       << '\n'
              << "- Total Reservations Made:  " << totalReservationsMade << '\n'
              << "- Total Revenue: $"
              << std::fixed << std::setprecision(2) << totalRevenue << "\n\n"
              << "Detailed Flight Performance:\n";

    int idx = 1;
    for (auto const& entry : flightRevenue)
    {
        std::cout << idx++ << ". Flight " << entry.first
                  << ": Reservations: " << flightReservations[entry.first]
                  << ", Revenue: $"    << entry.second << '\n';
    }

    std::cout << "\nReport generated successfully!\n";
}

/**
 * Displays the Crew Management submenu.
 */
void Administrator::assignCrew(void)
{
    int choice;

    do
    {
        std::cout << "\n\t\t\t-------- Crew Management --------\n"
                  << "1. Assign new crew member to a flight\n"
                  << "2. Change crew assignment\n"
                  << "3. Delete crew member\n"
                  << "4. Back to Manage Flights Menu\n"
                  << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
            case 1: assignNewCrew();      break;
            case 2: changeCrewAssignment(); break;
            case 3: deleteCrew();         break;
            case 4: return;
            default: std::cout << "Invalid choice! Please try again.\n"; break;
        }
    }
    while (choice != 4);
}

/**
 * Assigns a newly created Crew to a specified flight and saves to file.
 */
void Administrator::assignNewCrew(void)
{
    auto allFlights = Flight::loadFlights(PATH_OF_FLIGHTS_DATA_BASE);

    std::cout << "Enter the Flight Number to assign crew: ";
    std::string flightNumber;
    std::cin >> flightNumber;

    std::shared_ptr<Flight> selectedFlight;
    for (auto const& fl : allFlights)
    {
        if (fl.getFlightNumber() == flightNumber)
        {
            selectedFlight = std::make_shared<Flight>(fl);
            break;
        }
    }

    if (!selectedFlight)
    {
        std::cout << "Flight not found.\n";
        return;
    }

    std::cin.ignore();
    std::cout << "Enter Crew Member Name: ";
    std::string crewName;
    std::getline(std::cin, crewName);

    std::cout << "Enter Crew Member Role: ";
    std::string crewRole;
    std::getline(std::cin, crewRole);

    Crew newCrew(crewName, crewRole);
    newCrew.assignFlight(selectedFlight);
    newCrew.saveCrewToFile(PATH_OF_CREW_DATA_BASE);

    std::cout << "Crew member (" << crewName
              << ") assigned to flight (" << flightNumber << ") successfully!\n";
}

/**
 * Changes an existing crew assignment and updates the file.
 */
void Administrator::changeCrewAssignment(void)
{
    std::cout << "Enter the crew member's name to change assignment: ";
    std::cin.ignore();
    std::string crewName;
    std::getline(std::cin, crewName);

    Crew* crewMember = Crew::getCrewByName(crewName);
    if (!crewMember)
    {
        std::cout << "Crew member not found.\n";
        return;
    }

    auto allFlights = Flight::loadFlights(PATH_OF_FLIGHTS_DATA_BASE);

    std::cout << "Enter the Flight Number to assign crew: ";
    std::string newFlightNumber;
    std::cin >> newFlightNumber;

    std::shared_ptr<Flight> selectedFlight;
    for (auto const& fl : allFlights)
    {
        if (fl.getFlightNumber() == newFlightNumber)
        {
            selectedFlight = std::make_shared<Flight>(fl);
            break;
        }
    }

    if (!selectedFlight)
    {
        std::cout << "Flight not found.\n";
        return;
    }

    crewMember->assignFlight(selectedFlight);
    crewMember->saveCrewToFile(PATH_OF_CREW_DATA_BASE);

    std::cout << "Crew member (" << crewName
              << ") reassigned to flight " << newFlightNumber << " successfully!\n";
}

/**
 * Deletes a crew member record from the file.
 */
void Administrator::deleteCrew(void)
{
    std::cout << "Enter the name of the crew member to delete: ";
    std::cin.ignore();
    std::string crewName;
    std::getline(std::cin, crewName);

    bool removed = Crew::removeCrewFromFile(crewName, PATH_OF_CREW_DATA_BASE);
    if (removed)
    {
        std::cout << "Crew member (" << crewName << ") removed successfully.\n";
    }
    else
    {
        std::cout << "Crew member " << crewName << " not found or could not be removed.\n";
    }
}

/**
 * Displays the Manage Aircraft submenu.
 */
void Administrator::manageAircraft(void)
{
    int choice;

    do
    {
        std::cout << "\n--- Aircraft Management ---\n"
                  << "1. Add Aircraft\n"
                  << "2. Edit Aircraft\n"
                  << "3. Delete Aircraft\n"
                  << "4. Back to Main Menu\n"
                  << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
            case 1: addAircraft();  break;
            case 2: editAircraft(); break;
            case 3: deleteAircraft(); break;
            case 4: return;
            default: std::cout << "Invalid choice! Please try again.\n"; break;
        }
    }
    while (choice != 4);
}

/**
 * Prompts for aircraft details, then adds and persists a new Aircraft.
 */
void Administrator::addAircraft(void)
{
    std::cout << "Enter Aircraft ID: ";
    std::string aircraftID;
    std::cin >> aircraftID;

    std::cout << "Enter Aircraft Type: ";
    std::string aircraftType;
    std::cin >> aircraftType;

    std::cout << "Enter Aircraft Capacity: ";
    int capacity;
    std::cin >> capacity;

    std::cin.ignore();
    std::cout << "Enter Maintenance Details: ";
    std::string maintenanceDetails;
    std::getline(std::cin, maintenanceDetails);

    auto maint = std::make_shared<Maintenance>(aircraftID, maintenanceDetails);
    Aircraft newAircraft(aircraftID, aircraftType, capacity, maint);

    std::map<std::string, Aircraft> records;
    Aircraft::loadAircraftData(PATH_OF_AIR_CRAFT_DATA_BASE, records);

    records[aircraftID] = newAircraft;
    Aircraft::saveAircraftData(PATH_OF_AIR_CRAFT_DATA_BASE, records);

    std::cout << "Aircraft (" << aircraftID << ") added successfully.\n";
}

/**
 * Prompts for an aircraft ID and field, then updates that record.
 */
void Administrator::editAircraft(void)
{
    std::cout << "Enter Aircraft ID to edit: ";
    std::string aircraftID;
    std::cin >> aircraftID;

    std::cout << "Enter the field to update (aircraftType, capacity, maintenanceDetails): ";
    std::string field, newValue;
    std::cin >> field;

    std::cout << "Enter the new value for " << field << ": ";
    std::cin.ignore();
    std::getline(std::cin, newValue);

    bool ok = Aircraft::editAircraft(
        PATH_OF_AIR_CRAFT_DATA_BASE,
        aircraftID,
        field,
        newValue
    );

    if (ok)
        std::cout << "Aircraft (" << aircraftID << ") updated successfully.\n";
    else
        std::cout << "Failed to update aircraft " << aircraftID << ".\n";
}

/**
 * Prompts for an aircraft ID, then removes that record.
 */
void Administrator::deleteAircraft(void)
{
    std::cout << "Enter Aircraft ID to delete: ";
    std::string aircraftID;
    std::cin >> aircraftID;

    if (Aircraft::deleteAircraft(PATH_OF_AIR_CRAFT_DATA_BASE, aircraftID))
        std::cout << "Aircraft (" << aircraftID << ") removed successfully.\n";
    else
        std::cout << "Failed to remove aircraft " << aircraftID << ".\n";
}

/******************************************************************************************
 * END OF FILE
 ******************************************************************************************/
