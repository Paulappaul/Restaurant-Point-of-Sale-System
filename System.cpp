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
	Fl_Text_Buffer* orderList;
	std::shared_ptr<std::string> dailymessage;
	std::vector<std::string> currentOrder;
	std::vector < std::vector<std::string>>previousOrder;
	Till* systemTillptr;
	Fl_Box* orderNumber;
	double orderPrice = 0;
	int intOrderNumber = 1;
	
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
	std::cout << "getting food list" << std::endl;
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
		Fl_PNG_Image* wallpaper = new Fl_PNG_Image("Wallpaper\\red.png");
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
	//Fl_Text_Buffer* tb;
	

};

struct dataPass
{
	std::string sP;
	double price;
	Fl_Text_Buffer* tb;
	double* totalPrice; 

};

void inventoryCallback(Fl_Widget* widget, void* data)
{
	const int boxSize = 200; 
	const int xOffset = 500;  
	const int yOffset = 100;  
	//an inventory of all the items, back and forward buttons

	widget->parent()->hide();

	mainData* md = static_cast<mainData*>(data);
	inventoryUI* iUI = new inventoryUI; //make sure this is deleted

	iUI->previousParent = widget->parent();
	std::vector<Food> foodList = md->thisSystem->getFoodList();

	

	Fl_Window* mainWindow = new Fl_Window(1800, 1000, "Inventory");
	Fl_Box* mainBox = new Fl_Box(0, 0, 1800, 1000);
	Fl_PNG_Image* wallpaper = new Fl_PNG_Image("Wallpaper\\blue.png");
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
			iUI->button[buttonIndex]->color(FL_DARK_YELLOW);
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
					iUI->button[buttonIndex]->color(FL_RED);
				}
				else if (iUI->type == 1) {
					label = iUI->lunchList->at(buttonIndex);
					iUI->button[buttonIndex]->color(FL_GREEN);
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
				//delete iUI;
				
			}

			for (int row = 0; row < 4; row++) {
				for (int col = 0; col < 4; col++) {
					int buttonIndex = row * 4 + col;

					std::string label = "";
					if (iUI->type == 1) {
						label = iUI->lunchList->at(buttonIndex);
						iUI->button[buttonIndex]->color(FL_GREEN);
					}
					else if (iUI->type == 0) {
						label = iUI->breakfastList->at(buttonIndex);
						iUI->button[buttonIndex]->color(FL_DARK_YELLOW);
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

	Fl_PNG_Image* wallpaper = new Fl_PNG_Image("Wallpaper\\blue.png");
	mainBox->image(wallpaper);

	Fl_Button* employeeManual = new Fl_Button(100, 100, 200, 200, "Employee Manual");
	Fl_Button* harrassmentPolicy = new Fl_Button(325, 100, 200, 200, "Harrasment Policy");


	backButton->callback([](Fl_Widget* widget2, void* data)
		{

			widget2->parent()->hide();
			Fl_Widget* lastWidget = static_cast<Fl_Widget*>(data);
			lastWidget->parent()->show();
			delete widget2->parent();

		}, widget);

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

	Fl_PNG_Image* wallpaper = new Fl_PNG_Image("Wallpaper\\blue.png");
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

	Fl_PNG_Image* wallpaper = new Fl_PNG_Image("Wallpaper\\wood.png");
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



void orderCallback(Fl_Widget* widget, void* userData)
{
	// Cast the userData back to shared_ptr
	std::shared_ptr<dataPass>* passPtr = static_cast<std::shared_ptr<dataPass>*>(userData);
	std::shared_ptr<dataPass> pass = *passPtr;
	Fl_Text_Buffer* textBuffer = pass->tb;

	std::stringstream s_D;
	std::string sP = pass->sP;
	double price = pass->price;

	*(pass->totalPrice) += price;
	std::cout << "main data total Order Price: " << *(pass->totalPrice);


	s_D << price;
	std::string numStr = s_D.str();
	std::string result = sP + "\t" + numStr + "\n";
	const char* resultChar = result.c_str();
	


	// Add the new order information to the text display
	const char* newOrderText = pass->sP.c_str();
	textBuffer->append(resultChar);
	// Redraw the text display to update its content
	textBuffer->call_modify_callbacks();
	
	

}



void newOrderCallback(Fl_Widget* widget, void* data) 
{
	
	const int boxSize = 200;
	const int xOffset = 500;
	const int yOffset = 100;

	mainData* md = static_cast<mainData*>(data);
	widget->parent()->hide();
	//currentOrder is a vecotr that is not dynamically allocated...
	if (md->currentOrder.size() > 0)
		md->previousOrder.push_back(md->currentOrder);
	else
		std::cout << "no previous orders" << std::endl;

	Fl_Text_Buffer* textBuffer = md->orderList;
	textBuffer->remove(0, textBuffer->length());


	inventoryUI* iUI =		  new inventoryUI; //make sure to delete
	Fl_Window* mainWindow =	  new Fl_Window(1800, 1000, "New Order");
	Fl_Box* mainBox =		  new Fl_Box(0, 0, 1800, 1000);
	Fl_PNG_Image* wallpaper = new Fl_PNG_Image("Wallpaper\\red.png");
	Fl_Button* backButton =   new Fl_Button(100, 800, 150, 100, "@<-");
	Fl_Button* nextButton =   new Fl_Button(1550, 800, 150, 100, "@->");
	
	std::vector<Food> foodList = md->thisSystem->getFoodList();
	std::shared_ptr<std::vector<std::string>> breakfastList = std::make_shared<std::vector<std::string>>();
	std::shared_ptr<std::vector<std::string>> lunchList = std::make_shared<std::vector<std::string>>();
	std::shared_ptr<std::vector<std::string>> dinnerList = std::make_shared<std::vector<std::string>>();
	iUI->dinnerList = dinnerList;
	iUI->lunchList = lunchList;
	iUI->breakfastList = breakfastList;
	mainBox->image(wallpaper);
	iUI->previousParent = widget->parent();
	for (int i = 0; i < foodList.size(); i++)
	{
		if (foodList.at(i).getFoodType() == breakfast)
			breakfastList->push_back(foodList.at(i).getName());
		else if (foodList.at(i).getFoodType() == lunch)
			lunchList->push_back(foodList.at(i).getName());
		else if (foodList.at(i).getFoodType() == dinner)
			dinnerList->push_back(foodList.at(i).getName());
	}
	

	//Breakfast list
	int i = 0;
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			//review use of int and const here
			const int buttonIndex = row * 4 + col;
			const int x = col * boxSize + xOffset;
			const int y = row * boxSize + yOffset;

			std::string label = iUI->breakfastList->at(i);
			std::string* label2 = new std::string(iUI->breakfastList->at(i));
			const char* clabel = label.c_str(); // delete? 

			// Create a shared_ptr for dataPass
			std::shared_ptr<dataPass> pass = std::make_shared<dataPass>();
			pass->sP = iUI->breakfastList->at(i);
			pass->price = foodList.at(buttonIndex).getPrice();
			pass->tb = md->orderList;
			pass->totalPrice = &(md->orderPrice);


			iUI->button[buttonIndex] = new Fl_Button(x, y, boxSize, boxSize, strdup(clabel));
			iUI->button[buttonIndex]->color(FL_DARK_YELLOW);
			iUI->button[buttonIndex]->callback(orderCallback, new std::shared_ptr<dataPass>(pass));
			i++;
			
		}
	}

	;
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
						if (iUI->type == 2)
						{
							label = iUI->dinnerList->at(buttonIndex);
							iUI->button[buttonIndex]->color(FL_RED);
						}
						else if (iUI->type == 1)
						{
							label = iUI->lunchList->at(buttonIndex);
							iUI->button[buttonIndex]->color(FL_GREEN);
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

	std::cout << "passed" << std::endl;
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
					iUI->button[buttonIndex]->color(FL_GREEN);
				}
				else if (iUI->type == 0) {
					label = iUI->breakfastList->at(buttonIndex);
					iUI->button[buttonIndex]->color(FL_DARK_YELLOW);
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

void currentOrderCallback(Fl_Widget* widget, void* data)
{
	
	mainData* md = static_cast<mainData*>(data);
	md->orderList->remove(0, md->orderList->length());
	for (std::string line : md->currentOrder)
	{
		const char* cLine = line.c_str();
		md->orderList->append(cLine);
		md->orderList->append("\n");
		std::cout << "cLine: " << cLine << std::endl;

	}

}


void previousCallback(Fl_Widget* widget, void* data)
{
	mainData* md = static_cast<mainData*>(data);
	
	md->orderList->remove(0, md->orderList->length());
	//iterate through each Vector held in the previous order vector
	for (const std::vector<std::string>& innerVector : md->previousOrder)
	{
		//for each vector we dump its conents
		for (const std::string& str : innerVector) 
		{

			md->orderList->append(str.c_str());
			md->orderList->append("\n"); // Add a newline after each string
		}
	}
}

std::vector<char*>& System::getOrder2Free() 
{
	return order2free;
}


void updateOrderInfo(mainData* md)
{
    // Get the contents of the text buffer and store in the vector
    const char* bufferText = md->orderList->text();
    if (bufferText) {
        std::string text(bufferText);
        size_t pos = 0;
        while ((pos = text.find("\n")) != std::string::npos) {
            md->currentOrder.push_back(text.substr(0, pos));
            text.erase(0, pos + 1);
        }
        md->currentOrder.push_back(text);  // Add the last line
    }

    std::string submitedPrice = std::to_string(md->orderPrice);
    std::string finalPriceMessage = "Checkout: " + submitedPrice;
    const char* charPass = finalPriceMessage.c_str();

    md->orderList->append(charPass);

    md->intOrderNumber++;
    std::cout << "Order Number" << md->intOrderNumber << std::endl;

    // Convert the integer to a string and concatenate it with the string
    std::string finalOrder = "Current Order: " + std::to_string(md->intOrderNumber);
   const char* finalOrderMessage = finalOrder.c_str();
	char* duplicatedMessage = strdup(finalOrderMessage);

    std::cout << "final message = " << duplicatedMessage << std::endl;
    md->orderNumber->label(duplicatedMessage);
    md->orderNumber->redraw_label();

    // Print the lines in the vector
    for (const std::string& line : md->currentOrder) {
        std::cout << line << std::endl;
    }

	md->thisSystem->getOrder2Free().push_back(duplicatedMessage);
}

void submitButtonCallback(Fl_Widget* widget, void* data) 
{
    mainData* md = static_cast<mainData*>(data);

    std::cout << "total order price: " << md->orderPrice << " total balance " << md->systemTillptr->getBalance() << std::endl;

    if (md->orderPrice > md->systemTillptr->getBalance()) 
	{
        std::cout << "PRICE EXCEEDS TILL BALANCE" << std::endl;
    } 
	else
	{
        if (md->currentOrder.size() == 0)
		{
            std::cout << "no current order" << std::endl;
        } 
		else 
		{
            md->previousOrder.push_back(md->currentOrder);
            md->currentOrder.clear();
            std::cout << "current order vector cleared" << std::endl;
            std::cout << "size of the buffer: " << md->currentOrder.size() << std::endl;
        }
        updateOrderInfo(md);
    }
}




void deleteButtonCallback(Fl_Widget* widget, void* data)
{
	//flushes the contents of the current buffer
	mainData* md = static_cast<mainData*>(data);
	md->orderList->remove(0, md->orderList->length());

}

void System::posMain(User* currentUser)
{
	//GUI Button Parameters
	const int top = 50;
	const int rightoff = 75;
	const int boxSize = 300;

	//data structure intialization 
	mainData md;
	Till systemTill; 

	//Assign values to data structures 
	md.thisSystem = this;
	md.thisUser = currentUser;			
	md.systemTillptr = &systemTill;

	//debug
	md.dailymessage = dailyMesssageDemo();
	systemTill.setBalance(25.00);
	createFoodItems(this->getFoodList());

	//Username Cast
	std::string User = "Current User: " + md.thisUser->getUsername();
	const char* user = User.c_str();

	//Visual Intialization begin
	Fl_Window* mainWindow =            new Fl_Window(1800, 1000, "POS_SYSTEM");
	Fl_Box* mainBox =			       new Fl_Box(0, 0, 1800, 1000);
								      
	Fl_PNG_Image* wallpaper =	       new Fl_PNG_Image("Wallpaper\\blue.png");
	mainBox->image(wallpaper);	      
								      
	//Row 1						      
	Fl_Button* newOrder =              new Fl_Button(rightoff,                 top, boxSize, boxSize, "New Order");
	Fl_Button* outStandingOrders =     new Fl_Button(rightoff + boxSize, top, boxSize, boxSize, "Current Orders");
	Fl_Button* previousOrders =        new Fl_Button(rightoff + (boxSize * 2), top, boxSize, boxSize, "Previous Orders");
								      
	//Row 2						      
	Fl_Button* reserveTable =          new Fl_Button(rightoff,                 top + boxSize, boxSize, boxSize, "Reserve Table");
	Fl_Button* inventory =             new Fl_Button(rightoff + boxSize, top + boxSize, boxSize, boxSize, "Inventory");
	Fl_Button* managerControl =        new Fl_Button(rightoff + (boxSize * 2), top + boxSize, boxSize, boxSize, "Manager Control");
								      
	//Row 3						      
	Fl_Button* helpSupport =           new Fl_Button(rightoff,                 top + boxSize * 2, boxSize, boxSize, "Help/Support");
	Fl_Button* employeeMessage =       new Fl_Button(rightoff + boxSize, top + boxSize * 2, boxSize, boxSize, "Daily Message");
	Fl_Button* logout =                new Fl_Button(rightoff + (boxSize * 2), top + boxSize * 2, boxSize, boxSize, "Logout");

	//Order Number
	Fl_Box* orderNumber =			   new Fl_Box(1150, top, 580, 50, "Current Order: #1");

	//Order Display
	Fl_Text_Display* terminalDisplay = new Fl_Text_Display(1150, top + 50, 580, 500);
	Fl_Text_Buffer* textBuffer =	   new Fl_Text_Buffer();

	//Order Submission
	Fl_Button* submitButton =		   new Fl_Button(1150, 550, 290, 150, "Submit");
	Fl_Button* deleteButton =		   new Fl_Button(1440, 550, 290, 150, "Delete");

	//User Label
	Fl_Box* userLabel =				   new Fl_Box(1300, 900, 300, 100, user);

	//Clock
	Fl_Clock* clock =				   new Fl_Clock (1350, 750, 200, 150, "Current Time");
	clock->type(FL_ENGRAVED_FRAME);

	//GUI modifiers
	terminalDisplay->textfont(FL_COURIER);
	terminalDisplay->textsize(16);
	
	terminalDisplay->buffer(textBuffer);
	md.orderList = textBuffer;

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

	submitButton->color(FL_CYAN);
	deleteButton->color(FL_RED);

	userLabel->type(FL_UP_BOX);
	userLabel->labeltype(_FL_SHADOW_LABEL);
	userLabel->color(FL_WHITE);

	md.orderNumber = orderNumber;

	//callbacks
	newOrder->callback(newOrderCallback, &md);
	outStandingOrders->callback(currentOrderCallback, &md);
	previousOrders->callback(previousCallback, &md);
	reserveTable->callback(reserveTableCallback, &md);
	inventory->callback(inventoryCallback, &md);
	managerControl->callback(managerControlCallback, &md);
	helpSupport->callback(helpSupportCallback, &md);
	employeeMessage->callback(employeeMessageCallback, &md);
	logout->callback(logoutCallback, &md);
	submitButton->callback(submitButtonCallback, &md);
	deleteButton->callback(deleteButtonCallback, &md);

	mainWindow->show();
	Fl::run();
	
}

bool System::retrieveSettings()
{
	std::ifstream inputFile;

	// Open a file for reading
	inputFile.open("posdata.txt");

	// Check if the file opened successfully
	if (!inputFile.is_open())
	{
		std::cerr << "Unable to open the file." << std::endl;
		return 1; // Return an error code
	}

	
	std::string line;
	bool foundHeader = false;

	while (std::getline(inputFile, line))
	{
		if (line == "[posData]") {
			std::cout << "Header found" << std::endl;
			foundHeader = true;
		}
		else if (foundHeader && !line.empty())
		{
			std::istringstream sts(line);
			std::string chunk;

			sts >> chunk; // Read the first token
			if (chunk == "[~$USER~]")
			{
				std::cout << chunk;

				User* newUser = new User;
				std::cout << "User found" << std::endl;

				sts >> chunk; // Read username
				newUser->setUsername(chunk);
				std::cout << chunk;

				sts >> chunk; // Read password
				newUser->setPassword(chunk);
				std::cout << chunk;

				sts >> chunk; // Read privilege
				newUser->setUserPrivilege(chunk == "true" ? 0 : 1);
				this->userList.push_back(newUser);
				std::cout << chunk;

			}
			if (chunk == "[~$FOOD~]")
			{

				std::cout << chunk << " ";
				Food newFood;
				//get name
				sts >> chunk;
				newFood.setName(chunk);

				//Price
				sts >> chunk;
				newFood.setPrice(std::stoi(chunk));
				std::cout << std::stoi(chunk) << " ";

				//Stock
				sts >> chunk;
				newFood.setStock(std::stoi(chunk));
				std::cout << std::stoi(chunk) << " ";

				//Type
				sts >> chunk;
				newFood.setFoodType(std::stoi(chunk));
				std::cout << std::stoi(chunk) << " ";

				this->foodList.push_back(newFood);


			}
			if (chunk == "[~$TILL~]")
			{
				Till till;
				sts >> chunk;
				if (till.setBalance(std::stoi(chunk)))
				 std::cout << "Balance: " << chunk << std::endl;
				else
					std::cout << "no balance" << std::endl;

			}
		}
	}

	inputFile.close();



	//DEBUG MODE
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





