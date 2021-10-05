#pragma once

#include "MyList.h"

//栈底 : 链表头      栈顶 : 链表尾
template<typename T>
class CStack
{
public:
    bool Push(T val);           //入栈
    bool Pop();                 //出栈
    T &Top();                   //访问栈顶元素
    size_t Size();              //获得元素个数
    void Clear();               //清空栈
    bool IsEmpty();             //是否为空

private:
    CList<T> m_lst;             //保存栈数据
};

template<typename T>
inline bool CStack<T>::Push(T val)
{
    return this->m_lst.InsertTail(val) != nullptr;
}

template<typename T>
inline bool CStack<T>::Pop()
{
    return this->m_lst.DeleteTail();
}

template<typename T>
inline T &CStack<T>::Top()
{
    return this->m_lst.rbegin().m_pPos->m_val;
}

template<typename T>
inline size_t CStack<T>::Size()
{
    return this->m_lst.GetLen();
}

template<typename T>
inline void CStack<T>::Clear()
{
    return this->m_lst.Clear();
}

template<typename T>
inline bool CStack<T>::IsEmpty()
{
    return this->m_lst.IsEmpty();
}
