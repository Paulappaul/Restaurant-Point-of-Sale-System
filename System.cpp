#include "System.hpp"


System::System()
{
	//check there are any settings
	if (System::retrieveSettings())
	{
		//ask the user to login
		User* loggedInUser = System::login(); // Receive the user pointer
		posMain(loggedInUser); // Pass the user pointer to posMain
	}
	//If there are no settings
	else
		System::resetSettings(); // we load a blank session


}

struct LoginS
{
    Fl_Input* userName;
    Fl_Input* passWord;
    System* systemCopy;
    User* userReturn; 
	bool flag = false;
};

User* System::login()
{
	User* testUser = new User();
	testUser->setUsername("Paul");
	testUser->setPassword("Paul");
	this->setNewUser(testUser);

	std::shared_ptr<LoginS> login = std::make_shared<LoginS>();

    std::cout << "debug login " << std::endl;
	std::cout << "Stage 1: " << &testUser << std::endl;
    
    Fl_Window* loginWindow = new Fl_Window(500, 200, "User Login");
    login->userName = new Fl_Input(125, 20, 250, 40, "username");
    login->passWord = new Fl_Input(125, 100, 250, 40, "password");
    login->systemCopy = this;

    Fl_Button* loginB = new Fl_Button(300, 155, 75, 30, "login");

    loginB->callback([](Fl_Widget* widget, void* data)
    {
        LoginS* login = static_cast<LoginS*>(data);

        for (User* user : login->systemCopy->getUserlist())
        {
            if (login->userName->value() == user->getUsername())
            {
                std::cout << "Username Found" << std::endl;
                if (login->passWord->value() == user->getPassword())
                {
                    std::cout << "Password found" << std::endl;
                    login->userReturn = user;
					std::cout << "Stage 2: " << &user << std::endl;
					std::cout << "Stage 3: " << &login->userReturn << std::endl;
					login->flag = true;

					break;
                }
                else
                    std::cout << "Bad Password" << std::endl;
					break;
            }
            else
                std::cout << "Bad Username" << std::endl;
        }
    }, login.get());

    loginWindow->show();

	while (!login->flag)
	{
		Fl::wait(); 
	}
	std::cout << "Stage 4: " << &login->userReturn << std::endl;
	loginWindow->hide();
	return login->userReturn;
}



struct UIComp
{
	Fl_Window* welcomemWindow;
	Fl_Box* welcomeMessage;
	Fl_Button* confirmButton;
	Fl_Input* passwordInput;
	System* systemInstance; 
};

void System::resetSettings()
{
	
	UIComp uiComp;
	uiComp.systemInstance = this; 

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
					User* newAdmin = new User;
					newAdmin->setUserPrivilege(Admin);
					newAdmin->setUsername("defaultAdmin");
					newAdmin->setPassword(uiData->passwordInput->value());
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
	std::shared_ptr<std::string> dailymessage;
	
};

void System::setNewUser(User* newUser)
{
	userList.push_back(newUser);
}



