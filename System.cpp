#include "System.hpp"


System::System()
{
	//check there are any settings
	if (System::retrieveSettings())
		//ask the user to login
		posMain(System::login());
	//If there are no settings
	else
		System::resetSettings(); // we load a blank session


}

User& System::login()
{
	std::cout << "debug login " << std::endl;
	User debugUser; 

	return debugUser;

}


struct UIComp
{
	Fl_Window* welcomemWindow;
	Fl_Box* welcomeMessage;
	Fl_Button* confirmButton;
	Fl_Input* passwordInput;
	System* systemInstance; // Store a pointer to the current System instance
};

void System::resetSettings()
{
	
	UIComp uiComp;
	uiComp.systemInstance = this; // Store the pointer to the current System instance

	std::cout << "reset Settings Debug Move" << std::endl; 
	uiComp.welcomemWindow = new Fl_Window(600, 300, "Welcome!");
	uiComp.welcomeMessage =   new Fl_Box(50, 25, 300, 150, "Welcome to the POS intialization! Please find your provided Password!");
	uiComp.confirmButton = new Fl_Button(250, 150, 100, 50, "Confirm");
	uiComp.passwordInput= new Fl_Input(50, 50, 300, 100, "Password");
	uiComp.passwordInput->hide();

	uiComp.confirmButton->callback([](Fl_Widget* widget, void* data)
		{
			UIComp* uiData = static_cast<UIComp*>(data);
			uiData->welcomeMessage->hide();
			uiData->passwordInput->show();
			uiData->confirmButton->label("Submit");
			
			
			uiData->confirmButton->callback([](Fl_Widget* widget, void* data)
				{
					
			 
					UIComp* uiData = static_cast<UIComp*>(data);
					User newAdmin;
					newAdmin.setUserPrivilege(Admin);
					newAdmin.setUsername("defaultAdmin");
					newAdmin.setPassword(uiData->passwordInput->value());
					uiData->welcomemWindow->hide();
					uiData->systemInstance->posMain(newAdmin);
			
				}, uiData);


		}, &uiComp);

	uiComp.welcomemWindow->end();
	uiComp.welcomemWindow->show();
	Fl::run();

}

struct mainData
{
	System* thisSystem;
	User* thisUser;
	Fl_Input* newUsername;
    Fl_Input* newPassword;
	Fl_Widget* previousParent;
	


};

void System::setNewUser( User& newUser)
{
	System::userList.push_back(newUser);
}


std::vector<User>& System::getUserlist()
{
	return System::userList;
}

std::vector<Food>& System::getFoodList()
{

	return System::foodList;

}


void managerControlCallback(Fl_Widget* widget, void* data)
{
	mainData* md = static_cast<mainData*>(data);
	if (md->thisUser->getUserPrivilege() == true)
	{
		
		widget->parent()->hide();
		md->previousParent = widget;
		Fl_Window* mainWindow = new Fl_Window(1800, 1000, "Manager Window");
		Fl_Box* mainBox = new Fl_Box(0, 0, 1800, 1000);
		Fl_PNG_Image* wallpaper = new Fl_PNG_Image("Wallpaper\\bg.png");
		mainBox->image(wallpaper);
		const int top = 30;
		const int rightoff = 30;
		const int boxSize = 300;
		Fl_Button* newUser = new Fl_Button(rightoff, top, boxSize, boxSize, "Add Employee");

		newUser->callback([](Fl_Widget* widget, void* data)
			{
				mainData* mdpass =         static_cast<mainData*>(data);
				Fl_Window* newUserWindow = new Fl_Window(600, 300, "Add New User");
				mdpass->newUsername =      new Fl_Input(50, 50, 200, 100, "username");
				mdpass->newPassword =      new Fl_Input(50, 150, 200, 100, "password");
				Fl_Button* confirm =       new Fl_Button(150, 250, 50, 50);

				confirm->callback([](Fl_Widget* widget, void* data)
					{
						mainData* md = static_cast<mainData*>(data);
						User newUser;
						newUser.setUsername(md->newUsername->value());
						newUser.setPassword(md->newPassword->value());
						md->thisSystem->setNewUser(newUser);
						widget->parent()->hide();
						//add new admin? 
						
					}, mdpass);

				newUserWindow->show();
			
			}, md);

		Fl_Button* removeEmployee = new Fl_Button(rightoff + boxSize, top, boxSize, boxSize, "Remove Employee");
		removeEmployee->callback([](Fl_Widget* widget, void* data)
			{
				mainData* md = static_cast<mainData*>(data);
				std::vector<User> userList = md->thisSystem->getUserlist();

				for (User& user : userList)
				{
					//populate a dropDown menu  
					std::cout << "Username: " << user.getUsername() << std::endl;
					
				}

			}, md);




		Fl_Button* backButton = new Fl_Button(rightoff, 950, 300, 200, "@<-");
		backButton->callback([](Fl_Widget* widget, void* data)
			{
				mainData* md = static_cast<mainData*>(data);
				widget->parent()->hide();
				md->thisSystem->posMain(*md->thisUser);

			},md );


		mainWindow->show();
		Fl::run();

	}
	else { std::cout << "INVALID USER PRIVILEGE" << std::endl; }

}

