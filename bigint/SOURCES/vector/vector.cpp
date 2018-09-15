#include "vector.h"

void swap(vector &a, vector &b) {
	std::swap(a.is_big, b.is_big);
	std::swap(a._size, b._size);
	const std::size_t us = sizeof(vector::data);
	bool tmp[us]; 
	std::memcpy(tmp, &a.data, us);
	std::memcpy(&a.data, &b.data, us);
	std::memcpy(&b.data, tmp, us);
}


vector::vector(): _size(0), is_big(false) { 
	data.small = 0; 
}

vector::vector(std::size_t new_size, uint32_t x): _size(new_size), is_big(new_size > 1) {
	if (is_big) {
		new(&data.big)big_obj(new uint32_t[_size], _size);
		memset(data.big.data.get(), x, _size);
	}
	else {
		data.small = x;
	}
}

vector::vector(const vector &v) : _size(v._size), is_big(v.is_big) {
	if (is_big) {
		new(&data.big)big_obj(v.data.big);
	}
	else {
		data.small = v.data.small;
	}
}
vector& vector::operator=(vector const & rh) {
	vector rhs(rh);
	swap(*this, rhs);
	return *this;
}
//vector::vector(vector && rhs) : vector() {
//	swap(*this, rhs);
//}
//vector& vector::operator=(vector && rhs) {
//	swap(*this, rhs);
//	return *this;
//}
vector::vector(const std::initializer_list<uint32_t> &list) : _size(list.size()), is_big(list.size() > 1) {
	if (is_big) {
		new(&data.big)big_obj(new uint32_t[_size], _size);
		std::memcpy(data.big.data.get(), list.begin(), _size * sizeof(uint32_t));
	}
	else {
		data.small = *list.begin();
	}
}
vector::~vector() {
	if (is_big) {
		data.big.~big_obj();
	}
}
void vector::push_back(uint32_t value) {
    detach();

    if (!is_big && _size == 0) { 
        data.small = value;      
    } else {
        if (!is_big || _size == data.big.capacity) {
            increase_capasity();
        }
        data.big.data.get()[_size] = value;
    }
	_size++;
}
void vector::pop_back() {
	_size--;
}

std::size_t vector::size() const {
    return _size;
}

uint32_t &vector::operator[](std::size_t ind) {
	if (is_big) {
		detach();
		return data.big.data.get()[ind];
	}
	else {
		return data.small;
	}

}
const uint32_t &vector::operator[](std::size_t ind) const {
	if (is_big) {
		return data.big.data.get()[ind];
	}
	else {
		return data.small;
	}
}
const uint32_t &vector::back() const {
    if (is_big) {
        return data.big.data.get()[_size - 1];
    } else {
        return data.small;
    }
}
uint32_t &vector::back() {
	if (is_big) {
		detach();
		return data.big.data.get()[_size - 1];
	}
	else {
		return data.small;
	}
}
void vector::clear() {
	_size = 0;
	is_big = false;
	data.small = 0;
}
void vector::resize(std::size_t new_size, uint32_t value) {
	detach();
	if (new_size < _size) {
		_size = new_size;
		return;
	}
	while (_size != new_size) {
		this->push_back(value);
	}
}
void vector::do_detach() {
	uint32_t *clone = new uint32_t[data.big.capacity];
	std::memcpy(clone, data.big.data.get(), data.big.capacity * sizeof(uint32_t));
	data.big.data = std::shared_ptr<uint32_t[]>(clone);
}
void vector::increase_capasity() {
	if (is_big) {
		uint32_t *clone = new uint32_t[2 * data.big.capacity];
		std::memcpy(clone, data.big.data.get(), data.big.capacity * sizeof(uint32_t));
		data.big.data = std::shared_ptr<uint32_t[]>(clone);
	}
	else {
		uint32_t clone = data.small;
		new(&data.big)big_obj(new uint32_t[2], 1);
		*data.big.data.get() = clone;
		is_big = true;
	}
	data.big.capacity *= 2;
}