std::vector<User*>& System::getUserlist()
{
	return userList;
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
						User* newUser = new User;
						newUser->setUsername(md->newUsername->value());
						newUser->setPassword(md->newPassword->value());
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
				std::vector<User*> userList = md->thisSystem->getUserlist();

				for (User* user : userList)
				{
					//populate a dropDown menu  
					std::cout << "Username: " << user->getUsername() << std::endl;
					
				}
			}, md);




		Fl_Button* backButton = new Fl_Button(rightoff, 950, 300, 200, "@<-");
		backButton->callback([](Fl_Widget* widget, void* data)
			{
				mainData* md = static_cast<mainData*>(data);
				widget->parent()->hide();
				md->thisSystem->posMain(md->thisUser);

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
	Fl_Widget* previousParent;


};

void inventoryCallback(Fl_Widget* widget, void* data)
{
	const int boxSize = 200; 
	const int xOffset = 500;  
	const int yOffset = 100;  
	//an inventory of all the items, back and forward buttons

	widget->parent()->hide();

	mainData* md = static_cast<mainData*>(data);
	inventoryUI* iUI = new inventoryUI;

	iUI->previousParent = widget->parent();
	std::vector<Food> foodList = md->thisSystem->getFoodList();

	Fl_Window* mainWindow = new Fl_Window(1800, 1000, "Inventory");
	Fl_Box* mainBox = new Fl_Box(0, 0, 1800, 1000);
	Fl_PNG_Image* wallpaper = new Fl_PNG_Image("Wallpaper\\bg2.png");
	mainBox->image(wallpaper);

	Fl_Button* backButton = new Fl_Button(100, 800, 150, 100, "@<-");
	Fl_Button* nextButton = new Fl_Button(1550, 800, 150, 100, "@->");

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

	//Breakfast list
	int i = 0;
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			int buttonIndex = row * 4 + col;
			int x = col * boxSize + xOffset;
			int y = row * boxSize + yOffset;

			std::string label = iUI->breakfastList->at(i);
			const char* clabel = label.c_str();

			iUI->button[buttonIndex] = new Fl_Button(x, y, boxSize, boxSize, strdup(clabel));
			i++;
		}
	}


	nextButton->callback([](Fl_Widget* widget, void* data)
		{
	
			
	inventoryUI* iUI = static_cast<inventoryUI*>(data);
	std::cout << iUI->type << std::endl;
	if (iUI->type < 2) {
		iUI->type++;

		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				int buttonIndex = row * 4 + col;

				std::string label = "";
				if (iUI->type == 2) {
					label = iUI->dinnerList->at(buttonIndex);
				}
				else if (iUI->type == 1) {
					label = iUI->lunchList->at(buttonIndex);
				}

				const char* clabel = label.c_str();

				// Free the old label before setting the new one
				free((void*)iUI->button[buttonIndex]->label());

				iUI->button[buttonIndex]->label(strdup(clabel));
				iUI->button[buttonIndex]->redraw_label();
			}
		}
	}
		}, iUI);
	

	backButton->callback([](Fl_Widget* widget, void* data)
		{
			inventoryUI* iUI = static_cast<inventoryUI*>(data);
		std::cout << iUI->type << std::endl;
		if (iUI->type > -1) {
			iUI->type--;

			if (iUI->type == -1)
			{
				widget->parent()->hide();
				iUI->previousParent->show();
				
			}

			for (int row = 0; row < 4; row++) {
				for (int col = 0; col < 4; col++) {
					int buttonIndex = row * 4 + col;

					std::string label = "";
					if (iUI->type == 1) {
						label = iUI->lunchList->at(buttonIndex);
					}
					else if (iUI->type == 0) {
						label = iUI->breakfastList->at(buttonIndex);
					}


					const char* clabel = label.c_str();

					// Free the old label before setting the new one
					free((void*)iUI->button[buttonIndex]->label());

					iUI->button[buttonIndex]->label(strdup(clabel));
					iUI->button[buttonIndex]->redraw_label();
			}
		}
	}
		}, iUI);


	mainWindow->show();


}

void helpSupportCallback(Fl_Widget* widget, void* data)
{
	widget->parent()->hide();
	mainData* md = static_cast<mainData*>(data);
	Fl_Window* mainWindow = new Fl_Window(1800, 1000, "POS_SYSTEM");
	Fl_Box* mainBox = new Fl_Box(0, 0, 1800, 1000);
	Fl_Button* backButton = new Fl_Button(100, 700, 200, 200, "@<-");

	Fl_PNG_Image* wallpaper = new Fl_PNG_Image("Wallpaper\\bg4.png");
	mainBox->image(wallpaper);

	Fl_Button* employeeManual = new Fl_Button(100, 100, 200, 200, "Employee Manual");
	Fl_Button* harrassmentPolicy = new Fl_Button(325, 100, 200, 200, "Harrasment Policy");


	mainWindow->show();


}

