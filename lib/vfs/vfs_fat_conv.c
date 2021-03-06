/*
 * Copyright (c) 2009, 2011, ETH Zurich.
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * ETH Zurich D-INFK, Haldeneggsteig 4, CH-8092 Zurich. Attn: Systems Group.
 */

#include <stdio.h>
#include <string.h>
#include "vfs_fat_conv.h"

// map cp1252 to utf8, translating illegal chars to '?'
char dos2unix[256][4] = {
    { 0x3f, 0 },              // 0x0
    { 0x3f, 0 },              // 0x1
    { 0x3f, 0 },              // 0x2
    { 0x3f, 0 },              // 0x3
    { 0x3f, 0 },              // 0x4
    { 0x3f, 0 },              // 0x5
    { 0x3f, 0 },              // 0x6
    { 0x3f, 0 },              // 0x7
    { 0x3f, 0 },              // 0x8
    { 0x3f, 0 },              // 0x9
    { 0x3f, 0 },              // 0xa
    { 0x3f, 0 },              // 0xb
    { 0x3f, 0 },              // 0xc
    { 0x3f, 0 },              // 0xd
    { 0x3f, 0 },              // 0xe
    { 0x3f, 0 },              // 0xf
    { 0x3f, 0 },              // 0x10
    { 0x3f, 0 },              // 0x11
    { 0x3f, 0 },              // 0x12
    { 0x3f, 0 },              // 0x13
    { 0x3f, 0 },              // 0x14
    { 0x3f, 0 },              // 0x15
    { 0x3f, 0 },              // 0x16
    { 0x3f, 0 },              // 0x17
    { 0x3f, 0 },              // 0x18
    { 0x3f, 0 },              // 0x19
    { 0x3f, 0 },              // 0x1a
    { 0x3f, 0 },              // 0x1b
    { 0x3f, 0 },              // 0x1c
    { 0x3f, 0 },              // 0x1d
    { 0x3f, 0 },              // 0x1e
    { 0x3f, 0 },              // 0x1f
    { 0x20, 0 },              // 0x20
    { 0x21, 0 },              // 0x21
    { 0x22, 0 },              // 0x22
    { 0x23, 0 },              // 0x23
    { 0x24, 0 },              // 0x24
    { 0x25, 0 },              // 0x25
    { 0x26, 0 },              // 0x26
    { 0x27, 0 },              // 0x27
    { 0x28, 0 },              // 0x28
    { 0x29, 0 },              // 0x29
    { 0x2a, 0 },              // 0x2a
    { 0x2b, 0 },              // 0x2b
    { 0x2c, 0 },              // 0x2c
    { 0x2d, 0 },              // 0x2d
    { 0x2e, 0 },              // 0x2e
    { 0x2f, 0 },              // 0x2f
    { 0x30, 0 },              // 0x30
    { 0x31, 0 },              // 0x31
    { 0x32, 0 },              // 0x32
    { 0x33, 0 },              // 0x33
    { 0x34, 0 },              // 0x34
    { 0x35, 0 },              // 0x35
    { 0x36, 0 },              // 0x36
    { 0x37, 0 },              // 0x37
    { 0x38, 0 },              // 0x38
    { 0x39, 0 },              // 0x39
    { 0x3a, 0 },              // 0x3a
    { 0x3b, 0 },              // 0x3b
    { 0x3c, 0 },              // 0x3c
    { 0x3d, 0 },              // 0x3d
    { 0x3e, 0 },              // 0x3e
    { 0x3f, 0 },              // 0x3f
    { 0x40, 0 },              // 0x40
    { 0x41, 0 },              // 0x41
    { 0x42, 0 },              // 0x42
    { 0x43, 0 },              // 0x43
    { 0x44, 0 },              // 0x44
    { 0x45, 0 },              // 0x45
    { 0x46, 0 },              // 0x46
    { 0x47, 0 },              // 0x47
    { 0x48, 0 },              // 0x48
    { 0x49, 0 },              // 0x49
    { 0x4a, 0 },              // 0x4a
    { 0x4b, 0 },              // 0x4b
    { 0x4c, 0 },              // 0x4c
    { 0x4d, 0 },              // 0x4d
    { 0x4e, 0 },              // 0x4e
    { 0x4f, 0 },              // 0x4f
    { 0x50, 0 },              // 0x50
    { 0x51, 0 },              // 0x51
    { 0x52, 0 },              // 0x52
    { 0x53, 0 },              // 0x53
    { 0x54, 0 },              // 0x54
    { 0x55, 0 },              // 0x55
    { 0x56, 0 },              // 0x56
    { 0x57, 0 },              // 0x57
    { 0x58, 0 },              // 0x58
    { 0x59, 0 },              // 0x59
    { 0x5a, 0 },              // 0x5a
    { 0x5b, 0 },              // 0x5b
    { 0x5c, 0 },              // 0x5c
    { 0x5d, 0 },              // 0x5d
    { 0x5e, 0 },              // 0x5e
    { 0x5f, 0 },              // 0x5f
    { 0x60, 0 },              // 0x60
    { 0x61, 0 },              // 0x61
    { 0x62, 0 },              // 0x62
    { 0x63, 0 },              // 0x63
    { 0x64, 0 },              // 0x64
    { 0x65, 0 },              // 0x65
    { 0x66, 0 },              // 0x66
    { 0x67, 0 },              // 0x67
    { 0x68, 0 },              // 0x68
    { 0x69, 0 },              // 0x69
    { 0x6a, 0 },              // 0x6a
    { 0x6b, 0 },              // 0x6b
    { 0x6c, 0 },              // 0x6c
    { 0x6d, 0 },              // 0x6d
    { 0x6e, 0 },              // 0x6e
    { 0x6f, 0 },              // 0x6f
    { 0x70, 0 },              // 0x70
    { 0x71, 0 },              // 0x71
    { 0x72, 0 },              // 0x72
    { 0x73, 0 },              // 0x73
    { 0x74, 0 },              // 0x74
    { 0x75, 0 },              // 0x75
    { 0x76, 0 },              // 0x76
    { 0x77, 0 },              // 0x77
    { 0x78, 0 },              // 0x78
    { 0x79, 0 },              // 0x79
    { 0x7a, 0 },              // 0x7a
    { 0x7b, 0 },              // 0x7b
    { 0x7c, 0 },              // 0x7c
    { 0x7d, 0 },              // 0x7d
    { 0x7e, 0 },              // 0x7e
    { 0x3f, 0 },              // 0x7f
    { 0xe2, 0x82, 0xac, 0 },  // 0x80
    { 0x3f, 0 },              // 0x81
    { 0xe2, 0x80, 0x9a, 0 },  // 0x82
    { 0xc6, 0x92, 0 },        // 0x83
    { 0xe2, 0x80, 0x9e, 0 },  // 0x84
    { 0xe2, 0x80, 0xa6, 0 },  // 0x85
    { 0xe2, 0x80, 0xa0, 0 },  // 0x86
    { 0xe2, 0x80, 0xa1, 0 },  // 0x87
    { 0xcb, 0x86, 0 },        // 0x88
    { 0xe2, 0x80, 0xb0, 0 },  // 0x89
    { 0xc5, 0xa0, 0 },        // 0x8a
    { 0xe2, 0x80, 0xb9, 0 },  // 0x8b
    { 0xc5, 0x92, 0 },        // 0x8c
    { 0x3f, 0 },              // 0x8d
    { 0xc5, 0xbd, 0 },        // 0x8e
    { 0x3f, 0 },              // 0x8f
    { 0x3f, 0 },              // 0x90
    { 0xe2, 0x80, 0x98, 0 },  // 0x91
    { 0xe2, 0x80, 0x99, 0 },  // 0x92
    { 0xe2, 0x80, 0x9c, 0 },  // 0x93
    { 0xe2, 0x80, 0x9d, 0 },  // 0x94
    { 0xe2, 0x80, 0xa2, 0 },  // 0x95
    { 0xe2, 0x80, 0x93, 0 },  // 0x96
    { 0xe2, 0x80, 0x94, 0 },  // 0x97
    { 0xcb, 0x9c, 0 },        // 0x98
    { 0xe2, 0x84, 0xa2, 0 },  // 0x99
    { 0xc5, 0xa1, 0 },        // 0x9a
    { 0xe2, 0x80, 0xba, 0 },  // 0x9b
    { 0xc5, 0x93, 0 },        // 0x9c
    { 0x3f, 0 },              // 0x9d
    { 0xc5, 0xbe, 0 },        // 0x9e
    { 0xc5, 0xb8, 0 },        // 0x9f
    { 0xc2, 0xa0, 0 },        // 0xa0
    { 0xc2, 0xa1, 0 },        // 0xa1
    { 0xc2, 0xa2, 0 },        // 0xa2
    { 0xc2, 0xa3, 0 },        // 0xa3
    { 0xc2, 0xa4, 0 },        // 0xa4
    { 0xc2, 0xa5, 0 },        // 0xa5
    { 0xc2, 0xa6, 0 },        // 0xa6
    { 0xc2, 0xa7, 0 },        // 0xa7
    { 0xc2, 0xa8, 0 },        // 0xa8
    { 0xc2, 0xa9, 0 },        // 0xa9
    { 0xc2, 0xaa, 0 },        // 0xaa
    { 0xc2, 0xab, 0 },        // 0xab
    { 0xc2, 0xac, 0 },        // 0xac
    { 0xc2, 0xad, 0 },        // 0xad
    { 0xc2, 0xae, 0 },        // 0xae
    { 0xc2, 0xaf, 0 },        // 0xaf
    { 0xc2, 0xb0, 0 },        // 0xb0
    { 0xc2, 0xb1, 0 },        // 0xb1
    { 0xc2, 0xb2, 0 },        // 0xb2
    { 0xc2, 0xb3, 0 },        // 0xb3
    { 0xc2, 0xb4, 0 },        // 0xb4
    { 0xc2, 0xb5, 0 },        // 0xb5
    { 0xc2, 0xb6, 0 },        // 0xb6
    { 0xc2, 0xb7, 0 },        // 0xb7
    { 0xc2, 0xb8, 0 },        // 0xb8
    { 0xc2, 0xb9, 0 },        // 0xb9
    { 0xc2, 0xba, 0 },        // 0xba
    { 0xc2, 0xbb, 0 },        // 0xbb
    { 0xc2, 0xbc, 0 },        // 0xbc
    { 0xc2, 0xbd, 0 },        // 0xbd
    { 0xc2, 0xbe, 0 },        // 0xbe
    { 0xc2, 0xbf, 0 },        // 0xbf
    { 0xc3, 0x80, 0 },        // 0xc0
    { 0xc3, 0x81, 0 },        // 0xc1
    { 0xc3, 0x82, 0 },        // 0xc2
    { 0xc3, 0x83, 0 },        // 0xc3
    { 0xc3, 0x84, 0 },        // 0xc4
    { 0xc3, 0x85, 0 },        // 0xc5
    { 0xc3, 0x86, 0 },        // 0xc6
    { 0xc3, 0x87, 0 },        // 0xc7
    { 0xc3, 0x88, 0 },        // 0xc8
    { 0xc3, 0x89, 0 },        // 0xc9
    { 0xc3, 0x8a, 0 },        // 0xca
    { 0xc3, 0x8b, 0 },        // 0xcb
    { 0xc3, 0x8c, 0 },        // 0xcc
    { 0xc3, 0x8d, 0 },        // 0xcd
    { 0xc3, 0x8e, 0 },        // 0xce
    { 0xc3, 0x8f, 0 },        // 0xcf
    { 0xc3, 0x90, 0 },        // 0xd0
    { 0xc3, 0x91, 0 },        // 0xd1
    { 0xc3, 0x92, 0 },        // 0xd2
    { 0xc3, 0x93, 0 },        // 0xd3
    { 0xc3, 0x94, 0 },        // 0xd4
    { 0xc3, 0x95, 0 },        // 0xd5
    { 0xc3, 0x96, 0 },        // 0xd6
    { 0xc3, 0x97, 0 },        // 0xd7
    { 0xc3, 0x98, 0 },        // 0xd8
    { 0xc3, 0x99, 0 },        // 0xd9
    { 0xc3, 0x9a, 0 },        // 0xda
    { 0xc3, 0x9b, 0 },        // 0xdb
    { 0xc3, 0x9c, 0 },        // 0xdc
    { 0xc3, 0x9d, 0 },        // 0xdd
    { 0xc3, 0x9e, 0 },        // 0xde
    { 0xc3, 0x9f, 0 },        // 0xdf
    { 0xc3, 0xa0, 0 },        // 0xe0
    { 0xc3, 0xa1, 0 },        // 0xe1
    { 0xc3, 0xa2, 0 },        // 0xe2
    { 0xc3, 0xa3, 0 },        // 0xe3
    { 0xc3, 0xa4, 0 },        // 0xe4
    { 0xc3, 0xa5, 0 },        // 0xe5
    { 0xc3, 0xa6, 0 },        // 0xe6
    { 0xc3, 0xa7, 0 },        // 0xe7
    { 0xc3, 0xa8, 0 },        // 0xe8
    { 0xc3, 0xa9, 0 },        // 0xe9
    { 0xc3, 0xaa, 0 },        // 0xea
    { 0xc3, 0xab, 0 },        // 0xeb
    { 0xc3, 0xac, 0 },        // 0xec
    { 0xc3, 0xad, 0 },        // 0xed
    { 0xc3, 0xae, 0 },        // 0xee
    { 0xc3, 0xaf, 0 },        // 0xef
    { 0xc3, 0xb0, 0 },        // 0xf0
    { 0xc3, 0xb1, 0 },        // 0xf1
    { 0xc3, 0xb2, 0 },        // 0xf2
    { 0xc3, 0xb3, 0 },        // 0xf3
    { 0xc3, 0xb4, 0 },        // 0xf4
    { 0xc3, 0xb5, 0 },        // 0xf5
    { 0xc3, 0xb6, 0 },        // 0xf6
    { 0xc3, 0xb7, 0 },        // 0xf7
    { 0xc3, 0xb8, 0 },        // 0xf8
    { 0xc3, 0xb9, 0 },        // 0xf9
    { 0xc3, 0xba, 0 },        // 0xfa
    { 0xc3, 0xbb, 0 },        // 0xfb
    { 0xc3, 0xbc, 0 },        // 0xfc
    { 0xc3, 0xbd, 0 },        // 0xfd
    { 0xc3, 0xbe, 0 },        // 0xfe
};

