/******************************************************************************************
 * MODULE NAME    : Aircraft Module
 * FILE           : Aircraft.cpp
 * DESCRIPTION    : Implements the Aircraft class methods, including JSON persistence,
 *                  data loading, editing, and deletion.
 * AUTHOR         : Hassan Darwish
 * DATE CREATED   : May 2025
 ******************************************************************************************/

#include "../header/Aircraft.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <memory>

using namespace std;
using json = nlohmann::json;

/**
 * Constructor: Initializes an Aircraft with ID, type, capacity, and maintenance info.
 */
Aircraft::Aircraft(const string& id,
                   const string& type,
                   int cap,
                   shared_ptr<Maintenance> maint)
    : aircraftID(id),
      aircraftType(type),
      capacity(cap),
      maintenance(maint)
{}

/**
 * Getter: Returns the aircraft’s unique ID.
 */
string Aircraft::getAircraftID(void) const
{
    return aircraftID;
}

/**
 * Getter: Returns the aircraft’s type/model.
 */
string Aircraft::getAircraftType(void) const
{
    return aircraftType;
}

/**
 * Getter: Returns the aircraft’s seating capacity.
 */
int Aircraft::getCapacity(void) const
{
    return capacity;
}

/**
 * Getter: Returns the associated Maintenance object (if any).
 */
shared_ptr<Maintenance> Aircraft::getMaintenance(void) const
{
    return maintenance;
}

/**
 * Setter: Updates the aircraft’s ID.
 */
void Aircraft::setAircraftID(const string& id)
{
    aircraftID = id;
}

/**
 * Setter: Updates the aircraft’s type/model.
 */
void Aircraft::setAircraftType(const string& type)
{
    aircraftType = type;
}

/**
 * Setter: Updates the aircraft’s seating capacity.
 */
void Aircraft::setCapacity(int cap)
{
    capacity = cap;
}

/**
 * Setter: Assigns a new Maintenance object to this aircraft.
 */
void Aircraft::setMaintenance(shared_ptr<Maintenance> maint)
{
    maintenance = maint;
}

/**
 * Displays the aircraft’s details and its maintenance log if available.
 */
void Aircraft::displayAircraftInfo(void) const
{
    cout << "Aircraft ID:    " << aircraftID   << endl
         << "Aircraft Type:  " << aircraftType << endl
         << "Capacity:       " << capacity     << endl;

    if (maintenance) {
        maintenance->logMaintenance();
    }
    else {
        cout << "No maintenance data available.\n";
    }
}

/**
 * Converts this Aircraft object into a JSON representation.
 */
json Aircraft::toJson(void) const
{
    json maintJson = nullptr;
    if (maintenance) {
        maintJson = maintenance->toJson();
    }

    return json{
        {"aircraftID",   aircraftID},
        {"aircraftType", aircraftType},
        {"capacity",     capacity},
        {"maintenance",  maintJson}
    };
}

/**
 * Loads all aircraft records from a JSON file into the provided map.
 */
void Aircraft::loadAircraftData(const string& filename,
                                map<string, Aircraft>& aircraftRecords)
{
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open aircraft data file.\n";
        return;
    }

    json aircraftJson;
    try {
        file >> aircraftJson;
    }
    catch (const json::parse_error& e) {
        cerr << "Error parsing JSON: " << e.what() << endl;
        return;
    }
    file.close();

    for (auto& item : aircraftJson.items()) {
        string id           = item.key();
        auto   data         = item.value();
        shared_ptr<Maintenance> maintPtr;

        if (data.contains("maintenance") && !data["maintenance"].is_null()) {
            auto m = data["maintenance"];
            if (m.contains("maintenanceDetails") && !m["maintenanceDetails"].is_null()) {
                maintPtr = make_shared<Maintenance>(
                    m["aircraftID"].get<string>(),
                    m["maintenanceDetails"].get<string>()
                );
            }
            else {
                cerr << "Warning: Missing maintenanceDetails for aircraft "
                     << id << ".\n";
            }
        }

        Aircraft aircraft(
            id,
            data["aircraftType"].get<string>(),
            data["capacity"].get<int>(),
            maintPtr
        );

        aircraftRecords[id] = aircraft;
    }
}

/**
 * Saves all aircraft records from the map into a JSON file.
 */
void Aircraft::saveAircraftData(const string& filename,
                                const map<string, Aircraft>& aircraftRecords)
{
    json outJson;
    for (auto const& entry : aircraftRecords) {
        outJson[entry.first] = entry.second.toJson();
    }

    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file for saving aircraft data.\n";
        return;
    }

    file << setw(4) << outJson << endl;
}

/**
 * Edits a specific field of one aircraft record and persists the change.
 */
bool Aircraft::editAircraft(const string& filename,
                            const string& aircraftID,
                            const string& fieldToUpdate,
                            const string& newValue)
{
    map<string, Aircraft> records;
    loadAircraftData(filename, records);

    auto it = records.find(aircraftID);
    if (it == records.end()) {
        cout << "Aircraft ID not found.\n";
        return false;
    }

    Aircraft& ac = it->second;
    if (fieldToUpdate == "aircraftType") {
        if (newValue.empty()) {
            cout << "Error: aircraftType cannot be empty.\n";
            return false;
        }
        ac.setAircraftType(newValue);
    }
    else if (fieldToUpdate == "capacity") {
        try {
            ac.setCapacity(stoi(newValue));
        }
        catch (const invalid_argument&) {
            cout << "Error: Invalid number for capacity.\n";
            return false;
        }
    }
    else if (fieldToUpdate == "maintenanceStatus") {
        auto maintPtr = ac.getMaintenance();
        if (maintPtr) {
            maintPtr->setMaintenanceDetails(newValue);
        }
        else {
            cout << "Error: No maintenance data for " << aircraftID << ".\n";
            return false;
        }
    }
    else {
        cout << "Invalid field name: " << fieldToUpdate << endl;
        return false;
    }

    saveAircraftData(filename, records);
    return true;
}

/**
 * Removes an aircraft record by ID and persists the updated map.
 */
bool Aircraft::deleteAircraft(const string& filename,
                              const string& aircraftID)
{
    map<string, Aircraft> records;
    loadAircraftData(filename, records);

    auto it = records.find(aircraftID);
    if (it == records.end()) {
        cout << "Aircraft ID not found.\n";
        return false;
    }

    records.erase(it);
    saveAircraftData(filename, records);
    return true;
}
