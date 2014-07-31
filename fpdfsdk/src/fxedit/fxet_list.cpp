// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "../../include/fxedit/fxet_stub.h"
#include "../../include/fxedit/fxet_edit.h"
#include "../../include/fxedit/fxet_list.h"

/* ------------------------------- CFX_ListItem ---------------------------------- */

CFX_ListItem::CFX_ListItem() : m_pEdit(NULL),
	m_bSelected(FALSE),
	m_bCaret(FALSE),
	m_rcListItem(0.0f,0.0f,0.0f,0.0f)
{
	m_pEdit = IFX_Edit::NewEdit();
	ASSERT(m_pEdit != NULL);

	m_pEdit->SetAlignmentV(1);
	m_pEdit->Initialize();
}

CFX_ListItem::~CFX_ListItem()
{
	IFX_Edit::DelEdit(m_pEdit);
}

void CFX_ListItem::SetFontMap(IFX_Edit_FontMap * pFontMap)
{
	if (m_pEdit)
		m_pEdit->SetFontMap(pFontMap);
}

IFX_Edit* CFX_ListItem::GetEdit() const
{
	return m_pEdit;
}

IFX_Edit_Iterator*	CFX_ListItem::GetIterator() const
{
	if (m_pEdit)
		return m_pEdit->GetIterator();

	return NULL;
}

void CFX_ListItem::SetRect(const CLST_Rect & rect)
{
	m_rcListItem = rect;
}

CLST_Rect CFX_ListItem::GetRect() const
{
	return m_rcListItem;
}

FX_BOOL CFX_ListItem::IsSelected() const
{
	return m_bSelected;
}

void CFX_ListItem::SetSelect(FX_BOOL bSelected)
{
	m_bSelected = bSelected;
}

FX_BOOL CFX_ListItem::IsCaret() const
{
	return m_bCaret;
}

void CFX_ListItem::SetCaret(FX_BOOL bCaret)
{
	m_bCaret = bCaret;
}

void CFX_ListItem::SetText(FX_LPCWSTR text)
{
	if (m_pEdit)
		m_pEdit->SetText(text);
}

void CFX_ListItem::SetFontSize(FX_FLOAT fFontSize)
{
	if (m_pEdit)
		m_pEdit->SetFontSize(fFontSize);
}

FX_FLOAT CFX_ListItem::GetItemHeight() const
{
	if (m_pEdit)
		return m_pEdit->GetContentRect().Height();

	return 0.0f;
}

FX_WORD CFX_ListItem::GetFirstChar() const
{
	CPVT_Word word;

	if (IFX_Edit_Iterator*	pIterator = GetIterator())
	{
		pIterator->SetAt(1);		
		pIterator->GetWord(word);
	}

	return word.Word;
}

CFX_WideString CFX_ListItem::GetText() const
{
	if (m_pEdit)
		return m_pEdit->GetText();

	return L"";
}

/* ------------------------------------ CFX_List --------------------------------- */

CFX_List::CFX_List() : m_fFontSize(0.0f), m_pFontMap(NULL), m_bMultiple(FALSE)
{
}

CFX_List::~CFX_List()
{
	Empty();
}

void CFX_List::Empty()
{
	for (FX_INT32 i=0,sz=m_aListItems.GetSize(); i<sz; i++)
		delete m_aListItems.GetAt(i);

	m_aListItems.RemoveAll();
}

void CFX_List::SetFontMap(IFX_Edit_FontMap * pFontMap)
{
	m_pFontMap = pFontMap;
}

void CFX_List::SetFontSize(FX_FLOAT fFontSize)
{
	m_fFontSize = fFontSize;
}

void CFX_List::AddItem(FX_LPCWSTR str)
{	
	if (CFX_ListItem * pListItem = new CFX_ListItem())
	{
		pListItem->SetFontMap(m_pFontMap);
		pListItem->SetFontSize(m_fFontSize);
		pListItem->SetText(str);
		m_aListItems.Add(pListItem);
	}
}

