// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "../../include/fxcrt/fx_ext.h"
#include "mem_int.h"
#ifdef _FPDFAPI_MINI_
static FX_MEMCONFIG g_MemConfig = {
    1,
    5,
    8,
    4,
    12,
    8,
    2,
    4,
    32,
    64,
};
#else
static FX_MEMCONFIG g_MemConfig = {
    1,
    8,
    24,
    8,
    32,
    16,
    4,
    8,
    128,
    64,
};
#endif
void FXMEM_SetConfig(const FX_MEMCONFIG* memConfig)
{
    g_MemConfig = *memConfig;
}
#ifdef __cplusplus
extern "C" {
#endif
static void* FixedAlloc(FXMEM_SystemMgr* pMgr, size_t size, int flags)
{
    return ((CFXMEM_FixedMgr*)pMgr->user)->Alloc(size);
}
static void* FixedAllocDebug(FXMEM_SystemMgr* pMgr, size_t size, int flags, FX_LPCSTR file, int line)
{
    return ((CFXMEM_FixedMgr*)pMgr->user)->Alloc(size);
}
static void* FixedRealloc(FXMEM_SystemMgr* pMgr, void* pointer, size_t size, int flags)
{
    return ((CFXMEM_FixedMgr*)pMgr->user)->Realloc(pointer, size);
}
static void* FixedReallocDebug(FXMEM_SystemMgr* pMgr, void* pointer, size_t size, int flags, FX_LPCSTR file, int line)
{
    return ((CFXMEM_FixedMgr*)pMgr->user)->Realloc(pointer, size);
}
static void  FixedFree(FXMEM_SystemMgr* pMgr, void* pointer, int flags)
{
    ((CFXMEM_FixedMgr*)pMgr->user)->Free(pointer);
}
static void  FixedPurge(FXMEM_SystemMgr* pMgr)
{
    ((CFXMEM_FixedMgr*)pMgr->user)->Purge();
}
static void FixedCollectAll(FXMEM_SystemMgr* pMgr)
{
    ((CFXMEM_FixedMgr*)pMgr->user)->FreeAll();
}
#define FIXEDMEM_MINIMUMSIZE	(1024 * 1024 * 8)
FXMEM_FoxitMgr* FXMEM_CreateMemoryMgr(size_t size, FX_BOOL extensible)
{
    if (size < FIXEDMEM_MINIMUMSIZE) {
        size = FIXEDMEM_MINIMUMSIZE;
    }
    FX_LPVOID pMemory = malloc(size);
    if (!pMemory) {
        return NULL;
    }
    CFixedMgr_Proxy* pProxy = (CFixedMgr_Proxy*)pMemory;
    size_t offsetSize = (sizeof(CFixedMgr_Proxy) + 15) / 16 * 16;
    FXMEM_FoxitMgr* pFoxitMgr = pProxy->Initialize((FX_LPBYTE)pProxy + offsetSize, size - offsetSize, extensible);
    if (!pFoxitMgr) {
        free(pMemory);
        return NULL;
    }
    g_pDefFoxitMgr = (CFX_MemoryMgr*)pFoxitMgr;
    g_pDefFoxitMgr->m_pExternalMemory = pMemory;
    return pFoxitMgr;
}
FXMEM_FoxitMgr* FXMEM_CreateFixedMgr(void* pMemory, size_t size, FXMEM_SystemMgr2* pSystemMgr)
{
    if (pMemory == NULL || size < FX_FIXEDMEM_PAGESIZE) {
        return NULL;
    }
    if (!pSystemMgr && size >= FIXEDMEM_PROXYSIZE_1) {
        CFixedMgr_Proxy* pProxy = (CFixedMgr_Proxy*)pMemory;
        size_t offsetSize = (sizeof(CFixedMgr_Proxy) + 15) / 16 * 16;
        return pProxy->Initialize((FX_LPBYTE)pProxy + offsetSize, size - offsetSize, FALSE);
    }
    CFXMEM_FixedMgr* pHeader = (CFXMEM_FixedMgr*)pMemory;
    pHeader->Initialize(size);
    pHeader->m_pExtender = pSystemMgr;
    CFX_MemoryMgr* p = (CFX_MemoryMgr*)pHeader->Alloc(sizeof(CFX_MemoryMgr));
    if (p == NULL) {
        return NULL;
    }
    p->Init(&pHeader->m_SystemMgr);
    return (FXMEM_FoxitMgr*)p;
}
size_t FXMEM_GetBlockSizeInFixedMgr(FXMEM_FoxitMgr* pFoxitMgr, void* ptr)
{
    return pFoxitMgr ? ((CFXMEM_FixedMgr*)((CFX_MemoryMgr*)pFoxitMgr)->m_pSystemMgr->user)->GetSize(ptr) : 0;
}
#ifdef __cplusplus
}
#endif
const FX_MEMCONFIG g_ProxyMgr_MemConfigs[6] = {
    {1,      2,      4,      0,      2,      2,   2,       0,       0,     0},
    {1,      4,      8,      0,      2,      2,   2,       0,       0,     0},
    {1,      4,      16,     4,      8,      8,   2,       1,       16,    16},
    {1,      8,      24,     4,      12,     12,  4,       2,       32,    16},
    {1,      8,      24,     8,      16,     16,  4,       2,       64,    32},
    {1,      8,      24,     8,      24,     32,  4,       2,       128,   64},
};
const FX_MEMCONFIG*	FixedMgr_GetConfig(size_t nSize)
{
    int index = 5;
    if (nSize <= FIXEDMEM_PROXYSIZE_0) {
        index = 0;
    } else if (nSize <= FIXEDMEM_PROXYSIZE_1) {
        index = 1;
    } else if (nSize <= FIXEDMEM_PROXYSIZE_2) {
        index = 2;
    } else if (nSize <= FIXEDMEM_PROXYSIZE_3) {
        index = 3;
    } else if (nSize <= FIXEDMEM_PROXYSIZE_4) {
        index = 4;
    }
    return &g_ProxyMgr_MemConfigs[index];
}
FXMEM_FoxitMgr* CFixedMgr_Proxy::Initialize(FX_LPVOID pBuffer, size_t nSize, FX_BOOL bExtensible)
{
    FXSYS_assert(pBuffer != NULL && nSize >= FIXEDMEM_PROXYSIZE_1 - sizeof(CFixedMgr_Proxy));
    FXMEM_SetConfig(FixedMgr_GetConfig(nSize));
    m_SystemMgr.More = &CFixedMgr_Proxy::Common_More;
    m_SystemMgr.Free = &CFixedMgr_Proxy::Common_Free;
    m_pFixedPage = (CFXMEM_Page*)((FX_LPBYTE)pBuffer + FIXEDMEM_PROXYSIZE_0);
    m_pFixedPage->Initialize(nSize - FIXEDMEM_PROXYSIZE_0);
    m_pBuffer = pBuffer;
    m_nSize = nSize;
    m_bExtensible = bExtensible;
    return FXMEM_CreateFixedMgr(pBuffer, FIXEDMEM_PROXYSIZE_0, &m_SystemMgr);
}
FX_BOOL CFixedMgr_Proxy::Common_More(FXMEM_SystemMgr2* pMgr, size_t alloc_size, void** new_memory, size_t* new_size)
{
    CFixedMgr_Proxy* pProxyMgr = (CFixedMgr_Proxy*)pMgr;
    FXSYS_assert(pProxyMgr != NULL && pProxyMgr->m_pFixedPage != NULL);
    *new_size = alloc_size;
    *new_memory = pProxyMgr->m_pFixedPage->Alloc(alloc_size);
    if (*new_memory == NULL && pProxyMgr->m_bExtensible) {
        *new_memory = malloc(alloc_size);
    }
    return *new_memory != NULL;
}
void CFixedMgr_Proxy::Common_Free(FXMEM_SystemMgr2* pMgr, void* memory)
{
    CFixedMgr_Proxy* pProxyMgr = (CFixedMgr_Proxy*)pMgr;
    FXSYS_assert(pProxyMgr != NULL && pProxyMgr->m_pFixedPage != NULL);
    if (memory > pProxyMgr->m_pBuffer && memory < (FX_LPBYTE)pProxyMgr->m_pBuffer + pProxyMgr->m_nSize) {
        pProxyMgr->m_pFixedPage->Free(memory);
    } else if (pProxyMgr->m_bExtensible) {
        free(memory);
    }
}
void CFXMEM_Page::Initialize(size_t size)
{
    CFXMEM_Block *pFirstBlock = (CFXMEM_Block*)(this + 1);
    m_nAvailSize = size - sizeof(CFXMEM_Page) - sizeof(CFXMEM_Block);
    pFirstBlock->m_nBlockSize = m_nAvailSize;
    pFirstBlock->m_pNextBlock = NULL;
    m_AvailHead.m_nBlockSize = m_nAvailSize;
    m_AvailHead.m_pNextBlock = pFirstBlock;
    m_pLimitPos = (CFXMEM_Block*)((FX_LPBYTE)this + size);
}
FX_LPVOID CFXMEM_Page::Alloc(CFXMEM_Block* pPrevBlock, CFXMEM_Block* pNextBlock, size_t size, size_t oldsize)
{
    size_t gap = pNextBlock->m_nBlockSize - size;
    if (gap <= 64 + sizeof(CFXMEM_Block)) {
        pPrevBlock->m_pNextBlock = pNextBlock->m_pNextBlock;
        m_nAvailSize -= pNextBlock->m_nBlockSize;
    } else {
        m_nAvailSize -= size + sizeof(CFXMEM_Block);
        pNextBlock->m_nBlockSize = size;
        CFXMEM_Block *pNewBlock = (CFXMEM_Block*)((FX_LPBYTE)(pNextBlock + 1) + size);
        pNewBlock->m_nBlockSize = gap - sizeof(CFXMEM_Block);
        pNewBlock->m_pNextBlock = pNextBlock->m_pNextBlock;
        pPrevBlock->m_pNextBlock = pNewBlock;
    }
    return (FX_LPVOID)(pNextBlock + 1);
}
FX_LPVOID CFXMEM_Page::Alloc(size_t size)
{
    size_t oldsize = size;
#if _FX_WORDSIZE_ == _FX_W64_
    size = (size + 31) / 32 * 32;
#else
    size = (size + 7) / 8 * 8;
#endif
    if (m_nAvailSize < size) {
        return NULL;
    }
    CFXMEM_Block *pNextBlock;
    CFXMEM_Block *pPrevBlock = &m_AvailHead;
    while (TRUE) {
        pNextBlock = pPrevBlock->m_pNextBlock;
        if (!pNextBlock) {
            return NULL;
        }
        if (pNextBlock->m_nBlockSize >= size) {
            break;
        }
        pPrevBlock = pNextBlock;
    }
    return Alloc(pPrevBlock, pNextBlock, size, oldsize);
}
FX_LPVOID CFXMEM_Page::Realloc(FX_LPVOID p, size_t oldSize, size_t newSize)
{
    FXSYS_assert(p > (FX_LPVOID)this && p < (FX_LPVOID)m_pLimitPos);
    size_t oldnewSize = newSize;
#if _FX_WORDSIZE_ == _FX_W64_
    newSize = (newSize + 31) / 32 * 32;
#else
    newSize = (newSize + 7) / 8 * 8;
#endif
    CFXMEM_Block *pPrevBlock = &m_AvailHead;
    CFXMEM_Block *pNextBlock, *pPrevPrev;
    CFXMEM_Block *pBlock = (CFXMEM_Block*)p - 1;
    pPrevPrev = NULL;
    while (TRUE) {
        pNextBlock = pPrevBlock->m_pNextBlock;
        if (pNextBlock == NULL || pNextBlock > pBlock) {
            break;
        }
        if (pPrevBlock != &m_AvailHead && (FX_LPBYTE)pNextBlock == (FX_LPBYTE)(pPrevBlock + 1) + pPrevBlock->m_nBlockSize) {
            m_nAvailSize += sizeof(CFXMEM_Block);
            pPrevBlock->m_nBlockSize += pNextBlock->m_nBlockSize + sizeof(CFXMEM_Block);
            pPrevBlock->m_pNextBlock = pNextBlock->m_pNextBlock;
        } else {
            pPrevPrev = pPrevBlock;
            pPrevBlock = pNextBlock;
        }
    }
    if (pNextBlock) {
        CFXMEM_Block* pCurBlock = pNextBlock->m_pNextBlock;
        while ((FX_LPBYTE)pCurBlock == (FX_LPBYTE)(pNextBlock + 1) + pNextBlock->m_nBlockSize) {
            m_nAvailSize += sizeof(CFXMEM_Block);
            pNextBlock->m_nBlockSize += pCurBlock->m_nBlockSize + sizeof(CFXMEM_Block);
            pCurBlock = pCurBlock->m_pNextBlock;
            pNextBlock->m_pNextBlock = pCurBlock;
        }
    }
    size_t size = 0;
    FX_DWORD dwFlags = 0;
    if (pPrevBlock != &m_AvailHead && (FX_LPBYTE)pBlock == (FX_LPBYTE)(pPrevBlock + 1) + pPrevBlock->m_nBlockSize) {
        size += pPrevBlock->m_nBlockSize + oldSize + sizeof(CFXMEM_Block);
        dwFlags |= 0x10;
    }
    if (pNextBlock && (FX_LPBYTE)pNextBlock == (FX_LPBYTE)p + oldSize) {
        size += pNextBlock->m_nBlockSize + sizeof(CFXMEM_Block);
        dwFlags |= 0x01;
    }
    if (size >= newSize) {
        m_nAvailSize += pBlock->m_nBlockSize;
        CFXMEM_Block* pCurBlock = pBlock;
        if (dwFlags & 0x10) {
            pCurBlock = pPrevBlock;
            m_nAvailSize += sizeof(CFXMEM_Block);
            pCurBlock->m_nBlockSize += pBlock->m_nBlockSize + sizeof(CFXMEM_Block);
            pPrevBlock = pPrevPrev;
        }
        if (dwFlags & 0x01) {
            m_nAvailSize += sizeof(CFXMEM_Block);
            pCurBlock->m_nBlockSize += pNextBlock->m_nBlockSize + sizeof(CFXMEM_Block);
            pCurBlock->m_pNextBlock = pNextBlock->m_pNextBlock;
        }
        if (pCurBlock != pBlock) {
            FXSYS_memmove32((FX_LPVOID)(pCurBlock + 1), p, oldSize);
        }
        return Alloc(pPrevBlock, pCurBlock, newSize, oldnewSize);
    }
    return NULL;
}
void CFXMEM_Page::Free(FX_LPVOID p)
{
    FXSYS_assert(p > (FX_LPVOID)this && p < (FX_LPVOID)m_pLimitPos);
    CFXMEM_Block *pPrevBlock = &m_AvailHead;
    CFXMEM_Block *pNextBlock;
    CFXMEM_Block *pBlock = (CFXMEM_Block*)p - 1;
    m_nAvailSize += pBlock->m_nBlockSize;
    while (TRUE) {
        pNextBlock = pPrevBlock->m_pNextBlock;
        if (pNextBlock == NULL || pNextBlock > pBlock) {
            break;
        }
        if (pPrevBlock != &m_AvailHead && (FX_LPBYTE)pNextBlock == (FX_LPBYTE)(pPrevBlock + 1) + pPrevBlock->m_nBlockSize) {
            m_nAvailSize += sizeof(CFXMEM_Block);
            pPrevBlock->m_nBlockSize += pNextBlock->m_nBlockSize + sizeof(CFXMEM_Block);
            pPrevBlock->m_pNextBlock = pNextBlock->m_pNextBlock;
        } else {
            pPrevBlock = pNextBlock;
        }
    }
    while ((FX_LPBYTE)pNextBlock == (FX_LPBYTE)(pBlock + 1) + pBlock->m_nBlockSize) {
        m_nAvailSize += sizeof(CFXMEM_Block);
        pBlock->m_nBlockSize += pNextBlock->m_nBlockSize + sizeof(CFXMEM_Block);
        pNextBlock = pNextBlock->m_pNextBlock;
    }
    pBlock->m_pNextBlock = pNextBlock;
    if (pPrevBlock != &m_AvailHead && (FX_LPBYTE)pBlock == (FX_LPBYTE)(pPrevBlock + 1) + pPrevBlock->m_nBlockSize) {
        m_nAvailSize += sizeof(CFXMEM_Block);
        pPrevBlock->m_nBlockSize += pBlock->m_nBlockSize + sizeof(CFXMEM_Block);
        pPrevBlock->m_pNextBlock = pBlock->m_pNextBlock;
    } else {
        FXSYS_assert(pPrevBlock != pBlock);
        pPrevBlock->m_pNextBlock = pBlock;
    }
}
void CFXMEM_Pages::Initialize(FX_LPBYTE pStart, size_t pageSize, size_t pages)
{
    m_pStartPage = m_pCurPage = (CFXMEM_Page*)pStart;
    m_nPageSize = pageSize;
    for (size_t n = 0; n < pages; n++) {
        ((CFXMEM_Page*)pStart)->Initialize(pageSize);
        pStart += pageSize;
    }
    m_pLimitPos = (CFXMEM_Page*)pStart;
}
FX_BOOL CFXMEM_Pages::IsEmpty() const
{
    if (m_pStartPage >= m_pLimitPos) {
        return TRUE;
    }
    FX_LPBYTE pPage = (FX_LPBYTE)m_pStartPage;
    while (pPage < (FX_LPBYTE)m_pLimitPos) {
        if (!((CFXMEM_Page*)pPage)->IsEmpty()) {
            return FALSE;
        }
        pPage += m_nPageSize;
    }
    return TRUE;
}
FX_LPVOID CFXMEM_Pages::Alloc(size_t size)
{
    CFXMEM_Page *pCurPage = m_pCurPage;
    do {
        FX_LPVOID p = m_pCurPage->Alloc(size);
        if (p) {
            return p;
        }
        m_pCurPage = (CFXMEM_Page*)((FX_LPBYTE)m_pCurPage + m_nPageSize);
        if (m_pCurPage == m_pLimitPos) {
            m_pCurPage = m_pStartPage;
        }
    } while (m_pCurPage != pCurPage);
    return NULL;
}
FX_LPVOID CFXMEM_Pages::Realloc(FX_LPVOID p, size_t oldSize, size_t newSize)
{
    FXSYS_assert (p > (FX_LPVOID)m_pStartPage && p < (FX_LPVOID)m_pLimitPos);
    CFXMEM_Page* pPage = (CFXMEM_Page*)((FX_LPBYTE)m_pStartPage + ((FX_LPBYTE)p - (FX_LPBYTE)m_pStartPage) / m_nPageSize * m_nPageSize);
    return pPage->Realloc(p, oldSize, newSize);
}
void CFXMEM_Pages::Free(FX_LPVOID p)
{
    FXSYS_assert (p > (FX_LPVOID)m_pStartPage && p < (FX_LPVOID)m_pLimitPos);
    CFXMEM_Page* pPage = (CFXMEM_Page*)((FX_LPBYTE)m_pStartPage + ((FX_LPBYTE)p - (FX_LPBYTE)m_pStartPage) / m_nPageSize * m_nPageSize);
    pPage->Free(p);
}
void CFXMEM_Pool::Initialize(const FX_MEMCONFIG* pMemConfig, size_t size, size_t pageNum8Bytes, size_t pageNum16Bytes, size_t pageNum32Bytes, size_t pageNumMid)
{
    m_pPrevPool = NULL;
    m_pNextPool = NULL;
    m_bAlone = FALSE;
    FX_LPBYTE pPage = (FX_LPBYTE)this + sizeof(CFXMEM_Pool);
    size -= sizeof(CFXMEM_Pool);
    m_8BytesPages.Initialize(pPage, pageNum8Bytes);
    pPage += pageNum8Bytes * FX_FIXEDMEM_PAGESIZE;
    size -= pageNum8Bytes * FX_FIXEDMEM_PAGESIZE;
    m_16BytesPages.Initialize(pPage, pageNum16Bytes);
    pPage += pageNum16Bytes * FX_FIXEDMEM_PAGESIZE;
    size -= pageNum16Bytes * FX_FIXEDMEM_PAGESIZE;
    m_32BytesPages.Initialize(pPage, pageNum32Bytes);
    pPage += pageNum32Bytes * FX_FIXEDMEM_PAGESIZE;
    size -= pageNum32Bytes * FX_FIXEDMEM_PAGESIZE;
    m_MidPages.Initialize(pPage, pMemConfig->nPageSize_Mid * FX_FIXEDMEM_PAGESIZE, pageNumMid);
    pPage += pageNumMid * pMemConfig->nPageSize_Mid * FX_FIXEDMEM_PAGESIZE;
    size -= pageNumMid * pMemConfig->nPageSize_Mid * FX_FIXEDMEM_PAGESIZE;
    if (size < FX_FIXEDMEM_MIDBLOCKSIZE) {
        m_pLargePage = NULL;
    } else {
        m_pLargePage = (CFXMEM_Page*)pPage;
        m_pLargePage->Initialize(size);
    }
    m_pLimitPos = pPage + size;
}
FX_BOOL CFXMEM_Pool::IsEmpty() const
{
    if (!m_8BytesPages.IsEmpty()) {
        return FALSE;
    }
    if (!m_16BytesPages.IsEmpty()) {
        return FALSE;
    }
    if (!m_32BytesPages.IsEmpty()) {
        return FALSE;
    }
    if (!m_MidPages.IsEmpty()) {
        return FALSE;
    }
    return !m_pLargePage || m_pLargePage->IsEmpty();
}
size_t CFXMEM_Pool::GetSize(FX_LPVOID p) const
{
    FXSYS_assert(p > (FX_LPVOID)this && p < (FX_LPVOID)m_pLimitPos);
    if (p < (FX_LPVOID)m_8BytesPages.m_pLimitPos) {
        return 8;
    }
    if (p < (FX_LPVOID)m_16BytesPages.m_pLimitPos) {
        return 16;
    }
    if (p < (FX_LPVOID)m_32BytesPages.m_pLimitPos) {
        return 32;
    }
    return ((CFXMEM_Block*)p - 1)->m_nBlockSize;
}
FX_LPVOID CFXMEM_Pool::Realloc(FX_LPVOID p, size_t oldSize, size_t newSize)
{
    FXSYS_assert(p > (FX_LPVOID)this && p < (FX_LPVOID)m_pLimitPos);
    if (p > (FX_LPVOID)m_32BytesPages.m_pLimitPos) {
        if (p < (FX_LPVOID)m_MidPages.m_pLimitPos) {
            return m_MidPages.Realloc(p, oldSize, newSize);
        } else if (m_pLargePage) {
            return m_pLargePage->Realloc(p, oldSize, newSize);
        }
    }
    return NULL;
}
void CFXMEM_Pool::Free(FX_LPVOID p)
{
    FXSYS_assert(p > (FX_LPVOID)this && p < (FX_LPVOID)m_pLimitPos);
    if (p < (FX_LPVOID)m_32BytesPages.m_pLimitPos) {
        if (p < (FX_LPVOID)m_8BytesPages.m_pLimitPos) {
            m_8BytesPages.Free(p);
        } else if (p < (FX_LPVOID)m_16BytesPages.m_pLimitPos) {
            m_16BytesPages.Free(p);
        } else {
            m_32BytesPages.Free(p);
        }
        return;
    } else if (p < (FX_LPVOID)m_MidPages.m_pLimitPos) {
        m_MidPages.Free(p);
    } else {
        m_pLargePage->Free(p);
    }
}
void CFXMEM_FixedMgr::Initialize(size_t size)
{
    m_MemConfig = g_MemConfig;
    FXSYS_memset32(&m_SystemMgr, 0, sizeof m_SystemMgr);
    m_SystemMgr.Alloc = FixedAlloc;
    m_SystemMgr.AllocDebug = FixedAllocDebug;
    m_SystemMgr.Free = FixedFree;
    m_SystemMgr.Realloc = FixedRealloc;
    m_SystemMgr.ReallocDebug = FixedReallocDebug;
    m_SystemMgr.CollectAll = FixedCollectAll;
    m_SystemMgr.Purge = FixedPurge;
    m_SystemMgr.user = this;
    size -= sizeof(CFXMEM_FixedMgr);
    size_t nMidPages = 0;
    if (m_MemConfig.nPageSize_Mid) {
        nMidPages = (size - (m_MemConfig.nPageNum_Init8 + m_MemConfig.nPageNum_Init16 + m_MemConfig.nPageNum_Init32) * FX_FIXEDMEM_PAGESIZE) / (m_MemConfig.nPageSize_Mid * FX_FIXEDMEM_PAGESIZE);
        if (nMidPages > m_MemConfig.nPageNum_InitMid) {
            nMidPages = m_MemConfig.nPageNum_InitMid;
        }
    }
    m_FirstPool.Initialize(&m_MemConfig, size, m_MemConfig.nPageNum_Init8, m_MemConfig.nPageNum_Init16, m_MemConfig.nPageNum_Init32, nMidPages);
}
FX_LPVOID CFXMEM_FixedMgr::Alloc16(CFXMEM_Pool **pp32Pool, size_t size)
{
    CFXMEM_Pool *pPool = &m_FirstPool;
    do {
        CFXMEM_16BytesPages &pages = pPool->m_16BytesPages;
        if (pages.HasFreeBlock()) {
            return pages.Alloc(size);
        }
        if (pp32Pool && pPool->m_32BytesPages.HasFreeBlock()) {
            *pp32Pool = pPool;
        }
        pPool = pPool->m_pNextPool;
    } while(pPool);
    return NULL;
}
FX_LPVOID CFXMEM_FixedMgr::Alloc32(size_t size)
{
    if (size <= 8) {
        CFXMEM_8BytesPages &pages = m_FirstPool.m_8BytesPages;
        if (pages.HasFreeBlock()) {
            return pages.Alloc(size);
        }
    }
    CFXMEM_Pool *p32BytesPool;
    if (size <= 16) {
        p32BytesPool = NULL;
        FX_LPVOID p = Alloc16(&p32BytesPool, size);
        if (p) {
            return p;
        }
    } else {
        p32BytesPool = &m_FirstPool;
    }
    while (p32BytesPool) {
        CFXMEM_32BytesPages &pages = p32BytesPool->m_32BytesPages;
        if (pages.HasFreeBlock()) {
            return pages.Alloc(size);
        }
        p32BytesPool = p32BytesPool->m_pNextPool;
    }
    return NULL;
}
FX_LPVOID CFXMEM_FixedMgr::AllocSmall(size_t size)
{
    FX_LPVOID p = Alloc32(size);
    if (p) {
        return p;
    }
    if (!m_pExtender) {
        return NULL;
    }
    size_t requiredSize = (m_MemConfig.nPageNum_More16 + m_MemConfig.nPageNum_More32) * FX_FIXEDMEM_PAGESIZE;
    if (!requiredSize) {
        return NULL;
    }
    CFXMEM_Pool *pNewPool = NULL;
    requiredSize += sizeof(CFXMEM_Pool);
    size_t newSize = requiredSize;
    if (!m_pExtender->More(m_pExtender, newSize, (void**)&pNewPool, &newSize)) {
        return NULL;
    }
    size_t nMidPages = 0;
    if (m_MemConfig.nPageSize_Mid) {
        nMidPages = (newSize - requiredSize) / (m_MemConfig.nPageSize_Mid * FX_FIXEDMEM_PAGESIZE);
        if (nMidPages > m_MemConfig.nPageNum_MoreMid) {
            nMidPages = m_MemConfig.nPageNum_MoreMid;
        }
    }
    pNewPool->Initialize(&m_MemConfig, newSize, 0, m_MemConfig.nPageNum_More16, m_MemConfig.nPageNum_More32, nMidPages);
    pNewPool->m_pPrevPool = &m_FirstPool;
    CFXMEM_Pool *pPool = m_FirstPool.m_pNextPool;
    pNewPool->m_pNextPool = pPool;
    if (pPool) {
        pPool->m_pPrevPool = pNewPool;
    }
    m_FirstPool.m_pNextPool = pNewPool;
    return Alloc32(size);
}
FX_LPVOID CFXMEM_FixedMgr::AllocMid(size_t size)
{
    CFXMEM_Pool *pPool = &m_FirstPool;
    do {
        CFXMEM_Pages &pages = pPool->m_MidPages;
        if (pages.m_pLimitPos > pages.m_pStartPage) {
            FX_LPVOID p = pages.Alloc(size);
            if (p) {
                return p;
            }
        }
        pPool = pPool->m_pNextPool;
    } while(pPool);
    if (!m_pExtender) {
        return NULL;
    }
    size_t newSize = m_MemConfig.nPageSize_Mid * FX_FIXEDMEM_PAGESIZE * m_MemConfig.nPageNum_MoreMid;
    if (!newSize) {
        return NULL;
    }
    CFXMEM_Pool *pNewPool = NULL;
    newSize += sizeof(CFXMEM_Pool);
    if (!m_pExtender->More(m_pExtender, newSize, (void**)&pNewPool, &newSize)) {
        return NULL;
    }
    size_t nMidPages = (newSize - sizeof(CFXMEM_Pool)) / (m_MemConfig.nPageSize_Mid * FX_FIXEDMEM_PAGESIZE);
    if (nMidPages > m_MemConfig.nPageNum_MoreMid) {
        nMidPages = m_MemConfig.nPageNum_MoreMid;
    }
    pNewPool->Initialize(&m_MemConfig, newSize, 0, 0, 0, nMidPages);
    pNewPool->m_pPrevPool = &m_FirstPool;
    pPool = m_FirstPool.m_pNextPool;
    pNewPool->m_pNextPool = pPool;
    if (pPool) {
        pPool->m_pPrevPool = pNewPool;
    }
    m_FirstPool.m_pNextPool = pNewPool;
    return pNewPool->m_MidPages.Alloc(size);
}
FX_LPVOID CFXMEM_FixedMgr::AllocLarge(size_t size)
{
    CFXMEM_Pool *pPool = &m_FirstPool;
    do {
        if (!pPool->m_bAlone && pPool->m_pLargePage) {
            FX_LPVOID p = pPool->m_pLargePage->Alloc(size);
            if (p) {
                return p;
            }
        }
        pPool = pPool->m_pNextPool;
    } while(pPool);
    if (!m_pExtender || !m_MemConfig.nPageSize_Large) {
        return NULL;
    }
    CFXMEM_Pool *pNewPool = NULL;
#if _FX_WORDSIZE_ == _FX_W64_
    size_t newSize = ((size + 31) / 32 * 32 + sizeof(CFXMEM_Pool) + sizeof(CFXMEM_Page) + sizeof(CFXMEM_Block) + 4095) / 4096 * 4096;
#else
    size_t newSize = (size + 7) / 8 * 8 + sizeof(CFXMEM_Pool) + sizeof(CFXMEM_Page) + sizeof(CFXMEM_Block);
#endif
    if (newSize < m_MemConfig.nPageSize_Large * FX_FIXEDMEM_PAGESIZE) {
        newSize = m_MemConfig.nPageSize_Large * FX_FIXEDMEM_PAGESIZE;
    }
    if (!m_pExtender->More(m_pExtender, newSize, (void**)&pNewPool, &newSize)) {
        return NULL;
    }
    pNewPool->Initialize(&m_MemConfig, newSize, 0, 0, 0, 0);
    pNewPool->m_bAlone = size >= m_MemConfig.nPageSize_Alone * FX_FIXEDMEM_PAGESIZE;
    pNewPool->m_pPrevPool = &m_FirstPool;
    pPool = m_FirstPool.m_pNextPool;
    pNewPool->m_pNextPool = pPool;
    if (pPool) {
        pPool->m_pPrevPool = pNewPool;
    }
    m_FirstPool.m_pNextPool = pNewPool;
    return pNewPool->m_pLargePage->Alloc(size);
}
size_t CFXMEM_FixedMgr::GetSize(FX_LPVOID p) const
{
    const CFXMEM_Pool *pFind = &m_FirstPool;
    do {
        if (p > (FX_LPVOID)pFind && p < pFind->m_pLimitPos) {
            return pFind->GetSize(p);
        }
        pFind = pFind->m_pNextPool;
    } while (pFind);
    return 0;
}
FX_LPVOID CFXMEM_FixedMgr::Alloc(size_t size)
{
    FX_LPVOID p;
    if (size <= 32) {
        p = AllocSmall(size);
        if (p) {
            return p;
        }
    }
    if (size <= FX_FIXEDMEM_MIDBLOCKSIZE) {
        p = AllocMid(size);
        if (p) {
            return p;
        }
    }
    p = AllocLarge(size);
    return p;
}
FX_LPVOID CFXMEM_FixedMgr::ReallocSmall(CFXMEM_Pool* pPool, FX_LPVOID p, size_t oldSize, size_t newSize)
{
    FX_LPVOID np = AllocSmall(newSize);
    if (!np) {
        return NULL;
    }
    FXSYS_memcpy32(np, p, oldSize);
    pPool->Free(p);
    return np;
}
FX_LPVOID CFXMEM_FixedMgr::Realloc(FX_LPVOID p, size_t newSize)
{
    if (!p) {
        return Alloc(newSize);
    }
    size_t oldSize = 0;
    CFXMEM_Pool *pFind = &m_FirstPool;
    do {
        if (p > (FX_LPVOID)pFind && p < pFind->m_pLimitPos) {
            oldSize = pFind->GetSize(p);
            if (oldSize >= newSize) {
                return p;
            }
            break;
        }
        pFind = pFind->m_pNextPool;
    } while (pFind);
    if (!oldSize || !pFind) {
        return Alloc(newSize);
    }
    FX_LPVOID np = NULL;
    if (newSize <= 32) {
        np = ReallocSmall(pFind, p, oldSize, newSize);
        if (np) {
            return np;
        }
    }
    if (newSize <= FX_FIXEDMEM_MIDBLOCKSIZE) {
        np = pFind->Realloc(p, oldSize, newSize);
        if (np) {
            return np;
        }
    }
    np = Alloc(newSize);
    if (np) {
        FXSYS_memcpy32(np, p, oldSize);
        pFind->Free(p);
    }
    if (pFind->m_bAlone && pFind->IsEmpty()) {
        FreePool(pFind);
    }
    return np;
}
void CFXMEM_FixedMgr::Free(FX_LPVOID p)
{
    CFXMEM_Pool *pFind = &m_FirstPool;
    do {
        if (p > (FX_LPVOID)pFind && p < pFind->m_pLimitPos) {
            pFind->Free(p);
            if (pFind->m_bAlone && pFind->IsEmpty()) {
                FreePool(pFind);
            }
            return;
        }
        pFind = pFind->m_pNextPool;
    } while (pFind);
}
void CFXMEM_FixedMgr::FreePool(CFXMEM_Pool* pPool)
{
    FXSYS_assert(pPool->m_bAlone && pPool->IsEmpty());
    FXSYS_assert(m_pExtender != NULL);
    CFXMEM_Pool* pPrevPool = pPool->m_pPrevPool;
    CFXMEM_Pool* pNextPool = pPool->m_pNextPool;
    if (pPrevPool) {
        pPrevPool->m_pNextPool = pNextPool;
    }
    if (pNextPool) {
        pNextPool->m_pPrevPool = pPrevPool;
    }
    m_pExtender->Free(m_pExtender, pPool);
}
void CFXMEM_FixedMgr::FreeAll()
{
    if (!m_pExtender) {
        return;
    }
    CFXMEM_Pool* pPool = m_FirstPool.m_pNextPool;
    while (pPool) {
        CFXMEM_Pool* pPrevPool = pPool;
        pPool = pPool->m_pNextPool;
        m_pExtender->Free(m_pExtender, pPrevPool);
    }
    m_FirstPool.m_pNextPool = NULL;
}
void CFXMEM_FixedMgr::Purge()
{
    if (!m_pExtender) {
        return;
    }
    CFXMEM_Pool* pPool = m_FirstPool.m_pNextPool;
    while (pPool) {
        CFXMEM_Pool* pNextPool = pPool->m_pNextPool;
        if (pPool->IsEmpty()) {
            CFXMEM_Pool* pPrevPool = pPool->m_pPrevPool;
            pPrevPool->m_pNextPool = pNextPool;
            if (pNextPool) {
                pNextPool->m_pPrevPool = pPrevPool;
            }
            m_pExtender->Free(m_pExtender, pPool);
        }
        pPool = pNextPool;
    }
}
extern const FX_BYTE OneLeadPos[256] = {
    8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
extern const FX_BYTE ZeroLeadPos[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 7, 8,
};
