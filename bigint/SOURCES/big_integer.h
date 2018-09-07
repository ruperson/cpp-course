#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <string>
#include <algorithm> //std::max
#include <functional> //bitwise template
#include <limits> //numeric_limits
#ifdef HW3
	#include "vector/vector.h"
#else
	#include <vector> 
	using vector = std::vector <uint32_t>;
#endif
typedef uint32_t ui;
typedef uint64_t ull;
const vector POW10 = { 1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000 };
const ui MAX10 = std::numeric_limits<ui>::digits10;
const ui DIGIT_MASK = std::numeric_limits<ui>().max();
const ull BASE = static_cast<ull>(DIGIT_MASK) + 1;
const ui BITS_IN_BASE = std::numeric_limits<ui>::digits;
struct big_integer {
private:
	vector digits;
	bool sign;
	void erase_zeros();
	void to_sign_magnitude();
	void to_twos_compliment();
	template<typename T>
	big_integer &logical_operation(const big_integer &rhs, T const &op);

public:
	big_integer();
	big_integer(big_integer const& other);
	big_integer(int32_t a);
	big_integer(ui a);
	explicit big_integer(std::string const& str);

	~big_integer();
	big_integer& operator=(big_integer const& other);

	big_integer& operator+=(big_integer const& rhs);
	big_integer& operator-=(big_integer const& rhs);
	big_integer& operator*=(big_integer const& rhs);
	big_integer& operator/=(big_integer const& rhs);
	big_integer& operator%=(big_integer const& rhs);

	big_integer& operator&=(big_integer const& rhs);
	big_integer& operator|=(big_integer const& rhs);
	big_integer& operator^=(big_integer const& rhs);

	big_integer& operator<<=(ui rhs);
	big_integer& operator>>=(ui rhs);



	big_integer operator+() const;
	big_integer operator-() const;
	big_integer operator~() const;

	//friend void swap(big_integer & a, big_integer &b);
	friend big_integer abs(big_integer a);

	friend bool operator==(big_integer const& a, big_integer const& b);
	friend bool operator!=(big_integer const& a, big_integer const& b);
	friend bool operator<(big_integer const& a, big_integer const& b);
	friend bool operator>(big_integer const& a, big_integer const& b);
	friend bool operator<=(big_integer const& a, big_integer const& b);
	friend bool operator>=(big_integer const& a, big_integer const& b);

	friend std::string to_string(big_integer a);



	big_integer& operator/=(ui rhs);
	big_integer& operator/=(int32_t rhs);
	big_integer& operator*=(ui rhs);
	big_integer& operator*=(int32_t rhs);
};


//void swap(big_integer & a, big_integer &b);
big_integer abs(big_integer a);

big_integer operator+(big_integer a, big_integer const &b);
big_integer operator-(big_integer a, big_integer const &b);
big_integer operator*(big_integer a, big_integer const &b);
big_integer operator/(big_integer a, big_integer const &b);
big_integer operator%(big_integer a, big_integer const& b);

big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);

big_integer operator<<(big_integer a, ui b);
big_integer operator>>(big_integer a, ui b);

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);

std::string to_string(big_integer a);
std::ostream& operator<<(std::ostream& s, big_integer const& a);




#endif // BIG_INTEGER_H
