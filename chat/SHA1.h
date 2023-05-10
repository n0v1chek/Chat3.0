#pragma once

#include <string>
#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>

class SHA1 {
private:
	uint32_t left_rotate(uint32_t n, int d) const;
	uint32_t sha1_k(int i) const;
public:
	SHA1() = default;
	std::size_t operator()(const std::string& message) const;
};