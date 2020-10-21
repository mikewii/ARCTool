#include "utils.h"

void Utils::bswap16(u16& x){ // taken from byteswap.h for compatibility
    x =((u16) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)));
}

void Utils::bswap32(u32& x){ // taken from byteswap.h for compatibility
    x =
    ((((x) & 0xff000000u) >> 24) | (((x) & 0x00ff0000u) >> 8) \
    | (((x) & 0x0000ff00u) << 8) | (((x) & 0x000000ffu) << 24));
}
