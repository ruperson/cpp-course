#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <vector>
#include <cstddef>
#include <iosfwd>
#include <stdint.h>

typedef uint32_t ui;
typedef uint64_t ull;


struct big_integer {

private:
    std::vector<ui> digits;
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
