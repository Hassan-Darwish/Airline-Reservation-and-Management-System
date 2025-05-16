/******************************************************************************************
* MODULE NAME    : Maintenance Management Module
* FILE           : Maintenance.hpp
* DESCRIPTION    : Defines the Maintenance class to handle maintenance details associated 
*                  with aircraft and manage related JSON operations.
* AUTHOR         : Hassan Darwish
* DATE CREATED   : May 2025
******************************************************************************************/

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "json.hpp"

#define PATH_OF_MAINTENANCE_DATA_BASE "data_base/maintenanceData.json"

using std::string;
using std::vector;
using std::map;
using json = nlohmann::json;

/******************************************************************************************
* CLASS NAME     : Maintenance
* DESCRIPTION    : Represents a maintenance record for a specific aircraft.
******************************************************************************************/
class Maintenance 
{
private:
    string aircraftID;
    string maintenanceDetails;

public:
    /*
    * Constructor: Initializes a maintenance record with aircraft ID and details.
    */
    Maintenance(const string& id, const string& details);

    // Getters
    string getAircraftID(void) const;
    string getMaintenanceDetails(void) const;

    // Setters
    void setAircraftID(const string& id);
    void setMaintenanceDetails(const string& details);

    /*
    * Description: Schedules maintenance activity for the aircraft.
    */
    void scheduleMaintenance(void) const;

    /*
    * Description: Logs the maintenance activity to the system.
    */
    void logMaintenance(void) const;

    /*
    * Description: Converts this Maintenance object to JSON format.
    */
    json toJson(void) const;

    /*
    * Description: Loads all maintenance records from the JSON file into a map.
    */
    static void loadMaintenanceData(const string& filename, map<string, vector<Maintenance>>& maintenanceRecords);

    /*
    * Description: Saves all maintenance records from the map to the JSON file.
    */
    static void saveMaintenanceData(const string& filename, const map<string, vector<Maintenance>>& maintenanceRecords);

    /*
    * Description: Edits the maintenance details for a specific aircraft in the JSON file.
    */
    static bool editMaintenance(const string& filename, const string& aircraftID, const string& newDetails);

    /*
    * Description: Removes a specific maintenance record from the JSON file.
    */
    static bool removeMaintenance(const string& filename, const string& aircraftID);
};

/******************************************************************************************
* END OF FILE
******************************************************************************************/
