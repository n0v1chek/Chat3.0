#include "SHA1.h"


uint32_t SHA1::left_rotate(uint32_t n, int d) const{
    return (n << d) | (n >> (32 - d));
}

uint32_t SHA1::sha1_k(int i) const {
    if (i >= 0 && i <= 19) {
        return 0x5A827999;
    }
    else if (i >= 20 && i <= 39) {
        return 0x6ED9EBA1;
    }
    else if (i >= 40 && i <= 59) {
        return 0x8F1BBCDC;
    }
    else if (i >= 60 && i <= 79) {
        return 0xCA62C1D6;
    }
}

std::size_t SHA1::operator()(const std::string& input) const {
    std::string patched_input;
    uint64_t bit_length = input.length() * 8;
    patched_input = input + char(0x80);
    while ((patched_input.length() * 8) % 512 != 448) {
        patched_input += char(0x00);
    }
    patched_input += std::string(8, char(0x00));
    std::memcpy(&patched_input[patched_input.length() - 8], &bit_length, sizeof(bit_length));

    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xEFCDAB89;
    uint32_t h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xC3D2E1F0;

    for (int i = 0; i < patched_input.length(); i += 64) {
        const uint8_t* block = reinterpret_cast<const uint8_t*>(&patched_input[i]);
        uint32_t w[80];
        for (int j = 0; j < 16; ++j) {
            w[j] = (block[j * 4 + 0] << 24) |
                (block[j * 4 + 1] << 16) |
                (block[j * 4 + 2] << 8) |
                (block[j * 4 + 3]);
        }
        for (int j = 16; j < 80; ++j) {
            w[j] = left_rotate(w[j - 3] ^ w[j - 8] ^ w[j - 14] ^ w[j - 16], 1);
        }
        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t e = h4;
        for (int j = 0; j < 80; ++j) {
            uint32_t f;
            if (j >= 0 && j <= 19) {
                f = (b & c) | ((~b) & d);
            }
            else if (j >= 20 && j <= 39) {
                f = b ^ c ^ d;
            }
            else if (j >= 40 && j <= 59) {
                f = (b & c) | (b & d) | (c & d);
            }
            else if (j >= 60 && j <= 79) {
                f = b ^ c ^ d;
            }
            uint32_t temp = left_rotate(a, 5) + f + e + sha1_k(j) + w[j];
            e = d;
            d = c;
            c = left_rotate(b, 30);
            b = a;
            a = temp;
        }
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
    }

    std::stringstream result;
    result << std::hex << std::setfill('0');
    result << std::setw(8) << h0;
    result << std::setw(8) << h1;
    result << std::setw(8) << h2;
    result << std::setw(8) << h3;
    result << std::setw(8) << h4;
    
    size_t _result;
    result >> _result;
 
    return _result;
  
    
}