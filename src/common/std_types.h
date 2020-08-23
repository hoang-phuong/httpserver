#ifndef STD_TYPES_H
#define STD_TYPES_H

typedef char                  int8;         /**< -128..127                                 */
typedef short                 int16;        /**< -32768..32767                             */
typedef long                  int32;        /**< -2147483648..2147483647                   */
typedef long long             int64;        /**< -9223372036854775808..9223372036854775807 */
typedef unsigned char         uint8;        /**<  0..255                                   */
typedef unsigned short        uint16;       /**<  0..65535                                 */
typedef unsigned long         uint32;       /**<  0..4294967295                            */
typedef unsigned long long    uint64        /**<  0..18446744073709551615                  */

typedef uint32                boolean;

#define TRUE                  1UL
#define FALSE                 0UL

#endif /* #ifndef STD_TYPES_H */