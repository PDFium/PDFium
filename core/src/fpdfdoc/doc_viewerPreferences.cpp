// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "../../include/fpdfdoc/fpdf_doc.h"
CPDF_ViewerPreferences::CPDF_ViewerPreferences(CPDF_Document *pDoc): m_pDoc(pDoc)
{
}
CPDF_ViewerPreferences::~CPDF_ViewerPreferences()
{
}
FX_BOOL CPDF_ViewerPreferences::IsDirectionR2L() const
{
    CPDF_Dictionary *pDict = m_pDoc->GetRoot();
    pDict = pDict->GetDict(FX_BSTRC("ViewerPreferences"));
    if (!pDict)	{
        return FALSE;
    }
    return FX_BSTRC("R2L") == pDict->GetString(FX_BSTRC("Direction"));
}
FX_BOOL CPDF_ViewerPreferences::PrintScaling() const
{
    CPDF_Dictionary *pDict = m_pDoc->GetRoot();
    pDict = pDict->GetDict(FX_BSTRC("ViewerPreferences"));
    if (!pDict)	{
        return TRUE;
    }
    return FX_BSTRC("None") != pDict->GetString(FX_BSTRC("PrintScaling"));
}
