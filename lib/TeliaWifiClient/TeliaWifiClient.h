#ifndef __teliawificlient_h__
#define __teliawificlient_h__

#include "ESP8266HTTPClient.h"

class TeliaWifiClient {
	private:
		const String initUrl = "http://login.homerun.telia.com/sd/init";
		const String loginUrl = "http://login.homerun.telia.com/sd/login";

		HTTPClient http;

	public:
		TeliaWifiClient(void);
		
		int32_t connect(String user, String password);

		const int initFailed = -1;
		const int loginFailed = -2;
		const int loginSuccess = 0;
};

extern TeliaWifiClient TeliaWifi;

#endif