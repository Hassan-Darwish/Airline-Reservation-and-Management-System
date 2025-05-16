/******************************************************************************************
* MODULE NAME    : Flight Management Module
* FILE           : Flight.hpp
* DESCRIPTION    : This file defines the Flight class, responsible for storing flight 
*                  information and handling JSON file operations related to flight data.
* AUTHOR         : Hassan Darwish
* DATE CREATED   : May 2025
******************************************************************************************/

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "json.hpp"

#define PATH_OF_FLIGHTS_DATA_BASE "data_base/flights.json"

using json = nlohmann::json;
using namespace std;

/******************************************************************************************
* CLASS NAME     : Flight
* DESCRIPTION    : Represents a flight with relevant attributes such as origin, 
*                  destination, times, seat capacity, aircraft type, and price.
******************************************************************************************/
class Flight {
private:
    string flightNumber;
    string origin;
    string destination;
    string departureTime;
    string arrivalTime;
    string aircraftType;
    int totalSeats;
    string status;
    string flightPrice;

    // Private Setters
    void setFlightNumber(const string& flightNumber);
    void setOrigin(const string& origin);
    void setDestination(const string& destination);
    void setDepartureTime(const string& departureTime);
    void setArrivalTime(const string& arrivalTime);
    void setAircraftType(const string& aircraftType);
    void setTotalSeats(int totalSeats);
    void setStatus(const string& status);
    void setflightPrice(const string& price);

public:
    /*
    * Default Constructor: Initializes all attributes to default values.
    */
    Flight();

    /*
    * Parameterized Constructor: Initializes a flight object with full data.
    */
    Flight(const string& flightNumber, const string& origin, const string& destination,
           const string& departureTime, const string& arrivalTime,
           const string& aircraftType, int totalSeats, const string& status, const string& price);

    /*
    * Description: Displays all information related to this flight.
    */
    void displayFlightInfo(void) const;

    /*
    * Description: Converts this flight object into a JSON object.
    */
    json toJson(void) const;

    /*
    * Description: Creates and returns a Flight object from a JSON object.
    */
    static Flight fromJson(const json& j);

    /*
    * Description: Saves a vector of flights to a specified JSON file.
    */
    static void saveFlights(const vector<Flight>& flights, const string& filename);

    /*
    * Description: Loads a vector of flights from a specified JSON file.
    */
    static vector<Flight> loadFlights(const string& filename);

    /*
    * Description: Adds a single flight to the JSON file.
    */
    static void addFlightToFile(const Flight& newFlight, const string& filename);

    /*
    * Description: Removes a flight (by flight number) from the JSON file.
    */
    static void removeFlightFromFile(const string& flightNumber, const string& filename);

    /*
    * Description: Updates a field of a flight entry in the JSON file.
    */
    static void updateFlightInFile(const string& flightNumber, const string& fieldToUpdate, const string& newValue, const string& filename);

    // Getters
    string getFlightNumber(void) const;
    string getOrigin(void) const;
    string getDestination(void) const;
    string getDepartureTime(void) const;
    string getArrivalTime(void) const;
    string getAircraftType(void) const;
    string getflightPrice(void) const;
    int getTotalSeats(void) const;
    string getStatus(void) const;
};

/******************************************************************************************
* END OF FILE
******************************************************************************************/
