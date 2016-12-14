#include "stdint.h"
#include "ESP8266HTTPClient.h"
#include "TeliaWifiClient.h"

TeliaWifiClient::TeliaWifiClient(void) {
    http.setReuse(true);
}

int32_t TeliaWifiClient::connect(String user, String password) {
    // Do HTTP GET to initialize a session
    http.begin(initUrl);
    int initCode = http.GET();
    if(initCode != HTTP_CODE_OK) {
        // Serial.printf("[Telia wifi] Init failed, error: %d\n", initCode);
        String payload = http.getString();
        // Serial.println(payload);
        return(initFailed);
    }
    // Serial.println("[Telia wifi] Init OK");
    http.end();

    // Do HTTP POST with the login credentials
    http.begin(loginUrl);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String loginString = "username=" + user + "&password=" + password;
    int loginCode = http.POST(loginString);
    if(loginCode != HTTP_CODE_OK) {
        // Serial.printf("[Telia wifi] Login failed, error: %d\n", loginCode);
        String payload = http.getString();
        // Serial.println(payload);
        return(loginFailed);
    }
    String payload = http.getString();
    // Serial.println(payload);
    // Serial.println("[Telia wifi] Login OK");

    http.end();

    return loginSuccess;
}

TeliaWifiClient TeliaWifi;