void CFX_List::ReArrange(FX_INT32 nItemIndex)
{
	FX_FLOAT fPosY = 0.0f;

	if (CFX_ListItem * pPrevItem = m_aListItems.GetAt(nItemIndex - 1))
		fPosY = pPrevItem->GetRect().bottom;
	
	for (FX_INT32 i=nItemIndex,sz=m_aListItems.GetSize(); i<sz; i++)
	{
		if (CFX_ListItem * pListItem = m_aListItems.GetAt(i))
		{
			FX_FLOAT fListItemHeight = pListItem->GetItemHeight();
			pListItem->SetRect(CLST_Rect(0.0f,fPosY,0.0f,fPosY + fListItemHeight));
			fPosY += fListItemHeight;			
		}
	}

	SetContentRect(CLST_Rect(0.0f,0.0f,0.0f,fPosY));	
}

IFX_Edit * CFX_List::GetItemEdit(FX_INT32 nIndex) const
{
	if (CFX_ListItem * pListItem = m_aListItems.GetAt(nIndex))
	{
		return pListItem->GetEdit();
	}

	return NULL;
}

FX_INT32 CFX_List::GetCount() const
{
	return m_aListItems.GetSize();
}

CPDF_Rect CFX_List::GetPlateRect() const
{
	return CFX_ListContainer::GetPlateRect();
}

CPDF_Rect CFX_List::GetContentRect() const
{
	return InnerToOuter(CFX_ListContainer::GetContentRect());
}

FX_FLOAT CFX_List::GetFontSize() const
{
	return m_fFontSize;
}

FX_INT32 CFX_List::GetItemIndex(const CPDF_Point & point) const
{
	CPDF_Point pt = OuterToInner(point);

	FX_BOOL bFirst = TRUE;
	FX_BOOL bLast = TRUE;

	for (FX_INT32 i=0,sz=m_aListItems.GetSize(); i<sz; i++)
	{
		if (CFX_ListItem * pListItem = m_aListItems.GetAt(i))
		{
			CLST_Rect rcListItem = pListItem->GetRect();

			if (FX_EDIT_IsFloatBigger(pt.y, rcListItem.top))
			{			
				bFirst = FALSE;
			}

			if (FX_EDIT_IsFloatSmaller(pt.y, rcListItem.bottom))
			{
				bLast = FALSE;
			}

			if (pt.y >= rcListItem.top && pt.y < rcListItem.bottom)
			{
				return i;
			}
		}
	}

	if (bFirst) return 0;
	if (bLast) return m_aListItems.GetSize()-1;

	return -1;
}

FX_FLOAT CFX_List::GetFirstHeight() const
{
	if (CFX_ListItem * pListItem = m_aListItems.GetAt(0))
	{
		return pListItem->GetItemHeight();
	}

	return 1.0f;
}

FX_INT32 CFX_List::GetFirstSelected() const
{
	for (FX_INT32 i=0,sz=m_aListItems.GetSize(); i<sz; i++)
	{
		if (CFX_ListItem * pListItem = m_aListItems.GetAt(i))
		{
			if (pListItem->IsSelected())
				return i;
		}
	}
	return -1;
}

FX_INT32 CFX_List::GetLastSelected() const
{
	for (FX_INT32 i=m_aListItems.GetSize()-1; i>=0; i--)
	{
		if (CFX_ListItem * pListItem = m_aListItems.GetAt(i))
		{
			if (pListItem->IsSelected())
				return i;
		}
	}
	return -1;
}

FX_WCHAR CFX_List::Toupper(FX_WCHAR c) const
{
	if ( (c >= 'a') && (c <= 'z') )
		c = c - ('a' - 'A');
	return c;
}

FX_INT32 CFX_List::FindNext(FX_INT32 nIndex,FX_WCHAR nChar) const
{
	FX_INT32 nCircleIndex = nIndex;

	for (FX_INT32 i=0,sz=m_aListItems.GetSize(); i<sz; i++)
	{
		nCircleIndex ++;
		if (nCircleIndex >= sz) nCircleIndex = 0;

		if (CFX_ListItem * pListItem = m_aListItems.GetAt(nCircleIndex))
		{
			if (Toupper(pListItem->GetFirstChar()) == Toupper(nChar))
				return nCircleIndex;
		}
	}

	return nCircleIndex;
}

CPDF_Rect CFX_List::GetItemRect(FX_INT32 nIndex) const
{
	if (CFX_ListItem * pListItem = m_aListItems.GetAt(nIndex))
	{
		CPDF_Rect rcItem = pListItem->GetRect();
		rcItem.left = 0.0f;
		rcItem.right = GetPlateRect().Width();
		return InnerToOuter(rcItem);
	}

	return CPDF_Rect();
}

