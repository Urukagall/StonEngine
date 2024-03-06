#include "pch.h"
#include "Timer.h"
#include <Windows.h>

Timer::Timer() {
	// initialise timer and shit
	fSystemTime = timeGetTime();
	fPreviousSystemTime = fSystemTime;
	fGameTime = 0;
	fDeltaTime = 0;
	fSpeed = 1;
	fPreviousSpeed = fSpeed;
}

// System time in ms, so divide it to get seconds
float Timer::Get() {
	return fGameTime * 0.001f;
}

float Timer::GetDT()const {
	return fDeltaTime; //* 0.001f;
}

void Timer::Reset() {
	fGameTime = 0;
}

// used to pause the game, but stores the previous speed, so the game can be resumed using Resume()
void Timer::Stop() {
	fPreviousSpeed = fSpeed;
	this->SetSpeed(0);
}
// Resume game at the same speed it was running when Stop() was called
void Timer::Resume() {
	this->SetSpeed(fPreviousSpeed);
}

//used to speed up or down the game
void Timer::SetSpeed(float speed) { 
	fSpeed = speed;
}

// Timer update
void Timer::Tick() {
	fDeltaTime = 0;

	while (fDeltaTime < fMinDelta) {
		fSystemTime = timeGetTime();

		fDeltaTime = (fSystemTime - fPreviousSystemTime) * fSpeed;
	}
	if (fDeltaTime > fMaxDelta) {
		fDeltaTime = fMaxDelta;
	}

	fGameTime += fDeltaTime; //* fSpeed;

	fPreviousSystemTime = fSystemTime;
	//OutputDebugStringA("\n");
	//OutputDebugStringA(std::to_string(fDeltaTime).c_str());
}

Timer::~Timer() {
	//do nothing
}