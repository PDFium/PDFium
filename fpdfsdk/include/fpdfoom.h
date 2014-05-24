// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef _FPDFOOM_H_
#define _FPDFOOM_H_

#ifndef _FPDFVIEW_H_
#include "fpdfview.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef	struct _OOM_INFO
{
	/**
	* Version number of the interface. Currently must be 1.
	**/
	int version;
	
	/** 
	* Method: FSDK_OOM_Handler
	*			 Out-Of-Memory handling function.
	* Interface Version:
	*			1
	* Implementation Required:
	*			Yes
	* Parameters:
	*		pThis		-	Pointer to the interface structure itself.
	* 	Return value:
	* 		None.
	* */

	void(*FSDK_OOM_Handler)(_OOM_INFO* pThis);
}OOM_INFO;


/**
 * Function: FSDK_SetOOMHandler
 *			 Setup A Out-Of-Memory handler for foxit sdk. 
 * Parameters:
 *			oomInfo		-	Pointer to a OOM_INFO structure.
 * Return Value:
 *			TRUE means successful. FALSE means fails. 
 **/

DLLEXPORT FPDF_BOOL STDCALL FSDK_SetOOMHandler(OOM_INFO* oomInfo);


#ifdef __cplusplus
};
#endif




#endif
