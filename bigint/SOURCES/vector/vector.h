#ifndef BIGINT_VECTOR
#define BIGINT_VECTOR

#include <cassert> //for nothing
#include <memory> //smart pointer
#include <cstring> // for memcpy in gcc

struct big_obj {
	size_t capacity;
	std::shared_ptr<uint32_t> data;
	big_obj() : capacity(0), data(nullptr) {}
	big_obj(uint32_t *p, size_t new_capacity) : 
		capacity(new_capacity),
		data (std::shared_ptr<uint32_t>(p, std::default_delete<uint32_t[]>())  ) {}
};

struct vector {
	vector();
	vector(size_t size, uint32_t x = 0);
	vector(vector const &v);
	vector &operator=(vector const & rhs);
	//vector(vector && rhs);
	//vector& operator=(vector && rhs);
	vector(const std::initializer_list<uint32_t> &other);
	~vector();
	void push_back(uint32_t value);
	void pop_back();
	std::size_t size() const;
	uint32_t &operator[](size_t ind);
	const uint32_t &operator[](size_t ind) const;
	uint32_t &back();
	const uint32_t &back() const;
	void clear();
	void resize(std::size_t new_size, uint32_t value = 0);
	friend void swap(vector & a, vector &b);
private:
	size_t _size;
	bool is_big;
	union container {
		uint32_t small;
		big_obj big;
		container() : small(0) {} ///same shit
		~container() {}; ///function was implicitly deleted because 'vector::container' has a variant data member 'vector::container::big' with a non-trivial default constructor
	} data;
	void detach();
	void increase_capasity();
};
void swap(vector & a, vector &b);

#endif //BIGINT_VECTOR