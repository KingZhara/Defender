#pragma once
#include <type_traits>
#include <iostream>

template<typename T> requires(std::is_arithmetic_v<T>)
class Timer
{
public:
	bool preIncrement;
	T time;
	const T BASE;

public:
	Timer(T BASE_, bool preIncrement_ = true) : BASE(BASE_), preIncrement(preIncrement_), time(BASE_) {}

	bool tick(T deltatime)
	{
		if (preIncrement)
			time += deltatime;

		if (time >= BASE)
		{
			time -= BASE;

			return true;
		}

		if (!preIncrement)
			time += deltatime;

		return false;
	}

	/**
	 *
	 * @warning Only useful if using post-increment
	 * 
	 * @return If the timer has completed
	 */
	bool isComplete() const
	{
		return time >= BASE;
	}

	T getBase()
	{
		return BASE;
	}

	void addTime(T add_)
	{
		//std::cout << time << ' ' << add_ << '\n';
		time -= add_;
	}
};