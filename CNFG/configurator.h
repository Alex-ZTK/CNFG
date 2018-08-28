#pragma once
//Connecting libraries to work with ini-files.
#include <iostream>
#include <filesystem>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

struct Configuration
{
private:
	std::string app_dir = "\\WIMP\\";		//Application directory.
	std::string file_path = "config.ini";	//Default file name.
	//Configuration default parameters.
	int screen_resolution_x = 640;			//Default screen size.
	int screen_resolution_y = 480;
public:
	Configuration();
	void set_file_path(std::string path) { file_path = path; };
	void set_screen_resolution_x(int x);
	void set_screen_resolution_y(int y);
	std::string get_file_path() const { return file_path; };
	int get_screen_resolution_x() const { return screen_resolution_x; };
	int get_screen_resolution_y() const { return screen_resolution_y; };
};

Configuration::Configuration()
{
	//Generate full file path.
	file_path = getenv("LOCALAPPDATA") + app_dir + "config.ini";
	LOG(info, "Set location of configuration file: " + file_path);
}

void Configuration::set_screen_resolution_x(int x)
{
	if (x > 640) screen_resolution_x = x;
	else LOG(error, "Invalid resolution.");
}

void Configuration::set_screen_resolution_y(int y)
{
	if (y > 480) screen_resolution_y = y;
	else LOG(error, "Invalid resolution.");
}



struct IConfigurationManager 
{
public:
	Configuration Load();
	void Save(const Configuration& configuration);
};

//Load configurasion file.
Configuration IConfigurationManager::Load()
{
	LOG(info, "Loading configuration.");
	Configuration configuration;
	try
	{
		//Check the existence of the file.
		if (std::experimental::filesystem::exists(configuration.get_file_path()))
		{
			//If the file exists, then reading data.
			boost::property_tree::ptree file_config;
			boost::property_tree::ini_parser::read_ini(configuration.get_file_path(), file_config);
			//Reading parameters.
			configuration.set_screen_resolution_x(boost::lexical_cast<int>(file_config.get<std::string>("SCREEN.Resolution_X")));
			configuration.set_screen_resolution_y(boost::lexical_cast<int>(file_config.get<std::string>("SCREEN.Resolution_Y")));
		}
		else
		{
			//If the file does not exist.
			std::string error_message = "File " + configuration.get_file_path() + " not found!";
			throw error_message;
		}
	}
	catch (const std::string error_message)
	{
		LOG(error, error_message);
	}
	catch (...)
	{
		LOG(error, "Invalid data in " + configuration.get_file_path() + ". The default settings will be set.");
	}
	return configuration;
}

//Save configurasion file.
void IConfigurationManager::Save(const Configuration& configuration)
{
	LOG(info, "Saving configuration.");
	std::cout << "Saving " << configuration.get_file_path() << "..." << std::endl;
	try
	{
		boost::property_tree::ptree file_config;
		file_config.add<std::string>("SCREEN.Resolution_X", boost::lexical_cast<std::string>(configuration.get_screen_resolution_x()));
		file_config.add<std::string>("SCREEN.Resolution_Y", boost::lexical_cast<std::string>(configuration.get_screen_resolution_y()));
		file_config.swap(file_config);
		boost::property_tree::write_ini(configuration.get_file_path(), file_config);
	}
	catch (...)
	{
		LOG(error, "Unable to save " + configuration.get_file_path() + ".");
	}
}