#ifndef CONFIG_H
# define CONFIG_H

#include <stdint.h>

typedef uint8_t fxes_byte;
typedef uint16_t fxes_word;
typedef uint32_t fxes_dword;
typedef uint64_t fxes_qword;

#define FXES_CODE_SIZE ((2 << 16) << 4)
#define FXES_DATA_SIZE ((2 << 16) << 8)

#endif // CONFIG_H

