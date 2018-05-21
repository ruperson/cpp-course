#include "big_integer.h"
#include <string>
#include <algorithm>
#include <functional>
#include <assert.h>


const std::vector <int32_t> POW10 = { 1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000 };
const ui MAX10 = std::numeric_limits<ui>::digits10;
const ui DIGIT_MASK = std::numeric_limits<ui>().max();
const ull BASE = static_cast<ull>(DIGIT_MASK) + 1;
const ui BITS_IN_BASE = std::numeric_limits<ui>::digits;

big_integer abs(big_integer a) {
    a.sign = false;
    return a;
}

//void swap(big_integer &a, big_integer &b) {
//	std::swap(a.digits, b.digits);
//	std::swap(a.sign, b.sign);
//}

void big_integer::erase_zeros() {
    while (digits.size() > 1 && digits.back() == 0)
        digits.pop_back();
    if (digits.size() == 1 && digits.back() == 0)
        sign = false;
}
void big_integer::to_twos_compliment() {
    if (!sign) return;
    for (auto &x : digits) x = ~x;
    sign = false;
    *this += 1;
}
void big_integer::to_sign_magnitude() {
    if (sign) return;
    *this -= 1;
    for (auto &x : digits) x = ~x;
    sign = true;
}

big_integer::~big_integer() = default;
big_integer::big_integer(big_integer const& other) = default;
big_integer& big_integer::operator=(big_integer const& other) = default;

big_integer::big_integer() :
        digits({0}), sign(false) {
}
big_integer::big_integer(int32_t a) {
    sign = (a < 0);
    digits.push_back(static_cast<ui>(std::abs(a)));
}
big_integer::big_integer(ui a) {
    sign = false;
    digits.push_back(a);
}

big_integer::big_integer(std::string const& str) {
    for (size_t i = (str[0] == '-') ? 1 : 0; i < str.size(); i += MAX10) {
        if (i + MAX10 < str.size()) {
            *this *= POW10.back();
            *this += std::stoi(str.substr(i, MAX10));
        }
        else {
            *this *= POW10[ str.size() - i];
            *this += std::stoi(str.substr(i, str.size() - i));
        }
    }
    sign = (str[0] == '-');
}


