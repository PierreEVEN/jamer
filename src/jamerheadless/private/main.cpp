
#include <array>
#include <asio.h>
#include <asiodrivers.h>
#include <iostream>
#include <vector>

extern AsioDrivers* asioDrivers;
bool   loadAsioDriver(char* name);

constexpr uint32_t MAX_DRIVER_COUNT = 64;

std::vector<std::string> get_driver_names()
{

	std::array<char*, MAX_DRIVER_COUNT> driver_names;
	for (int i = 0; i < MAX_DRIVER_COUNT; ++i)
	{
		driver_names[i] = static_cast<char*>(std::calloc(256, sizeof(char)));
	}
		
	const auto driver_name_count = asioDrivers->getDriverNames(driver_names.data(), driver_names.size());

	std::vector<std::string> names;
	names.resize(driver_name_count);
	for (int i = 0; i < driver_name_count; ++i)
	{
		names[i] = driver_names[i];
	}

	for (int i = 0; i < MAX_DRIVER_COUNT; ++i)
	{
		std::free(driver_names[i]);
	}

	return names;
}



int main(int argc, char* argv[])
{
	// Create an empty driver interface
	asioDrivers = new AsioDrivers();

	
	// Iterate over every driver
	for (const auto& driver : get_driver_names())
	{
		// Load desired driver
		if (driver == "Focusrite USB ASIO") loadAsioDriver("Focusrite USB ASIO");
		std::cout << "found drivers : " << driver << std::endl;
	}	


	// Init selected driver
	ASIODriverInfo driver_infos;
	memset(&driver_infos, 0, sizeof driver_infos);
    if (ASIOInit(&driver_infos) != ASE_OK)
    {
        asioDrivers->removeCurrentDriver();
        std::cout << "The audio driver could not be initialized." << std::endl;
    }

	std::cout << "done" << std::endl;


	
	ASIOExit();
	asioDrivers->removeCurrentDriver();
}