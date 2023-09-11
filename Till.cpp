#include "Till.hpp"

bool Till::setBalance(int balancePass)
{
	if (balancePass > 0)
		balance = balancePass;
	else
		return false;
	return true;

}

int Till::getBalance()
{

	return balance;

}