#include "base64.h"
#include <algorithm>

static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

static const std::string base64_chars_url =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789-_";

static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, size_t in_len, bool url) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    
    const std::string& chars = url ? base64_chars_url : base64_chars;

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i <4) ; i++)
                ret += chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for(j = 0; j < i; j++)
            char_array_3[j] = char_array_3[j];

        char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

        for (j = 0; (j < i + 1); j++)
            ret += chars[char_array_4[j]];

        if (!url) {
            while((i++ < 3))
                ret += '=';
        }
    }

    return ret;
}

std::string base64_encode(std::string const& s, bool url) {
    return base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length(), url);
}

std::string base64_encode_pem(std::string const& s) {
    return base64_encode(s, false);
}

std::string base64_encode_mime(std::string const& s) {
    return base64_encode(s, false);
}

std::string base64_decode(std::string const& encoded_string, bool remove_linebreaks) {
    (void)remove_linebreaks; // Suppress unused parameter warning
    size_t in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && ( encoded_string[in] != '=') && is_base64(encoded_string[in])) {
        char_array_4[i++] = encoded_string[in]; in++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]) & 0xff;

            char_array_3[0] = ( char_array_4[0] << 2       ) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) +   char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = 0; j < i; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]) & 0xff;

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}

#if __cplusplus >= 201703L
std::string base64_encode(std::string_view s, bool url) {
    return base64_encode(reinterpret_cast<const unsigned char*>(s.data()), s.length(), url);
}

std::string base64_encode_pem(std::string_view s) {
    return base64_encode(s, false);
}

std::string base64_encode_mime(std::string_view s) {
    return base64_encode(s, false);
}

std::string base64_decode(std::string_view s, bool remove_linebreaks) {
    return base64_decode(std::string(s), remove_linebreaks);
}
#endif 