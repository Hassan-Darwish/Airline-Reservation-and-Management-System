/******************************************************************************************
 * MODULE NAME    : Flight Management Module
 * FILE           : Flight.cpp
 * DESCRIPTION    : Implements the Flight class methods for creating, displaying,
 *                  and persisting flight records in JSON files.
 * AUTHOR         : Hassan Darwish
 * DATE CREATED   : May 2025
 ******************************************************************************************/

#include "../header/Flight.hpp"
#include "../header/json.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;
using json = nlohmann::json;

/**
 * Parameterized Constructor: Initializes a Flight with all details.
 */
Flight::Flight(const string& flightNumber,
               const string& origin,
               const string& destination,
               const string& departureTime,
               const string& arrivalTime,
               const string& aircraftType,
               int totalSeats,
               const string& status,
               const string& flightPrice)
    : flightNumber(flightNumber),
      origin(origin),
      destination(destination),
      departureTime(departureTime),
      arrivalTime(arrivalTime),
      aircraftType(aircraftType),
      totalSeats(totalSeats),
      status(status),
      flightPrice(flightPrice)
{}

/**
 * Default Constructor: Initializes an empty Flight with default values.
 */
Flight::Flight(void)
    : flightNumber(""),
      origin(""),
      destination(""),
      departureTime(""),
      arrivalTime(""),
      aircraftType(""),
      totalSeats(0),
      status(""),
      flightPrice("")
{}

/**
 * Displays all flight details to the console.
 */
void Flight::displayFlightInfo(void) const
{
    cout << "Flight Number : " << flightNumber   << '\n'
         << "Origin        : " << origin         << '\n'
         << "Destination   : " << destination    << '\n'
         << "Departure Time: " << departureTime  << '\n'
         << "Arrival Time  : " << arrivalTime    << '\n'
         << "Aircraft Type : " << aircraftType   << '\n'
         << "Total Seats   : " << totalSeats     << '\n'
         << "Flight Status : " << status         << '\n'
         << "Flight Price  : " << flightPrice    << '\n'
         << "------------------------------------" << endl;
}

/**
 * Converts this Flight object into a JSON representation.
 */
json Flight::toJson(void) const
{
    return json{
        {"flightNumber", flightNumber},
        {"origin",       origin},
        {"destination",  destination},
        {"departureTime", departureTime},
        {"arrivalTime",  arrivalTime},
        {"aircraftType", aircraftType},
        {"totalSeats",   totalSeats},
        {"status",       status},
        {"price",        flightPrice}
    };
}

/**
 * Creates a Flight object from its JSON representation.
 */
Flight Flight::fromJson(const json& j)
{
    return Flight(
        j.at("flightNumber").get<string>(),
        j.at("origin").get<string>(),
        j.at("destination").get<string>(),
        j.at("departureTime").get<string>(),
        j.at("arrivalTime").get<string>(),
        j.at("aircraftType").get<string>(),
        j.at("totalSeats").get<int>(),
        j.at("status").get<string>(),
        j.at("price").get<string>()
    );
}

/**
 * Saves a list of flights to the specified JSON file (overwrites).
 */
void Flight::saveFlights(const vector<Flight>& flights, const string& filename)
{
    json flightsArray = json::array();
    for (auto const& flight : flights)
        flightsArray.push_back(flight.toJson());

    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Cannot open file " << filename << " for writing.\n";
        return;
    }

    file << setw(4) << flightsArray << endl;
}

/**
 * Loads all flights from the specified JSON file into a vector.
 */
vector<Flight> Flight::loadFlights(const string& filename)
{
    vector<Flight> flights;
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "No existing flights data found at " << filename << ".\n";
        return flights;
    }

    json flightsJson;
    file >> flightsJson;

    for (auto const& flightJson : flightsJson)
        flights.push_back(Flight::fromJson(flightJson));

    return flights;
}

/**
 * Appends a new flight to the file and confirms success.
 */
void Flight::addFlightToFile(const Flight& newFlight, const string& filename)
{
    auto flights = loadFlights(filename);
    flights.push_back(newFlight);
    saveFlights(flights, filename);
    cout << "Flight added successfully to " << filename << "!\n";
}

/**
 * Removes a flight by number from the file and confirms success.
 */
void Flight::removeFlightFromFile(const string& flightNumber, const string& filename)
{
    auto flights = loadFlights(filename);
    vector<Flight> updated;
    bool found = false;

    for (auto const& fl : flights)
    {
        if (fl.getFlightNumber() != flightNumber)
            updated.push_back(fl);
        else
            found = true;
    }

    if (!found)
    {
        cout << "Flight " << flightNumber << " not found in " << filename << ".\n";
        return;
    }

    saveFlights(updated, filename);
    cout << "Flight " << flightNumber << " removed successfully from " << filename << "!\n";
}

/**
 * Updates a specific field of a flight in the file and confirms success.
 */
void Flight::updateFlightInFile(const string& flightNumber,
                                const string& fieldToUpdate,
                                const string& newValue,
                                const string& filename)
{
    auto flights = loadFlights(filename);
    bool found = false;

    for (auto& fl : flights)
    {
        if (fl.getFlightNumber() == flightNumber)
        {
            found = true;

            if      (fieldToUpdate == "departureTime") fl.setDepartureTime(newValue);
            else if (fieldToUpdate == "arrivalTime")   fl.setArrivalTime(newValue);
            else if (fieldToUpdate == "status")        fl.setStatus(newValue);
            else if (fieldToUpdate == "origin")        fl.setOrigin(newValue);
            else if (fieldToUpdate == "destination")   fl.setDestination(newValue);
            else if (fieldToUpdate == "aircraftType")  fl.setAircraftType(newValue);
            else if (fieldToUpdate == "flightPrice")   fl.setflightPrice(newValue);
            else
            {
                cout << "Invalid field name: " << fieldToUpdate << "\n";
                return;
            }
            break;
        }
    }

    if (!found)
    {
        cout << "Flight " << flightNumber << " not found in " << filename << ".\n";
        return;
    }

    saveFlights(flights, filename);
    cout << "Flight " << flightNumber << " updated successfully in " << filename << "!\n";
}


void Flight::setFlightNumber(const string& fn)   { flightNumber = fn; }
void Flight::setOrigin(const string& o)          { origin = o; }
void Flight::setDestination(const string& d)     { destination = d; }
void Flight::setDepartureTime(const string& dt)  { departureTime = dt; }
void Flight::setArrivalTime(const string& at)    { arrivalTime = at; }
void Flight::setAircraftType(const string& atype){ aircraftType = atype; }
void Flight::setTotalSeats(int seats)            { totalSeats = seats; }
void Flight::setStatus(const string& st)         { status = st; }
void Flight::setflightPrice(const string& price) { flightPrice = price; }


string Flight::getFlightNumber(void) const { return flightNumber; }
string Flight::getflightPrice(void) const  { return flightPrice; }
string Flight::getOrigin(void) const       { return origin; }
string Flight::getDestination(void) const  { return destination; }
string Flight::getDepartureTime(void) const{ return departureTime; }
string Flight::getArrivalTime(void) const  { return arrivalTime; }
string Flight::getAircraftType(void) const { return aircraftType; }
int    Flight::getTotalSeats(void) const   { return totalSeats; }
string Flight::getStatus(void) const       { return status; }

/******************************************************************************************
 * END OF FILE
 ******************************************************************************************/
