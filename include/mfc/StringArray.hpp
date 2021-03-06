
#include "Archive.hpp"

struct CStringArray
{
  
  DECLARE_SERIAL(CStringArray);
  
  CString* m_pData;   // the actual array of data
  int m_nSize;     // # of elements (upperBound - 1)
  int m_nMaxSize;  // MAX allocated
  int m_nGrowBy;   // grow amount
  
  
  // local typedefs for struct templates
  typedef CString BASE_TYPE;
  typedef LPCTSTR BASE_ARG_TYPE;
  
  
  _AFXCOLL_INLINE int CStringArray::GetSize() const
  { return m_nSize; }
  _AFXCOLL_INLINE int CStringArray::GetUpperBound() const
  { return m_nSize-1; }
  _AFXCOLL_INLINE void CStringArray::RemoveAll()
  { SetSize(0); }
  _AFXCOLL_INLINE CString CStringArray::GetAt(int nIndex) const
  { ASSERT(nIndex >= 0 && nIndex < m_nSize);
  return m_pData[nIndex]; }
  _AFXCOLL_INLINE void CStringArray::SetAt(int nIndex, LPCTSTR newElement)
  { ASSERT(nIndex >= 0 && nIndex < m_nSize);
  m_pData[nIndex] = newElement; }
  
  _AFXCOLL_INLINE void CStringArray::SetAt(int nIndex, const CString& newElement)
  { ASSERT(nIndex >= 0 && nIndex < m_nSize);
  m_pData[nIndex] = newElement; }
  
  _AFXCOLL_INLINE CString& CStringArray::ElementAt(int nIndex)
  { ASSERT(nIndex >= 0 && nIndex < m_nSize);
  return m_pData[nIndex]; }
  _AFXCOLL_INLINE const CString* CStringArray::GetData() const
  { return (const CString*)m_pData; }
  _AFXCOLL_INLINE CString* CStringArray::GetData()
  { return (CString*)m_pData; }
  _AFXCOLL_INLINE int CStringArray::Add(LPCTSTR newElement)
  { int nIndex = m_nSize;
  SetAtGrow(nIndex, newElement);
  return nIndex; }
  
  _AFXCOLL_INLINE int CStringArray::Add(const CString& newElement)
  { int nIndex = m_nSize;
  SetAtGrow(nIndex, newElement);
  return nIndex; }
  
  _AFXCOLL_INLINE CString CStringArray::operator[](int nIndex) const
  { return GetAt(nIndex); }
  _AFXCOLL_INLINE CString& CStringArray::operator[](int nIndex)
  { return ElementAt(nIndex); }
  
  
  
  CStringArray::CStringArray()
  {
    m_pData = NULL;
    m_nSize = m_nMaxSize = m_nGrowBy = 0;
  }
  
  CStringArray::~CStringArray()
  {
    ASSERT_VALID(this);
    
    
    DestructElements(m_pData, m_nSize);
    delete[] (BYTE*)m_pData;
  }
  
