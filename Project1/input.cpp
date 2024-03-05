#include "pch.h"
#include "input.h"
#include "defines.h"
#include <iostream>

using namespace std;
Input::Input() {
	inputs["S"] = 0;				// pitchUp
	inputs["Z"] = 0;				// pitchDown
	inputs["Q"] = 0;				// yawLeft
	inputs["D"] = 0;				// yawRight
	inputs["A"] = 0;				// rollLeft
	inputs["E"] = 0;				// rollRight
	inputs["UP"] = 0;				// throttleUp
	inputs["DOWN"] = 0;				// throttleDown
	inputs["LEFT"] = 0;				// Gun
	inputs["RIGHT"] = 0;			// Missile
}

Input::~Input() {}

void Input::keyState(const char& key, const string& index) {
	if (GetAsyncKeyState(key) & 0x8000) {
		switch (inputs[index]) {
		case 0: // Down
			inputs[index] = 1;
			break;
		case 1: // Hold
			inputs[index] = 2;
			break;
		default:
			inputs[index] = 2;
			break;
		}
	}
	else if (inputs[index] != 0 && inputs[index] != 3) { // Up
		inputs[index] = 3;
	}
	else {
		inputs[index] = 0;
	}
}

void Input::Store() {
	keyState('Z', pitchDown);
	keyState('S', pitchUp);
	keyState('Q', yawLeft);
	keyState('D', yawRight);
	keyState('A', rollLeft);
	keyState('E', rollRight);
	keyState(VK_UP, ARROW_UP);
	keyState(VK_DOWN, ARROW_DOWN);
	keyState(VK_LEFT, ARROW_LEFT);
	keyState(VK_RIGHT, ARROW_RIGHT);
}

#pragma region getKeys Methods
bool Input::getKeyUp(const string& c) {
	auto iterator = inputs.find(c);
	int value = iterator->second;

	return (value == KEY_UP) ? true : false;
}

bool Input::getKeyDown(const string& c) {
	auto iterator = inputs.find(c);
	int value = iterator->second;

	return (value == KEY_DOWN) ? true : false;
}

bool Input::getKey(const string& c) {
	auto iterator = inputs.find(c);
	int value = iterator->second;

	return (value == KEY_DOWN || value == KEY_HOLD) ? true : false;
}
#pragma endregion