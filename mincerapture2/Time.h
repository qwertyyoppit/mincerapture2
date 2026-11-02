#pragma once

class Time
{
private:
	static inline double _realTime = 0.f;
	static inline float _realDelta{};
	static inline float _time = 0.f;
	static inline float _delta{};
public:
	static inline const double& realTime = _realTime;
	static inline const float& realDelta = _realDelta;
	static inline const float& time = _time;
	static inline const float& delta = _delta;
	static inline float timeScale = 1.0f;
	static inline float maximumDelta = 0.3f;

	static void Update();
};