# Moxa-UC8100-Debian-8-modbus-poll-
simple modbus poll for serial port 




How to for MOXA UC 8100 

STEP 1
Update package list
sudo apt update
STEP 2 
Install compiler and build tools
sudo apt install build-essential
STEP3 
Install Modbus library
sudo apt install libmodbus-dev
STEP 4
Check where the Modbus header is
ls /usr/include/modbus/modbus.h
ls /usr/include/modbus.h
STEP 5
In your code, include the correct header
If the file is in /usr/include/modbus/modbus.h use #include <modbus/modbus.h>
If the file is in /usr/include/modbus.h use #include <modbus.h>
STEP 6
Compile your program
g++ -o modbus_poll modbus_poll.cpp -lmodbus
STEP 7 
Run your program
./modbus_poll


example :

admin@UC8100:~$ sudo /usr/local/bin/modbus_poll /dev/ttyM1 9600 8 N 1 51  40084 1 1000 H
Reg 40084: 0xb1 (177)
