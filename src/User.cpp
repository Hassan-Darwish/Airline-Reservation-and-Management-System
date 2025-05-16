/******************************************************************************************
 * MODULE NAME    : User Module
 * FILE           : User.cpp
 * DESCRIPTION    : Implements the User base class methods for authentication and
 *                  role-based interface.
 * AUTHOR         : Hassan Darwish
 * DATE CREATED   : May 2025
 ******************************************************************************************/

#include "../header/User.hpp"

/**
 * Constructor: Initializes a User with credentials and role.
 * @param username User's login name.
 * @param password User's password.
 * @param role     User's role (e.g., Administrator, Passenger).
 */
User::User(std::string username,
           std::string password,
           std::string role)
    : username(username),
      password(password),
      role(role)
{}

/**
 * Getter: Returns the username.
 * @return Stored username.
 */
std::string User::getUsername(void) const
{
    return username;
}

/**
 * Getter: Returns the user role.
 * @return Stored role.
 */
std::string User::getRole(void) const
{
    return role;
}

/**
 * Validates login credentials against stored username/password.
 * @param inputUsername Entered username.
 * @param inputPassword Entered password.
 * @return True if credentials match; false otherwise.
 */
bool User::login(std::string inputUsername,
                 std::string inputPassword)
{
    return (inputUsername == username && inputPassword == password);
}

/******************************************************************************************
 * END OF FILE
 ******************************************************************************************/