void employeeMessageCallback(Fl_Widget* widget, void* data)
{
	widget->parent()->hide();
	mainData* md = static_cast<mainData*>(data);\

	Fl_Window* mainWindow = new Fl_Window(1800, 1000, "POS_SYSTEM");
	Fl_Box* mainBox = new Fl_Box(0, 0, 1800, 1000);
	Fl_Box* dialogue = new Fl_Box(400, 200, 1000, 600, "TEST");
	Fl_Button* backButton = new Fl_Button(100, 700, 200, 200, "@<-");

	Fl_PNG_Image* wallpaper = new Fl_PNG_Image("Wallpaper\\bg3.png");
	mainBox->image(wallpaper);

	

	const char* messageP = md->dailymessage->c_str();
	dialogue->label(messageP);

	backButton->callback([](Fl_Widget* widget2, void* data) 
		{
			
			widget2->parent()->hide();
		    Fl_Widget* lastWidget = static_cast<Fl_Widget*>(data);
			lastWidget->parent()->show();
			delete widget2->parent();
		
		}, widget);

	mainWindow->show();
	


}


void logoutCallback(Fl_Widget* widget, void* data)
{

	mainData* md = static_cast<mainData*>(data);
	widget->parent()->hide();

	if (fl_ask("Are you sure you want to log out?") == 1)
	{
		//Fl::delete_widget(widget); // if crash this is likelyy
		md->thisSystem->posMain(md->thisSystem->login());
		//delete md;
	}
	else
		widget->parent()->show();

}

void reserveTableCallback(Fl_Widget* widget, void* data)
{

	widget->parent()->hide();

	Fl_Window* mainWindow = new Fl_Window(1800, 1000, "Reserve Table");
	Fl_Box* mainBox = new Fl_Box(0, 0, 1800, 1000);

	Fl_PNG_Image* wallpaper = new Fl_PNG_Image("Wallpaper\\bg.png");
	mainBox->image(wallpaper);

	Fl_Button* table1 = new Fl_Button(100, 100, 200, 400, "Table 1");
	Fl_Button* table2 = new Fl_Button(400, 100, 200, 400, "Table 2");

	Fl_Button* table3 = new Fl_Button(600, 500, 200, 400, "Table 3");
	Fl_Button* table4 = new Fl_Button(900, 500, 200, 400, "Table 4");

	Fl_Button* table5 = new Fl_Button(1200, 100, 200, 600, "Table 5");
	Fl_Button* table6 = new Fl_Button(1500, 100, 200, 600, "Table 6");

	Fl_Button* bar = new Fl_Button(700, 100, 400, 200, "Bar");

	Fl_Box* mainLabel = new Fl_Box(100, 600, 200, 300, "Click the table you'd like to reserve");



	Fl_Button* backButton = new Fl_Button(100, 850, 100, 50, "@<-");

	backButton->callback([](Fl_Widget* widget2, void* data)
		{

			widget2->parent()->hide();
	Fl_Widget* lastWidget = static_cast<Fl_Widget*>(data);
	lastWidget->parent()->show();
	delete widget2->parent();

		}, widget);

	mainWindow->show();
}



