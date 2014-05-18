// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef _FX_ARABIC_IMP
#define _FX_ARABIC_IMP
class CFX_BidiChar : public IFX_BidiChar, public CFX_Object
{
public:
    CFX_BidiChar();
    virtual void		Release()
    {
        delete this;
    }
    virtual void		SetPolicy(FX_BOOL bSeparateNeutral = TRUE)
    {
        m_bSeparateNeutral = bSeparateNeutral;
    }
    virtual FX_BOOL		AppendChar(FX_WCHAR wch);
    virtual FX_BOOL		EndChar();
    virtual FX_INT32	GetBidiInfo(FX_INT32 &iStart, FX_INT32 &iCount);
    virtual void		Reset();
protected:
    FX_BOOL		m_bSeparateNeutral;
    FX_INT32	m_iCurStart;
    FX_INT32	m_iCurCount;
    FX_INT32	m_iCurBidi;
    FX_INT32	m_iLastBidi;
    FX_INT32	m_iLastStart;
    FX_INT32	m_iLastCount;
};
#endif
