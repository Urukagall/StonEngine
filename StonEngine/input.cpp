#include "input.h"
#include <iostream>

using namespace std;
Input::Input() {
	inputs["pitchUp"] = false;
	inputs["pitchDown"] = false;
	inputs["yawLeft"] = false;
	inputs["yawRight"] = false;
	inputs["rollLeft"] = false;
	inputs["rollRight"] = false;
	inputs["throttleUp"] = false;
	inputs["throttleDown"] = false;
	inputs["Gun"] = false;
	inputs["Missile"] = false;
}

Input::~Input() {}

void Input::Store() {
	(GetAsyncKeyState('Z') & 0x8000) ? inputs["pitchUp"] = true : inputs["pitchUp"] = false;					// KEY Z			-	pitch UP
	(GetAsyncKeyState('S') & 0x8000) ? inputs["pitchDown"] = true : inputs["pitchDown"] = false;				// KEY S			-	pitch DOWN 
	(GetAsyncKeyState('Q') & 0x8000) ? inputs["yawLeft"] = true : inputs["yawLeft"] = false;					// KEY Q			-	yaw LEFT
	(GetAsyncKeyState('D') & 0x8000) ? inputs["yawRight"] = true : inputs["yawRight"] = false;					// KEY D			-	yaw RIGHT
	(GetAsyncKeyState('A') & 0x8000) ? inputs["rollLeft"] = true : inputs["rollLeft"] = false;					// KEY A			-	roll LEFT
	(GetAsyncKeyState('E') & 0x8000) ? inputs["rollRight"] = true : inputs["rollRight"] = false;				// KEY E			-	roll RIGHT
	(GetAsyncKeyState(VK_UP) & 0x8000) ? inputs["throttleUp"] = true : inputs["throttleUp"] = false;			// KEY ARROW_UP		-	throttle UP
	(GetAsyncKeyState(VK_DOWN) & 0x8000) ? inputs["throttleDown"] = true : inputs["throttleDown"] = false;		// KEY ARROW_DOWN	-	throttle DOWN
	(GetAsyncKeyState(VK_LEFT) & 0x8000) ? inputs["Gun"] = true : inputs["Gun"] = false;						// KEY ARROW_LEFT	-	Gun
	(GetAsyncKeyState(VK_RIGHT) & 0x8000) ? inputs["Missile"] = true : inputs["Missile"] = false;				// KEY ARROW_RIGHT	-	Missile
}

map<string, bool> Input::Get() {
	
	return inputs;
}
