#include "System.hpp"
#include <FL/Fl.H>

void exitFunction(System& thisSystem)
{
    for (User* user : thisSystem.getUserlist())
    {
        std::cout << "deleting: " << user->getUsername();
        delete user;

    }


    thisSystem.getUserlist().clear();
}


int main()
{
	System POE;
	exitFunction(POE);
	return 0;
}