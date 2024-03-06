#pragma once
#include <map>
#include <string>
using namespace std;

class Input
{
private:
	map<string, int>	inputs;
	

public:
	
						Input(); // Constructor
						~Input();
	bool				getKeyUp(const string& c);
	bool				getKeyDown(const string& c);
	bool				getKey(const string& c);
	void				keyState(const char& key, const string& index);
	void				Store();

};