FX_BOOL CFX_List::IsItemSelected(FX_INT32 nIndex) const
{
	if (CFX_ListItem * pListItem = m_aListItems.GetAt(nIndex))
	{
		return pListItem->IsSelected();
	}

	return FALSE;
}

void CFX_List::SetItemSelect(FX_INT32 nItemIndex, FX_BOOL bSelected)
{
	if (CFX_ListItem * pListItem = m_aListItems.GetAt(nItemIndex))
	{
		pListItem->SetSelect(bSelected);
	}
}

void CFX_List::SetItemCaret(FX_INT32 nItemIndex, FX_BOOL bCaret)
{
	if (CFX_ListItem * pListItem = m_aListItems.GetAt(nItemIndex))
	{
		pListItem->SetCaret(bCaret);		
	}
}

void CFX_List::SetMultipleSel(FX_BOOL bMultiple)
{
	m_bMultiple = bMultiple;
}

FX_BOOL CFX_List::IsMultipleSel() const
{
	return m_bMultiple;
}

FX_BOOL CFX_List::IsValid(FX_INT32 nItemIndex) const
{
	return nItemIndex >= 0 && nItemIndex < m_aListItems.GetSize();
}

CFX_WideString CFX_List::GetItemText(FX_INT32 nIndex) const
{
	if (CFX_ListItem * pListItem = m_aListItems.GetAt(nIndex))
	{
		return pListItem->GetText();
	}

	return L"";
}

/* ------------------------------------ CPLST_Select ---------------------------------- */

CPLST_Select::CPLST_Select()
{
}

CPLST_Select::~CPLST_Select()
{
	for (FX_INT32 i=0,sz=m_aItems.GetSize(); i<sz; i++)
		delete m_aItems.GetAt(i);

	m_aItems.RemoveAll();
}

void CPLST_Select::Add(FX_INT32 nItemIndex)
{
	FX_INT32 nIndex = Find(nItemIndex);

	if (nIndex < 0) 
		m_aItems.Add(new CPLST_Select_Item(nItemIndex,1));
	else
	{
		if (CPLST_Select_Item * pItem = m_aItems.GetAt(nIndex))
		{
			pItem->nState = 1;
		}
	}
}

void CPLST_Select::Add(FX_INT32 nBeginIndex, FX_INT32 nEndIndex)
{
	if (nBeginIndex > nEndIndex)
	{
		FX_INT32 nTemp = nEndIndex;
		nEndIndex = nBeginIndex;
		nBeginIndex = nTemp;
	}

	for (FX_INT32 i=nBeginIndex; i<=nEndIndex; i++)	Add(i);
}

void CPLST_Select::Sub(FX_INT32 nItemIndex)
{
	for (FX_INT32 i=m_aItems.GetSize()-1; i>=0; i--)
	{
		if (CPLST_Select_Item * pItem = m_aItems.GetAt(i))
			if (pItem->nItemIndex == nItemIndex)
				pItem->nState = -1;
	}
}

void CPLST_Select::Sub(FX_INT32 nBeginIndex, FX_INT32 nEndIndex)
{
	if (nBeginIndex > nEndIndex)
	{
		FX_INT32 nTemp = nEndIndex;
		nEndIndex = nBeginIndex;
		nBeginIndex = nTemp;
	}

	for (FX_INT32 i=nBeginIndex; i<=nEndIndex; i++)	Sub(i);
}

FX_INT32 CPLST_Select::Find(FX_INT32 nItemIndex) const
{
	for (FX_INT32 i=0,sz=m_aItems.GetSize(); i<sz; i++)
	{
		if (CPLST_Select_Item * pItem = m_aItems.GetAt(i))
		{
			if (pItem->nItemIndex == nItemIndex)
				return i;
		}
	}

	return -1;
}

FX_BOOL CPLST_Select::IsExist(FX_INT32 nItemIndex) const
{
	return Find(nItemIndex) >= 0;
}

FX_INT32 CPLST_Select::GetCount() const
{
	return m_aItems.GetSize();
}

