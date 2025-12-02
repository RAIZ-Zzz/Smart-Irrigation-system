#include <stdio.h>
#include <my_wifi.h>

#define SSID "NIRVANA"
#define PASSWORD "NIRVANAFOREVER"

void app_main(void)
{
    wifi_init(SSID, PASSWORD);
}
