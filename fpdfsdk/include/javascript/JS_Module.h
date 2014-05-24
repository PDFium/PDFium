// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef _JS_MODULE_H_
#define _JS_MODULE_H_

class CJS_GlobalData;
class CJS_ConsoleDlg;

class CJS_Module : public IReader_Module
{
public:
	CJS_Module(HMODULE hModule, CReader_App* pApp);
	virtual ~CJS_Module();

	virtual void					Destroy(){delete this;}
	virtual CFX_ByteString			GetModuleName();

public:
	static CJS_Module*				GetModule(CReader_App* pApp);

	IFXJS_Runtime*					NewJSRuntime();
	CJS_GlobalData*					NewGlobalData();
	void							ReleaseGlobalData();

public:
	//console
	void							ShowConsole();
	void							HideConsole();
	void							ClearConsole();
	void							PrintLineConsole(FX_LPCWSTR string);

private:
	HMODULE							m_hModule;
	CReader_App*					m_pApp;

	FX_BOOL							m_bInitial;
	CJS_GlobalData*					m_pGlobalData;
	FX_INT32						m_nGlobalDataCount;

	CJS_ConsoleDlg*					m_pConsole;
};

#endif //_JS_MODULE_H_