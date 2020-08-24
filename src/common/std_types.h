#ifndef STDTYPES_H
#define STDTYPES_H

typedef char                  sint8;        /**< -128..127                                 */
typedef short                 sint16;       /**< -32768..32767                             */
typedef long                  sint32;       /**< -2147483648..2147483647                   */
typedef long long             sint64;       /**< -9223372036854775808..9223372036854775807 */
typedef unsigned char         uint8;        /**<  0..255                                   */
typedef unsigned short        uint16;       /**<  0..65535                                 */
typedef unsigned long         uint32;       /**<  0..4294967295                            */
typedef unsigned long long    uint64;       /**<  0..18446744073709551615                  */

typedef uint32                boolean;

#define TRUE                  1UL
#define FALSE                 0UL

#define NULL_PTR              (void*)0

#endif /* #ifndef STDTYPES_H */