FX_INT32 CPLST_Select::GetItemIndex(FX_INT32 nIndex) const
{
	if (nIndex >= 0 && nIndex < m_aItems.GetSize())
		if (CPLST_Select_Item * pItem = m_aItems.GetAt(nIndex))
			return pItem->nItemIndex;

	return -1;
}

FX_INT32 CPLST_Select::GetState(FX_INT32 nIndex) const
{
	if (nIndex >= 0 && nIndex < m_aItems.GetSize())
		if (CPLST_Select_Item * pItem = m_aItems.GetAt(nIndex))
			return pItem->nState;

	return 0;
}

void CPLST_Select::DeselectAll()
{
	for (FX_INT32 i=0,sz=m_aItems.GetSize(); i<sz; i++)
	{
		if (CPLST_Select_Item * pItem = m_aItems.GetAt(i))
		{
			pItem->nState = -1;
		}
	}
}

void CPLST_Select::Done()
{
	for (FX_INT32 i=m_aItems.GetSize()-1; i>=0; i--)
	{	
		if (CPLST_Select_Item * pItem = m_aItems.GetAt(i))
		{
			if (pItem->nState == -1)
			{
				delete pItem;
				m_aItems.RemoveAt(i);
			}
			else
			{
				pItem->nState = 0;
			}
		}
	}
}

/* ------------------------------------ CFX_ListCtrl --------------------------------- */

CFX_ListCtrl::CFX_ListCtrl() : m_pNotify(NULL),
	m_bNotifyFlag(FALSE),
	m_ptScrollPos(0.0f,0.0f),
	m_nSelItem(-1),
	m_nFootIndex(-1),
	m_bCtrlSel(FALSE),
	m_nCaretIndex(-1)
{
}

CFX_ListCtrl::~CFX_ListCtrl()
{
}

void CFX_ListCtrl::SetNotify(IFX_List_Notify * pNotify)
{
	m_pNotify = pNotify;
}

CPDF_Point CFX_ListCtrl::InToOut(const CPDF_Point & point) const
{
	CPDF_Rect rcPlate = GetPlateRect();

	return CPDF_Point(point.x - (m_ptScrollPos.x - rcPlate.left),
		point.y - (m_ptScrollPos.y - rcPlate.top));
}

CPDF_Point CFX_ListCtrl::OutToIn(const CPDF_Point & point) const
{
	CPDF_Rect rcPlate = GetPlateRect();

	return CPDF_Point(point.x + (m_ptScrollPos.x - rcPlate.left),
		point.y + (m_ptScrollPos.y - rcPlate.top));
}

CPDF_Rect CFX_ListCtrl::InToOut(const CPDF_Rect & rect) const
{
	CPDF_Point ptLeftBottom = InToOut(CPDF_Point(rect.left,rect.bottom));
	CPDF_Point ptRightTop = InToOut(CPDF_Point(rect.right,rect.top));

	return CPDF_Rect(ptLeftBottom.x,ptLeftBottom.y,ptRightTop.x,ptRightTop.y);
}

CPDF_Rect CFX_ListCtrl::OutToIn(const CPDF_Rect & rect) const
{
	CPDF_Point ptLeftBottom = OutToIn(CPDF_Point(rect.left,rect.bottom));
	CPDF_Point ptRightTop = OutToIn(CPDF_Point(rect.right,rect.top));

	return CPDF_Rect(ptLeftBottom.x,ptLeftBottom.y,ptRightTop.x,ptRightTop.y);
}

void CFX_ListCtrl::OnMouseDown(const CPDF_Point & point,FX_BOOL bShift,FX_BOOL bCtrl)
{
	FX_INT32 nHitIndex = this->GetItemIndex(point);

	if (IsMultipleSel())
	{
		if (bCtrl)
		{
			if (IsItemSelected(nHitIndex))
			{
				m_aSelItems.Sub(nHitIndex);
				SelectItems();
				m_bCtrlSel = FALSE;
			}
			else
			{
				m_aSelItems.Add(nHitIndex);
				SelectItems();
				m_bCtrlSel = TRUE;
			}		
			
			m_nFootIndex = nHitIndex;
		}
		else if (bShift)
		{
			m_aSelItems.DeselectAll();
			m_aSelItems.Add(m_nFootIndex,nHitIndex);
			SelectItems();
		}
		else
		{
			m_aSelItems.DeselectAll();
			m_aSelItems.Add(nHitIndex);
			SelectItems();

			m_nFootIndex = nHitIndex;
		}

		SetCaret(nHitIndex);
	}
	else
	{
		SetSingleSelect(nHitIndex);
	}

	if (!this->IsItemVisible(nHitIndex))
		this->ScrollToListItem(nHitIndex);
}

