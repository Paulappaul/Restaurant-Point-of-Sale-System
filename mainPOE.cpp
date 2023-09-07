#include "System.hpp"
#include <FL/Fl.H>

void exitFunction(System& thisSystem)
{

	std::cout << "Exit called" << std::endl;

}

int main()
{
	System POE;
	exitFunction(POE);
	return 0;
}