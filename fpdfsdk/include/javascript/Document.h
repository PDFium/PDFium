// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_



class PrintParamsObj : public CJS_EmbedObj
{
public:
	PrintParamsObj(CJS_Object* pJSObject);
	virtual ~PrintParamsObj(){}
	
public:
	FX_BOOL bUI;
	int nStart;
	int nEnd;
	FX_BOOL bSilent;
	FX_BOOL bShrinkToFit;
	FX_BOOL bPrintAsImage;
	FX_BOOL bReverse;
	FX_BOOL bAnnotations;
};

class CJS_PrintParamsObj : public CJS_Object
{
public:
	CJS_PrintParamsObj(JSFXObject pObject) : CJS_Object(pObject) {}
	virtual ~CJS_PrintParamsObj(){}
	
	DECLARE_JS_CLASS(CJS_PrintParamsObj);
};


class Icon;
class Field;

struct IconElement
{
	IconElement() : IconName(L""), IconStream(NULL), NextIcon(NULL){}
	virtual ~IconElement()
	{
	}
	CFX_WideString	IconName;
	IconElement*	NextIcon;
	Icon*			IconStream;
};

class IconTree
{
public:
	IconTree():m_pHead(NULL), m_pEnd(NULL), m_iLength(0)
	{

	}

	virtual ~IconTree()
	{
	}

public:
	void			InsertIconElement(IconElement* pNewIcon);
	void			DeleteIconElement(CFX_WideString swIconName);
	void			DeleteIconTree();
	int				GetLength();
	IconElement*	operator[](int iIndex);

private:
	IconElement*	m_pHead;
	IconElement*	m_pEnd;
	int				m_iLength;
};

struct CJS_DelayData;
struct CJS_DelayAnnot;
struct CJS_AnnotObj;

class Document : public CJS_EmbedObj
{
public:
	Document(CJS_Object* pJSObject);
	virtual ~Document();

public:
	FX_BOOL	ADBE(OBJ_PROP_PARAMS);
	FX_BOOL	author(OBJ_PROP_PARAMS);
	FX_BOOL	baseURL(OBJ_PROP_PARAMS);
	FX_BOOL	bookmarkRoot(OBJ_PROP_PARAMS);
	FX_BOOL	calculate(OBJ_PROP_PARAMS);
	FX_BOOL	Collab(OBJ_PROP_PARAMS);
	FX_BOOL	creationDate(OBJ_PROP_PARAMS);
	FX_BOOL	creator(OBJ_PROP_PARAMS);
	FX_BOOL	delay(OBJ_PROP_PARAMS);
	FX_BOOL	dirty(OBJ_PROP_PARAMS);
	FX_BOOL	documentFileName(OBJ_PROP_PARAMS);
	FX_BOOL external(OBJ_PROP_PARAMS);
	FX_BOOL	filesize(OBJ_PROP_PARAMS);
	FX_BOOL	icons(OBJ_PROP_PARAMS);
	FX_BOOL	info(OBJ_PROP_PARAMS);
	FX_BOOL	keywords(OBJ_PROP_PARAMS);
	FX_BOOL	layout(OBJ_PROP_PARAMS);
	FX_BOOL	media(OBJ_PROP_PARAMS);
	FX_BOOL	modDate(OBJ_PROP_PARAMS);
	FX_BOOL	mouseX(OBJ_PROP_PARAMS);
	FX_BOOL	mouseY(OBJ_PROP_PARAMS);
	FX_BOOL	numFields(OBJ_PROP_PARAMS);
	FX_BOOL	numPages(OBJ_PROP_PARAMS);
	FX_BOOL	pageNum(OBJ_PROP_PARAMS);
	FX_BOOL	pageWindowRect(OBJ_PROP_PARAMS);
	FX_BOOL	path(OBJ_PROP_PARAMS);
	FX_BOOL	producer(OBJ_PROP_PARAMS);
	FX_BOOL	subject(OBJ_PROP_PARAMS);
	FX_BOOL	title(OBJ_PROP_PARAMS);
	FX_BOOL	zoom(OBJ_PROP_PARAMS);
	FX_BOOL	zoomType(OBJ_PROP_PARAMS);