void CFX_ListCtrl::OnMouseMove(const CPDF_Point & point,FX_BOOL bShift,FX_BOOL bCtrl)
{
	FX_INT32 nHitIndex = this->GetItemIndex(point);

	if (IsMultipleSel())
	{			
		if (bCtrl)
		{
			if (m_bCtrlSel)
				m_aSelItems.Add(m_nFootIndex,nHitIndex);
			else
				m_aSelItems.Sub(m_nFootIndex,nHitIndex);			
			
			SelectItems();
		}
		else
		{
			m_aSelItems.DeselectAll();
			m_aSelItems.Add(m_nFootIndex,nHitIndex);
			SelectItems();
		}

		SetCaret(nHitIndex);
	}
	else
	{
		SetSingleSelect(nHitIndex);
	}

	if (!this->IsItemVisible(nHitIndex))
		this->ScrollToListItem(nHitIndex);
}

void CFX_ListCtrl::OnVK(FX_INT32 nItemIndex,FX_BOOL bShift,FX_BOOL bCtrl)
{
	if (IsMultipleSel())
	{
		if (nItemIndex >= 0 && nItemIndex < GetCount())
		{
			if (bCtrl)
			{
			}
			else if (bShift)
			{
				m_aSelItems.DeselectAll();
				m_aSelItems.Add(m_nFootIndex,nItemIndex);
				SelectItems();
			}
			else
			{
				m_aSelItems.DeselectAll();
				m_aSelItems.Add(nItemIndex);
				SelectItems();
				m_nFootIndex = nItemIndex;
			}

			SetCaret(nItemIndex);	
		}
	}
	else
	{
		SetSingleSelect(nItemIndex);
	}

	if (!this->IsItemVisible(nItemIndex))
		this->ScrollToListItem(nItemIndex);
}

void CFX_ListCtrl::OnVK_UP(FX_BOOL bShift,FX_BOOL bCtrl)
{
	OnVK(IsMultipleSel() ? GetCaret()-1 : GetSelect()-1, bShift, bCtrl);
}

void CFX_ListCtrl::OnVK_DOWN(FX_BOOL bShift,FX_BOOL bCtrl)
{
	OnVK(IsMultipleSel() ? GetCaret()+1 : GetSelect()+1, bShift, bCtrl);
}

void CFX_ListCtrl::OnVK_LEFT(FX_BOOL bShift,FX_BOOL bCtrl)
{
	OnVK(0, bShift, bCtrl);
}

void CFX_ListCtrl::OnVK_RIGHT(FX_BOOL bShift,FX_BOOL bCtrl)
{
	OnVK(GetCount()-1, bShift, bCtrl);
}

void CFX_ListCtrl::OnVK_HOME(FX_BOOL bShift,FX_BOOL bCtrl)
{
	OnVK(0, bShift, bCtrl);
}

void CFX_ListCtrl::OnVK_END(FX_BOOL bShift,FX_BOOL bCtrl)
{
	OnVK(GetCount()-1, bShift, bCtrl);
}

FX_BOOL	CFX_ListCtrl::OnChar(FX_WORD nChar,FX_BOOL bShift,FX_BOOL bCtrl)
{
	FX_INT32 nIndex = GetLastSelected();	
	FX_INT32 nFindIndex = FindNext(nIndex,nChar);

	if (nFindIndex != nIndex)
	{
		OnVK(nFindIndex, bShift, bCtrl);
		return TRUE;
	}
	return FALSE;
}

/* -------- inner methods ------- */

void CFX_ListCtrl::SetPlateRect(const CPDF_Rect & rect)
{
	CFX_ListContainer::SetPlateRect(rect);
	m_ptScrollPos.x = rect.left;
	SetScrollPos(CPDF_Point(rect.left,rect.top));	
	ReArrange(0);
	InvalidateItem(-1);
}

