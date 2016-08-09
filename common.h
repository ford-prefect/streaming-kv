#ifndef COMMON_H
#define COMMON_H

#include <vector>

#include <stdint.h>

using byte = unsigned char;
using ByteVector = std::vector<byte>;

struct MessageHeader {
  uint8_t version; /* == 1, for extensibility */
  uint32_t key_size;
  uint32_t value_size;
} __attribute((packed));

#endif /* COMMON_H */