	FX_BOOL addAnnot(OBJ_METHOD_PARAMS);
	FX_BOOL	addField(OBJ_METHOD_PARAMS);
	FX_BOOL	addLink(OBJ_METHOD_PARAMS);
	FX_BOOL	addIcon(OBJ_METHOD_PARAMS);
	FX_BOOL	calculateNow(OBJ_METHOD_PARAMS);
	FX_BOOL	closeDoc(OBJ_METHOD_PARAMS);
	FX_BOOL	createDataObject(OBJ_METHOD_PARAMS);
	FX_BOOL deletePages(OBJ_METHOD_PARAMS);
	FX_BOOL	exportAsText(OBJ_METHOD_PARAMS);
	FX_BOOL	exportAsFDF(OBJ_METHOD_PARAMS);
	FX_BOOL	exportAsXFDF(OBJ_METHOD_PARAMS);
	FX_BOOL extractPages(OBJ_METHOD_PARAMS);
	FX_BOOL	getAnnot(OBJ_METHOD_PARAMS);
	FX_BOOL	getAnnots(OBJ_METHOD_PARAMS);
	FX_BOOL	getAnnot3D(OBJ_METHOD_PARAMS);
	FX_BOOL	getAnnots3D(OBJ_METHOD_PARAMS);
	FX_BOOL	getField(OBJ_METHOD_PARAMS);
	FX_BOOL	getIcon(OBJ_METHOD_PARAMS);
	FX_BOOL	getLinks(OBJ_METHOD_PARAMS);
	FX_BOOL	getNthFieldName(OBJ_METHOD_PARAMS);
	FX_BOOL	getOCGs(OBJ_METHOD_PARAMS);
	FX_BOOL	getPageBox(OBJ_METHOD_PARAMS);
	FX_BOOL	getPageNthWord(OBJ_METHOD_PARAMS);
	FX_BOOL	getPageNthWordQuads(OBJ_METHOD_PARAMS);
	FX_BOOL	getPageNumWords(OBJ_METHOD_PARAMS);
	FX_BOOL getPrintParams(OBJ_METHOD_PARAMS);
	FX_BOOL getURL(OBJ_METHOD_PARAMS);
	FX_BOOL	importAnFDF(OBJ_METHOD_PARAMS);
	FX_BOOL	importAnXFDF(OBJ_METHOD_PARAMS);
	FX_BOOL	importTextData(OBJ_METHOD_PARAMS);
	FX_BOOL insertPages(OBJ_METHOD_PARAMS);
	FX_BOOL	mailForm(OBJ_METHOD_PARAMS);
	FX_BOOL	print(OBJ_METHOD_PARAMS);
	FX_BOOL	removeField(OBJ_METHOD_PARAMS);
	FX_BOOL replacePages(OBJ_METHOD_PARAMS);
	FX_BOOL	resetForm(OBJ_METHOD_PARAMS);
	FX_BOOL	saveAs(OBJ_METHOD_PARAMS);
	FX_BOOL	submitForm(OBJ_METHOD_PARAMS);
	FX_BOOL	mailDoc(OBJ_METHOD_PARAMS);
	FX_BOOL	removeIcon(OBJ_METHOD_PARAMS);
	
public:
	void AttachDoc(CPDFSDK_Document* pDoc);
	CPDFSDK_Document* GetReaderDoc();

	static FX_BOOL				ExtractFileName(CPDFSDK_Document* pDoc, CFX_ByteString& strFileName);
	static FX_BOOL				ExtractFolderName(CPDFSDK_Document* pDoc, CFX_ByteString& strFolderName);

public:
	void						AddDelayData(CJS_DelayData* pData);
	void						DoFieldDelay(const CFX_WideString& sFieldName, int nControlIndex);

	void						AddDelayAnnotData(CJS_AnnotObj *pData);
	void						DoAnnotDelay();
	void						SetIsolate(v8::Isolate* isolate) {m_isolate = isolate;}

private:
	CFX_WideString				ReversalStr(CFX_WideString cbFrom);
	CFX_WideString				CutString(CFX_WideString cbFrom);
	bool						IsEnclosedInRect(CFX_FloatRect rect, CFX_FloatRect LinkRect);
	int							CountWords(CPDF_TextObject* pTextObj);
	CFX_WideString				GetObjWordStr(CPDF_TextObject* pTextObj, int nWordIndex);

	FX_BOOL						ParserParams(JSObject *pObj,CJS_AnnotObj& annotobj);

private:
	v8::Isolate*					m_isolate;
	IconTree*					m_pIconTree;
	CPDFSDK_Document*			m_pDocument;
	CFX_WideString				m_cwBaseURL;

	FX_BOOL								m_bDelay;
	CFX_ArrayTemplate<CJS_DelayData*>	m_DelayData;
	CFX_ArrayTemplate<CJS_AnnotObj*>	m_DelayAnnotData;
};