void System::posMain(User* currentUser)
{
	const int top = 50;
	const int rightoff = 75;
	const int boxSize = 300;
	mainData md;
	md.thisSystem = this;
	md.thisUser = currentUser; // Pass the user pointer

	createFoodItems(this->getFoodList());
	md.dailymessage = dailyMesssageDemo();

	// Now, you can access the user using md.thisUser as a pointer
	if (md.thisUser != nullptr)
	{
		std::cout << "Logged-in Username: " << md.thisUser->getUsername() << std::endl;
	}

	Fl_Window* mainWindow =        new Fl_Window(1800, 1000, "POS_SYSTEM");
	Fl_Box* mainBox =			   new Fl_Box(0, 0, 1800, 1000);
	
	Fl_PNG_Image* wallpaper =	   new Fl_PNG_Image("Wallpaper\\bg1.png");
	mainBox->image(wallpaper);
	
	//Row 1
	Fl_Button* newOrder =          new Fl_Button(rightoff,                 top, boxSize, boxSize, "New Order");
	Fl_Button* outStandingOrders = new Fl_Button(rightoff + boxSize, top, boxSize, boxSize, "Current Orders");
	Fl_Button* previousOrders =    new Fl_Button(rightoff + (boxSize * 2), top, boxSize, boxSize, "Previous Orders");

	//Row 2
	Fl_Button* reserveTable =      new Fl_Button(rightoff,                 top + boxSize, boxSize, boxSize, "Reserve Table");
	Fl_Button* inventory =         new Fl_Button(rightoff + boxSize, top + boxSize, boxSize, boxSize, "Inventory");
	Fl_Button* managerControl =    new Fl_Button(rightoff + (boxSize * 2), top + boxSize, boxSize, boxSize, "Manager Control");

	//Row 3
	Fl_Button* helpSupport =       new Fl_Button(rightoff,                 top + boxSize * 2, boxSize, boxSize, "Help/Support");
	Fl_Button* employeeMessage =   new Fl_Button(rightoff + boxSize, top + boxSize * 2, boxSize, boxSize, "Daily Message");
	Fl_Button* logout =            new Fl_Button(rightoff + (boxSize * 2), top + boxSize * 2, boxSize, boxSize, "Logout");

	newOrder->color(FL_DARK_RED);
	outStandingOrders->color(FL_DARK_RED);
	previousOrders->color(FL_DARK_RED);

	reserveTable->color(FL_DARK_GREEN);
	inventory->color(FL_DARK_MAGENTA);
	managerControl->color(FL_DARK_CYAN);

	helpSupport->color(FL_BLUE);
	employeeMessage->color(FL_BLUE);
	logout->color(FL_RED);

	newOrder->type(FL_BORDER_BOX);
	outStandingOrders->type(FL_BORDER_BOX);
	previousOrders->type(FL_BORDER_BOX);

	reserveTable->type(FL_BORDER_BOX);
	inventory->type(FL_BORDER_BOX);
	managerControl->type(FL_BORDER_BOX);

	helpSupport->type(FL_BORDER_BOX);
	employeeMessage->type(FL_BORDER_BOX);
	logout->type(FL_BORDER_BOX);

	newOrder->labelsize(36);
	outStandingOrders->labelsize(36);
	previousOrders->labelsize(36);

	reserveTable->labelsize(36);
	inventory->labelsize(36);
	managerControl->labelsize(36);

	helpSupport->labelsize(36);
	employeeMessage->labelsize(36);
	managerControl->labelsize(36);
	logout->labelsize(36);



	//callbacks
	newOrder->callback();
	outStandingOrders->callback();
	previousOrders->callback();
	reserveTable->callback(reserveTableCallback, &md);
	inventory->callback(inventoryCallback, &md);


	managerControl->callback(managerControlCallback, &md);

	helpSupport->callback(helpSupportCallback, &md);
	employeeMessage->callback(employeeMessageCallback, &md);
	logout->callback(logoutCallback, &md);


	Fl_Text_Display* terminalDisplay = new Fl_Text_Display(1150, top, 580, 500);
	terminalDisplay->textfont(FL_COURIER);
	terminalDisplay->textsize(16);
	Fl_Text_Buffer* textBuffer = new Fl_Text_Buffer();
	terminalDisplay->buffer(textBuffer);

	Fl_Clock clock(1300, 600, 300, 300, "Clock");

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
		else if (temp == 'F' || temp == 'f')
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

std::shared_ptr<std::string> dailyMesssageDemo()
{
	auto dailymessage = std::make_shared<std::string>(
		"We hope this message finds you well. ..."); // Your long message
	return dailymessage;
}





