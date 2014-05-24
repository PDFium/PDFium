// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "../../../include/fpdfapi/fpdf_page.h"
#include "../../../include/fpdfapi/fpdf_module.h"
#include "pageint.h"
#if defined(_FPDFAPI_MINI_)
extern const FX_LPCSTR _PDF_CharType;
void CPDF_StreamContentParser::InputData(FX_LPCBYTE src_buf, FX_DWORD src_size)
{
    if (m_Level > _FPDF_MAX_FORM_LEVEL_) {
        return;
    }
    for (FX_DWORD i = 0; i < src_size; i ++) {
        int ch = src_buf[i];
        int type = _PDF_CharType[ch];
start:
        switch (m_WordState) {
            case 0:
                if (type == 'W') {
                } else if (type == 'N') {
                    m_WordState = 5;
                    m_pWordBuf[0] = ch;
                    m_WordSize = 1;
                } else if (type == 'R') {
                    m_WordState = 4;
                    m_pWordBuf[0] = ch;
                    m_WordSize = 1;
                } else switch (ch) {
                        case '/':
                            m_WordState = 2;
                            m_WordSize = 0;
                            break;
                        case '[':
                            StartArray();
                            break;
                        case ']':
                            EndArray();
                            break;
                        case '(':
                            m_WordState = 7;
                            m_StringLevel = 1;
                            m_StringState = 0;
                            m_StringBuf.Clear();
                            break;
                        case '<':
                            m_WordState = 3;
                            break;
                        case '>':
                            m_WordState = 8;
                            break;
                        case '%':
                            m_WordState = 1;
                            break;
                    }
                break;
            case 1:
                if (ch == '\n' || ch == '\r') {
                    m_WordState = 0;
                }
                break;
            case 2:
                if (type != 'R' && type != 'N') {
                    EndName();
                    m_WordState = 0;
                    goto start;
                }
                if (m_WordSize < 256) {
                    m_pWordBuf[m_WordSize++] = ch;
                }
                break;
            case 3:
                if (ch == '<') {
                    StartDict();
                    m_WordState = 0;
                } else {
                    m_StringBuf.Clear();
                    m_WordState = 6;
                    goto start;
                }
                break;
            case 4:
                if (type != 'R' && type != 'N') {
                    m_WordState = 0;
                    EndKeyword();
                    if (m_bAbort) {
                        return;
                    }
                    goto start;
                }
                if (m_WordSize < 256) {
                    m_pWordBuf[m_WordSize++] = ch;
                }
                break;
            case 5:
                if (type != 'N') {
                    EndNumber();
                    m_WordState = 0;
                    goto start;
                }
                if (m_WordSize < 256) {
                    m_pWordBuf[m_WordSize++] = ch;
                }
                break;
            case 6:
                if (ch == '>') {
                    EndHexString();
                    m_WordState = 0;
                } else {
                    m_StringBuf.AppendByte(ch);
                }
                break;
            case 7:
                switch (m_StringState) {
                    case 0:
                        if (ch == ')') {
                            m_StringLevel --;
                            if (m_StringLevel == 0) {
                                EndString();
                                m_WordState = 0;
                                break;
                            }
                            m_StringBuf.AppendByte(')');
                        } else if (ch == '(') {
                            m_StringLevel ++;
                            m_StringBuf.AppendByte('(');
                        } else if (ch == '\\') {
                            m_StringState = 1;
                        } else {
                            m_StringBuf.AppendByte((char)ch);
                        }
                        break;
                    case 1:
                        if (ch >= '0' && ch <= '7') {
                            m_EscCode = ch - '0';
                            m_StringState = 2;
                            break;
                        }
                        if (ch == 'n') {
                            m_StringBuf.AppendByte('\n');
                        } else if (ch == 'r') {
                            m_StringBuf.AppendByte('\r');
                        } else if (ch == 't') {
                            m_StringBuf.AppendByte('\t');
                        } else if (ch == 'b') {
                            m_StringBuf.AppendByte('\b');
                        } else if (ch == 'f') {
                            m_StringBuf.AppendByte('\f');
                        } else if (ch == '\\') {
                            m_StringBuf.AppendByte('\\');
                        } else if (ch == '(') {
                            m_StringBuf.AppendByte('(');
                        } else if (ch == ')') {
                            m_StringBuf.AppendByte(')');
                        } else if (ch == '\r') {
                            m_StringState = 4;
                            break;
                        } else if (ch == '\n') {
                        } else {
                            m_StringBuf.AppendByte(ch);
                        }
                        m_StringState = 0;
                        break;
                    case 2:
                        if (ch >= '0' && ch <= '7') {
                            m_EscCode = m_EscCode * 8 + ch - '0';
                            m_StringState = 3;
                        } else {
                            m_StringBuf.AppendByte(m_EscCode);
                            m_StringState = 0;
                            goto start;
                        }
                        break;
                    case 3:
                        if (ch >= '0' && ch <= '7') {
                            m_EscCode = m_EscCode * 8 + ch - '0';
                            m_StringBuf.AppendByte(m_EscCode);
                            m_StringState = 0;
                        } else {
                            m_StringBuf.AppendByte(m_EscCode);
                            m_StringState = 0;
                            goto start;
                        }
                        break;
                    case 4:
                        m_StringState = 0;
                        if (ch != '\n') {
                            goto start;
                        }
                        break;
                }
                break;
            case 8:
                m_WordState = 0;
                if (ch == '>') {
                    EndDict();
                } else {
                    goto start;
                }
                break;
            case 9:
                switch (m_InlineImageState) {
                    case 0:
                        if (type == 'W' || type == 'D') {
                            m_InlineImageState = 1;
                            m_InlineWhiteChar = ch;
                        } else {
                            m_StringBuf.AppendByte(ch);
                        }
                        break;
                    case 1:
                        m_StringBuf.AppendByte(m_InlineWhiteChar);
                        if (ch == 'I') {
                            m_InlineImageState = 2;
                        } else {
                            m_InlineImageState = 0;
                            goto start;
                        }
                        break;
                    case 2:
                        if (ch == 'D') {
                            m_InlineImageState = 3;
                        } else {
                            m_StringBuf.AppendByte('I');
                            m_InlineImageState = 0;
                            goto start;
                        }
                        break;
                    case 3:
                        EndImageDict();
                        break;
                }
                break;
            case 10:
                switch (m_InlineImageState) {
                    case 0:
                        if (type == 'W') {
                            m_InlineImageState = 1;
                            m_InlineWhiteChar = ch;
                        } else {
                            m_ImageSrcBuf.AppendByte(ch);
                        }
                        break;
                    case 1:
                        if (ch == 'E') {
                            m_InlineImageState = 2;
                        } else {
                            m_ImageSrcBuf.AppendByte(m_InlineWhiteChar);
                            m_InlineImageState = 0;
                            goto start;
                        }
                        break;
                    case 2:
                        if (ch == 'I') {
                            m_InlineImageState = 3;
                        } else {
                            m_ImageSrcBuf.AppendByte(m_InlineWhiteChar);
                            m_ImageSrcBuf.AppendByte('E');
                            m_InlineImageState = 0;
                            goto start;
                        }
                        break;
                    case 3:
                        if (type == 'W') {
                            EndInlineImage();
                        } else {
                            m_ImageSrcBuf.AppendByte(m_InlineWhiteChar);
                            m_ImageSrcBuf.AppendByte('E');
                            m_ImageSrcBuf.AppendByte('I');
                            m_InlineImageState = 0;
                            goto start;
                        }
                        break;
                }
                break;
            case 11:
                if (m_InlineImageState < m_ImageSrcBuf.GetSize()) {
                    m_ImageSrcBuf.GetBuffer()[m_InlineImageState ++] = ch;
                } else {
                    if (ch == 'I') {
                        EndInlineImage();
                    }
                }
                break;
        }
    }
}
void CPDF_StreamContentParser::Finish()
{
    switch (m_WordState) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            EndName();
            break;
        case 3:
            break;
        case 4:
            EndKeyword();
            break;
        case 5:
            EndNumber();
            break;
        case 6:
            EndHexString();
            break;
        case 7:
            EndString();
            break;
        case 8:
            break;
        case 9:
            break;
        case 10:
            EndInlineImage();
            break;
    }
    m_WordState = 0;
}
void CPDF_StreamContentParser::AddContainer(CPDF_Object* pObject)
{
    if (m_ObjectSize) {
        m_pObjectState[m_ObjectSize] = SetToCurObj(pObject);
    }
    FXSYS_assert(m_ObjectSize < _FPDF_MAX_OBJECT_STACK_SIZE_);
    m_pObjectStack[m_ObjectSize++] = pObject;
}
FX_BOOL CPDF_StreamContentParser::SetToCurObj(CPDF_Object* pObject)
{
    if (m_ObjectSize == 0) {
        AddObjectParam(pObject);
        return TRUE;
    }
    FX_BOOL bInArrayOrDict = TRUE;
    CPDF_Object* pCurObj = m_pObjectStack[m_ObjectSize - 1];
    if (pCurObj->GetType() == PDFOBJ_ARRAY) {
        ((CPDF_Array*)pCurObj)->Add(pObject, m_pDocument);
    } else {
        if (!m_bDictName && m_pDictName[0]) {
            ((CPDF_Dictionary*)pCurObj)->SetAt((FX_LPCSTR)m_pDictName, pObject, m_pDocument);
        } else {
            bInArrayOrDict = FALSE;
        }
        m_bDictName = TRUE;
    }
    return bInArrayOrDict;
}
void CPDF_StreamContentParser::StartArray()
{
    if (m_ObjectSize)
        if (m_pObjectStack[0]->GetType() != PDFOBJ_DICTIONARY && m_pObjectStack[m_ObjectSize - 1]->GetType() == PDFOBJ_ARRAY) {
            return;
        }
    CPDF_Array* pArray = FX_NEW CPDF_Array;
    AddContainer(pArray);
}
void CPDF_StreamContentParser::EndArray()
{
    if (m_ObjectSize == 0) {
        return;
    }
    CPDF_Object* pCurObj = m_pObjectStack[m_ObjectSize - 1];
    if (pCurObj->GetType() != PDFOBJ_ARRAY) {
        return;
    }
    m_ObjectSize --;
    if (m_ObjectSize == 0) {
        AddObjectParam(pCurObj);
    } else {
        if (!m_pObjectState[m_ObjectSize]) {
            pCurObj->Release();
        }
    }
    m_pObjectState[m_ObjectSize] = FALSE;
}
void CPDF_StreamContentParser::StartDict()
{
    CPDF_Dictionary* pDict = FX_NEW CPDF_Dictionary;
    AddContainer(pDict);
    m_bDictName = TRUE;
}
void CPDF_StreamContentParser::EndDict()
{
    if (m_ObjectSize == 0) {
        return;
    }
    CPDF_Object* pCurObj = m_pObjectStack[m_ObjectSize - 1];
    if (pCurObj->GetType() != PDFOBJ_DICTIONARY) {
        return;
    }
    m_ObjectSize --;
    if (m_ObjectSize == 0) {
        AddObjectParam(pCurObj);
    } else {
        if (!m_pObjectState[m_ObjectSize]) {
            pCurObj->Release();
        }
    }
    m_pObjectState[m_ObjectSize] = FALSE;
}
void CPDF_StreamContentParser::EndName()
{
    if (m_ObjectSize == 0) {
        AddNameParam((FX_LPCSTR)m_pWordBuf, m_WordSize);
        return;
    }
    CPDF_Object* pCurObj = m_pObjectStack[m_ObjectSize - 1];
    if (pCurObj->GetType() == PDFOBJ_ARRAY) {
        ((CPDF_Array*)pCurObj)->AddName(CFX_ByteString(m_pWordBuf, m_WordSize));
    } else {
        if (m_bDictName) {
            FXSYS_memcpy32(m_pDictName, m_pWordBuf, m_WordSize);
            m_pDictName[m_WordSize] = 0;
        } else {
            if (m_pDictName[0] != 0) {
                ((CPDF_Dictionary*)pCurObj)->SetAtName((FX_LPCSTR)m_pDictName, CFX_ByteString(m_pWordBuf, m_WordSize));
            }
        }
        m_bDictName = !m_bDictName;
    }
}
void CPDF_StreamContentParser::EndNumber()
{
    if (m_ObjectSize == 0) {
        AddNumberParam((FX_LPCSTR)m_pWordBuf, m_WordSize);
        return;
    }
    CPDF_Number *pObj = FX_NEW CPDF_Number(CFX_ByteStringC(m_pWordBuf, m_WordSize));
    if (!SetToCurObj(pObj)) {
        pObj->Release();
    }
}
extern CFX_ByteString _FPDF_ByteStringFromHex(CFX_BinaryBuf& src_buf);
void CPDF_StreamContentParser::EndHexString()
{
    CPDF_String *pObj = FX_NEW CPDF_String(_FPDF_ByteStringFromHex(m_StringBuf), TRUE);
    if (!SetToCurObj(pObj)) {
        pObj->Release();
    }
}
void CPDF_StreamContentParser::EndString()
{
    CPDF_String *pObj = FX_NEW CPDF_String(m_StringBuf.GetByteString());
    if (!SetToCurObj(pObj)) {
        pObj->Release();
    }
}
void CPDF_StreamContentParser::Handle_BeginImage(void)
{
    m_WordState = 9;
    m_InlineImageState = 0;
    m_StringBuf.Clear();
}
void _PDF_ReplaceAbbr(CPDF_Object* pObj);
void CPDF_StreamContentParser::EndImageDict()
{
    if (m_StringBuf.GetSize() != m_LastImageDict.GetSize() ||
            FXSYS_memcmp32(m_StringBuf.GetBuffer(), m_LastImageDict.GetBuffer(), m_StringBuf.GetSize())) {
        m_WordState = 0;
        StartDict();
        InputData(m_StringBuf.GetBuffer(), m_StringBuf.GetSize());
        Finish();
        m_bSameLastDict = FALSE;
        if (m_pLastImageDict && m_bReleaseLastDict) {
            m_pLastImageDict->Release();
            m_pLastImageDict = NULL;
        }
        if (!m_ObjectSize) {
            m_InlineImageState = 0;
            return;
        }
        m_pLastImageDict = (CPDF_Dictionary*)m_pObjectStack[--m_ObjectSize];
        m_bReleaseLastDict = !m_pObjectState[m_ObjectSize];
        m_pObjectState[m_ObjectSize] = FALSE;
        _PDF_ReplaceAbbr(m_pLastImageDict);
        m_LastImageDict.TakeOver(m_StringBuf);
        if (m_pLastImageDict->KeyExist(FX_BSTRC("ColorSpace"))) {
            CPDF_Object* pCSObj = m_pLastImageDict->GetElementValue(FX_BSTRC("ColorSpace"));
            if (pCSObj->GetType() == PDFOBJ_NAME) {
                CFX_ByteString name = pCSObj->GetString();
                if (name != FX_BSTRC("DeviceRGB") && name != FX_BSTRC("DeviceGray") && name != FX_BSTRC("DeviceCMYK")) {
                    pCSObj = FindResourceObj(FX_BSTRC("ColorSpace"), name);
                    if (pCSObj) {
                        if (!pCSObj->GetObjNum()) {
                            pCSObj = pCSObj->Clone();
                        }
                        m_pLastImageDict->SetAt(FX_BSTRC("ColorSpace"), pCSObj, m_pDocument);
                    }
                }
            }
        }
    } else {
        m_bSameLastDict = TRUE;
    }
    m_ImageSrcBuf.Clear();
    if (m_pLastCloneImageDict) {
        m_pLastCloneImageDict->Release();
    }
    m_pLastCloneImageDict = (CPDF_Dictionary*)m_pLastImageDict->Clone();
    if (m_pLastCloneImageDict->KeyExist(FX_BSTRC("Filter"))) {
        m_WordState = 10;
        m_InlineImageState = 0;
    } else {
        int width = m_pLastCloneImageDict->GetInteger(FX_BSTRC("Width"));
        int height = m_pLastCloneImageDict->GetInteger(FX_BSTRC("Height"));
        int OrigSize = 0;
        CPDF_Object* pCSObj = m_pLastCloneImageDict->GetElementValue(FX_BSTRC("ColorSpace"));
        if (pCSObj != NULL) {
            int bpc = m_pLastCloneImageDict->GetInteger(FX_BSTRC("BitsPerComponent"));
            int nComponents = 1;
            CPDF_ColorSpace* pCS = m_pDocument->LoadColorSpace(pCSObj);
            if (pCS == NULL) {
                nComponents = 3;
            } else {
                nComponents = pCS->CountComponents();
                m_pDocument->GetPageData()->ReleaseColorSpace(pCSObj);
            }
            int pitch = (width * bpc * nComponents + 7) / 8;
            OrigSize = pitch * height;
        } else {
            OrigSize = ((width + 7) / 8) * height;
        }
        m_ImageSrcBuf.AppendBlock(NULL, OrigSize);
        m_WordState = 11;
        m_InlineImageState = 0;
    }
}
void CPDF_StreamContentParser::EndInlineImage()
{
    CFX_AffineMatrix ImageMatrix;
    ImageMatrix.Copy(m_pCurStates->m_CTM);
    ImageMatrix.Concat(m_mtContentToUser);
    m_LastImageData.CopyData(m_ImageSrcBuf.GetBuffer(), m_ImageSrcBuf.GetSize());
    CPDF_Stream* pStream = CPDF_Stream::Create(m_ImageSrcBuf.GetBuffer(), m_ImageSrcBuf.GetSize(),
                           m_pLastCloneImageDict);
    m_ImageSrcBuf.DetachBuffer();
    m_pLastCloneImageDict = NULL;
    CPDF_InlineImages* pImages = FX_NEW CPDF_InlineImages;
    pImages->m_pStream = pStream;
    SetGraphicStates(pImages, !m_pLastCloneImageDict->KeyExist(FX_BSTRC("ColorSpace")), FALSE, FALSE);
    pImages->AddMatrix(ImageMatrix);
    m_pObjectList->m_ObjectList.AddTail(pImages);
    m_WordState = 0;
}
#define FXDWORD_TRUE FXDWORD_FROM_LSBFIRST(0x65757274)
#define FXDWORD_NULL FXDWORD_FROM_LSBFIRST(0x6c6c756e)
#define FXDWORD_FALS FXDWORD_FROM_LSBFIRST(0x736c6166)
void CPDF_StreamContentParser::EndKeyword()
{
    CPDF_Object *pObj = NULL;
    if (m_WordSize == 4) {
        if (*(FX_DWORD*)m_pWordBuf == FXDWORD_TRUE) {
            pObj = CPDF_Boolean::Create(TRUE);
            if (!SetToCurObj(pObj)) {
                pObj->Release();
            }
            return;
        } else if (*(FX_DWORD*)m_pWordBuf == FXDWORD_NULL) {
            pObj = CPDF_Null::Create();
            if (!SetToCurObj(pObj)) {
                pObj->Release();
            }
            return;
        }
    } else if (m_WordSize == 5) {
        if (*(FX_DWORD*)m_pWordBuf == FXDWORD_FALS && m_pWordBuf[4] == 'e') {
            pObj = CPDF_Boolean::Create(FALSE);
            if (!SetToCurObj(pObj)) {
                pObj->Release();
            }
            return;
        }
    }
    m_pWordBuf[m_WordSize] = 0;
    OnOperator((char*)m_pWordBuf);
    ClearAllParams();
}
#define PAGEPARSE_STAGE_PARSE			2
#define PAGEPARSE_STAGE_CHECKCLIP		3
CPDF_ContentParser::CPDF_ContentParser()
{
    m_pParser = NULL;
    m_Status = Ready;
    m_pStreamFilter = NULL;
    m_pType3Char = NULL;
}
CPDF_ContentParser::~CPDF_ContentParser()
{
    Clear();
}
void CPDF_ContentParser::Clear()
{
    if (m_pParser) {
        delete m_pParser;
    }
    if (m_pStreamFilter) {
        delete m_pStreamFilter;
    }
    m_pParser = NULL;
    m_Status = Ready;
}
void CPDF_ContentParser::Start(CPDF_Page* pPage, CPDF_ParseOptions* pOptions)
{
    if (m_Status != Ready || pPage == NULL || pPage->m_pDocument == NULL || pPage->m_pFormDict == NULL) {
        m_Status = Done;
        return;
    }
    m_pObjects = pPage;
    m_bForm = FALSE;
    if (pOptions) {
        m_Options = *pOptions;
    }
    CPDF_Object* pContent = pPage->m_pFormDict->GetElementValue(FX_BSTRC("Contents"));
    if (pContent == NULL) {
        m_Status = Done;
        return;
    }
    if (pContent->GetType() == PDFOBJ_STREAM) {
        m_nStreams = 1;
    } else if (pContent->GetType() == PDFOBJ_ARRAY) {
        m_nStreams = ((CPDF_Array*)pContent)->GetCount();
    } else {
        m_Status = Done;
        return;
    }
    m_Status = ToBeContinued;
    m_InternalStage = PAGEPARSE_STAGE_PARSE;
    m_CurrentOffset = 0;
    m_pParser = FX_NEW CPDF_StreamContentParser;
    m_pParser->Initialize();
    m_pParser->PrepareParse(pPage->m_pDocument, pPage->m_pResources, NULL, NULL, pPage,
                            pPage->m_pResources, &pPage->m_BBox, &m_Options, NULL, 0);
    m_pParser->m_pCurStates->m_ColorState.GetModify()->Default();
}
void CPDF_ContentParser::Start(CPDF_Form* pForm, CPDF_AllStates* pGraphicStates, CFX_AffineMatrix* pParentMatrix,
                               CPDF_Type3Char* pType3Char, CPDF_ParseOptions* pOptions, int level)
{
    m_pType3Char = pType3Char;
    m_pObjects = pForm;
    m_bForm = TRUE;
    CFX_AffineMatrix form_matrix = pForm->m_pFormDict->GetMatrix(FX_BSTRC("Matrix"));
    if (pGraphicStates) {
        form_matrix.Concat(pGraphicStates->m_CTM);
    }
    CPDF_Array* pBBox = pForm->m_pFormDict->GetArray(FX_BSTRC("BBox"));
    CFX_FloatRect form_bbox;
    CPDF_Path ClipPath;
    if (pBBox) {
        form_bbox = pBBox->GetRect();
        ClipPath.New();
        ClipPath.AppendRect(form_bbox.left, form_bbox.bottom, form_bbox.right, form_bbox.top);
        ClipPath.Transform(&form_matrix);
        if (pParentMatrix) {
            ClipPath.Transform(pParentMatrix);
        }
        form_bbox.Transform(&form_matrix);
    }
    CPDF_Dictionary* pResources = pForm->m_pFormDict->GetDict(FX_BSTRC("Resources"));
    m_pParser = FX_NEW CPDF_StreamContentParser;
    m_pParser->Initialize();
    m_pParser->PrepareParse(pForm->m_pDocument, pForm->m_pPageResources, pForm->m_pResources, pParentMatrix, pForm,
                            pResources, &form_bbox, pOptions, pGraphicStates, level);
    m_pParser->m_pCurStates->m_CTM = form_matrix;
    if (ClipPath.NotNull()) {
        m_pParser->m_pCurStates->m_ClipPath.AppendPath(ClipPath, FXFILL_WINDING, TRUE);
    }
    if (pForm->m_Transparency & PDFTRANS_GROUP) {
        CPDF_GeneralStateData* pData = m_pParser->m_pCurStates->m_GeneralState.GetModify();
        pData->m_BlendType = FXDIB_BLEND_NORMAL;
        pData->m_StrokeAlpha = 1.0f;
        pData->m_FillAlpha = 1.0f;
        pData->m_pSoftMask = NULL;
    }
    m_pStreamFilter = pForm->m_pFormStream->GetStreamFilter();
    m_nStreams = 1;
    m_Status = ToBeContinued;
    m_InternalStage = PAGEPARSE_STAGE_PARSE;
    m_CurrentOffset = 0;
}
void CPDF_ContentParser::Continue(IFX_Pause* pPause)
{
    while (m_Status == ToBeContinued) {
        if (m_InternalStage == PAGEPARSE_STAGE_PARSE) {
            if (m_pStreamFilter == NULL) {
                if (m_CurrentOffset == m_nStreams) {
                    m_InternalStage = PAGEPARSE_STAGE_CHECKCLIP;
                    if (m_pType3Char) {
                        m_pType3Char->m_bColored = m_pParser->m_bColored;
                        m_pType3Char->m_Width = FXSYS_round(m_pParser->m_Type3Data[0] * 1000);
                        m_pType3Char->m_BBox.left = FXSYS_round(m_pParser->m_Type3Data[2] * 1000);
                        m_pType3Char->m_BBox.bottom = FXSYS_round(m_pParser->m_Type3Data[3] * 1000);
                        m_pType3Char->m_BBox.right = FXSYS_round(m_pParser->m_Type3Data[4] * 1000);
                        m_pType3Char->m_BBox.top = FXSYS_round(m_pParser->m_Type3Data[5] * 1000);
                        m_pType3Char->m_bPageRequired = m_pParser->m_bResourceMissing;
                    }
                    delete m_pParser;
                    m_pParser = NULL;
                    continue;
                }
                CPDF_Object* pContent = m_pObjects->m_pFormDict->GetElementValue(FX_BSTRC("Contents"));
                if (pContent->GetType() == PDFOBJ_STREAM) {
                    m_pStreamFilter = ((CPDF_Stream*)pContent)->GetStreamFilter();
                } else {
                    CPDF_Stream* pStream = ((CPDF_Array*)pContent)->GetStream(m_CurrentOffset);
                    if (pStream == NULL) {
                        m_CurrentOffset ++;
                        continue;
                    }
                    m_pStreamFilter = pStream->GetStreamFilter();
                }
            }
            FX_DWORD len = m_pStreamFilter->ReadBlock(m_pParser->m_pStreamBuf, STREAM_PARSE_BUFSIZE);
            m_pParser->InputData(m_pParser->m_pStreamBuf, len);
            if (m_pParser->m_bAbort) {
                delete m_pStreamFilter;
                m_pStreamFilter = NULL;
                m_Status = Done;
                delete m_pParser;
                m_pParser = NULL;
                return;
            }
            if (len < STREAM_PARSE_BUFSIZE) {
                m_pParser->Finish();
                m_CurrentOffset ++;
                delete m_pStreamFilter;
                m_pStreamFilter = NULL;
            }
            if (pPause && pPause->NeedToPauseNow()) {
                return;
            }
        }
        if (m_InternalStage == PAGEPARSE_STAGE_CHECKCLIP) {
            FX_POSITION pos = m_pObjects->m_ObjectList.GetHeadPosition();
            while (pos) {
                CPDF_PageObject* pObj = (CPDF_PageObject*)m_pObjects->m_ObjectList.GetNext(pos);
                if (pObj == NULL) {
                    continue;
                }
                if (pObj->m_ClipPath.IsNull()) {
                    continue;
                }
                if (pObj->m_ClipPath.GetPathCount() != 1) {
                    continue;
                }
                if (pObj->m_ClipPath.GetTextCount()) {
                    continue;
                }
                CPDF_Path ClipPath = pObj->m_ClipPath.GetPath(0);
                if (!ClipPath.IsRect() || pObj->m_Type == PDFPAGE_SHADING) {
                    continue;
                }
                CFX_FloatRect old_rect(ClipPath.GetPointX(0), ClipPath.GetPointY(0),
                                       ClipPath.GetPointX(2), ClipPath.GetPointY(2));
                CFX_FloatRect obj_rect(pObj->m_Left, pObj->m_Bottom, pObj->m_Right, pObj->m_Top);
                if (old_rect.Contains(obj_rect)) {
                    pObj->m_ClipPath.SetNull();
                }
            }
            if (m_pObjects->m_ObjectList.GetCount() == 1) {
                CPDF_PageObject* pObj = (CPDF_PageObject*)m_pObjects->m_ObjectList.GetAt(m_pObjects->m_ObjectList.GetHeadPosition());
                if (pObj && pObj->m_Type == PDFPAGE_TEXT) {
                    CPDF_TextObject* pText = (CPDF_TextObject*)pObj;
                }
            }
            m_Status = Done;
            return;
        }
    }
}
int CPDF_ContentParser::EstimateProgress()
{
    if (m_Status == Ready) {
        return 0;
    }
    if (m_Status == Done) {
        return 100;
    }
    if (m_InternalStage == PAGEPARSE_STAGE_CHECKCLIP) {
        return 90;
    }
    if (m_pStreamFilter == NULL) {
        return 90 * m_CurrentOffset / m_nStreams;
    }
    int total_raw_size = m_pStreamFilter->GetStream()->GetRawSize() * m_nStreams;
    int parsed_raw_size = m_pStreamFilter->GetStream()->GetRawSize() * m_CurrentOffset +
                          m_pStreamFilter->GetSrcPos();
    return 90 * parsed_raw_size / total_raw_size;
}
CPDF_InlineImages::CPDF_InlineImages()
{
    m_Type = PDFPAGE_INLINES;
    m_pStream = NULL;
    m_pBitmap = NULL;
}
CPDF_InlineImages::~CPDF_InlineImages()
{
    if (m_pStream) {
        m_pStream->Release();
    }
    if (m_pBitmap) {
        delete m_pBitmap;
    }
}
void CPDF_InlineImages::AddMatrix(CFX_AffineMatrix& matrix)
{
    m_Matrices.Add(matrix);
    CFX_FloatRect rect = matrix.GetUnitRect();
    if (m_Matrices.GetSize() > 1) {
        CFX_FloatRect rect1(m_Left, m_Bottom, m_Right, m_Top);
        rect.Union(rect1);
    }
    m_Left = rect.left;
    m_Right = rect.right;
    m_Top = rect.top;
    m_Bottom = rect.bottom;
}
#endif
