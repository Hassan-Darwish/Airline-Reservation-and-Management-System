/******************************************************************************************
 * MODULE NAME    : Maintenance Management Module
 * FILE           : Maintenance.cpp
 * DESCRIPTION    : Implements the Maintenance class methods, including logging,
 *                  JSON conversion, and loading/saving maintenance records.
 * AUTHOR         : Hassan Darwish
 * DATE CREATED   : May 2025
 ******************************************************************************************/

#include "../header/Maintenance.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <vector>

using namespace std;
using json = nlohmann::json;

/**
 * Constructor: Initializes a Maintenance record with aircraft ID and details.
 */
Maintenance::Maintenance(const string& id, const string& details)
    : aircraftID(id),
      maintenanceDetails(details)
{}

/**
 * Getter: Returns the associated aircraft ID.
 */
string Maintenance::getAircraftID(void) const
{
    return aircraftID;
}

/**
 * Getter: Returns the maintenance details.
 */
string Maintenance::getMaintenanceDetails(void) const
{
    return maintenanceDetails;
}

/**
 * Setter: Updates the aircraft ID.
 */
void Maintenance::setAircraftID(const string& id)
{
    aircraftID = id;
}

/**
 * Setter: Updates the maintenance details.
 */
void Maintenance::setMaintenanceDetails(const string& details)
{
    maintenanceDetails = details;
}

/**
 * Logs maintenance details to the console.
 */
void Maintenance::logMaintenance(void) const
{
    cout << "Maintenance log for aircraft " << aircraftID
         << " - " << maintenanceDetails << endl;
}

/**
 * Converts this Maintenance object into a JSON representation.
 */
json Maintenance::toJson(void) const
{
    return json{
        {"aircraftID",         aircraftID},
        {"maintenanceDetails", maintenanceDetails}
    };
}

/**
 * Loads maintenance data from a JSON file into the provided map.
 */
void Maintenance::loadMaintenanceData(const string& filename,
                                      map<string, vector<Maintenance>>& maintenanceRecords)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Unable to open maintenance data file.\n";
        return;
    }

    json maintenanceJson;
    file >> maintenanceJson;
    file.close();

    for (auto& item : maintenanceJson.items())
    {
        string id             = item.key();
        auto   dataArray      = item.value();
        vector<Maintenance> list;

        for (auto const& entry : dataArray)
        {
            string details = entry["maintenanceDetails"].get<string>();
            list.emplace_back(id, details);
        }

        maintenanceRecords[id] = move(list);
    }
}

/**
 * Saves all maintenance records from the map into a JSON file.
 */
void Maintenance::saveMaintenanceData(const string& filename,
                                      const map<string, vector<Maintenance>>& maintenanceRecords)
{
    json outJson;

    for (auto const& pair : maintenanceRecords)
    {
        json arr = json::array();
        for (auto const& rec : pair.second)
            arr.push_back(rec.toJson());
        outJson[pair.first] = arr;
    }

    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Unable to open file for saving maintenance data.\n";
        return;
    }

    file << setw(4) << outJson << endl;
}

/******************************************************************************************
 * END OF FILE
 ******************************************************************************************/
