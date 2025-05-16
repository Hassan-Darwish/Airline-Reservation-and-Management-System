/******************************************************************************************
* MODULE NAME    : Payment Module
* FILE           : Payment.hpp
* DESCRIPTION    : Declares the Payment class responsible for handling all types of payment
*                  processes, including card and cash payments, and saving card information.
* AUTHOR         : Hassan Darwish
* DATE CREATED   : May 2025
******************************************************************************************/

#pragma once

#include <string>
#include <memory>
#include "Reservation.hpp" 

#define PATH_OF_USER_CARD_DATA_BASE "data_base/user_cards.json"

/******************************************************************************************
* CLASS NAME     : Payment
* DESCRIPTION    : Handles payment operations including card and cash payments for
*                  reservations and saving user card information securely.
******************************************************************************************/
class Payment {
public:
    /*
    * Processes payment for a reservation.
    */
    static bool processPayment(std::shared_ptr<Reservation>& reservation);

    /*
    * Handles card-based payment method.
    */
    static bool processCardPayment(std::shared_ptr<Reservation>& reservation);

    /*
    * Saves card information securely for a user.
    */
    static void saveCardInfo(const std::string& cardNumber, const std::string& cvv,
                             const std::string& expDate, const std::string& cardHolder,
                             const std::string& username);
};

/******************************************************************************************
* END OF FILE
******************************************************************************************/
