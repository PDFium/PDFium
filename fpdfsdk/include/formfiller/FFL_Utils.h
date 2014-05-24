// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#define FFL_BASE_USERUNIT			1.0f / 72.0f

template<class T> T FFL_MIN (const T & i, const T & j) { return ((i < j) ? i : j); }
template<class T> T FFL_MAX (const T & i, const T & j) { return ((i > j) ? i : j); }

class CFFL_Utils
{
public:
	static CPDF_Rect				MaxRect(const CPDF_Rect & rect1,const CPDF_Rect & rect2);
	static CPDF_Rect				InflateRect(const CPDF_Rect & crRect, const FX_FLOAT & fSize);
	static CPDF_Rect				DeflateRect(const CPDF_Rect & crRect, const FX_FLOAT & fSize);
	static FX_BOOL					TraceObject(CPDF_Object* pObj);
};

