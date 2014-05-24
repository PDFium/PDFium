// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef _JBIG2_IMAGE_H_
#define _JBIG2_IMAGE_H_
#include "JBig2_Define.h"
#include "JBig2_Module.h"
typedef enum {
    JBIG2_COMPOSE_OR		= 0,
    JBIG2_COMPOSE_AND		= 1,
    JBIG2_COMPOSE_XOR		= 2,
    JBIG2_COMPOSE_XNOR		= 3,
    JBIG2_COMPOSE_REPLACE	= 4
} JBig2ComposeOp;
struct FX_RECT;
class CJBig2_Image : public CJBig2_Object
{
public:

    CJBig2_Image(FX_INT32 w, FX_INT32 h);

    CJBig2_Image(FX_INT32 w, FX_INT32 h, FX_INT32 stride, FX_BYTE*pBuf);

    CJBig2_Image(CJBig2_Image &im);

    ~CJBig2_Image();

    FX_BOOL getPixel(FX_INT32 x, FX_INT32 y);

    FX_INT32 setPixel(FX_INT32 x, FX_INT32 y, FX_BOOL v);

    void copyLine(FX_INT32 hTo, FX_INT32 hFrom);

    void fill(FX_BOOL v);

    FX_BOOL composeTo(CJBig2_Image *pDst, FX_INT32 x, FX_INT32 y, JBig2ComposeOp op);
    FX_BOOL composeTo(CJBig2_Image *pDst, FX_INT32 x, FX_INT32 y, JBig2ComposeOp op, const FX_RECT* pSrcRect);

    FX_BOOL composeTo_unopt(CJBig2_Image *pDst, FX_INT32 x, FX_INT32 y, JBig2ComposeOp op);

    FX_BOOL composeTo_opt(CJBig2_Image *pDst, FX_INT32 x, FX_INT32 y, JBig2ComposeOp op);

    FX_BOOL composeTo_opt2(CJBig2_Image *pDst, FX_INT32 x, FX_INT32 y, JBig2ComposeOp op);
    FX_BOOL composeTo_opt2(CJBig2_Image *pDst, FX_INT32 x, FX_INT32 y, JBig2ComposeOp op, const FX_RECT* pSrcRect);

    FX_BOOL composeFrom(FX_INT32 x, FX_INT32 y, CJBig2_Image *pSrc, JBig2ComposeOp op);
    FX_BOOL composeFrom(FX_INT32 x, FX_INT32 y, CJBig2_Image *pSrc, JBig2ComposeOp op, const FX_RECT* pSrcRect);
    CJBig2_Image *subImage_unopt(FX_INT32 x, FX_INT32 y, FX_INT32 w, FX_INT32 h);

    CJBig2_Image *subImage(FX_INT32 x, FX_INT32 y, FX_INT32 w, FX_INT32 h);

    void expand(FX_INT32 h, FX_BOOL v);
public:

    FX_INT32 m_nWidth;

    FX_INT32 m_nHeight;

    FX_INT32 m_nStride;

    FX_BYTE *m_pData;

    FX_BOOL m_bNeedFree;
};
#endif
