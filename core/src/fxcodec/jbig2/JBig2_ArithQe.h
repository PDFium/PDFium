// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef _JBIG2_ARITH_QE_H_
#define _JBIG2_ARITH_QE_H_
typedef struct {
    unsigned int Qe;
    unsigned int NMPS;
    unsigned int NLPS;
    unsigned int nSwitch;
} JBig2ArithQe;
const JBig2ArithQe QeTable[] = {
    { 0x5601,  1,  1, 1 },
    { 0x3401,  2,  6, 0 },
    { 0x1801,  3,  9, 0 },
    { 0x0AC1,  4, 12, 0 },
    { 0x0521,  5, 29, 0 },
    { 0x0221, 38, 33, 0 },
    { 0x5601,  7,  6, 1 },
    { 0x5401,  8, 14, 0 },
    { 0x4801,  9, 14, 0 },
    { 0x3801, 10, 14, 0 },
    { 0x3001, 11, 17, 0 },
    { 0x2401, 12, 18, 0 },
    { 0x1C01, 13, 20, 0 },
    { 0x1601, 29, 21, 0 },
    { 0x5601, 15, 14, 1 },
    { 0x5401, 16, 14, 0 },
    { 0x5101, 17, 15, 0 },
    { 0x4801, 18, 16, 0 },
    { 0x3801, 19, 17, 0 },
    { 0x3401, 20, 18, 0 },
    { 0x3001, 21, 19, 0 },
    { 0x2801, 22, 19, 0 },
    { 0x2401, 23, 20, 0 },
    { 0x2201, 24, 21, 0 },
    { 0x1C01, 25, 22, 0 },
    { 0x1801, 26, 23, 0 },
    { 0x1601, 27, 24, 0 },
    { 0x1401, 28, 25, 0 },
    { 0x1201, 29, 26, 0 },
    { 0x1101, 30, 27, 0 },
    { 0x0AC1, 31, 28, 0 },
    { 0x09C1, 32, 29, 0 },
    { 0x08A1, 33, 30, 0 },
    { 0x0521, 34, 31, 0 },
    { 0x0441, 35, 32, 0 },
    { 0x02A1, 36, 33, 0 },
    { 0x0221, 37, 34, 0 },
    { 0x0141, 38, 35, 0 },
    { 0x0111, 39, 36, 0 },
    { 0x0085, 40, 37, 0 },
    { 0x0049, 41, 38, 0 },
    { 0x0025, 42, 39, 0 },
    { 0x0015, 43, 40, 0 },
    { 0x0009, 44, 41, 0 },
    { 0x0005, 45, 42, 0 },
    { 0x0001, 45, 43, 0 },
    { 0x5601, 46, 46, 0 }
};
#endif
