These files work as-is in the Arduino IDE as follows:
    make a directory with a name like wifi_AP or whatever you like
    copy these files into that directory, and rename main.cpp wifi_AP.ino or the name you chose
    all the other cpp and header files compile as-is
    
    delete the file platformio.ini 

This code works with my Android app "simple_wifi-android". The default IP for the wifi_AP is 192.168.4.1 which you will have to enter in that app in the "scan ips" page