big_integer& big_integer::operator+=(big_integer const& rhs) {
    bool carry = 0;
    if (sign == rhs.sign) {
        sign = false;

        for (size_t i = 0; i < std::max(digits.size(), rhs.digits.size()) || carry; ++i) {
            if (i == digits.size())
                digits.push_back(0);
            ull res = 1ull * digits[i] + carry + (i < rhs.digits.size() ? rhs.digits[i] : 0);
            carry = res >= BASE;
            if (carry)  res -= BASE;
            digits[i] = static_cast <ui>(res);
        }
        return *this;
    }
    else if (sign) {
        return *this -= -rhs;
    }
    else {
        *this = -*this;
        *this -= rhs;
        return *this = -*this;
    }


}
big_integer& big_integer::operator-=(big_integer const& rhs) {
    if (!rhs.sign) {
        return *this += -rhs;
    }

    if (!sign) {
        *this = (-*this);
        *this += rhs;
        return *this = (-*this);
    }
    else if (*this > rhs) {
        big_integer res = rhs;
        res -= *this;
        return *this = -res;
    }
    else {
        bool carry = 0;
        for (size_t i = 0; i < rhs.digits.size() || carry; ++i) {
            ull res = static_cast<ull>(digits[i]);
            ull subs = 1ull * carry + (i < rhs.digits.size() ? rhs.digits[i] : 0);
            carry = (res < subs);
            if (carry)  res += (BASE - subs);
            else res -= subs;
            digits[i] = static_cast <ui>(res);
        }
        erase_zeros();
        return *this;
    }

}
big_integer& big_integer::operator*=(big_integer const& rhs) {

    bool final_sign = (sign != rhs.sign);
    if (rhs.digits.size() == 1) {
        *this *= rhs.digits[0];
        sign = final_sign;
        return *this;
    }



    big_integer res;
    res.digits.resize(digits.size() + rhs.digits.size());

    for (size_t i = 0; i < digits.size(); ++i) {
        for (size_t j = 0, carry = 0; j < rhs.digits.size() || carry; ++j) {
            ull cur = res.digits[i + j] + digits[i] * 1ull * (j < rhs.digits.size() ? rhs.digits[j] : 0) + carry;
            res.digits[i + j] = cur & DIGIT_MASK;
            carry = cur >> BITS_IN_BASE;
        }
    }

    res.sign = final_sign;
    res.erase_zeros();
    return *this = res;
}
big_integer &big_integer::operator/=(big_integer const &rh) {
    big_integer rhs = rh;
    if (*this == 0 || abs(*this) < abs(rhs)) return *this  = 0;
    bool final_sign = (sign != rhs.sign);
    sign = rhs.sign = false;
    if (rhs.digits.size() == 1) {
        *this /= rhs.digits[0];
        sign = final_sign;
        return *this;
    }
    ui normalization = static_cast<ui>(BASE / (rhs.digits.back() + 1));
    *this *= normalization;
    rhs *= normalization;
    big_integer result;
    result.digits.resize(digits.size() - rhs.digits.size() + 1);
    big_integer remainder;
    remainder.digits.assign(digits.end() - rhs.digits.size() + 1, digits.end());

    for (size_t i = result.digits.size() - 1; i < result.digits.size(); --i) {
        remainder <<= BITS_IN_BASE;
        remainder += digits[i];
        ull rem_highest = remainder.digits.back();
        if (remainder.digits.size() > rhs.digits.size()) {
            rem_highest <<= BITS_IN_BASE;
            rem_highest += remainder.digits[remainder.digits.size() - 2];
        }

        ui quotient = rem_highest / rhs.digits.back();				//quotient selection step
        remainder -= rhs * quotient;
        while (remainder < 0) {
            quotient--;
            remainder += rhs;
        }
        result.digits[i] = quotient;
    }
    result.erase_zeros();
    result.sign = final_sign;
    return *this = result;
}
big_integer& big_integer::operator%=(big_integer const& rhs) {
    return *this = *this - (*this / rhs) * rhs;
}

big_integer& big_integer::operator*=(int32_t a) {

    sign = sign != (a < 0);
    if (a < 0)  a = -a;
    return *this *= static_cast<ui> (a);
}
big_integer& big_integer::operator*=(ui a) {
    ui carry = 0;
    for (auto &digit : digits) {
        ull cur = 1ull * digit * a + carry;
        digit = cur & DIGIT_MASK;
        carry = cur >> BITS_IN_BASE;
    }
    digits.push_back(carry);
    erase_zeros();
    return *this;
}
big_integer& big_integer::operator/=(int32_t a) {

    sign = sign != (a < 0);
    if (a < 0)  a = -a;
    return *this /= static_cast<ui> (a);
}
big_integer& big_integer::operator/=(ui a) {
    ui carry = 0;
    for (size_t i = digits.size() - 1; i < digits.size(); --i) {
        ull cur = (static_cast<ull>(carry) << BITS_IN_BASE) + digits[i];
        carry = static_cast<ui>(cur % a);
        digits[i] = static_cast<ui>(cur / a);
    }
    erase_zeros();
    return *this;
}

template<typename T>
big_integer &big_integer::logical_operation(big_integer const &rhs, T const &op) {
    big_integer rh = rhs;
    bool lwn = false, rwn = false;
    if (rhs.sign) {
        rh.to_twos_compliment();
        rwn = true;
    }
    if (sign) {
        to_twos_compliment();
        lwn = true;
    }
    size_t num = std::max(digits.size(), rhs.digits.size());
    for (size_t i = 0; i < num; ++i) {
        if (i == digits.size()) digits.push_back(lwn ? DIGIT_MASK : 0);
        digits[i] = op(digits[i], i < rh.digits.size() ? rh.digits[i] : rwn ? DIGIT_MASK : 0);
    }
    if (op(lwn , rwn))
        to_sign_magnitude();
    return *this;
}

big_integer& big_integer::operator&=(big_integer const& rhs) {
    return logical_operation(rhs, std::bit_and<>());
}
big_integer& big_integer::operator|=(big_integer const& rhs) {
    return logical_operation(rhs, std::bit_or<>());
}
big_integer& big_integer::operator^=(big_integer const& rhs) {
    return logical_operation(rhs, std::bit_xor<>());
}




