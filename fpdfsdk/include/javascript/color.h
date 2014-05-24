// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef _COLOR_H_
#define _COLOR_H_

class color : public CJS_EmbedObj
{
public:
	color(CJS_Object* pJSObject);
	virtual ~color(void);

	FX_BOOL black(OBJ_PROP_PARAMS);
	FX_BOOL blue(OBJ_PROP_PARAMS);
	FX_BOOL cyan(OBJ_PROP_PARAMS);	
	FX_BOOL dkGray(OBJ_PROP_PARAMS);
	FX_BOOL gray(OBJ_PROP_PARAMS);
	FX_BOOL green(OBJ_PROP_PARAMS);
	FX_BOOL ltGray(OBJ_PROP_PARAMS);
	FX_BOOL magenta(OBJ_PROP_PARAMS);
	FX_BOOL red(OBJ_PROP_PARAMS);	
	FX_BOOL transparent(OBJ_PROP_PARAMS);
	FX_BOOL white(OBJ_PROP_PARAMS);
	FX_BOOL yellow(OBJ_PROP_PARAMS);

	FX_BOOL convert(OBJ_METHOD_PARAMS);
	FX_BOOL equal(OBJ_METHOD_PARAMS);

public:  
	static void		ConvertPWLColorToArray(const CPWL_Color& color, CJS_Array& array);
	static void		ConvertArrayToPWLColor(CJS_Array& array, CPWL_Color& color);

private:
	CPWL_Color		m_crTransparent;
	CPWL_Color		m_crBlack;
	CPWL_Color		m_crWhite;
	CPWL_Color		m_crRed;
	CPWL_Color		m_crGreen;
	CPWL_Color		m_crBlue;
	CPWL_Color		m_crCyan;
	CPWL_Color		m_crMagenta;
	CPWL_Color		m_crYellow;
	CPWL_Color		m_crDKGray;
	CPWL_Color		m_crGray;
	CPWL_Color		m_crLTGray;
};

class CJS_Color : public CJS_Object
{
public:
	CJS_Color(JSFXObject  pObject) : CJS_Object(pObject) {};
	virtual ~CJS_Color(void){};

	DECLARE_JS_CLASS(CJS_Color);

	JS_STATIC_PROP(black, color);
	JS_STATIC_PROP(blue, color);
	JS_STATIC_PROP(cyan, color);	
	JS_STATIC_PROP(dkGray, color);
	JS_STATIC_PROP(gray, color);
	JS_STATIC_PROP(green, color);
	JS_STATIC_PROP(ltGray, color);
	JS_STATIC_PROP(magenta, color);
	JS_STATIC_PROP(red, color);	
	JS_STATIC_PROP(transparent, color);
	JS_STATIC_PROP(white, color);
	JS_STATIC_PROP(yellow, color);

	JS_STATIC_METHOD(convert,color);
	JS_STATIC_METHOD(equal,color);

};

#endif //_COLOR_H_

