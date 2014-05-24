// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef _JS_CONSOLE_H_
#define _JS_CONSOLE_H_

#include "../res/resource.h"

#define WST_NONE		0x00					// No size changed
#define WST_LEFT		0x01					// size to left
#define WST_TOP			0x02					// size to top
#define WST_RIGHT		0x04					// size to right
#define WST_BOTTOM		0x08					// size to bottom
#define WST_TOPLEFT		(WST_TOP|WST_LEFT)		// size to top & left
#define WST_TOPRIGHT	(WST_TOP|WST_RIGHT)		// size to top & right
#define WST_BOTTOMRIGHT	(WST_BOTTOM|WST_RIGHT)	// size to bottom & right
#define WST_BOTTOMLEFT	(WST_BOTTOM|WST_LEFT)	// size to bottom & right

#ifndef IDC_DLGSIZEBOX
#define IDC_DLGSIZEBOX  50
#endif	/* IDC_DLGSIZEBOX */

enum { m_idSizeIcon = IDC_DLGSIZEBOX };
enum {				// possible Control reSize Type
	CST_NONE = 0,
	CST_RESIZE,		// NOMOVE + SIZE, add all delta-size of dlg to control
	CST_REPOS,		// MOVE(absolutely) + NOSIZE, move control's pos by delta-size
	CST_RELATIVE,	// MOVE(proportional)  + NOSIZE, keep control always at a relative pos
	CST_ZOOM,		// MOVE + SIZE (both are automatically proportional)
	CST_DELTA_ZOOM	// MOVE(proportional, set manually) + SIZE(proportional, set manuall)
};

// contained class to hold item state
//
class CJS_ItemCtrl
{
public:
	UINT	m_nID;
	UINT	m_stxLeft  	   : 4;			// when left resizing ...
	UINT	m_stxRight     : 4;			// when right resizing ...
	UINT	m_styTop   	   : 4;			// when top resizing ...
	UINT	m_styBottom    : 4;			// when bottom resizing ...
	UINT	m_bFlickerFree : 1;
	UINT	m_bInvalidate  : 1;			// Invalidate ctrl's rect(eg. no-automatical update for static when resize+move)
	UINT	m_r0		   : 14;
	CRect	m_wRect;
	double	m_xRatio, m_cxRatio;
	double	m_yRatio, m_cyRatio;

protected:
	void Assign(const CJS_ItemCtrl& src);

public:
	CJS_ItemCtrl();
	CJS_ItemCtrl(const CJS_ItemCtrl& src);

	HDWP OnSize(HDWP hdwp, int sizeType, CRect *pnCltRect, CRect *poCltRect, CRect *pR0, CWnd *pDlg);

	CJS_ItemCtrl& operator=(const CJS_ItemCtrl& src);
};

class CJS_ResizeDlg : public CDialog
{
//	DECLARE_DYNAMIC(CJS_ResizeDlg)
public:
	CJS_ResizeDlg(UINT nID,CWnd *pParentWnd = NULL);
	virtual ~CJS_ResizeDlg();


public:
	std::vector<CJS_ItemCtrl>	m_Items;           // array of controlled items
	CRect					m_cltRect, m_cltR0;
	int						m_xMin, m_yMin;
	int						m_xSt,  m_ySt;		//step?
	UINT					m_nDelaySide;		//drag side of window
	CStatic					m_wndSizeIcon;     // size icon window

protected:
	void 					AddControl( UINT nID, int xl, int xr, int yt, int yb, int bFlickerFree = 0, 
									    double xRatio = -1.0, double cxRatio = -1.0,
									    double yRatio = -1.0, double cyRatio = -1.0 );
	void 					AllowSizing(int xst, int yst);
	void 					HideSizeIcon(void);	
	virtual BOOL			OnInitDialog();

	void					OnSizing(UINT nSide, LPRECT lpRect);
	void					OnSize(UINT nType, int cx, int cy);
	void					OnGetMinMaxInfo(MINMAXINFO *pmmi);
	BOOL					OnEraseBkgnd(CDC* pDC);

public:
	int						UpdateControlRect(UINT nID, CRect *pnr);
};


