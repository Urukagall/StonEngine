#pragma once
class Timer
{
public:
	Timer();
	float Get();
	float GetDT();
	void Reset();
	void Stop();
	void Resume();
	void SetSpeed(float speed);
	void Tick();
	~Timer();
private:
	float fSystemTime;
	float fPreviousSystemTime = 0.0f;
	float fGameTime;
	float fDeltaTime;

	const float maxFps = 60;
	const float fMinDelta = 1000/maxFps; // minimum frame time in s
	const float fMaxDelta = 50; // maximum time in s

	float fSpeed; // time multiplier
	float fPreviousSpeed;
};