struct inventoryUI 
{
	Fl_Button* button[16];
	std::shared_ptr<std::vector<std::string>> lunchList;
	std::shared_ptr<std::vector<std::string>> dinnerList;
	std::shared_ptr<std::vector<std::string>> breakfastList;
	int type = 0; 


};

void inventoryCallback(Fl_Widget* widget, void* data)
{
	const int boxSize = 200; 
	const int xOffset = 75;  
	const int yOffset = 50;  
	//an inventory of all the items, back and forward buttons

	widget->parent()->hide();

	mainData* md = static_cast<mainData*>(data);
	inventoryUI* iUI = new inventoryUI;
	std::vector<Food> foodList = md->thisSystem->getFoodList();

	Fl_Window* mainWindow = new Fl_Window(1800, 1000, "Inventory");
	Fl_Box* mainBox = new Fl_Box(0, 0, 1800, 1000);
	Fl_PNG_Image* wallpaper = new Fl_PNG_Image("Wallpaper\\bg2.png");
	mainBox->image(wallpaper);

	Fl_Button* backButton = new Fl_Button(xOffset, 650, 300, 200, "@<-");
	Fl_Button* nextButton = new Fl_Button(1550, 650, 300, 200, "@->");

	std::shared_ptr<std::vector<std::string>> breakfastList = std::make_shared<std::vector<std::string>>();
	std::shared_ptr<std::vector<std::string>> lunchList =     std::make_shared<std::vector<std::string>>();
	std::shared_ptr<std::vector<std::string>> dinnerList =    std::make_shared<std::vector<std::string>>();

	
	iUI->dinnerList = dinnerList;
	iUI->lunchList = lunchList;
	iUI->breakfastList = breakfastList;

	for (int i = 0; i < foodList.size(); i++)
	{
		if(foodList.at(i).getFoodType() == breakfast)
			breakfastList->push_back(foodList.at(i).getName());
		else if(foodList.at(i).getFoodType() == lunch)
			lunchList->push_back(foodList.at(i).getName());
		else if(foodList.at(i).getFoodType() == dinner)
			dinnerList->push_back(foodList.at(i).getName());
	}
	
	for (int i = 0; i < breakfastList->size(); i++)
	{
		std::cout << "Breakfast Item " << i << ": " << breakfastList->at(i) << std::endl;
	}

	for (int i = 0; i < lunchList->size(); i++)
	{
		std::cout << "Breakfast Item " << i << ": " << lunchList->at(i) << std::endl;
	}

	//Breakfast list
	int i = 0;

	

	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			int buttonIndex = row * 4 + col;
			int x = col * boxSize + xOffset;
			int y = row * boxSize + yOffset;


			iUI->button[buttonIndex] = new Fl_Button(x, y, boxSize, boxSize, strdup(breakfastList->at(i).c_str()));
			i++;
			std::cout << i << std::endl;
			
		}
	}


	nextButton->callback([](Fl_Widget* widget, void* data)
		{
	
			inventoryUI* iUI = static_cast<inventoryUI*>(data);

	for (int i = 0; i < iUI->lunchList->size(); i++)
	{
		std::cout << "lunch Item " << i << ": " << iUI->lunchList->at(i) << std::endl;
	}

			std::cout << "type" << iUI->type << std::endl;
			if (iUI->type < 2) { iUI->type++;}
			std::cout << "type after ++" << iUI->type << std::endl;

			

				if (iUI->type == 1)
				{
					

					//lunchList
					for (int row = 0; row < 4; row++)
					{
						for (int col = 0; col < 4; col++)
						{
							int buttonIndex = row * 4 + col;
							
							std::string label = iUI->lunchList->at(buttonIndex);
							const char* clabel = label.c_str();

							iUI->button[buttonIndex]->label(strdup(clabel));
							iUI->button[buttonIndex]->redraw_label();

						}
					}
				}
				else if (iUI->type == 2)
				{

					//dinnerList
					for (int row = 0; row < 4; row++)
					{
						for (int col = 0; col < 4; col++)
						{
							int buttonIndex = row * 4 + col;

							std::string label = iUI->dinnerList->at(buttonIndex);
							const char* clabel = label.c_str();
							iUI->button[buttonIndex]->label(strdup(clabel));
							iUI->button[buttonIndex]->redraw_label();
						}
					}



				}
				else { std::cout << "Cannot proceed" << std::endl; }

		}, iUI);


	backButton->callback([](Fl_Widget* widget, void* data)
		{
			inventoryUI* iUI = static_cast<inventoryUI*>(data);

				if (iUI->type > 0) { iUI->type--;}

				if (iUI->type == 1)
				{

					//lunchList
					for (int row = 0; row < 4; row++)
					{
						for (int col = 0; col < 4; col++)
						{
							int buttonIndex = row * 4 + col;
							iUI->button[buttonIndex]->label(iUI->lunchList->at(buttonIndex).c_str());
			
						}
					}
				}
				else if (iUI->type == 0)
				{

					//breakfastList
					for (int row = 0; row < 4; row++)
					{
						for (int col = 0; col < 4; col++)
						{
							int buttonIndex = row * 4 + col;
							iUI->button[buttonIndex]->label(iUI->breakfastList->at(buttonIndex).c_str());
			
						}
					}
			
			
			
				}
				else { std::cout << "Cannot proceed" << std::endl; }
			


		});

	mainWindow->show();



}




