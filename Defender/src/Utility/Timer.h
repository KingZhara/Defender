#pragma once
#include <type_traits>
#include <iostream>

template<typename T> requires(std::is_arithmetic_v<T>)
class Timer
{
private:
	T time;
	const T BASE;

public:
	explicit Timer(T BASE_) : BASE(BASE_), time(BASE_) {}

	bool tick(T deltatime)
	{
		// Increment the time
		time += deltatime;

		// Reset if needed
		if (time >= BASE)
		{
			time = 0;
			return true;
		}

		return false;
	}
};