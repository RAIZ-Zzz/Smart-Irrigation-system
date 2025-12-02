#ifndef MY_WIFI_H  
#define MY_WIFI_H
/*
Directly use the function "ifi_init(const char *SSID, const char *PASSWORD)" to initiate wifi connection:
example:
#define SSID "SSID of your wifi"
#define PASSWORD "PASSWORD of our wifi"

void app_main(void)
{
    wifi_init(SSID, PASSWORD);
    
}
Then esp32 will connect to wifi successfully.

*/
void wifi_init(const char *SSID, const char *PASSWORD);

#endif