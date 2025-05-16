# Airline Reservation & Management System  
**Author**: Hassan Darwish  
**Language**: C++ (C++17)  
**Build System**: Makefile (GNU Make)  
**Data Files**: JSON under `data_base/`

## Table of Contents  
1. [Project Overview](#project-overview)  
2. [Features](#features)  
3. [Architecture & Modules](#architecture--modules)  
4. [Directory Structure](#directory-structure)  
5. [Dependencies](#dependencies)  
6. [Build & Run Instructions](#build--run-instructions)  
7. [Usage Guide](#usage-guide)  
8. [Data Files & Formats](#data-files--formats)  
9. [Code Style & Conventions](#code-style--conventions)

## Project Overview  
This console-based Airline Reservation & Management System supports three user roles—**Administrator**, **Booking Agent**, and **Passenger**—each with its own menu-driven interface. All data (users, flights, aircraft, crew, maintenance, reservations, cards) is stored in JSON files under `data_base/`.

## Features  
- Role-based authentication & registration  
- Administrator: manage flights, aircraft, crew, users, and generate reports  
- Booking Agent: search/book flights for passengers; view/modify/cancel reservations; handle payments  
- Passenger: search/book flights; view/cancel reservations; check in; confirm cash payments  
- JSON persistence for all entities  
- Masked input for passwords and CVV  
- Boarding-pass generation

## Architecture & Modules  
- **User**: base class for credentials & role  
- **Administrator**: flight, aircraft, crew, user management & reports  
- **BookingAgent**: books flights for others; reservation management  
- **Passenger**: self-service booking & check-in  
- **Flight**: flight details; JSON load/save  
- **Aircraft**: aircraft & maintenance data  
- **Crew**: crew assignments  
- **Maintenance**: logs & JSON persistence  
- **Reservation**: booking details; boarding pass; file persistence  
- **Payment**: cash/card processing; card storage  
- **main.cpp**: entry point & role dispatch

## Directory Structure  
├── data_base/  
│   ├── aircraftDataBase.json  
│   ├── crewData.json  
│   ├── flights.json  
│   ├── maintenanceData.json  
│   ├── reservation.json  
│   ├── bookingAgentReservation.json  
│   ├── user_cards.json  
│   └── users.json  
├── header/  
│   ├── Administrator.hpp  
│   ├── BookingAgent.hpp  
│   ├── Crew.hpp  
│   ├── Flight.hpp  
│   ├── Aircraft.hpp  
│   ├── Maintenance.hpp  
│   ├── Passenger.hpp  
│   ├── Payment.hpp  
│   ├── Reservation.hpp  
│   └── User.hpp  
├── src/  
│   ├── main.cpp  
│   ├── Administrator.cpp  
│   ├── BookingAgent.cpp  
│   ├── Crew.cpp  
│   ├── Flight.cpp  
│   ├── Aircraft.cpp  
│   ├── Maintenance.cpp  
│   ├── Passenger.cpp  
│   ├── Payment.cpp  
│   ├── Reservation.cpp  
│   └── User.cpp  
├── Makefile  
└── README.md

## Dependencies  
- C++17 or later  
- nlohmann/json (single-header JSON library)  
- GNU Make  
- POSIX termios (for masked input on Linux/macOS)

## Build & Run Instructions  
Clone or download the repository, then run:  
```  
make  
./airline_reservation.exe  
```
## Usage Guide  
1. Select Role: Administrator, Booking Agent, or Passenger  
2. Login/Register: use existing credentials or create a new account  
3. Navigate Menus: use numeric choices; all input is validated  
4. Logout: select "Logout" from the menu or close the console

## Data Files & Formats  
- `users.json`: contains user credentials and roles  
  Example:  
  { "alice": { "password": "pass123", "role": "Passenger" } }  
- `flights.json`: array of flight objects  
- `aircraftDataBase.json`, `crewData.json`, `maintenanceData.json`, `reservation.json`, `bookingAgentReservation.json`, `user_cards.json`: JSON objects keyed by ID or username

## Code Style & Conventions  
- Module/file headers with description, author, date  
- Doxygen-style comments for public functions  
- 4-space indentation; braces on the same line  
- Error messages printed via `cerr`  
- Platform-agnostic masked input wrapper for password/CVV
