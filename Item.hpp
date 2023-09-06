#pragma once
#include <iostream>

enum foodtType
{
	breakfast, lunch, dinner, desert

};

class Item 
{
	public:
	
		bool        setName(const std::string& name);
		std::string& getName();

		bool        setPrice(double price);
		double      getPrice();

		bool        setStock(int stock);
		int         getStock();


	private:
		std::string name;
		double price = 0.0;
		int stock    = 0;

};

class Food : public Item
{
	public:
		bool	    setFoodType(int type);
		int		    getFoodType();
			

	private:	
		int foodType = 0;

};


class Beverage : public Item
{
	public:
		bool setAlcohol(bool state);
		bool getAlcohol();


	private:
		bool alcoholic = false;

};