CPDF_Rect CFX_ListCtrl::GetItemRect(FX_INT32 nIndex) const
{
	return InToOut(CFX_List::GetItemRect(nIndex));
}

void CFX_ListCtrl::AddString(FX_LPCWSTR string)
{
	AddItem(string);
	ReArrange(GetCount() - 1);
}

void CFX_ListCtrl::SetMultipleSelect(FX_INT32 nItemIndex, FX_BOOL bSelected)
{
	if (!IsValid(nItemIndex)) return;

	if (bSelected != this->IsItemSelected(nItemIndex))
	{
		if (bSelected)
		{
			SetItemSelect(nItemIndex,TRUE);
			InvalidateItem(nItemIndex);
		}
		else
		{
			SetItemSelect(nItemIndex,FALSE);
			InvalidateItem(nItemIndex);
		}
	}
}

void CFX_ListCtrl::SetSingleSelect(FX_INT32 nItemIndex)
{
	if (!IsValid(nItemIndex)) return;

	if (m_nSelItem != nItemIndex)
	{
		if (m_nSelItem >= 0)
		{
			SetItemSelect(m_nSelItem,FALSE);
			InvalidateItem(m_nSelItem);
		}
		
		SetItemSelect(nItemIndex,TRUE);
		InvalidateItem(nItemIndex);
		m_nSelItem = nItemIndex;		
	}
}

void CFX_ListCtrl::SetCaret(FX_INT32 nItemIndex)
{
	if (!IsValid(nItemIndex)) return;

	if (this->IsMultipleSel())
	{		
		FX_INT32 nOldIndex = m_nCaretIndex;

		if (nOldIndex != nItemIndex)
		{
			m_nCaretIndex = nItemIndex;

			SetItemCaret(nOldIndex, FALSE);
			SetItemCaret(nItemIndex,TRUE);

			InvalidateItem(nOldIndex);
			InvalidateItem(nItemIndex);			
		}
	}
}

void CFX_ListCtrl::InvalidateItem(FX_INT32 nItemIndex)
{
	if (m_pNotify)
	{
		if (nItemIndex == -1)
		{
			if (!m_bNotifyFlag)
			{
				m_bNotifyFlag = TRUE;
				CPDF_Rect rcRefresh = GetPlateRect();
				m_pNotify->IOnInvalidateRect(&rcRefresh);
				m_bNotifyFlag = FALSE;
			}
		}
		else
		{
			if (!m_bNotifyFlag)
			{
				m_bNotifyFlag = TRUE;
				CPDF_Rect rcRefresh = GetItemRect(nItemIndex);
				rcRefresh.left -= 1.0f;
				rcRefresh.right += 1.0f;
				rcRefresh.bottom -= 1.0f;
				rcRefresh.top += 1.0f;

				m_pNotify->IOnInvalidateRect(&rcRefresh);
				m_bNotifyFlag = FALSE;
			}
		}
	}
}

void CFX_ListCtrl::SelectItems()
{
	for (FX_INT32 i=0,sz=m_aSelItems.GetCount(); i<sz; i++)
	{
		FX_INT32 nItemIndex = m_aSelItems.GetItemIndex(i);
		FX_INT32 nState = m_aSelItems.GetState(i);

		switch(nState)
		{
		case 1:
			SetMultipleSelect(nItemIndex, TRUE);			
			break;
		case -1:
			SetMultipleSelect(nItemIndex, FALSE);
			break;
		}
	}

	m_aSelItems.Done();
}

void CFX_ListCtrl::Select(FX_INT32 nItemIndex)
{
	if (!IsValid(nItemIndex)) return;

	if (this->IsMultipleSel())
	{
		m_aSelItems.Add(nItemIndex);
		SelectItems();
	}
	else
		SetSingleSelect(nItemIndex);
}

FX_BOOL	CFX_ListCtrl::IsItemVisible(FX_INT32 nItemIndex) const
{
	CPDF_Rect rcPlate = this->GetPlateRect();
	CPDF_Rect rcItem = this->GetItemRect(nItemIndex);

	return rcItem.bottom >= rcPlate.bottom && rcItem.top <= rcPlate.top;
}

