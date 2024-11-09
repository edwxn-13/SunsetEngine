#pragma once

class Time
{
	float currentTime;
	float delta_time;
	float pastTime;
	float tickrate;
public:
	Time();
	float DeltaTime();
	void updateTime();
	bool fixedUpdateChecker();
};