big_integer& big_integer::operator<<=(ui rhs) {
    while (rhs >= BITS_IN_BASE) {
        digits.insert(digits.begin(), 0);
        rhs -= BITS_IN_BASE;
    }
    return *this *= (1U << rhs);
}
big_integer& big_integer::operator>>=(ui rhs) {
    ui cnt = 0;
    while (rhs >= BITS_IN_BASE) {
        ++cnt;
        digits.pop_back();
        rhs -= BITS_IN_BASE;
    }
    if (!sign) return *this /= (1U << rhs);
    else {
        to_twos_compliment();

        ui tmp_mask = (1U << rhs) - 1;
        digits.insert(digits.begin(), cnt, DIGIT_MASK);
        for (size_t i = 0; i < digits.size(); ++i) {
            if (i) {
                ui to_add = digits[i] & tmp_mask;
                digits[i - 1] |= to_add << (BITS_IN_BASE - rhs);
            }
            digits[i] >>= rhs;
        }
        ui lul = (tmp_mask << (BITS_IN_BASE - rhs));
        digits.back() |= lul;

        to_sign_magnitude();

        return *this;
    }
}

big_integer operator+(big_integer a, big_integer const &b) {
    a += b;
    return a;
}
big_integer operator-(big_integer a, big_integer const &b) {
    a -= b;
    return a;
}
big_integer operator*(big_integer a, big_integer const &b)
{
    a *= b;
    return a;
}
big_integer operator/(big_integer a, big_integer const &b)
{
    a /= b;
    return a;
}
big_integer operator%(big_integer a, big_integer const &b)
{
    a %= b;
    return a;
}
big_integer operator&(big_integer a, big_integer const& b) {
    return a &= b;
}
big_integer operator|(big_integer a, big_integer const& b) {
    return a |= b;
}
big_integer operator^(big_integer a, big_integer const& b) {
    return a ^= b;
}

big_integer operator<<(big_integer a, ui b) {
    a <<= b;
    return a;
}
big_integer operator>>(big_integer a, ui b) {
    a >>= b;
    return a;
}

big_integer big_integer::operator+() const {
    return *this;
}
big_integer big_integer::operator-() const {
    big_integer res = *this;
    if (res != 0)
        res.sign = !res.sign;
    return res;
}
big_integer big_integer::operator~() const {
    return -*this - 1;
}

bool operator<(big_integer const &a, big_integer const &b)
{
    if (a.sign != b.sign) return a.sign > b.sign;
    if (a.digits.size() != b.digits.size())
        return a.sign ? a.digits.size() > b.digits.size() : a.digits.size() < b.digits.size();

    for (size_t i = a.digits.size() - 1; i < a.digits.size(); --i) {
        if (a.digits[i] != b.digits[i])
            return a.sign ? a.digits[i] > b.digits[i] : a.digits[i] < b.digits[i];
    }
    return false; //equal lol
}
bool operator==(big_integer const& a, big_integer const& b) { return !(a < b) && !(b < a); }
bool operator!=(big_integer const& a, big_integer const& b) { return !(a == b); }
bool operator>(big_integer const& a, big_integer const& b) { return b < a; }
bool operator<=(big_integer const& a, big_integer const& b) { return !(b < a); }
bool operator>=(big_integer const& a, big_integer const& b) { return !(a < b); }

std::string to_string(big_integer a) {
    a.erase_zeros();
    std::string ans;
    while (abs(a) >= POW10[MAX10]) {
        std::string block = std::to_string((a % POW10[MAX10]).digits[0]);
        std::reverse(block.begin(), block.end());
        while (block.size() < MAX10) block.push_back('0');
        ans.append(block);
        a /= POW10[MAX10];
    }
    std::string last = std::to_string(a.digits[0]);
    std::reverse(last.begin(), last.end());
    ans.append(last);
    if (a.sign) ans.push_back('-');
    std::reverse(ans.begin(), ans.end());
    return ans;
}
std::ostream& operator<<(std::ostream& s, big_integer const& a)
{
    return s << to_string(a);
}
