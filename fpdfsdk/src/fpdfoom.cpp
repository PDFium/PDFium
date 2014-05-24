// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "../include/fsdk_define.h"
#include "../include/fpdfoom.h"

void OOM_Handler(FXMEM_FoxitMgr* pFoxitMgr, void* param)
{
	if (!param) return;
	((OOM_INFO*)param)->FSDK_OOM_Handler((OOM_INFO*)param);
}


DLLEXPORT FX_BOOL STDCALL FSDK_SetOOMHandler(OOM_INFO* oomInfo)
{
#ifndef _FXSDK_OPENSOURCE_
	if (!oomInfo || oomInfo->version!=1)
		return FALSE;
	FXMEM_SetOOMHandler(FXMEM_GetDefaultMgr(),OOM_Handler,oomInfo);
	return TRUE;
#else
	return TRUE;
#endif
}