class CJS_Document : public CJS_Object
{
public:
	CJS_Document(JSFXObject pObject) : CJS_Object(pObject) {};
	virtual ~CJS_Document(){};

	virtual FX_BOOL	InitInstance(IFXJS_Context* cc);	

	DECLARE_JS_CLASS(CJS_Document);

	JS_STATIC_PROP(ADBE, Document);
	JS_STATIC_PROP(author, Document);
	JS_STATIC_PROP(baseURL, Document);
	JS_STATIC_PROP(bookmarkRoot, Document);
	JS_STATIC_PROP(calculate, Document);
	JS_STATIC_PROP(Collab, Document);
	JS_STATIC_PROP(creationDate, Document);
	JS_STATIC_PROP(creator, Document);
	JS_STATIC_PROP(delay, Document);
	JS_STATIC_PROP(dirty, Document);
	JS_STATIC_PROP(documentFileName, Document);
	JS_STATIC_PROP(external, Document);
	JS_STATIC_PROP(filesize, Document);
	JS_STATIC_PROP(icons, Document);
	JS_STATIC_PROP(info, Document);
	JS_STATIC_PROP(keywords, Document);
	JS_STATIC_PROP(layout, Document);
	JS_STATIC_PROP(media, Document);
	JS_STATIC_PROP(modDate, Document);
	JS_STATIC_PROP(mouseX, Document);
	JS_STATIC_PROP(mouseY, Document);
	JS_STATIC_PROP(numFields, Document);
	JS_STATIC_PROP(numPages, Document);
	JS_STATIC_PROP(pageNum, Document);
	JS_STATIC_PROP(pageWindowRect, Document);
	JS_STATIC_PROP(path, Document);
	JS_STATIC_PROP(producer, Document);
	JS_STATIC_PROP(subject, Document);
	JS_STATIC_PROP(title, Document);
	JS_STATIC_PROP(zoom, Document);
	JS_STATIC_PROP(zoomType, Document);

	JS_STATIC_METHOD(addAnnot,Document);
	JS_STATIC_METHOD(addField, Document);
	JS_STATIC_METHOD(addLink, Document);
	JS_STATIC_METHOD(addIcon, Document);
	JS_STATIC_METHOD(calculateNow, Document);
	JS_STATIC_METHOD(closeDoc, Document);
	JS_STATIC_METHOD(createDataObject, Document);
	JS_STATIC_METHOD(deletePages, Document);
	JS_STATIC_METHOD(exportAsText, Document);
	JS_STATIC_METHOD(exportAsFDF, Document);
	JS_STATIC_METHOD(exportAsXFDF, Document);
	JS_STATIC_METHOD(extractPages, Document);
	JS_STATIC_METHOD(getAnnot, Document);
	JS_STATIC_METHOD(getAnnots, Document);
	JS_STATIC_METHOD(getAnnot3D, Document);
	JS_STATIC_METHOD(getAnnots3D, Document);
	JS_STATIC_METHOD(getField, Document);
	JS_STATIC_METHOD(getIcon, Document);
	JS_STATIC_METHOD(getLinks, Document);
	JS_STATIC_METHOD(getNthFieldName, Document);
	JS_STATIC_METHOD(getOCGs, Document);
	JS_STATIC_METHOD(getPageBox, Document);
	JS_STATIC_METHOD(getPageNthWord, Document);
	JS_STATIC_METHOD(getPageNthWordQuads, Document);
	JS_STATIC_METHOD(getPageNumWords, Document);
	JS_STATIC_METHOD(getPrintParams, Document);
	JS_STATIC_METHOD(getURL, Document);
	JS_STATIC_METHOD(importAnFDF, Document);
	JS_STATIC_METHOD(importAnXFDF, Document);
	JS_STATIC_METHOD(importTextData, Document);
	JS_STATIC_METHOD(insertPages, Document);
	JS_STATIC_METHOD(mailForm, Document);
	JS_STATIC_METHOD(print, Document);
	JS_STATIC_METHOD(removeField, Document);
	JS_STATIC_METHOD(replacePages, Document);
	JS_STATIC_METHOD(removeIcon, Document);
	JS_STATIC_METHOD(resetForm, Document);
	JS_STATIC_METHOD(saveAs, Document);
	JS_STATIC_METHOD(submitForm, Document);
	JS_STATIC_METHOD(mailDoc, Document);
};

#endif//_DOCUMENT_H_

