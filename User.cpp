#include "User.hpp"

void User::setUsername(const std::string& uname)
{
    username = uname;
}
void User::setPassword(const std::string& pword)
{
    password = pword;
}
const std::string User::getUsername()
{
    return username;
}
const std::string User::getPassword() 
{
    return password;
}


void User::setUserPrivilege(int up)
{
    if (up == Admin)
        User::userPrivilege = true;
    else
        User::userPrivilege = false;

}
bool User::getUserPrivilege()
{
    return User::userPrivilege;

}