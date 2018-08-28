#include "stdafx.h"
#include <iostream>
#include <boost/di.hpp>

#include "logging.h"
#include "configurator.h"

int main()
{
	//Initializing the logging.
	Logger_Initialization();
	LOG(info, "Starting programm.");

	//Create configuration.
	Configuration user_config;
	//Create configuration manager.
	IConfigurationManager config_manager;
	//Load configuration.
	user_config = config_manager.Load();
	//Save configuration.
	config_manager.Save(user_config);
	std::cout << "Done!" << std::endl;
	LOG(info, "End programm.");
    return 0;
}

