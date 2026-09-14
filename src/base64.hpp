#pragma once

#include <string>

namespace base64 {

static const char kEncodeLookup[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char kPadCharacter = '=';

inline std::string encode(const std::string& input) {
    std::string encoded;
    encoded.reserve(((input.length() / 3) + (input.length() % 3 > 0)) * 4);
    int temp = 0;
    for (size_t i = 0; i < input.length() / 3; ++i) {
        temp  = (*(input.data() + i * 3) & 0xFF) << 16;
        temp += (*(input.data() + i * 3 + 1) & 0xFF) << 8;
        temp += (*(input.data() + i * 3 + 2) & 0xFF);
        encoded.append(1, kEncodeLookup[(temp & 0x00FC0000) >> 18]);
        encoded.append(1, kEncodeLookup[(temp & 0x0003F000) >> 12]);
        encoded.append(1, kEncodeLookup[(temp & 0x00000FC0) >> 6]);
        encoded.append(1, kEncodeLookup[(temp & 0x0000003F)]);
    }
    switch (input.length() % 3) {
    case 1:
        temp = (*(input.data() + input.length() - 1) & 0xFF) << 16;
        encoded.append(1, kEncodeLookup[(temp & 0x00FC0000) >> 18]);
        encoded.append(1, kEncodeLookup[(temp & 0x0003F000) >> 12]);
        encoded.append(2, kPadCharacter);
        break;
    case 2:
        temp  = (*(input.data() + input.length() - 2) & 0xFF) << 16;
        temp += (*(input.data() + input.length() - 1) & 0xFF) << 8;
        encoded.append(1, kEncodeLookup[(temp & 0x00FC0000) >> 18]);
        encoded.append(1, kEncodeLookup[(temp & 0x0003F000) >> 12]);
        encoded.append(1, kEncodeLookup[(temp & 0x00000FC0) >> 6]);
        encoded.append(1, kPadCharacter);
        break;
    }
    return encoded;
}

inline std::string decode(const std::string& input) {
    if (input.empty()) return "";
    std::string decoded;
    decoded.reserve(input.length() / 4 * 3);
    auto getVal = [](char c) -> int {
        if (c >= 'A' && c <= 'Z') return c - 'A';
        if (c >= 'a' && c <= 'z') return c - 'a' + 26;
        if (c >= '0' && c <= '9') return c - '0' + 52;
        if (c == '+') return 62;
        if (c == '/') return 63;
        return -1;
    };
    int temp = 0;
    for (size_t i = 0; i < input.length(); i += 4) {
        int v0 = getVal(input[i]);
        if (v0 < 0) break; // Reached padding or invalid char
        int v1 = i + 1 < input.length() ? getVal(input[i+1]) : -1;
        int v2 = i + 2 < input.length() ? getVal(input[i+2]) : -1;
        int v3 = i + 3 < input.length() ? getVal(input[i+3]) : -1;
        if (v1 < 0) break;
        temp = (v0 << 18) + (v1 << 12);
        if (v2 >= 0) temp += (v2 << 6);
        if (v3 >= 0) temp += v3;
        decoded.append(1, (char)((temp & 0x00FF0000) >> 16));
        if (v2 >= 0) decoded.append(1, (char)((temp & 0x0000FF00) >> 8));
        if (v3 >= 0) decoded.append(1, (char)(temp & 0x000000FF));
    }
    return decoded;
}

}