  void CStringArray::SetSize(int nNewSize, int nGrowBy = -1)
  {
    ASSERT_VALID(this);
    ASSERT(nNewSize >= 0);
    
    if (nGrowBy != -1)
      m_nGrowBy = nGrowBy;  // set new size
    
    if (nNewSize == 0)
    {
      // shrink to nothing
      
      DestructElements(m_pData, m_nSize);
      delete[] (BYTE*)m_pData;
      m_pData = NULL;
      m_nSize = m_nMaxSize = 0;
    }
    else if (m_pData == NULL)
    {
      // create one with exact size
#ifdef SIZE_T_MAX
      ASSERT(nNewSize <= SIZE_T_MAX/sizeof(CString));    // no overflow
#endif
      m_pData = (CString*) new BYTE[nNewSize * sizeof(CString)];
      
      ConstructElements(m_pData, nNewSize);
      
      m_nSize = m_nMaxSize = nNewSize;
    }
    else if (nNewSize <= m_nMaxSize)
    {
      // it fits
      if (nNewSize > m_nSize)
      {
        // initialize the new elements
        
        ConstructElements(&m_pData[m_nSize], nNewSize-m_nSize);
        
      }
      
      else if (m_nSize > nNewSize)  // destroy the old elements
        DestructElements(&m_pData[nNewSize], m_nSize-nNewSize);
      
      m_nSize = nNewSize;
    }
    else
    {
      // otherwise, grow array
      int nGrowBy = m_nGrowBy;
      if (nGrowBy == 0)
      {
        // heuristically determine growth when nGrowBy == 0
        //  (this avoids heap fragmentation in many situations)
        nGrowBy = MIN(1024, MAX(4, m_nSize / 8));
      }
      int nNewMax;
      if (nNewSize < m_nMaxSize + nGrowBy)
        nNewMax = m_nMaxSize + nGrowBy;  // granularity
      else
        nNewMax = nNewSize;  // no slush
      
      ASSERT(nNewMax >= m_nMaxSize);  // no wrap around
#ifdef SIZE_T_MAX
      ASSERT(nNewMax <= SIZE_T_MAX/sizeof(CString)); // no overflow
#endif
      CString* pNewData = (CString*) new BYTE[nNewMax * sizeof(CString)];
      
      // copy new data from old
      memcpy(pNewData, m_pData, m_nSize * sizeof(CString));
      
      // construct remaining elements
      ASSERT(nNewSize > m_nSize);
      
      ConstructElements(&pNewData[m_nSize], nNewSize-m_nSize);
      
      
      // get rid of old stuff (note: no destructors called)
      delete[] (BYTE*)m_pData;
      m_pData = pNewData;
      m_nSize = nNewSize;
      m_nMaxSize = nNewMax;
    }
  }
  
  int CStringArray::Append(const CStringArray& src)
  {
    ASSERT_VALID(this);
    ASSERT(this != &src);   // cannot append to itself
    
    int nOldSize = m_nSize;
    SetSize(m_nSize + src.m_nSize);
    
    CopyElements(m_pData + nOldSize, src.m_pData, src.m_nSize);
    
    return nOldSize;
  }
  
  void CStringArray::Copy(const CStringArray& src)
  {
    ASSERT_VALID(this);
    ASSERT(this != &src);   // cannot append to itself
    
    SetSize(src.m_nSize);
    
    CopyElements(m_pData, src.m_pData, src.m_nSize);
    
  }
  
