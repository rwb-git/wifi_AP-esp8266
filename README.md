These files work in platformio, and also work as-is in the Arduino IDE as follows:

make a directory with a name like wifi_AP or whatever you like
copy all the cpp and h files (skip platformio.ini) files into that directory, and rename main.cpp wifi_AP.ino or the name you chose
all the other cpp and header files compile as-is.

This code works with my Android app "simple_wifi-android". The default IP for the wifi_AP is 192.168.4.1 which you will have to enter in that app in the "scan ips" page
