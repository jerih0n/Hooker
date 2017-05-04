# Hooker
A low-level global case sensitive hook build in C++ and using Windows API 
# Important!!! #
This software is generally a **malware**. Spying other people and using it for a malicious purpose is **1) extremely immoral** and **2) illegal in most of the countries. You SHOULD not use this code, part of it or modified version of it for any illegal purposes!** If you for some reason, decide to ignore this warning, you **MUST** know, that I **should not be kept responsible** for the consequences of your actions. Everything you do with this software is at your own risk!
## Short Description ##
This is a low-level window global hook. This means that the program will fire an event every time a key from the keyboard is pressed, and it's able to make the difference between upper and lower case chars
On start the program will add new key in windows registers and in the future it will start on windows reboot automatically
The application is creating a .txt file where the logged information is stored( I will not add logic for sendin the collected information over the network)
## Note ##
If you run the .exe file, your computer will be infected. In order to remove the malware, you need to remove the register from HKEY_LOCAL_MACHINE(if the windows account is Administrator) or in HKEY_CURRENT_USER (in all other cases)
