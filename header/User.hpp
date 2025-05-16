/*******************************************************************************************************************************
 * MODULE NAME  : User Module
 * FILE         : User.hpp
 * DESCRIPTION  : Declares the abstract base User class for system users with authentication and role-based menu interface.
 * AUTHOR       : Hassan Darwish
 * DATE CREATED : May 2025
 *******************************************************************************************************************************/

#pragma once

#include <iostream>
#include <string>

/*******************************************************************************************************************************
 * CLASS NAME   : User
 * DESCRIPTION  : Abstract base class representing a system user. Contains user credentials and pure virtual method for menus.
 *******************************************************************************************************************************/
class User {
protected:
    std::string username;  /**< Stores the username */
    std::string password;  /**< Stores the password */
    std::string role;      /**< Stores the role of the user */

public:
    /*
     * Constructor: Initializes User object with username, password, and role.
     */
    User(std::string username, std::string password, std::string role);

    /*
     * Displays the menu specific to the user role.
     */
    virtual void displayMenu(void) = 0;

    /*
     * Gets the username of the user.
     */
    std::string getUsername(void) const;

    /*
     * Gets the role of the user.
     */
    std::string getRole(void) const;

    /*
     * Validates the login credentials.
     */
    bool login(std::string inputUsername, std::string inputPassword);
};



/*******************************************************************************************************************************
 * END OF FILE
 *******************************************************************************************************************************/
