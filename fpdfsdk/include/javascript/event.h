// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef _EVENT_H_
#define _EVENT_H_

class event : public CJS_EmbedObj
{
public:
	event(CJS_Object * pJSObject);
	virtual ~event(void);

public:
	FX_BOOL change(OBJ_PROP_PARAMS);
	FX_BOOL changeEx(OBJ_PROP_PARAMS);
	FX_BOOL commitKey(OBJ_PROP_PARAMS);
	FX_BOOL fieldFull(OBJ_PROP_PARAMS);
	FX_BOOL keyDown(OBJ_PROP_PARAMS);
	FX_BOOL modifier(OBJ_PROP_PARAMS);
	FX_BOOL name(OBJ_PROP_PARAMS);
	FX_BOOL rc(OBJ_PROP_PARAMS);
	FX_BOOL richChange(OBJ_PROP_PARAMS);
	FX_BOOL richChangeEx(OBJ_PROP_PARAMS);
	FX_BOOL richValue(OBJ_PROP_PARAMS);
	FX_BOOL selEnd(OBJ_PROP_PARAMS);
	FX_BOOL selStart(OBJ_PROP_PARAMS);
	FX_BOOL shift(OBJ_PROP_PARAMS);
	FX_BOOL source(OBJ_PROP_PARAMS);
	FX_BOOL target(OBJ_PROP_PARAMS);
	FX_BOOL targetName(OBJ_PROP_PARAMS);
	FX_BOOL type(OBJ_PROP_PARAMS);
	FX_BOOL value(OBJ_PROP_PARAMS);
	FX_BOOL willCommit(OBJ_PROP_PARAMS);

};

class CJS_Event : public CJS_Object
{
public:
	CJS_Event(JSFXObject  pObject) : CJS_Object(pObject) {};
	virtual ~CJS_Event(void){};

	DECLARE_JS_CLASS(CJS_Event);

	JS_STATIC_PROP(change, event);
	JS_STATIC_PROP(changeEx, event);
	JS_STATIC_PROP(commitKey, event);
	JS_STATIC_PROP(fieldFull, event);
	JS_STATIC_PROP(keyDown, event);
	JS_STATIC_PROP(modifier, event);
	JS_STATIC_PROP(name, event);
	JS_STATIC_PROP(rc, event);
	JS_STATIC_PROP(richChange, event);
	JS_STATIC_PROP(richChangeEx, event);
	JS_STATIC_PROP(richValue, event);
	JS_STATIC_PROP(selEnd, event);
	JS_STATIC_PROP(selStart, event);
	JS_STATIC_PROP(shift, event);
	JS_STATIC_PROP(source, event);
	JS_STATIC_PROP(target, event);
	JS_STATIC_PROP(targetName, event);
	JS_STATIC_PROP(type, event);
	JS_STATIC_PROP(value, event);
	JS_STATIC_PROP(willCommit, event);
};

#endif //_EVENT_H_
