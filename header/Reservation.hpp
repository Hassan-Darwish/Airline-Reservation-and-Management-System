/******************************************************************************************
* MODULE NAME    : Reservation Module
* FILE           : Reservation.hpp
* DESCRIPTION    : Declares the Reservation class, which handles passenger flight bookings,
*                  including details about the flight, seat, and payment information.
* AUTHOR         : Hassan Darwish
* DATE CREATED   : May 2025
******************************************************************************************/

#pragma once

#include "Flight.hpp"
#include "json.hpp"
#include <string>
#include <vector>
#include <memory>  // For shared_ptr

#define PATH_OF_RESERVATION_DATA_BASE "data_base/reservation.json"
#define PATH_OF_BOOKING_AGENT_RESERVATION_DATA_BASE "data_base/bookingAgentReservation.json"

/***********************************************************************************************************************
 * CLASS NAME : Reservation
 * DESCRIPTION : Represents a flight reservation made by a passenger, containing flight details, payment info,
 *               and provides methods to serialize/deserialize and manage reservations.
 ***********************************************************************************************************************/
class Reservation {
public:
    /*
     * Constructor to initialize a reservation with all details.
     */
    Reservation(std::string reservationID, std::string passengerName, std::shared_ptr<Flight> flight, 
                std::string seatNumber, std::string paymentMethod, std::string paymentDetails);

    /*
     * Default constructor.
     */
    Reservation(void);

    /*
     * Gets the unique reservation ID.

     */
    std::string getReservationID(void) const;

    /*
     * Gets the passenger's name.
     */
    std::string getPassengerName(void) const;

    /*
     * Gets the flight associated with this reservation.
     */
    std::shared_ptr<Flight> getFlight(void) const;

    /*
     * Gets the assigned seat number.
     */
    std::string getSeatNumber(void) const;

    /*
     * Gets the payment method used.
     */
    std::string getPaymentMethod(void) const;

    /*
     * Gets additional payment details.

     */
    std::string getPaymentDetails(void) const;

    /*
     * Converts this reservation object to JSON format.
     */
    nlohmann::json toJson(void) const;

    /*
     * Creates a Reservation object from a JSON representation.
     */
    static Reservation fromJson(const nlohmann::json& j);

    /*
     * Displays reservation details on the console.
     */
    void displayReservation(void) const;

    /*
     * Prints the boarding pass for the reservation.
     */
    void printBoardingPass(void) const;

    /*
     * Displays reservation details when payment is on hold.
     */
    void displayHoldingReservation(void) const;

    /*
     * Saves a list of reservations to the JSON file.
     */
    static void saveReservations(const std::vector<std::shared_ptr<Reservation>>& reservations);

    /*
     * Loads all reservations from the JSON file.
     */
    static std::vector<std::shared_ptr<Reservation>> loadReservations(void);

    /*
     * Adds a new reservation to the storage.
     */
    void addReservation(const Reservation& newReservation);

    /*
     * Removes a reservation identified by reservationID from storage.
     */
    static void removeReservation(const std::string& reservationID);

    /*
     * Saves booking agent reservations to the JSON file.
     */
    static void saveBookingAgentReservations(const std::vector<std::shared_ptr<Reservation>>& reservations);

    /*
     * Adds a new booking agent reservation to storage.
     */
    void addBookingAgentReservation(const Reservation& newReservation);

    /*
     * Loads booking agent reservations from the JSON file.
     */
    static std::vector<std::shared_ptr<Reservation>> loadBookingAgentReservations(void);

    /*
     * Sets the payment status of the reservation.
     */
    void setIsPaid(bool status);

    /**
     * Gets the payment status of the reservation.
     */
    bool getIsPaid(void) const;

    /*
     * Sets the payment method.
     */
    void setPaymentMethod(const std::string& method);

    /*
     * Sets additional payment details.
     */
    void setPaymentDetails(const std::string& details);

private:
    std::string reservationID;
    std::string passengerName;
    std::shared_ptr<Flight> flight;
    std::string seatNumber;
    std::string paymentMethod;
    std::string paymentDetails;
    bool isPaid;
};
