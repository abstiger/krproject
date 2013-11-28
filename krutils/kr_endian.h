#ifndef __KR_ENDIAN_H__
#define __KR_ENDIAN_H__

#include "kr_macros.h"

#ifdef HAVE_ENDIAN_H
#include <endian.h>
#else
# define __LITTLE_ENDIAN 1234
# define __BIG_ENDIAN    4321
# define __PDP_ENDIAN    3412

# ifdef WORDS_BIGENDIAN
#  define __BYTE_ORDER __BIG_ENDIAN
# else
#  define __BYTE_ORDER __LITTLE_ENDIAN
# endif
#endif

#ifdef defined HAVE_BYTESWAP_H 
#include <byteswap.h>
#else
static inline unsigned short bswap_16 (unsigned short __x)
{
    return (__x >> 8) | (__x << 8);
}

static inline unsigned int bswap_32 (unsigned int __x)
{
    return (bswap_16 (__x & 0xffff) << 16) | (bswap_16 (__x >> 16));
}

static inline unsigned long long bswap_64 (unsigned long long __x)
{
    return (((unsigned long long) bswap_32 (__x & 0xffffffffull)) << 32) | (bswap_32 (__x >> 32));
}
#endif

#include <endian.h>

# if __BYTE_ORDER == __LITTLE_ENDIAN
#  define kr_htobe16(x) bswap_16 (x)
#  define kr_htole16(x) (x)
#  define kr_be16toh(x) bswap_16 (x)
#  define kr_le16toh(x) (x)

#  define kr_htobe32(x) bswap_32 (x)
#  define kr_htole32(x) (x)
#  define kr_be32toh(x) bswap_32 (x)
#  define kr_le32toh(x) (x)

#  define kr_htobe64(x) bswap_64 (x)
#  define kr_htole64(x) (x)
#  define kr_be64toh(x) bswap_64 (x)
#  define kr_le64toh(x) (x)
# else
#  define kr_htobe16(x) (x)
#  define kr_htole16(x) bswap_16 (x)
#  define kr_be16toh(x) (x)
#  define kr_le16toh(x) bswap_16 (x)

#  define kr_htobe32(x) (x)
#  define kr_htole32(x) bswap_32 (x)
#  define kr_be32toh(x) (x)
#  define kr_le32toh(x) bswap_32 (x)

#  define kr_htobe64(x) (x)
#  define kr_htole64(x) bswap_64 (x)
#  define kr_be64toh(x) (x)
#  define kr_le64toh(x) bswap_64 (x)
# endif
#endif

#endif /* __KR_ENDIAN_H__ */