void System::posMain(User& currentUser)
{
	const int top      = 50;
	const int rightoff = 75;
	const int boxSize  = 300;
	mainData md;
	md.thisSystem = this;
	md.thisUser = &currentUser;

	createFoodItems(this->getFoodList());

	Fl_Window* mainWindow =        new Fl_Window(1800, 1000, "POS_SYSTEM");
	Fl_Box* mainBox = new Fl_Box(0, 0, 1800, 1000);
	
	Fl_PNG_Image* wallpaper = new Fl_PNG_Image("Wallpaper\\bg1.png");
	mainBox->image(wallpaper);
	
	//Row 1
	Fl_Button* newOrder =          new Fl_Button(rightoff,                 top, boxSize, boxSize, "New Order");
	Fl_Button* outStandingOrders = new Fl_Button(rightoff + boxSize, top, boxSize, boxSize, "Outstanding Orders");
	Fl_Button* previousOrders =    new Fl_Button(rightoff + (boxSize * 2), top, boxSize, boxSize, "Previous Orders");

	//Row 2
	Fl_Button* reserveTable =      new Fl_Button(rightoff,                 top + boxSize, boxSize, boxSize, "Reserve Table");
	Fl_Button* inventory =         new Fl_Button(rightoff + boxSize, top + boxSize, boxSize, boxSize, "Inventory");
	Fl_Button* managerControl =    new Fl_Button(rightoff + (boxSize * 2), top + boxSize, boxSize, boxSize, "Manager Control");

	//Row 3
	Fl_Button* helpSupport =       new Fl_Button(rightoff,                 top + boxSize * 2, boxSize, boxSize, "Help/Support");
	Fl_Button* employeeMessage =   new Fl_Button(rightoff + boxSize, top + boxSize * 2, boxSize, boxSize, "Employee Message");
	Fl_Button* logout =            new Fl_Button(rightoff + (boxSize * 2), top + boxSize * 2, boxSize, boxSize, "Logout");


	//callbacks
	newOrder->callback();
	outStandingOrders->callback();
	previousOrders->callback();
	reserveTable->callback();
	inventory->callback(inventoryCallback, &md);


	managerControl->callback(managerControlCallback, &md);

	helpSupport->callback();
	employeeMessage->callback();
	logout->callback();


	Fl_Text_Display* terminalDisplay = new Fl_Text_Display(1150, top, 580, 900);
	terminalDisplay->textfont(FL_COURIER);
	terminalDisplay->textsize(16);
	Fl_Text_Buffer* textBuffer = new Fl_Text_Buffer();
	terminalDisplay->buffer(textBuffer);


	mainWindow->show();
	Fl::run();

	
}

