// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef _FXM_MEM_INT_H_
#define _FXM_MEM_INT_H_
struct FX_DefAllocator {
    IFX_Allocator			m_Allocator;
    struct CFX_MemoryMgr*	m_pFoxitMgr;
};
struct CFX_MemoryMgr {
public:
    FXMEM_SystemMgr*	m_pSystemMgr;
    FX_DefAllocator		m_DefAllocator;
    FX_LPVOID			m_pExternalMemory;
    FX_BOOL				m_bReleaseMgr;
    void			Init(FXMEM_SystemMgr* pSystemMgr);
    void*			Alloc(size_t size, int flags);
    void*			AllocDebug(size_t size, int flags, FX_LPCSTR file, int line);
    void*			Realloc(void* p, size_t size, int flags);
    void*			ReallocDebug(void* p, size_t size, int flags, FX_LPCSTR file, int line);
    void			Free(void* p, int flags);
    void			PurgeMgr();
};
extern CFX_MemoryMgr* g_pDefFoxitMgr;
#define FIXEDMEM_PAGE_EXTRASPACE		sizeof(size_t)
#define FIXEDMEM_BLOCKNUM(bs)			(8 * (FX_FIXEDMEM_PAGESIZE - FIXEDMEM_PAGE_EXTRASPACE) / (8 * bs + 1))
#define FIXEDMEM_8BYTES_BLOCKNUM		FIXEDMEM_BLOCKNUM(8)
#define FIXEDMEM_16BYTES_BLOCKNUM		FIXEDMEM_BLOCKNUM(16)
#define FIXEDMEM_32BYTES_BLOCKNUM		FIXEDMEM_BLOCKNUM(32)
extern const FX_BYTE ZeroLeadPos[256];
extern const FX_BYTE OneLeadPos[256];
template <size_t blockNum, size_t blockSize>
class CFXMEM_FixedPage
{
public:
    void		Initialize()
    {
        m_nAvailCount = blockNum;
        FXSYS_memset32(m_BusyMap, 0, (blockNum + 7) / 8);
    }
    FX_BOOL		HasFreeBlock() const
    {
        return (FX_BOOL)m_nAvailCount;
    }
    FX_LPVOID	Alloc(size_t size)
    {
        FXSYS_assert(m_nAvailCount);
        FX_LPDWORD pFind = (FX_LPDWORD)m_BusyMap;
        size_t i = 0;
        while (i < (blockNum + 7) / 8 / 4 && pFind[i] == 0xFFFFFFFF) {
            i ++;
        }
        i *= 4;
        while (m_BusyMap[i] == 0xFF) {
            i ++;
        }
        size_t pos = ZeroLeadPos[m_BusyMap[i]];
        m_BusyMap[i] |= 1 << (7 - pos);
        m_nAvailCount --;
        return (FX_LPBYTE)(this + 1) + (i * 8 + pos) * blockSize;
    }
    void		Free(FX_LPVOID p)
    {
        FXSYS_assert(p > (FX_LPVOID)this && p < (FX_LPVOID)((FX_LPBYTE)this + FX_FIXEDMEM_PAGESIZE));
        size_t pos = ((FX_LPBYTE)p - (FX_LPBYTE)(this + 1)) / blockSize;
        m_BusyMap[pos / 8] &= ~(1 << (7 - (pos % 8)));
        m_nAvailCount ++;
    }
    volatile size_t	m_nAvailCount;
    FX_BYTE			m_BusyMap[(blockNum + 7) / 8];
};
typedef CFXMEM_FixedPage<FIXEDMEM_8BYTES_BLOCKNUM, 8>	CFXMEM_8BytesPage;
typedef CFXMEM_FixedPage<FIXEDMEM_16BYTES_BLOCKNUM, 16>	CFXMEM_16BytesPage;
typedef CFXMEM_FixedPage<FIXEDMEM_32BYTES_BLOCKNUM, 32>	CFXMEM_32BytesPage;
template <size_t blockNum, size_t blockSize>
class CFXMEM_FixedPages
{
public:
    typedef CFXMEM_FixedPage<blockNum, blockSize> T;
    FX_LPBYTE		m_pStartPage;
    FX_LPBYTE		m_pLimitPos;
    FX_LPBYTE		m_pCurPage;
    volatile size_t	m_nAvailBlocks;
    void		Initialize(FX_LPBYTE pStart, size_t pages)
    {
        m_pStartPage = m_pCurPage = pStart;
        m_nAvailBlocks = pages * blockNum;
        for (size_t n = 0; n < pages; n ++) {
            ((T*)pStart)->Initialize();
            pStart += FX_FIXEDMEM_PAGESIZE;
        }
        m_pLimitPos = pStart;
    }
    FX_BOOL		IsEmpty() const
    {
        return m_nAvailBlocks == (m_pLimitPos - m_pStartPage) / FX_FIXEDMEM_PAGESIZE * blockNum;
    }
    FX_BOOL		HasFreeBlock() const
    {
        return (FX_BOOL)m_nAvailBlocks;
    }
    FX_LPVOID	Alloc(size_t size)
    {
        FXSYS_assert(m_nAvailBlocks);
        do {
            if (((T*)m_pCurPage)->HasFreeBlock()) {
                m_nAvailBlocks --;
                return ((T*)m_pCurPage)->Alloc(size);
            }
            m_pCurPage += FX_FIXEDMEM_PAGESIZE;
            if (m_pCurPage == m_pLimitPos) {
                m_pCurPage = m_pStartPage;
            }
        } while (TRUE);
        return NULL;
    }
    void		Free(FX_LPVOID p)
    {
        FXSYS_assert(p > (FX_LPVOID)m_pStartPage && p < (FX_LPVOID)m_pLimitPos);
        ((T*)(m_pStartPage + ((FX_LPBYTE)p - m_pStartPage) / FX_FIXEDMEM_PAGESIZE * FX_FIXEDMEM_PAGESIZE))->Free(p);
        m_nAvailBlocks ++;
    }
};
typedef CFXMEM_FixedPages<FIXEDMEM_8BYTES_BLOCKNUM, 8>		CFXMEM_8BytesPages;
typedef CFXMEM_FixedPages<FIXEDMEM_16BYTES_BLOCKNUM, 16>	CFXMEM_16BytesPages;
typedef CFXMEM_FixedPages<FIXEDMEM_32BYTES_BLOCKNUM, 32>	CFXMEM_32BytesPages;
class CFXMEM_Block
{
public:
    size_t			m_nBlockSize;
    CFXMEM_Block*	m_pNextBlock;
};
class CFXMEM_Page
{
public:
    size_t			m_nAvailSize;
    CFXMEM_Block*	m_pLimitPos;
    CFXMEM_Block	m_AvailHead;
    void		Initialize(size_t size);
    FX_BOOL		IsEmpty() const
    {
        return m_AvailHead.m_pNextBlock && m_AvailHead.m_nBlockSize == m_AvailHead.m_pNextBlock->m_nBlockSize;
    }
    FX_LPVOID	Alloc(size_t size);
    FX_LPVOID	Realloc(FX_LPVOID p, size_t oldSize, size_t newSize);
    void		Free(FX_LPVOID p);
protected:
    FX_LPVOID	Alloc(CFXMEM_Block* pPrevBlock, CFXMEM_Block* pNextBlock, size_t size, size_t oldsize);
};
class CFXMEM_Pages
{
public:
    CFXMEM_Page*	m_pStartPage;
    CFXMEM_Page*	m_pLimitPos;
    CFXMEM_Page*	m_pCurPage;
    size_t			m_nPageSize;
    void		Initialize(FX_LPBYTE pStart, size_t pageSize, size_t pages);
    FX_BOOL		IsEmpty() const;
    FX_LPVOID	Alloc(size_t size);
    FX_LPVOID	Realloc(FX_LPVOID p, size_t oldSize, size_t newSize);
    void		Free(FX_LPVOID p);
};
class CFXMEM_Pool
{
public:
    CFXMEM_Pool*			m_pPrevPool;
    CFXMEM_Pool*			m_pNextPool;
    CFXMEM_8BytesPages		m_8BytesPages;
    CFXMEM_16BytesPages		m_16BytesPages;
    CFXMEM_32BytesPages		m_32BytesPages;
    CFXMEM_Pages			m_MidPages;
    FX_BOOL					m_bAlone;
    FX_DWORD				m_dwReserved[3];
    FX_LPVOID				m_pLimitPos;
    CFXMEM_Page*			m_pLargePage;
    void		Initialize(const FX_MEMCONFIG* pMemConfig, size_t size, size_t pageNum8Bytes, size_t pageNum16Bytes, size_t pageNum32Bytes, size_t pageNumMid);
    FX_BOOL		IsEmpty() const;
    size_t		GetSize(FX_LPVOID p) const;
    FX_LPVOID	Realloc(FX_LPVOID p, size_t oldSize, size_t newSize);
    void		Free(FX_LPVOID p);
};
class CFXMEM_FixedMgr
{
public:
    void			Initialize(size_t size);
    FX_LPVOID		Alloc(size_t size);
    FX_LPVOID		Realloc(FX_LPVOID p, size_t newSize);
    void			Free(FX_LPVOID p);
    void			FreeAll();
    void			Purge();
    CFXMEM_Pool*	GetFirstPool()
    {
        return &m_FirstPool;
    }
    size_t			GetSize(FX_LPVOID p) const;
    FXMEM_SystemMgr		m_SystemMgr;
    FXMEM_SystemMgr2*	m_pExtender;
    FX_LPVOID			m_pReserved;
    FX_MEMCONFIG		m_MemConfig;
protected:
    FX_LPVOID		Alloc16(CFXMEM_Pool **pp32Pool = NULL, size_t size = 0);
    FX_LPVOID		Alloc32(size_t size);
    FX_LPVOID		AllocSmall(size_t size);
    FX_LPVOID		AllocMid(size_t size);
    FX_LPVOID		AllocLarge(size_t size);
    FX_LPVOID		ReallocSmall(CFXMEM_Pool* pPool, FX_LPVOID p, size_t oldSize, size_t newSize);
    void			FreePool(CFXMEM_Pool* pPool);
    CFXMEM_Pool		m_FirstPool;
};
#define FIXEDMEM_PROXYSIZE_0	(1024 * 1024 * 8)
#define FIXEDMEM_PROXYSIZE_1	(1024 * 1024 * 16)
#define FIXEDMEM_PROXYSIZE_2	(1024 * 1024 * 32)
#define FIXEDMEM_PROXYSIZE_3	(1024 * 1024 * 64)
#define FIXEDMEM_PROXYSIZE_4	(1024 * 1024 * 128)
#define FIXEDMEM_PROXYSIZE_5	(1024 * 1024 * 256)
const FX_MEMCONFIG*	FixedMgr_GetConfig(size_t nSize);
class CFixedMgr_Proxy
{
public:
    FXMEM_FoxitMgr*	Initialize(FX_LPVOID pBuffer, size_t nSize, FX_BOOL bExtensible);
    static FX_BOOL	Common_More(FXMEM_SystemMgr2* pMgr, size_t alloc_size, void** new_memory, size_t* new_size);
    static void		Common_Free(FXMEM_SystemMgr2* pMgr, void* memory);
    FXMEM_SystemMgr2	m_SystemMgr;
    CFXMEM_Page*		m_pFixedPage;
    FX_LPVOID			m_pBuffer;
    size_t				m_nSize;
    FX_BOOL				m_bExtensible;
};
#endif