void CFX_ListCtrl::ScrollToListItem(FX_INT32 nItemIndex)
{
	if (!IsValid(nItemIndex)) return;

	CPDF_Rect rcPlate = this->GetPlateRect();
	CPDF_Rect rcItem = CFX_List::GetItemRect(nItemIndex);
	CPDF_Rect rcItemCtrl = GetItemRect(nItemIndex);

	if (FX_EDIT_IsFloatSmaller(rcItemCtrl.bottom, rcPlate.bottom))
	{
		if (FX_EDIT_IsFloatSmaller(rcItemCtrl.top, rcPlate.top))
		{
			SetScrollPosY(rcItem.bottom + rcPlate.Height());
		}
	}
	else if (FX_EDIT_IsFloatBigger(rcItemCtrl.top, rcPlate.top))
	{
		if (FX_EDIT_IsFloatBigger(rcItemCtrl.bottom, rcPlate.bottom))
		{
			SetScrollPosY(rcItem.top);
		}
	}
}

void CFX_ListCtrl::SetScrollInfo()
{
	if (m_pNotify)
	{
		CPDF_Rect rcPlate = GetPlateRect();
		CPDF_Rect rcContent = CFX_List::GetContentRect();

		if (!m_bNotifyFlag)
		{
			m_bNotifyFlag = TRUE;
			m_pNotify->IOnSetScrollInfoY(rcPlate.bottom, rcPlate.top, 
					rcContent.bottom, rcContent.top, GetFirstHeight(), rcPlate.Height());
			m_bNotifyFlag = FALSE;
		}
	}
}

void CFX_ListCtrl::SetScrollPos(const CPDF_Point & point)
{
	SetScrollPosY(point.y);
}

void CFX_ListCtrl::SetScrollPosY(FX_FLOAT fy)
{
	if (!FX_EDIT_IsFloatEqual(m_ptScrollPos.y,fy))
	{
		CPDF_Rect rcPlate = this->GetPlateRect();
		CPDF_Rect rcContent = CFX_List::GetContentRect();

		if (rcPlate.Height() > rcContent.Height())
		{
			fy = rcPlate.top;
		}
		else
		{
			if (FX_EDIT_IsFloatSmaller(fy - rcPlate.Height(), rcContent.bottom))
			{
				fy = rcContent.bottom + rcPlate.Height();
			}
			else if (FX_EDIT_IsFloatBigger(fy, rcContent.top))
			{
				fy = rcContent.top;
			}
		}

		m_ptScrollPos.y = fy;
		InvalidateItem(-1);

		if (m_pNotify) 
		{
			if (!m_bNotifyFlag)
			{
				m_bNotifyFlag = TRUE;
				m_pNotify->IOnSetScrollPosY(fy);
				m_bNotifyFlag = FALSE;
			}
		}
	}
}

CPDF_Rect CFX_ListCtrl::GetContentRect() const
{
	return InToOut(CFX_List::GetContentRect());
}

void CFX_ListCtrl::ReArrange(FX_INT32 nItemIndex)
{
	CFX_List::ReArrange(nItemIndex);
	SetScrollInfo();
}

void CFX_ListCtrl::SetTopItem(FX_INT32 nIndex)
{
	if (IsValid(nIndex))
	{
		this->GetPlateRect();
		CPDF_Rect rcItem = CFX_List::GetItemRect(nIndex);
		SetScrollPosY(rcItem.top);
	}
}

FX_INT32 CFX_ListCtrl::GetTopItem() const
{
	FX_INT32 nItemIndex = this->GetItemIndex(this->GetBTPoint());

	if (!IsItemVisible(nItemIndex) && IsItemVisible(nItemIndex + 1))
			nItemIndex += 1;

	return nItemIndex;
}

void CFX_ListCtrl::Empty()
{
	CFX_List::Empty();
	InvalidateItem(-1);
}

void CFX_ListCtrl::Cancel()
{
	m_aSelItems.DeselectAll();
}

FX_INT32 CFX_ListCtrl::GetItemIndex(const CPDF_Point & point) const
{
	return CFX_List::GetItemIndex(OutToIn(point));
}

CFX_WideString CFX_ListCtrl::GetText() const
{
	if (this->IsMultipleSel())
		return this->GetItemText(this->m_nCaretIndex);	
	else
		return this->GetItemText(this->m_nSelItem);
}

