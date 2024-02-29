#pragma once
#include <Windows.h>
#include <map>
#include <string>
using namespace std;
class Input
{
private:

	char				m_cInput;
	map<string, bool>	inputs;
	

public:
	
						Input(); // Constructor
						~Input();
	map<string, bool>	Get();
	void				Store();

};

