#pragma once
#include <iostream>
#include <vector>
#include "Till.hpp"
#include "User.hpp"
#include "Item.hpp"


#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <Fl/Fl_Box.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Clock.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Choice.H>
#include <fstream>
#include <sstream>
#include <string>


class System
{
public:
	System();
	void posMain(User* currentUser);

	void resetSettings();
	bool retrieveSettings();
	User* login();

	void setNewUser(User* newUser);
	std::vector<User*>& getUserlist();
	std::vector<Food>& getFoodList();
	std::vector<char*>& getOrder2Free();

private:
		std::vector<User*> userList;
	 	std::vector<Food> foodList;
		std::vector<Beverage> beverageList;
		std::vector<char*> order2free;
};

struct UIComp;
struct mainData;
void createFoodItems(std::vector<Food>& foodItems);

std::shared_ptr<std::string> dailyMesssageDemo();