//------------------------CIconListBox for CWndElementList-------------------------------------

class CIconListBox : public CListBox
{
public:
	CIconListBox();
	virtual ~CIconListBox();

public:
	int				InsertString(int nIndex, LPCWSTR lpszItem , int nImage);
	virtual	void	ResetContent();
	virtual void	GetText(int nIndex, CString& rString);

	virtual void	DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void			MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	int				CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);

protected:
	// Generated message map functions
	//{{AFX_MSG(CIconListBox)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG	
	DECLARE_MESSAGE_MAP()

protected:
	struct ItemDatas 
	{
		CString	csText;
		int		nImage;
	};
};
//----------------------------------CWndElementList--------------------------------------------
#define IDC_LIST_JS_ELEMENT 10070

#define ELEMENT_LIST_WIDTH 140
#define ELEMENT_LIST_HEIGHT 180
#define ELEMENT_LIST_TOP_OFFSET 13

#define ELEMENT_TYPE_NAME	0
#define ELEMENT_TYPE_CONST	1
#define ELEMENT_TYPE_FUN	2
#define ELEMENT_TYPE_PRO	3

class CWndElementList : public CWnd
{
public:
	CWndElementList();
	virtual ~CWndElementList();
	
public:
	virtual void	OnSize(UINT nType, int cx, int cy);
	virtual BOOL	Create(CWnd* pParentWnd);
	virtual BOOL	ShowWindow(int nCmdShow);
	void			RemoveAllElement();
	void			SetElementList(LPCWSTR* pElement, int* pType ,  int iCount);
	void			AddElement(CFX_WideString csValue , int nType);
	BOOL			GetElementSel(CString &csElement);
	BOOL			SelectNext();
	BOOL			SelectPrevious();
	BOOL			SelectFirst();
	BOOL			SelectLast();
	BOOL			SelectNextPage();
	BOOL			SelectPreviousPage();
	int				GetListHeight();
	
protected:
	// Generated message map functions
	//{{AFX_MSG(CWndElementList)
	afx_msg void OnPaint();
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnSelJSElement();
	afx_msg void OnDblclkJSElement();
	afx_msg void OnDestroy();
	//}}AFX_MSG	
	DECLARE_MESSAGE_MAP()
protected:
	CIconListBox	m_ListBox;
	BOOL			m_bBlock;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CJS_ConsoleDlg 对话框
class CJS_ConsoleDlg : public CJS_ResizeDlg
{
	DECLARE_DYNAMIC(CJS_ConsoleDlg)

public:
	CJS_ConsoleDlg(CReader_App* pApp, CWnd* pParent);	
	virtual ~CJS_ConsoleDlg();

	enum { IDD = IDD_JS_CONSOLE };

	void				Create();

	void				AppendConsoleText(const CFX_WideString& swText);
	void				SetConsoleText(const CFX_WideString& swText);
	CFX_WideString		GetConsoleText() const;
	CFX_WideString		GetScriptText() const;

	BOOL				ResetElementList(LPCWSTR lpstrRef);
	IFXJS_Runtime*		GetJSRuntime();

protected:
	virtual void		DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

protected:
	virtual BOOL		OnInitDialog();	
	virtual void		OnCancel();

	virtual BOOL		PreTranslateMessage(MSG* pMsg);

protected:
	// Generated message map functions
	//{{AFX_MSG(CJS_ConsoleDlg)
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickTips();
	afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO *pmmi);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChangeScriptEdit(WPARAM wParam , LPARAM lParam);
	afx_msg void OnMove(int x , int y);
	public:
	virtual int	DoModal();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CGW_LineNumberEdit			m_edtSC;
	BOOL						m_bTips;
	const UINT					m_uTextlimited;
	FX_HGLOBAL					m_hGlobal;
	CReader_App *				m_pApp;
	CWndElementList				m_WndElementList;
};

#endif //_JS_CONSOLE_H_