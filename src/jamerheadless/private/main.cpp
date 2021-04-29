
#include <array>
#include <asio.h>
#include <asiodrivers.h>
#include <iostream>
#include <vector>
#include <cpputils/logger.hpp>

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
		
	const auto driver_name_count = asioDrivers->getDriverNames(driver_names.data(), static_cast<int>(driver_names.size()));

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
	/**
	 *
	 *	LOAD ASIO DRIVERS
	 * 
	 */
	
	// Create an empty driver interface
	asioDrivers = new AsioDrivers();

	// Iterate over every driver
	for (const auto& driver : get_driver_names())
	{
		// Load desired driver
		if (driver == "Focusrite USB ASIO") {
			loadAsioDriver("Focusrite USB ASIO");
			LOG_INFO("Load asio driver : %s", driver.c_str());
		}
	}

	/**
	 *
	 *	INIT ASIO DRIVERS
	 *
	 */
	
	// Init selected driver
	ASIODriverInfo driver_infos;
	memset(&driver_infos, 0, sizeof driver_infos);
    if (ASIOInit(&driver_infos) != ASE_OK)
    {
        asioDrivers->removeCurrentDriver();
		LOG_FATAL("The audio driver could not be initialized.");
    }
	LOG_INFO("Initialized asio driver :\n"
		"\t- asio version = %ld\n"
		"\t- driver version = %lu\n"
		"\t- driver name = %s",
		driver_infos.asioVersion,
		driver_infos.driverVersion,
		driver_infos.name
	);

	long input_latency = 0, output_latency = 0;
	if (ASIOGetLatencies(&input_latency, &output_latency) == ASE_NotPresent) LOG_FATAL("Failed to get asio driver latency");
	LOG_INFO("Asio driver latency : input=%ld / output=%ld", input_latency, output_latency);

	long input_channels = 0, output_channels = 0;
	if (ASIOGetChannels(&input_channels, &output_channels) == ASE_NotPresent) LOG_FATAL("Failed to get asio driver channel number");
	LOG_INFO("Asio driver channel count : input=%ld / output=%ld", input_channels, output_channels);
	
	ASIOSampleRate sample_rate;
	if (ASIOGetSampleRate(&sample_rate) == ASE_NotPresent) LOG_FATAL("Failed to get asio driver sample rate");
	LOG_INFO("Asio driver sample rate : %lf", sample_rate);

	long buffer_min_size = 0, buffer_max_size = 0, buffer_preferred_size = 0, buffer_granularity = 0;
	if (ASIOGetBufferSize(&buffer_min_size, &buffer_max_size, &buffer_preferred_size, &buffer_granularity) == ASE_NotPresent)  LOG_FATAL("Failed to get asio driver buffer size");
	LOG_INFO("Asio driver buffer infos :\n"
		"\t- size = [%ld - %ld]\n"
		"\t- preferred size = %ld\n"
		"\t- buffer granularity = %ld",
		buffer_min_size,
		buffer_max_size,
		buffer_preferred_size,
		buffer_granularity
	);
	
	
	ASIOBufferInfo buffer_info;
	memset(&buffer_info, 0, sizeof buffer_info);
	
	//ASIOCreateBuffers(&buffer_info, )
	

	LOG_INFO("done");
	
	ASIOExit();
	asioDrivers->removeCurrentDriver();
}