bool System::retrieveSettings()
{
	char temp = ' ';
	std::cout << "Debug mode entered" <<
			     "Enter T or F " << std::endl;
	

	while (true)
	{
		std::cin >> temp;
		if (temp == 'T' || temp == 't')
			return true;
		else if (temp == 'N' || temp == 'n')
			return false;
		else
			std::cout << "BAD INPUT. Enter Again" << std::endl;
	}
	
	/*
	std::string temp;
	//getLine
	if(temp == " ")
		return false;
	else
	//populate 

	return true;
	*/
}


void createFoodItems(std::vector<Food>& foodItems)
{
	// Common American breakfast, lunch, dinner, and dessert names
	std::vector<std::string> breakfastNames = {
		"Pancakes", "Waffles", "Eggs Benedict", "Omelette",
		"French Toast", "Bacon and Eggs", "Bagel", "Cereal",
		"Scrambled Eggs", "Pork Sausage", "Ham and Cheese Omelette", "Blueberry Pancakes",
		"Belgian Waffles", "Vegetable Omelette", "Granola with Yogurt", "Fruit Parfait"
	};

	std::vector<std::string> lunchNames = {
		"Hamburger", "Cheeseburger", "BLT Sandwich", "Club Sandwich",
		"Caesar Salad", "Chicken Wrap", "Pizza", "Sushi",
		"Grilled Chicken Salad", "Turkey Panini", "Mushroom Swiss Burger", "Greek Salad",
		"Veggie Wrap", "Pepperoni Pizza", "California Roll", "Cobb Salad"
	};

	std::vector<std::string> dinnerNames = {
		"Steak", "Salmon", "Spaghetti Bolognese", "Chicken Alfredo",
		"Filet Mignon", "Shrimp Scampi", "Beef Stroganoff", "Veggie Stir-Fry",
		"Grilled Salmon", "Lobster Tail", "Fettuccine Alfredo", "Pork Chops",
		"Vegetable Stir-Fry", "Sesame Crusted Tuna", "Lamb Chops", "Creamy Pesto Pasta"
	};

	std::vector<std::string> dessertNames = {
		"Apple Pie", "Chocolate Cake", "Ice Cream Sundae", "Cheesecake",
		"Brownie", "Fruit Tart", "Tiramisu", "Key Lime Pie",
		"Peach Cobbler", "Red Velvet Cake", "Banana Split", "Lemon Meringue Pie",
		"Molten Chocolate Cake", "Strawberry Shortcake", "Chocolate Fondue", "Pecan Pie"
	};

	// Prices for food items
	double breakfastPrice = 8.99;
	double lunchPrice = 12.99;
	double dinnerPrice = 18.99;
	double dessertPrice = 5.99;


		for (const std::string& name : breakfastNames)
		{
			Food breakfastItem;
			breakfastItem.setName(name);
			breakfastItem.setPrice(breakfastPrice);
			breakfastItem.setFoodType(breakfast);
			foodItems.push_back(breakfastItem);
		}

		for (const std::string& name : lunchNames)
		{
			Food lunchItem;
			lunchItem.setName(name);
			lunchItem.setPrice(lunchPrice);
			lunchItem.setFoodType(lunch);
			foodItems.push_back(lunchItem);
		}

		for (const std::string& name : dinnerNames)
		{
			Food dinnerItem;
			dinnerItem.setName(name);
			dinnerItem.setPrice(dinnerPrice);
			dinnerItem.setFoodType(dinner);
			foodItems.push_back(dinnerItem);
		}

		for (const std::string& name : dessertNames)
		{
			Food dessertItem;
			dessertItem.setName(name);
			dessertItem.setPrice(dessertPrice);
			dessertItem.setFoodType(desert);
			foodItems.push_back(dessertItem);
		}
	
}




void System::newOrder(Fl_Widget* widget, void*)
{


}
void System::outStandingOrders(Fl_Widget* widget, void*)
{


}
void System::previousOrders(Fl_Widget* widget, void*)
{


}
void System::reserveTable(Fl_Widget* widget, void*)
{


}

void System::managerControl(Fl_Widget* widget, void* data)
{


}
void System::helpSupport(Fl_Widget* widget, void*)
{

	
}
void System::employeeMessage(Fl_Widget* widget, void*)
{


}
void System::logout(Fl_Widget* widget, void*)
{


}



