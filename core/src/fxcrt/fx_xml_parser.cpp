// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "../../include/fxcrt/fx_xml.h"
#include "xml_int.h"
CXML_Parser::~CXML_Parser()
{
    if (m_bOwnedStream) {
        m_pDataAcc->Release();
    }
}
FX_BOOL CXML_Parser::Init(FX_LPBYTE pBuffer, size_t size)
{
    if (m_pAllocator) {
        m_pDataAcc = FX_NewAtAllocator(m_pAllocator)CXML_DataBufAcc(pBuffer, size, m_pAllocator);
    } else {
        m_pDataAcc = FX_NEW CXML_DataBufAcc(pBuffer, size, NULL);
    }
    if (!m_pDataAcc) {
        return FALSE;
    }
    return Init(TRUE);
}
FX_BOOL CXML_Parser::Init(IFX_FileRead *pFileRead)
{
    if (m_pAllocator) {
        m_pDataAcc = FX_NewAtAllocator(m_pAllocator)CXML_DataStmAcc(pFileRead, m_pAllocator);
    } else {
        m_pDataAcc = FX_NEW CXML_DataStmAcc(pFileRead, NULL);
    }
    if (!m_pDataAcc) {
        return FALSE;
    }
    return Init(TRUE);
}
FX_BOOL CXML_Parser::Init(IFX_BufferRead *pBuffer)
{
    if (!pBuffer) {
        return FALSE;
    }
    m_pDataAcc = pBuffer;
    return Init(FALSE);
}
FX_BOOL CXML_Parser::Init(FX_BOOL bOwndedStream)
{
    m_bOwnedStream = bOwndedStream;
    m_nOffset = 0;
    return ReadNextBlock();
}
FX_BOOL CXML_Parser::ReadNextBlock()
{
    if (!m_pDataAcc->ReadNextBlock()) {
        return FALSE;
    }
    m_pBuffer = m_pDataAcc->GetBlockBuffer();
    m_dwBufferSize = m_pDataAcc->GetBlockSize();
    m_nBufferOffset = m_pDataAcc->GetBlockOffset();
    m_dwIndex = 0;
    return m_dwBufferSize > 0;
}
FX_BOOL CXML_Parser::IsEOF()
{
    if (!m_pDataAcc->IsEOF()) {
        return FALSE;
    }
    return m_dwIndex >= m_dwBufferSize;
}
#define FXCRTM_XML_CHARTYPE_Normal			0x00
#define FXCRTM_XML_CHARTYPE_SpaceChar		0x01
#define FXCRTM_XML_CHARTYPE_Letter			0x02
#define FXCRTM_XML_CHARTYPE_Digital			0x04
#define FXCRTM_XML_CHARTYPE_NameIntro		0x08
#define FXCRTM_XML_CHARTYPE_NameChar		0x10
#define FXCRTM_XML_CHARTYPE_HexDigital		0x20
#define FXCRTM_XML_CHARTYPE_HexLowerLetter	0x40
#define FXCRTM_XML_CHARTYPE_HexUpperLetter	0x60
#define FXCRTM_XML_CHARTYPE_HexChar			0x60
FX_BYTE g_FXCRT_XML_ByteTypes[256] = {
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x00,
    0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A,
    0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x18,
    0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A,
    0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A,
    0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A,
    0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A,
    0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A,
    0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A,
    0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A,
    0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A,
    0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x01, 0x01,
};
FX_BOOL g_FXCRT_XML_IsWhiteSpace(FX_BYTE ch)
{
    return (g_FXCRT_XML_ByteTypes[ch] & FXCRTM_XML_CHARTYPE_SpaceChar) != 0;
}
FX_BOOL g_FXCRT_XML_IsLetter(FX_BYTE ch)
{
    return (g_FXCRT_XML_ByteTypes[ch] & FXCRTM_XML_CHARTYPE_Letter) != 0;
}
FX_BOOL g_FXCRT_XML_IsDigital(FX_BYTE ch)
{
    return (g_FXCRT_XML_ByteTypes[ch] & FXCRTM_XML_CHARTYPE_Digital) != 0;
}
FX_BOOL g_FXCRT_XML_IsNameIntro(FX_BYTE ch)
{
    return (g_FXCRT_XML_ByteTypes[ch] & FXCRTM_XML_CHARTYPE_NameIntro) != 0;
}
FX_BOOL g_FXCRT_XML_IsNameChar(FX_BYTE ch)
{
    return (g_FXCRT_XML_ByteTypes[ch] & FXCRTM_XML_CHARTYPE_NameChar) != 0;
}
FX_BOOL g_FXCRT_XML_IsHexChar(FX_BYTE ch)
{
    return (g_FXCRT_XML_ByteTypes[ch] & FXCRTM_XML_CHARTYPE_HexChar) != 0;
}
void CXML_Parser::SkipWhiteSpaces()
{
    m_nOffset = m_nBufferOffset + (FX_FILESIZE)m_dwIndex;
    if (IsEOF()) {
        return;
    }
    do {
        while (m_dwIndex < m_dwBufferSize && g_FXCRT_XML_IsWhiteSpace(m_pBuffer[m_dwIndex])) {
            m_dwIndex ++;
        }
        m_nOffset = m_nBufferOffset + (FX_FILESIZE)m_dwIndex;
        if (m_dwIndex < m_dwBufferSize || IsEOF()) {
            break;
        }
    } while (ReadNextBlock());
}
void CXML_Parser::GetName(CFX_ByteStringL &space, CFX_ByteStringL &name)
{
    m_nOffset = m_nBufferOffset + (FX_FILESIZE)m_dwIndex;
    if (IsEOF()) {
        return;
    }
    CFX_ByteTextBuf buf(m_pAllocator);
    FX_BYTE ch;
    do {
        while (m_dwIndex < m_dwBufferSize) {
            ch = m_pBuffer[m_dwIndex];
            if (ch == ':') {
                buf.GetByteStringL(space);
                buf.Clear();
            } else if (g_FXCRT_XML_IsNameChar(ch)) {
                buf.AppendChar(ch);
            } else {
                break;
            }
            m_dwIndex ++;
        }
        m_nOffset = m_nBufferOffset + (FX_FILESIZE)m_dwIndex;
        if (m_dwIndex < m_dwBufferSize || IsEOF()) {
            break;
        }
    } while (ReadNextBlock());
    buf.GetByteStringL(name);
}
void CXML_Parser::SkipLiterals(FX_BSTR str)
{
    m_nOffset = m_nBufferOffset + (FX_FILESIZE)m_dwIndex;
    if (IsEOF()) {
        return;
    }
    FX_INT32 i = 0, iLen = str.GetLength();
    do {
        while (m_dwIndex < m_dwBufferSize) {
            if (str.GetAt(i) != m_pBuffer[m_dwIndex ++]) {
                i = 0;
            } else {
                i ++;
                if (i == iLen) {
                    break;
                }
            }
        }
        m_nOffset = m_nBufferOffset + (FX_FILESIZE)m_dwIndex;
        if (i == iLen) {
            return;
        }
        if (m_dwIndex < m_dwBufferSize || IsEOF()) {
            break;
        }
    } while (ReadNextBlock());
    while (!m_pDataAcc->IsEOF()) {
        ReadNextBlock();
        m_nOffset = m_nBufferOffset + (FX_FILESIZE)m_dwBufferSize;
    }
    m_dwIndex = m_dwBufferSize;
}
FX_DWORD CXML_Parser::GetCharRef()
{
    m_nOffset = m_nBufferOffset + (FX_FILESIZE)m_dwIndex;
    if (IsEOF()) {
        return 0;
    }
    FX_BYTE ch;
    FX_INT32 iState = 0;
    CFX_ByteTextBuf buf(m_pAllocator);
    FX_DWORD code = 0;
    do {
        while (m_dwIndex < m_dwBufferSize) {
            ch = m_pBuffer[m_dwIndex];
            switch (iState) {
                case 0:
                    if (ch == '#') {
                        m_dwIndex ++;
                        iState = 2;
                        break;
                    }
                    iState = 1;
                case 1:
                    m_dwIndex ++;
                    if (ch == ';') {
                        CFX_ByteStringC ref = buf.GetByteString();
                        if (ref == FX_BSTRC("gt")) {
                            code = '>';
                        } else if (ref == FX_BSTRC("lt")) {
                            code = '<';
                        } else if (ref == FX_BSTRC("amp")) {
                            code = '&';
                        } else if (ref == FX_BSTRC("apos")) {
                            code = '\'';
                        } else if (ref == FX_BSTRC("quot")) {
                            code = '"';
                        }
                        iState = 10;
                        break;
                    }
                    buf.AppendByte(ch);
                    break;
                case 2:
                    if (ch == 'x') {
                        m_dwIndex ++;
                        iState = 4;
                        break;
                    }
                    iState = 3;
                case 3:
                    m_dwIndex ++;
                    if (ch == ';') {
                        iState = 10;
                        break;
                    }
                    if (g_FXCRT_XML_IsDigital(ch)) {
                        code = code * 10 + ch - '0';
                    }
                    break;
                case 4:
                    m_dwIndex ++;
                    if (ch == ';') {
                        iState = 10;
                        break;
                    }
                    FX_BYTE nHex = g_FXCRT_XML_ByteTypes[ch] & FXCRTM_XML_CHARTYPE_HexChar;
                    if (nHex) {
                        if (nHex == FXCRTM_XML_CHARTYPE_HexDigital) {
                            code = (code << 4) + ch - '0';
                        } else if (nHex == FXCRTM_XML_CHARTYPE_HexLowerLetter) {
                            code = (code << 4) + ch - 87;
                        } else {
                            code = (code << 4) + ch - 55;
                        }
                    }
                    break;
            }
            if (iState == 10) {
                break;
            }
        }
        m_nOffset = m_nBufferOffset + (FX_FILESIZE)m_dwIndex;
        if (iState == 10 || m_dwIndex < m_dwBufferSize || IsEOF()) {
            break;
        }
    } while (ReadNextBlock());
    return code;
}
void CXML_Parser::GetAttrValue(CFX_WideStringL &value)
{
    m_nOffset = m_nBufferOffset + (FX_FILESIZE)m_dwIndex;
    if (IsEOF()) {
        return;
    }
    CFX_UTF8Decoder decoder(m_pAllocator);
    FX_BYTE mark = 0, ch;
    do {
        while (m_dwIndex < m_dwBufferSize) {
            ch = m_pBuffer[m_dwIndex];
            if (mark == 0) {
                if (ch != '\'' && ch != '"') {
                    return;
                }
                mark = ch;
                m_dwIndex ++;
                ch = 0;
                continue;
            }
            m_dwIndex ++;
            if (ch == mark) {
                break;
            }
            if (ch == '&') {
                decoder.AppendChar(GetCharRef());
                if (IsEOF()) {
                    decoder.GetResult(value);
                    return;
                }
            } else {
                decoder.Input(ch);
            }
        }
        m_nOffset = m_nBufferOffset + (FX_FILESIZE)m_dwIndex;
        if (ch == mark || m_dwIndex < m_dwBufferSize || IsEOF()) {
            break;
        }
    } while (ReadNextBlock());
    decoder.GetResult(value);
}
void CXML_Parser::GetTagName(CFX_ByteStringL &space, CFX_ByteStringL &name, FX_BOOL &bEndTag, FX_BOOL bStartTag)
{
    m_nOffset = m_nBufferOffset + (FX_FILESIZE)m_dwIndex;
    if (IsEOF()) {
        return;
    }
    bEndTag = FALSE;
    FX_BYTE ch;
    FX_INT32 iState = bStartTag ? 1 : 0;
    do {
        while (m_dwIndex < m_dwBufferSize) {
            ch = m_pBuffer[m_dwIndex];
            switch (iState) {
                case 0:
                    m_dwIndex ++;
                    if (ch != '<') {
                        break;
                    }
                    iState = 1;
                    break;
                case 1:
                    if (ch == '?') {
                        m_dwIndex ++;
                        SkipLiterals(FX_BSTRC("?>"));
                        iState = 0;
                        break;
                    } else if (ch == '!') {
                        m_dwIndex ++;
                        SkipLiterals(FX_BSTRC("-->"));
                        iState = 0;
                        break;
                    }
                    if (ch == '/') {
                        m_dwIndex ++;
                        GetName(space, name);
                        bEndTag = TRUE;
                    } else {
                        GetName(space, name);
                        bEndTag = FALSE;
                    }
                    return;
            }
        }
        m_nOffset = m_nBufferOffset + (FX_FILESIZE)m_dwIndex;
        if (m_dwIndex < m_dwBufferSize || IsEOF()) {
            break;
        }
    } while (ReadNextBlock());
}
CXML_Element* CXML_Parser::ParseElement(CXML_Element* pParent, FX_BOOL bStartTag)
{
    m_nOffset = m_nBufferOffset + (FX_FILESIZE)m_dwIndex;
    if (IsEOF()) {
        return NULL;
    }
    CFX_ByteStringL tag_name, tag_space;
    FX_BOOL bEndTag;
    GetTagName(tag_space, tag_name, bEndTag, bStartTag);
    if (tag_name.IsEmpty() || bEndTag) {
        tag_space.Empty(m_pAllocator);
        return NULL;
    }
    CXML_Element* pElement;
    if (m_pAllocator) {
        pElement = FX_NewAtAllocator(m_pAllocator)CXML_Element(m_pAllocator);
    } else {
        pElement = FX_NEW CXML_Element;
    }
    if (pElement) {
        pElement->m_pParent = pParent;
        pElement->SetTag(tag_space, tag_name);
    }
    tag_space.Empty(m_pAllocator);
    tag_name.Empty(m_pAllocator);
    if (!pElement) {
        return NULL;
    }
    do {
        CFX_ByteStringL attr_space, attr_name;
        while (m_dwIndex < m_dwBufferSize) {
            SkipWhiteSpaces();
            if (IsEOF()) {
                break;
            }
            if (!g_FXCRT_XML_IsNameIntro(m_pBuffer[m_dwIndex])) {
                break;
            }
            attr_space.Empty(m_pAllocator);
            attr_name.Empty(m_pAllocator);
            GetName(attr_space, attr_name);
            SkipWhiteSpaces();
            if (IsEOF()) {
                break;
            }
            if (m_pBuffer[m_dwIndex] != '=') {
                break;
            }
            m_dwIndex ++;
            SkipWhiteSpaces();
            if (IsEOF()) {
                break;
            }
            CFX_WideStringL attr_value;
            GetAttrValue(attr_value);
            pElement->m_AttrMap.SetAt(attr_space, attr_name, attr_value, m_pAllocator);
            attr_value.Empty(m_pAllocator);
        }
        attr_space.Empty(m_pAllocator);
        attr_name.Empty(m_pAllocator);
        m_nOffset = m_nBufferOffset + (FX_FILESIZE)m_dwIndex;
        if (m_dwIndex < m_dwBufferSize || IsEOF()) {
            break;
        }
    } while (ReadNextBlock());
    SkipWhiteSpaces();
    if (IsEOF()) {
        return pElement;
    }
    FX_BYTE ch = m_pBuffer[m_dwIndex ++];
    if (ch == '/') {
        m_dwIndex ++;
        m_nOffset = m_nBufferOffset + (FX_FILESIZE)m_dwIndex;
        return pElement;
    }
    if (ch != '>') {
        m_nOffset = m_nBufferOffset + (FX_FILESIZE)m_dwIndex;
        if (m_pAllocator) {
            FX_DeleteAtAllocator(pElement, m_pAllocator, CXML_Element);
        } else {
            delete pElement;
        }
        return NULL;
    }
    SkipWhiteSpaces();
    if (IsEOF()) {
        return pElement;
    }
    CFX_UTF8Decoder decoder(m_pAllocator);
    CFX_WideTextBuf content(m_pAllocator);
    FX_BOOL bCDATA = FALSE;
    FX_INT32 iState = 0;
    do {
        while (m_dwIndex < m_dwBufferSize) {
            ch = m_pBuffer[m_dwIndex ++];
            switch (iState) {
                case 0:
                    if (ch == '<') {
                        iState = 1;
                    } else if (ch == '&') {
                        decoder.ClearStatus();
                        decoder.AppendChar(GetCharRef());
                    } else {
                        decoder.Input(ch);
                    }
                    break;
                case 1:
                    if (ch == '!') {
                        iState = 2;
                    } else if (ch == '?') {
                        SkipLiterals(FX_BSTRC("?>"));
                        SkipWhiteSpaces();
                        iState = 0;
                    } else if (ch == '/') {
                        CFX_ByteStringL space, name;
                        GetName(space, name);
                        space.Empty(m_pAllocator);
                        name.Empty(m_pAllocator);
                        SkipWhiteSpaces();
                        m_dwIndex ++;
                        iState = 10;
                    } else {
                        content << decoder.GetResult();
                        CFX_WideStringL dataStr;
                        content.GetWideStringL(dataStr);
                        if (!bCDATA && !m_bSaveSpaceChars) {
                            dataStr.TrimRight((FX_LPCWSTR)L" \t\r\n");
                        }
                        InsertContentSegment(bCDATA, dataStr, pElement);
                        dataStr.Empty(m_pAllocator);
                        content.Clear();
                        decoder.Clear();
                        bCDATA = FALSE;
                        iState = 0;
                        m_dwIndex --;
                        CXML_Element* pSubElement = ParseElement(pElement, TRUE);
                        if (pSubElement == NULL) {
                            break;
                        }
                        pSubElement->m_pParent = pElement;
                        pElement->m_Children.Add((FX_LPVOID)CXML_Element::Element);
                        pElement->m_Children.Add(pSubElement);
                        SkipWhiteSpaces();
                    }
                    break;
                case 2:
                    if (ch == '[') {
                        SkipLiterals(FX_BSTRC("]]>"));
                    } else if (ch == '-') {
                        m_dwIndex ++;
                        SkipLiterals(FX_BSTRC("-->"));
                    } else {
                        SkipLiterals(FX_BSTRC(">"));
                    }
                    decoder.Clear();
                    SkipWhiteSpaces();
                    iState = 0;
                    break;
            }
            if (iState == 10) {
                break;
            }
        }
        m_nOffset = m_nBufferOffset + (FX_FILESIZE)m_dwIndex;
        if (iState == 10 || m_dwIndex < m_dwBufferSize || IsEOF()) {
            break;
        }
    } while (ReadNextBlock());
    content << decoder.GetResult();
    CFX_WideStringL dataStr;
    content.GetWideStringL(dataStr);
    if (!m_bSaveSpaceChars) {
        dataStr.TrimRight((FX_LPCWSTR)L" \t\r\n");
    }
    InsertContentSegment(bCDATA, dataStr, pElement);
    dataStr.Empty(m_pAllocator);
    content.Clear();
    decoder.Clear();
    bCDATA = FALSE;
    return pElement;
}
void CXML_Parser::InsertContentSegment(FX_BOOL bCDATA, FX_WSTR content, CXML_Element* pElement)
{
    if (content.IsEmpty()) {
        return;
    }
    CXML_Content* pContent;
    if (m_pAllocator) {
        pContent = FX_NewAtAllocator(m_pAllocator)CXML_Content;
    } else {
        pContent = FX_NEW CXML_Content;
    }
    if (!pContent) {
        return;
    }
    pContent->Set(bCDATA, content, m_pAllocator);
    pElement->m_Children.Add((FX_LPVOID)CXML_Element::Content);
    pElement->m_Children.Add(pContent);
}
static CXML_Element* XML_ContinueParse(CXML_Parser &parser, FX_BOOL bSaveSpaceChars, FX_FILESIZE* pParsedSize)
{
    parser.m_bSaveSpaceChars = bSaveSpaceChars;
    CXML_Element* pElement = parser.ParseElement(NULL, FALSE);
    if (pParsedSize) {
        *pParsedSize = parser.m_nOffset;
    }
    return pElement;
}
CXML_Element* CXML_Element::Parse(const void* pBuffer, size_t size, FX_BOOL bSaveSpaceChars, FX_FILESIZE* pParsedSize, IFX_Allocator* pAllocator)
{
    CXML_Parser parser(pAllocator);
    if (!parser.Init((FX_LPBYTE)pBuffer, size)) {
        return NULL;
    }
    return XML_ContinueParse(parser, bSaveSpaceChars, pParsedSize);
}
CXML_Element* CXML_Element::Parse(IFX_FileRead *pFile, FX_BOOL bSaveSpaceChars, FX_FILESIZE* pParsedSize, IFX_Allocator* pAllocator)
{
    CXML_Parser parser(pAllocator);
    if (!parser.Init(pFile)) {
        return NULL;
    }
    return XML_ContinueParse(parser, bSaveSpaceChars, pParsedSize);
}
CXML_Element* CXML_Element::Parse(IFX_BufferRead *pBuffer, FX_BOOL bSaveSpaceChars, FX_FILESIZE* pParsedSize, IFX_Allocator* pAllocator)
{
    CXML_Parser parser(pAllocator);
    if (!parser.Init(pBuffer)) {
        return NULL;
    }
    return XML_ContinueParse(parser, bSaveSpaceChars, pParsedSize);
}
CXML_Element::CXML_Element(IFX_Allocator* pAllocator)
    : m_pParent(NULL)
    , m_QSpaceName()
    , m_TagName()
    , m_AttrMap()
    , m_Children(pAllocator)
{
}
CXML_Element::CXML_Element(FX_BSTR qSpace, FX_BSTR tagName, IFX_Allocator* pAllocator)
    : m_pParent(NULL)
    , m_QSpaceName()
    , m_TagName()
    , m_AttrMap()
    , m_Children(pAllocator)
{
    m_QSpaceName.Set(qSpace, pAllocator);
    m_TagName.Set(tagName, pAllocator);
}
CXML_Element::CXML_Element(FX_BSTR qTagName, IFX_Allocator* pAllocator)
    : m_pParent(NULL)
    , m_QSpaceName()
    , m_TagName()
    , m_AttrMap()
    , m_Children(pAllocator)
{
    SetTag(qTagName);
}
CXML_Element::~CXML_Element()
{
    Empty();
}
void CXML_Element::Empty()
{
    IFX_Allocator* pAllocator = m_Children.m_pAllocator;
    m_QSpaceName.Empty(pAllocator);
    m_TagName.Empty(pAllocator);
    m_AttrMap.RemoveAll(pAllocator);
    RemoveChildren();
}
void CXML_Element::RemoveChildren()
{
    IFX_Allocator* pAllocator = m_Children.m_pAllocator;
    for (int i = 0; i < m_Children.GetSize(); i += 2) {
        ChildType type = (ChildType)(FX_UINTPTR)m_Children.GetAt(i);
        if (type == Content) {
            CXML_Content* content = (CXML_Content*)m_Children.GetAt(i + 1);
            if (pAllocator) {
                FX_DeleteAtAllocator(content, pAllocator, CXML_Content);
            } else {
                delete content;
            }
        } else if (type == Element) {
            CXML_Element* child = (CXML_Element*)m_Children.GetAt(i + 1);
            child->RemoveChildren();
            if (pAllocator) {
                FX_DeleteAtAllocator(child, pAllocator, CXML_Element);
            } else {
                delete child;
            }
        }
    }
    m_Children.RemoveAll();
}
CFX_ByteString CXML_Element::GetTagName(FX_BOOL bQualified) const
{
    if (!bQualified || m_QSpaceName.IsEmpty()) {
        return m_TagName;
    }
    CFX_ByteString bsTag = m_QSpaceName;
    bsTag += ":";
    bsTag += m_TagName;
    return bsTag;
}
void CXML_Element::GetTagName(CFX_ByteStringL &tagName, FX_BOOL bQualified) const
{
    IFX_Allocator* pAllocator = m_Children.m_pAllocator;
    if (!bQualified || m_QSpaceName.IsEmpty()) {
        tagName.Set(m_TagName, pAllocator);
        return;
    }
    FX_LPSTR str = tagName.AllocBuffer(m_QSpaceName.GetLength() + m_TagName.GetLength() + 2, pAllocator);
    if (!str) {
        return;
    }
    FXSYS_memcpy32(str, m_QSpaceName.GetCStr(), m_QSpaceName.GetLength());
    str += m_QSpaceName.GetLength();
    *str = ':';
    str ++;
    FXSYS_memcpy32(str, m_TagName.GetCStr(), m_TagName.GetLength());
    str += m_TagName.GetLength();
    *str = '\0';
}
CFX_ByteString CXML_Element::GetNamespace(FX_BOOL bQualified) const
{
    if (bQualified) {
        return m_QSpaceName;
    }
    return GetNamespaceURI(m_QSpaceName);
}
void CXML_Element::GetNamespace(CFX_ByteStringL &nameSpace, FX_BOOL bQualified) const
{
    IFX_Allocator* pAllocator = m_Children.m_pAllocator;
    if (bQualified) {
        nameSpace.Set(m_QSpaceName, pAllocator);
        return;
    }
    GetNamespaceURI(m_QSpaceName, nameSpace);
}
CFX_ByteString CXML_Element::GetNamespaceURI(FX_BSTR qName) const
{
    const CFX_WideStringL* pwsSpace;
    const CXML_Element *pElement = this;
    do {
        if (qName.IsEmpty()) {
            pwsSpace = pElement->m_AttrMap.Lookup(FX_BSTRC(""), FX_BSTRC("xmlns"));
        } else {
            pwsSpace = pElement->m_AttrMap.Lookup(FX_BSTRC("xmlns"), qName);
        }
        if (pwsSpace) {
            break;
        }
        pElement = pElement->GetParent();
    } while(pElement);
    return pwsSpace ? FX_UTF8Encode(*pwsSpace) : CFX_ByteString();
}
void CXML_Element::GetNamespaceURI(FX_BSTR qName, CFX_ByteStringL &uri) const
{
    IFX_Allocator* pAllocator = m_Children.m_pAllocator;
    const CFX_WideStringL* pwsSpace;
    const CXML_Element *pElement = this;
    do {
        if (qName.IsEmpty()) {
            pwsSpace = pElement->m_AttrMap.Lookup(FX_BSTRC(""), FX_BSTRC("xmlns"));
        } else {
            pwsSpace = pElement->m_AttrMap.Lookup(FX_BSTRC("xmlns"), qName);
        }
        if (pwsSpace) {
            break;
        }
        pElement = pElement->GetParent();
    } while(pElement);
    if (pwsSpace) {
        FX_UTF8Encode(pwsSpace->GetPtr(), pwsSpace->GetLength(), uri, pAllocator);
    }
}
void CXML_Element::GetAttrByIndex(int index, CFX_ByteString& space, CFX_ByteString& name, CFX_WideString& value) const
{
    if (index < 0 || index >= m_AttrMap.GetSize()) {
        return;
    }
    CXML_AttrItem& item = m_AttrMap.GetAt(index);
    space = item.m_QSpaceName;
    name = item.m_AttrName;
    value = item.m_Value;
}
void CXML_Element::GetAttrByIndex(int index, CFX_ByteStringL &space, CFX_ByteStringL &name, CFX_WideStringL &value) const
{
    if (index < 0 || index >= m_AttrMap.GetSize()) {
        return;
    }
    IFX_Allocator* pAllocator = m_Children.m_pAllocator;
    CXML_AttrItem& item = m_AttrMap.GetAt(index);
    space.Set(item.m_QSpaceName, pAllocator);
    name.Set(item.m_AttrName, pAllocator);
    value.Set(item.m_Value, pAllocator);
}
FX_BOOL CXML_Element::HasAttr(FX_BSTR name) const
{
    CFX_ByteStringC bsSpace, bsName;
    FX_XML_SplitQualifiedName(name, bsSpace, bsName);
    return m_AttrMap.Lookup(bsSpace, bsName) != NULL;
}
FX_BOOL CXML_Element::GetAttrValue(FX_BSTR name, CFX_WideString& attribute) const
{
    CFX_ByteStringC bsSpace, bsName;
    FX_XML_SplitQualifiedName(name, bsSpace, bsName);
    const CFX_WideStringL* pValue = m_AttrMap.Lookup(bsSpace, bsName);
    if (pValue) {
        attribute = CFX_WideString(pValue->GetPtr(), pValue->GetLength());
        return TRUE;
    }
    return FALSE;
}
const CFX_WideStringL* CXML_Element::GetAttrValuePtr(FX_BSTR name) const
{
    CFX_ByteStringC bsSpace, bsName;
    FX_XML_SplitQualifiedName(name, bsSpace, bsName);
    return m_AttrMap.Lookup(bsSpace, bsName);
}
FX_BOOL CXML_Element::GetAttrValue(FX_BSTR space, FX_BSTR name, CFX_WideString& attribute) const
{
    const CFX_WideStringL* pValue = m_AttrMap.Lookup(space, name);
    if (pValue) {
        attribute = CFX_WideString(pValue->GetPtr(), pValue->GetLength());
        return TRUE;
    }
    return FALSE;
}
const CFX_WideStringL* CXML_Element::GetAttrValuePtr(FX_BSTR space, FX_BSTR name) const
{
    return m_AttrMap.Lookup(space, name);
}
FX_BOOL CXML_Element::GetAttrInteger(FX_BSTR name, int& attribute) const
{
    CFX_ByteStringC bsSpace, bsName;
    FX_XML_SplitQualifiedName(name, bsSpace, bsName);
    const CFX_WideStringL* pwsValue = m_AttrMap.Lookup(bsSpace, bsName);
    if (pwsValue) {
        attribute = pwsValue->GetInteger();
        return TRUE;
    }
    return FALSE;
}
FX_BOOL	CXML_Element::GetAttrInteger(FX_BSTR space, FX_BSTR name, int& attribute) const
{
    const CFX_WideStringL* pwsValue = m_AttrMap.Lookup(space, name);
    if (pwsValue) {
        attribute = pwsValue->GetInteger();
        return TRUE;
    }
    return FALSE;
}
FX_BOOL CXML_Element::GetAttrFloat(FX_BSTR name, FX_FLOAT& attribute) const
{
    CFX_ByteStringC bsSpace, bsName;
    FX_XML_SplitQualifiedName(name, bsSpace, bsName);
    return GetAttrFloat(bsSpace, bsName, attribute);
}
FX_BOOL CXML_Element::GetAttrFloat(FX_BSTR space, FX_BSTR name, FX_FLOAT& attribute) const
{
    CFX_WideString value;
    const CFX_WideStringL* pValue = m_AttrMap.Lookup(space, name);
    if (pValue) {
        attribute = pValue->GetFloat();
        return TRUE;
    }
    return FALSE;
}
FX_DWORD CXML_Element::CountChildren() const
{
    return m_Children.GetSize() / 2;
}
CXML_Element::ChildType CXML_Element::GetChildType(FX_DWORD index) const
{
    index <<= 1;
    if (index >= (FX_DWORD)m_Children.GetSize()) {
        return Invalid;
    }
    return (ChildType)(FX_UINTPTR)m_Children.GetAt(index);
}
CFX_WideString CXML_Element::GetContent(FX_DWORD index) const
{
    index <<= 1;
    if (index >= (FX_DWORD)m_Children.GetSize() ||
            (ChildType)(FX_UINTPTR)m_Children.GetAt(index) != Content) {
        return CFX_WideString();
    }
    CXML_Content* pContent = (CXML_Content*)m_Children.GetAt(index + 1);
    if (pContent) {
        return pContent->m_Content;
    }
    return CFX_WideString();
}
const CFX_WideStringL* CXML_Element::GetContentPtr(FX_DWORD index) const
{
    index <<= 1;
    if (index >= (FX_DWORD)m_Children.GetSize() ||
            (ChildType)(FX_UINTPTR)m_Children.GetAt(index) != Content) {
        return NULL;
    }
    CXML_Content* pContent = (CXML_Content*)m_Children.GetAt(index + 1);
    if (pContent) {
        return &pContent->m_Content;
    }
    return NULL;
}
CXML_Element* CXML_Element::GetElement(FX_DWORD index) const
{
    index <<= 1;
    if (index >= (FX_DWORD)m_Children.GetSize() ||
            (ChildType)(FX_UINTPTR)m_Children.GetAt(index) != Element) {
        return NULL;
    }
    return (CXML_Element*)m_Children.GetAt(index + 1);
}
FX_DWORD CXML_Element::CountElements(FX_BSTR space, FX_BSTR tag) const
{
    int count = 0;
    for (int i = 0; i < m_Children.GetSize(); i += 2) {
        ChildType type = (ChildType)(FX_UINTPTR)m_Children.GetAt(i);
        if (type != Element) {
            continue;
        }
        CXML_Element* pKid = (CXML_Element*)m_Children.GetAt(i + 1);
        if ((space.IsEmpty() || pKid->m_QSpaceName == space) && pKid->m_TagName == tag) {
            count ++;
        }
    }
    return count;
}
CXML_Element* CXML_Element::GetElement(FX_BSTR space, FX_BSTR tag, int index) const
{
    if (index < 0) {
        return NULL;
    }
    for (int i = 0; i < m_Children.GetSize(); i += 2) {
        ChildType type = (ChildType)(FX_UINTPTR)m_Children.GetAt(i);
        if (type != Element) {
            continue;
        }
        CXML_Element* pKid = (CXML_Element*)m_Children.GetAt(i + 1);
        if ((!space.IsEmpty() && pKid->m_QSpaceName != space) || pKid->m_TagName != tag) {
            continue;
        }
        if (index -- == 0) {
            return pKid;
        }
    }
    return NULL;
}
FX_DWORD CXML_Element::FindElement(CXML_Element *pChild) const
{
    for (int i = 0; i < m_Children.GetSize(); i += 2) {
        if ((ChildType)(FX_UINTPTR)m_Children.GetAt(i) == Element &&
                (CXML_Element*)m_Children.GetAt(i + 1) == pChild) {
            return (FX_DWORD)(i >> 1);
        }
    }
    return (FX_DWORD) - 1;
}
const CFX_WideStringL* CXML_AttrMap::Lookup(FX_BSTR space, FX_BSTR name) const
{
    if (m_pMap == NULL) {
        return NULL;
    }
    for (int i = 0; i < m_pMap->GetSize(); i ++) {
        CXML_AttrItem& item = GetAt(i);
        if ((space.IsEmpty() || item.m_QSpaceName == space) && item.m_AttrName == name) {
            return &item.m_Value;
        }
    }
    return NULL;
}
void CXML_AttrMap::SetAt(FX_BSTR space, FX_BSTR name, FX_WSTR value, IFX_Allocator* pAllocator)
{
    for (int i = 0; i < GetSize(); i ++) {
        CXML_AttrItem& item = GetAt(i);
        if ((space.IsEmpty() || item.m_QSpaceName == space) && item.m_AttrName == name) {
            item.m_Value.Set(value, pAllocator);
            return;
        }
    }
    if (!m_pMap) {
        if (pAllocator) {
            m_pMap = FX_NewAtAllocator(pAllocator)CFX_ObjectArray<CXML_AttrItem>(pAllocator);
        } else {
            m_pMap = FX_NEW CFX_ObjectArray<CXML_AttrItem>;
        }
    }
    if (!m_pMap) {
        return;
    }
    CXML_AttrItem* pItem = (CXML_AttrItem*)m_pMap->AddSpace();
    if (!pItem) {
        return;
    }
    pItem->m_QSpaceName.Set(space, pAllocator);
    pItem->m_AttrName.Set(name, pAllocator);
    pItem->m_Value.Set(value, pAllocator);
}
void CXML_AttrMap::RemoveAt(FX_BSTR space, FX_BSTR name, IFX_Allocator* pAllocator)
{
    if (m_pMap == NULL) {
        return;
    }
    for (int i = 0; i < m_pMap->GetSize(); i ++) {
        CXML_AttrItem& item = GetAt(i);
        if ((space.IsEmpty() || item.m_QSpaceName == space) && item.m_AttrName == name) {
            item.Empty(pAllocator);
            m_pMap->RemoveAt(i);
            return;
        }
    }
}
int CXML_AttrMap::GetSize() const
{
    return m_pMap == NULL ? 0 : m_pMap->GetSize();
}
CXML_AttrItem& CXML_AttrMap::GetAt(int index) const
{
    ASSERT(m_pMap != NULL);
    return (*m_pMap)[index];
}
void CXML_AttrMap::RemoveAll(IFX_Allocator* pAllocator)
{
    if (!m_pMap) {
        return;
    }
    for (int i = 0; i < m_pMap->GetSize(); i ++) {
        CXML_AttrItem& item = (*m_pMap)[i];
        item.Empty(pAllocator);
    }
    m_pMap->RemoveAll();
    if (pAllocator) {
        FX_DeleteAtAllocator(m_pMap, pAllocator, CFX_ObjectArray<CXML_AttrItem>);
    } else {
        delete m_pMap;
    }
    m_pMap = NULL;
}
