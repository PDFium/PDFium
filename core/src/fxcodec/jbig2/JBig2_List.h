// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef _JBIG2_LIST_H_
#define _JBIG2_LIST_H_
#include "JBig2_Define.h"
#include "JBig2_Object.h"
template <class TYPE>
class CJBig2_List : public CJBig2_Object
{
public:

    CJBig2_List(FX_INT32 nSize = 8)
    {
        m_nSize = nSize;
        m_pArray = (TYPE**)m_pModule->JBig2_Malloc2(sizeof(TYPE*), nSize);
        m_nLength = 0;
    }

    ~CJBig2_List()
    {
        clear();
        m_pModule->JBig2_Free(m_pArray);
    }

    void clear()
    {
        FX_INT32 i;
        for(i = 0; i < m_nLength; i++) {
            delete m_pArray[i];
        }
        m_nLength = 0;
    }

    void addItem(TYPE *pItem)
    {
        if(m_nLength >= m_nSize) {
            m_nSize += 8;
            m_pArray = (TYPE**)m_pModule->JBig2_Realloc(m_pArray, sizeof(TYPE*)*m_nSize);
        }
        m_pArray[m_nLength++] = pItem;
    }


    FX_INT32 getLength()
    {
        return m_nLength;
    }

    TYPE *getAt(FX_INT32 nIndex)
    {
        return m_pArray[nIndex];
    }

    TYPE *getLast()
    {
        return m_pArray[m_nLength - 1];
    }
private:
    FX_INT32 m_nSize;
    TYPE **m_pArray;
    FX_INT32 m_nLength;
};
#endif
