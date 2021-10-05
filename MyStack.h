#pragma once

#include "MyList.h"

//ջ�� : ����ͷ      ջ�� : ����β
template<typename T>
class CStack
{
public:
    bool Push(T val);           //��ջ
    bool Pop();                 //��ջ
    T &Top();                   //����ջ��Ԫ��
    size_t Size();              //���Ԫ�ظ���
    void Clear();               //���ջ
    bool IsEmpty();             //�Ƿ�Ϊ��

private:
    CList<T> m_lst;             //����ջ����
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
