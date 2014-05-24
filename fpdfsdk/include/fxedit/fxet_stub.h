// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef _FXET_STUB_H_
#define _FXET_STUB_H_

#include "../../../core/include/fpdfapi/fpdf_module.h" 
#include "../../../core/include/fpdfapi/fpdf_render.h" 
#include "../../../core/include/fpdfapi/fpdf_pageobj.h" 
#include "../../../core/include/fpdfdoc/fpdf_vt.h" 
#include "../fx_systemhandler.h"
#ifdef FX_READER_DLL
	#ifdef _DEBUG
		#pragma comment(lib, "X:/pdf/fxcore/Lib/dbg_w32_vc6/fxcoredll[dbg,w32,vc6].lib")
		#pragma comment(lib, "X:/pdf/fxcore/Lib/dbg_w32_vc6/fpdfdocdll[dbg,w32,vc6].lib")
	#else
		#pragma comment(lib, "X:/pdf/fxcore/Lib/rel_w32_vc6/fxcoredll[rel,w32,vc6].lib")
		#pragma comment(lib, "X:/pdf/fxcore/Lib/rel_w32_vc6/fpdfdocdll[rel,w32,vc6].lib")
	#endif
#endif

#endif 