  void CStringArray::FreeExtra()
  {
    ASSERT_VALID(this);
    
    if (m_nSize != m_nMaxSize)
    {
      // shrink to desired size
#ifdef SIZE_T_MAX
      ASSERT(m_nSize <= SIZE_T_MAX/sizeof(CString)); // no overflow
#endif
      CString* pNewData = NULL;
      if (m_nSize != 0)
      {
        pNewData = (CString*) new BYTE[m_nSize * sizeof(CString)];
        // copy new data from old
        memcpy(pNewData, m_pData, m_nSize * sizeof(CString));
      }
      
      // get rid of old stuff (note: no destructors called)
      delete[] (BYTE*)m_pData;
      m_pData = pNewData;
      m_nMaxSize = m_nSize;
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  void CStringArray::SetAtGrow(int nIndex, LPCTSTR newElement)
  {
    ASSERT_VALID(this);
    ASSERT(nIndex >= 0);
    
    if (nIndex >= m_nSize)
      SetSize(nIndex+1);
    m_pData[nIndex] = newElement;
  }
  
  
  void CStringArray::SetAtGrow(int nIndex, const CString& newElement)
  {
    ASSERT_VALID(this);
    ASSERT(nIndex >= 0);
    
    if (nIndex >= m_nSize)
      SetSize(nIndex+1);
    m_pData[nIndex] = newElement;
  }
  
  
  
  void CStringArray::InsertEmpty(int nIndex, int nCount = 1)
  {
    ASSERT_VALID(this);
    ASSERT(nIndex >= 0);    // will expand to meet need
    ASSERT(nCount > 0);     // zero or negative size not allowed
    
    if (nIndex >= m_nSize)
    {
      // adding after the end of the array
      SetSize(nIndex + nCount);  // grow so nIndex is valid
    }
    else
    {
      // inserting in the middle of the array
      int nOldSize = m_nSize;
      SetSize(m_nSize + nCount);  // grow it to new size
      // shift old data up to fill gap
      memmove(&m_pData[nIndex+nCount], &m_pData[nIndex],
        (nOldSize-nIndex) * sizeof(CString));
      
      // re-init slots we copied from
      
      ConstructElements(&m_pData[nIndex], nCount);
      
    }
    
    // insert new value in the gap
    ASSERT(nIndex + nCount <= m_nSize);
  }
  
  
  void CStringArray::InsertAt(int nIndex, LPCTSTR newElement, int nCount = 1)
  {
    
    // make room for new elements
    InsertEmpty(nIndex, nCount);
    
    
    
    // copy elements into the empty space
    CString temp = newElement;
    while (nCount--)
      m_pData[nIndex++] = temp;
    
  }
  
  
  void CStringArray::InsertAt(int nIndex, const CString& newElement, int nCount = 1)
  {
    // make room for new elements
    InsertEmpty(nIndex, nCount);
    
    // copy elements into the empty space
    while (nCount--)
      m_pData[nIndex++] = newElement;
  }
  
  
  void CStringArray::RemoveAt(int nIndex, int nCount = 1)
  {
    ASSERT_VALID(this);
    ASSERT(nIndex >= 0);
    ASSERT(nCount >= 0);
    ASSERT(nIndex + nCount <= m_nSize);
    
    // just remove a range
    int nMoveCount = m_nSize - (nIndex + nCount);
    
    DestructElements(&m_pData[nIndex], nCount);
    
    if (nMoveCount)
      memmove(&m_pData[nIndex], &m_pData[nIndex + nCount],
      nMoveCount * sizeof(CString));
    m_nSize -= nCount;
  }
  
  void CStringArray::InsertAt(int nStartIndex, CStringArray* pNewArray)
  {
    ASSERT_VALID(this);
    ASSERT(pNewArray != NULL);
    ASSERT_KINDOF(CStringArray, pNewArray);
    ASSERT_VALID(pNewArray);
    ASSERT(nStartIndex >= 0);
    
    if (pNewArray->GetSize() > 0)
    {
      InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());
      for (int i = 0; i < pNewArray->GetSize(); i++)
        SetAt(nStartIndex + i, pNewArray->GetAt(i));
    }
  }
  
  
  /////////////////////////////////////////////////////////////////////////////
  // Serialization
  
  void CStringArray::Serialize(CArchive& ar)
  {
    ASSERT_VALID(this);
    
    
    
    if (ar.IsStoring())
    {
      ar.WriteCount(m_nSize);
      for (int i = 0; i < m_nSize; i++)
        ar << m_pData[i];
    }
    else
    {
      DWORD nOldSize = ar.ReadCount();
      SetSize(nOldSize);
      for (int i = 0; i < m_nSize; i++)
        ar >> m_pData[i];
    }
  }
  
  
  
  /////////////////////////////////////////////////////////////////////////////
  // Diagnostics
  
#ifdef _DEBUG
  void CStringArray::Dump(CDumpContext& dc) const
  {
    
    
    dc << "with " << m_nSize << " elements";
    if (dc.GetDepth() > 0)
    {
      for (int i = 0; i < m_nSize; i++)
        dc << "\n\t[" << i << "] = " << m_pData[i];
    }
    
    dc << "\n";
  }
  
  void CStringArray::AssertValid() const
  {
    
    
    if (m_pData == NULL)
    {
      ASSERT(m_nSize == 0);
      ASSERT(m_nMaxSize == 0);
    }
    else
    {
      ASSERT(m_nSize >= 0);
      ASSERT(m_nMaxSize >= 0);
      ASSERT(m_nSize <= m_nMaxSize);
      ASSERT(AfxIsValidAddress(m_pData, m_nMaxSize * sizeof(CString)));
    }
  }
#endif //_DEBUG
  
  
};