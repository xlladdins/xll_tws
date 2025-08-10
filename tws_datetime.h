// tws_date_time.h - TWS date and time foibles
#pragma once
#include <ctime>
#include <string>

namespace tws {

	// Convert time_t to a localtime string using format.
	inline std::string formatTime(time_t t, const char* format)
	{
		if (t % 1000 == 0) {
			t /= 1000; // Convert milliseconds to seconds
		}
		struct tm timeinfo;
		char buffer[20] = { 0 };
		errno_t err = localtime_s(&timeinfo, &t);
		if (err == 0) {
			strftime(buffer, sizeof(buffer), format, &timeinfo);
		}
		else {
			err = strerror_s(buffer, sizeof(buffer), err);
		}

		return std::string(buffer);
	}
	inline const char* const YMDHMS = "%Y%m%d %H:%M:%S";
	inline const char* const YMD = "%Y%m%d";
	inline std::string DateTime(time_t t) 
	{ 
		return formatTime(t, YMDHMS); 
	}
	inline std::string Date(time_t t) 
	{ 
		return formatTime(t, YMD); 
	}

}