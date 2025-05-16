/******************************************************************************************
 * MODULE NAME    : Payment Module
 * FILE           : Payment.cpp
 * DESCRIPTION    : Implements the Payment class methods, handling cash and card payments,
 *                  and saving user card details to JSON.
 * AUTHOR         : Hassan Darwish
 * DATE CREATED   : May 2025
 ******************************************************************************************/

#include "../header/Payment.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <memory>

using namespace std;
using json = nlohmann::json;

/**
 * Prompts user for payment method (cash or card) and processes accordingly.
 * @param reservation Shared pointer to the Reservation object.
 * @return True if payment or hold is successful; false otherwise.
 */
bool Payment::processPayment(shared_ptr<Reservation>& reservation)
{
    cout << "Choose Payment Method:\n"
         << "\t1 - Cash\n"
         << "\t2 - Card\n"
         << "Enter 1 or 2: ";

    int method;
    cin >> method;

    if (method == 1)
    {
        cout << "Reservation on hold. Please pay at the airport.\n";
        reservation->setPaymentMethod("Cash");
        reservation->setIsPaid(false);
        return true;
    }
    else if (method == 2)
    {
        return processCardPayment(reservation);
    }
    else
    {
        cout << "Invalid payment method!\n";
        return false;
    }
}

/**
 * Handles card payment flow: existing card CVV check or new card entry.
 * @param reservation Shared pointer to the Reservation object.
 * @return True if card payment is successful; false otherwise.
 */
bool Payment::processCardPayment(shared_ptr<Reservation>& reservation)
{
    string username = reservation->getPassengerName();
    ifstream inFile(PATH_OF_USER_CARD_DATA_BASE);
    json allCards;

    if (!inFile.is_open())
    {
        cerr << "Error: Unable to open card database.\n";
        return false;
    }

    inFile >> allCards;
    inFile.close();

    // Existing card path
    if (allCards.contains(username))
    {
        cout << "Enter saved card CVV: ";
        string cvv;
        cin >> cvv;

        if (allCards[username]["cvv"] == cvv)
        {
            cout << "Payment successful with saved card!\n";
            reservation->setIsPaid(true);
            return true;
        }
        else
        {
            cout << "Invalid CVV. Please enter new card details.\n";
        }
    }

    // New card path
    cout << "Enter card number: ";
    string cardNumber;
    cin >> cardNumber;

    cout << "Enter expiration date (MM/YY): ";
    string expDate;
    cin >> expDate;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter cardholder name: ";
    string cardHolder;
    getline(cin, cardHolder);

    cout << "Enter CVV: ";
    string cvv;
    cin >> cvv;

    reservation->setPaymentMethod(cardNumber);
    reservation->setPaymentDetails(cvv);

    saveCardInfo(cardNumber, cvv, expDate, cardHolder, username);

    cout << "Payment successful!\n";
    reservation->setIsPaid(true);
    return true;
}

/**
 * Saves or updates a user's card information in the JSON database.
 * @param cardNumber Card number string.
 * @param cvv        Card CVV string.
 * @param expDate    Card expiration date string.
 * @param cardHolder Card holder name string.
 * @param username   Username key for JSON.
 */
void Payment::saveCardInfo(const string& cardNumber,
                          const string& cvv,
                          const string& expDate,
                          const string& cardHolder,
                          const string& username)
{
    json userCard = {
        {"cardNumber", cardNumber},
        {"cvv",        cvv},
        {"expDate",    expDate},
        {"cardHolder", cardHolder}
    };

    json allCards;
    ifstream inFile(PATH_OF_USER_CARD_DATA_BASE);
    if (inFile.is_open())
    {
        inFile >> allCards;
        inFile.close();
    }

    allCards[username] = userCard;

    ofstream outFile(PATH_OF_USER_CARD_DATA_BASE);
    if (outFile.is_open())
    {
        outFile << setw(4) << allCards << endl;
        outFile.close();
    }
    else
    {
        cerr << "Error: Unable to save card information for user " << username << ".\n";
    }
}
