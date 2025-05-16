/******************************************************************************************
* MODULE NAME    : Aircraft Module
* FILE           : Aircraft.hpp
* DESCRIPTION    : This file defines the Aircraft class which manages aircraft-related 
*                  information and operations such as maintenance, data persistence, 
*                  and editing/deletion from database.
* AUTHOR         : Hassan Darwish
* DATE CREATED   : May 2025
******************************************************************************************/

#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <map>
#include "Maintenance.hpp"
#include "json.hpp"

#define PATH_OF_AIR_CRAFT_DATA_BASE "data_base/aircraftDataBase.json"

using namespace std;

/******************************************************************************************
* CLASS NAME     : Aircraft
* DESCRIPTION    : This class encapsulates aircraft details including ID, type, capacity, 
*                  and maintenance, and supports persistent storage and modification.
******************************************************************************************/
class Aircraft {
private:
    string aircraftID;
    string aircraftType;
    int capacity;
    shared_ptr<Maintenance> maintenance;

public:
    /*
    * Constructor: Default constructor initializing with empty or null values.
    */
    Aircraft() : aircraftID(""), aircraftType(""), capacity(0), maintenance(nullptr) {}

    /*
    * Constructor: Initializes an aircraft object with specific values.
    */
    Aircraft(const string& id, const string& type, int cap, shared_ptr<Maintenance> maint);

    /*
    * Description: Gets the aircraft ID.
    */
    string getAircraftID(void) const;

    /*
    * Description: Gets the aircraft type.
    */
    string getAircraftType(void) const;

    /*
    * Description: Gets the capacity of the aircraft.
    */
    int getCapacity(void) const;

    /*
    * Description: Returns the associated Maintenance object.
    */
    shared_ptr<Maintenance> getMaintenance(void) const;

    /*
    * Description: Sets the aircraft ID.
    */
    void setAircraftID(const string& id);

    /*
    * Description: Sets the aircraft type.
    */
    void setAircraftType(const string& type);

    /*
    * Description: Sets the capacity of the aircraft.
    */
    void setCapacity(int cap);

    /*
    * Description: Sets the Maintenance object for the aircraft.
    */
    void setMaintenance(shared_ptr<Maintenance> maint);

    /*
    * Description: Displays the aircraft's information in human-readable format.
    */
    void displayAircraftInfo(void) const;

    /*
    * Description: Converts the aircraft object to a JSON representation.
    */
    json toJson(void) const;

    /*
    * Description: Loads aircraft data from a JSON file into a map of records.
    */
    static void loadAircraftData(const string& filename, map<string, Aircraft>& aircraftRecords);

    /*
    * Description: Saves aircraft data to a JSON file from a map of records.
    */
    static void saveAircraftData(const string& filename, const map<string, Aircraft>& aircraftRecords);

    /*
    * Description: Edits a specific field of an aircraft record.
    * Returns     : True if update was successful, otherwise false.
    */
    static bool editAircraft(const string& filename, const string& aircraftID, const string& fieldToUpdate, const string& newValue);

    /*
    * Description: Deletes an aircraft record from the data file.
    * Returns     : True if deletion was successful, otherwise false.
    */
    static bool deleteAircraft(const string& filename, const string& aircraftID);
};

/******************************************************************************************
* END OF FILE
******************************************************************************************/
