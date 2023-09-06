#pragma once
#include <iostream>

enum userType
{
    Admin,
    Employee,

};

class User
{
public:
    void setUsername(const std::string& uname);

    void setPassword(const std::string& pword);

    const std::string getUsername();

    const std::string getPassword();

    void setUserPrivilege(int up);
    bool getUserPrivilege();



private:
    std::string username;
    std::string password;
    bool userPrivilege = false;
};

class Admin : public User 
{
public:
    
};

class Employee : public User 
{
public:
    
};
