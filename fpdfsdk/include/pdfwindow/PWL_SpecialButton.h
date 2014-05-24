// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef _PWL_SPECIALBUTTON_H_
#define _PWL_SPECIALBUTTON_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PWL_CLASS CPWL_PushButton : public CPWL_Button  
{
public:
	CPWL_PushButton();
	virtual ~CPWL_PushButton();

public:
	virtual CFX_ByteString		GetClassName() const;
	virtual CPDF_Rect			GetFocusRect() const;
};

class PWL_CLASS CPWL_CheckBox : public CPWL_Button
{
public:
	CPWL_CheckBox();
	virtual ~CPWL_CheckBox();

public:
	virtual CFX_ByteString		GetClassName() const;
	virtual FX_BOOL				OnLButtonUp(const CPDF_Point & point);
	virtual FX_BOOL				OnChar(FX_WORD nChar);

	void						SetCheck(FX_BOOL bCheck);
	FX_BOOL						IsChecked() const;

private:
	FX_BOOL						m_bChecked;
};

class PWL_CLASS CPWL_RadioButton : public CPWL_Button
{
public:
	CPWL_RadioButton();
	virtual ~CPWL_RadioButton();

public:
	virtual CFX_ByteString		GetClassName() const;
	virtual FX_BOOL				OnLButtonUp(const CPDF_Point & point);
	virtual FX_BOOL				OnChar(FX_WORD nChar);

	void						SetCheck(FX_BOOL bCheck);
	FX_BOOL						IsChecked() const;

private:
	FX_BOOL						m_bChecked;
};

#endif


