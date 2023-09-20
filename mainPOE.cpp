#include "System.hpp"
#include <FL/Fl.H>

void exitFunction(System& thisSystem)
{
    for (User* user : thisSystem.getUserlist())
    {
        std::cout << "deleting: " << user->getUsername() << std::endl;
        delete user;

    }
    for (char* charD : thisSystem.getOrder2Free())
    {
        std::cout << "charD: " << charD << std::endl;
        free (charD);

    }


    thisSystem.getUserlist().clear();
}


int main()
{
	System POE;
	exitFunction(POE);
	return 0;
}