#include "Item.hpp"


bool Item::setName(const std::string& name)
{
	if (name.length() > 0)
		this->name = name;
	else
		return false;

	return true;
}

std::string& Item::getName()
{
	std::string error = "error";

	if (name.length() > 0)
		return name;
	else
		return error;
	
}


bool Item::setPrice(double price)
{
	if (price >= 0.00)
		this->price = price;
	else
		return false;

	return true;

}

double Item::getPrice()
{

	return this->price;

}
					  
bool Item::setStock(int stock)
{
	if (stock >= 0)
		this->stock = stock;
	else
		return false;

	return true;


}
int  Item::getStock()
{

	return this->stock;

}

bool Food::setFoodType(int type)
{
	if (type < 5 && type >= 0)
		this->foodType = type;
	else
		return false;

	return true;

}

int Food::getFoodType()
{
	return this->foodType;
}

bool Beverage::setAlcohol(bool state)
{
	this->alcoholic = state;
	return true;

}
bool Beverage::getAlcohol()
{
	return this->alcoholic;

}