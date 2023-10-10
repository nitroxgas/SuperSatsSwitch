#ifndef _WMANAGER_H
#define _WMANAGER_H
#ifndef RESET_PIN
 #define RESET_PIN 4
#endif
void init_WifiManager();
void wifiManagerProcess();
void reset_configuration();

#endif // _WMANAGER_H
