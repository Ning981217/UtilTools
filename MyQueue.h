#pragma once

#include "MyList.h"

template<typename T>
class CQueue
{
public:
    void Push(T tVal);
    void Pop();
    T Front();
    size_t Size();
    bool IsEmpty();
    void Clear();

private:
    CList<T> m_lst;
};

template<typename T>
inline void CQueue<T>::Push(T tVal)
{
    m_lst.InsertTail(tVal);
}

template<typename T>
inline void CQueue<T>::Pop()
{
    m_lst.DeleteHead();
}

template<typename T>
inline T CQueue<T>::Front()
{
    return *m_lst.begin();
}

template<typename T>
inline size_t CQueue<T>::Size()
{
    return m_lst.GetLen();
}

template<typename T>
inline bool CQueue<T>::IsEmpty()
{
    return m_lst.IsEmpty();
}

template<typename T>
inline void CQueue<T>::Clear()
{
    m_lst.Clear();
}