/*
 * Convert 8.3 style dos file name to sane, utf-8 encoded representation.
 * Assumes 8.3 file names to be encoded in cp1252.
 *
 * @arg dosfn input: short filename 8 name, 3 ext, \0 byte
 * @arg buf target buffer: short filename in utf8
 * @arg buflen buffer length including terminating 0
 * @return 1 if target buffer too small, else 0
 */
int
dos2unixfn(const unsigned char *dosfn, unsigned char *buf, size_t buflen)
{
    size_t len = 0;
    int fnpadding, extpadding;
    // search for padding indices
    for (fnpadding = 8; fnpadding > 0 && dosfn[fnpadding-1] == ' '; fnpadding--);
    for (extpadding = 11; extpadding > 8 && dosfn[extpadding-1] == ' '; extpadding--);

    // calculate output length
    for (int i = 0; i < 12; i++) {
        // jump over name padding
        if (fnpadding < 8 && i == fnpadding) {
            i = 8;
        }
        // ignore ext padding
        if (i == extpadding) {
            break;
        }
        unsigned char c = dosfn[i];
        // handle deletion marker in dosfn
        if (i == 0 && c == 0x05) {
            c = 0xe5;
        }
        len += strlen(dos2unix[c]);
    }

    // add space for additional characters
    if (extpadding > 8) {
        len += 1; // '.' between name and ext
    }
    len += 1; // 0 byte at end

    if (len > buflen) {
        return 1;
    }
    //printf("utf8 len = %zd\n", len);

    int bufindex = 0;
    // copy name
    for (int i = 0; i < fnpadding; i++) {
        unsigned char c = dosfn[i];
        // handle deletion marker in dosfn
        if (i == 0 && c == 0x05) {
            c = 0xe5;
        }
        int charlen = strlen(dos2unix[c]);
        memcpy(buf+bufindex, dos2unix[c], charlen);
        bufindex += charlen;
    }

    // if dosfn has extension add '.' and extension to output buffer
    if (extpadding > 8) {
        // insert point
        buf[bufindex++] = '.';
        // copy extension
        for (int i = 8; i < extpadding; i++) {
            unsigned char c = dosfn[i];
            int charlen = strlen(dos2unix[c]);
            memcpy(buf+bufindex, dos2unix[c], charlen);
            bufindex += charlen;
        }
    }

    // add terminating NUL byte
    buf[bufindex] = 0;

    return 0;
}
