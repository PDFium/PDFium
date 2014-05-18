// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "../../include/reflow/reflowengine.h"
#include "reflowedpage.h"
IPDF_ReflowedPage* IPDF_ReflowedPage::Create()
{
    CPDF_ReflowedPage* pRefPage = FX_NEW CPDF_ReflowedPage(NULL);
    return pRefPage;
}
IPDF_ReflowedPage* Create_ReflowPage()
{
    return IPDF_ReflowedPage::Create();
}
IPDF_ProgressiveReflowPageParser* Create_ReflowPageParser()
{
    return IPDF_ProgressiveReflowPageParser::Create();
}
IPDF_ProgressiveReflowPageParser* IPDF_ProgressiveReflowPageParser::Create()
{
    CPDF_ProgressiveReflowPageParser* pParser = FX_NEW CPDF_ProgressiveReflowPageParser;
    if (NULL == pParser) {
        return NULL;
    }
    pParser->Init();
    return pParser;
}
IPDF_ProgressiveReflowPageRender* Create_ReflowPageRender()
{
    return IPDF_ProgressiveReflowPageRender::Create();
}
IPDF_ProgressiveReflowPageRender* IPDF_ProgressiveReflowPageRender::Create()
{
    return FX_NEW CPDF_ProgressiveReflowPageRender;
}
