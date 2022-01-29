#ifndef RAND_H_INCLUDED
#define RAND_H_INCLUDED

#include <random>

template<typename T>
class Rand {
public:
	Rand(T high, T low) :dist(low, high)
	{
		std::random_device r;
		re.seed(r());
	};
	T operator()() { return dist(re); };
	void seed(T s) { re.seed(s); }
private:
	std::default_random_engine re;
	std::uniform_int_distribution<T> dist;
};

#endif