#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "User.hpp"
#include "Item.hpp"


#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <Fl/Fl_Box.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Input.H>

#include <FL/Fl_PNG_Image.H>

class System
{
public:
	System();
	void posMain(User& currentUser);

	void resetSettings();
	bool retrieveSettings();
	User& login();

	void setNewUser(User& newUser);
	std::vector<User>& getUserlist();
	std::vector<Food>& getFoodList();

	//FLTK Callbacks
	void newOrder(Fl_Widget* widget, void*);
	void outStandingOrders(Fl_Widget* widget, void*);
	void previousOrders(Fl_Widget* widget, void*);
	void reserveTable(Fl_Widget* widget, void*);
	void inventory(Fl_Widget* widget, void*);
	void managerControl(Fl_Widget* widget, void* data);
	void helpSupport(Fl_Widget* widget, void*);
	void employeeMessage(Fl_Widget* widget, void*);
	void logout(Fl_Widget* widget, void*);


private:
		std::vector<User> userList;
	 	std::vector<Food> foodList;
		std::vector<Beverage> beverageList;
};

struct UIComp;
struct mainData;
void createFoodItems(std::vector<Food>& foodItems);