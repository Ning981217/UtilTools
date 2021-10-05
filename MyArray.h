#pragma once

#include <iostream>
#include <string.h>
#include <assert.h>
#include <math.h>

template<typename T>
class CArray
{
public:
    CArray();
    CArray(const CArray &obj);
    virtual ~CArray();

    CArray &operator= (const CArray &obj);

    bool Insert(size_t nIdx, T &val);    //ָ��λ�ò���
    bool InsertHead(T &val);             //ͷ������
    bool InsertTail(T &val);             //β������

    bool Delete(size_t nIdx);            //ɾ��ָ��λ��
    bool DeleteHead();                   //ɾ��ͷ��
    bool DeleteTail();                   //ɾ��β��

    int Find(T val);                     //����

    T &operator[](size_t nIdx);          //�޸�

    size_t GetLen();                     //��ȡԪ�صĸ���

    bool IsEmpty();                      //�����Ƿ�Ϊ��

    void Clear();                        //�������

private:
    T *m_pBuff;                          //ָ�����ݵĻ�����
    size_t m_nBuffSize;                  //�������Ĵ�С
    size_t m_nLen;                       //������Ԫ�صĸ���

    void Init();                         //��ʼ��
};

template<typename T>
inline CArray<T>::CArray()
{
    Init();
}

template<typename T>
inline CArray<T>::CArray(const CArray &obj)
{
    Init();
    *this = obj;
}

template<typename T>
inline CArray<T>::~CArray()
{
    Clear();
}

template<typename T>
inline CArray<T> &CArray<T>::operator=(const CArray &obj)
{
    Clear();

    for (size_t i = 0; i < obj.m_nLen; i++)
    {
        InsertTail(obj.m_pBuff[i]);
    }

    return *this;
}

template<typename T>
inline bool CArray<T>::Insert(size_t nIdx, T &val)
{
    //���
    assert(nIdx <= this->m_nLen);

    //��ָ���ж�
    if (this->m_pBuff == nullptr)
    {
        this->m_pBuff = new T[1];
        this->m_nBuffSize = 1;
    }

    //����ԭ�ռ�����
    T *pTempData = nullptr;
    pTempData = new T[this->m_nLen];
    if (pTempData == nullptr)
    {
        return false;
    }
    //memcpy(pTempData, this->m_pBuff, this->m_nLen * sizeof(T));
    for (int i = 0; i < this->m_nLen; i++)
    {
        pTempData[i] = this->m_pBuff[i];
    }

    //�жϿռ��Ƿ��㹻
    if ((this->m_nLen + 1) > (this->m_nBuffSize * 0.75))
    {
        //�ռ䲻�㣬�����¿ռ�
        delete[] this->m_pBuff;
        size_t nSize = ceil(this->m_nBuffSize * 1.25);
        this->m_pBuff = new T[nSize];
        this->m_nBuffSize = nSize;
    }

    //������
    //ָ��λ��ǰ������
    //memcpy(this->m_pBuff, pTempData, nIdx * sizeof(T));
    for (int i = 0; i < nIdx; i++)
    {
        this->m_pBuff[i] = pTempData[i];
    }
    //ָ��λ������
    //memcpy((T *)(this->m_pBuff + nIdx), &val, sizeof(T));
    this->m_pBuff[nIdx] = val;
    //������ʣ������
    //memcpy((T *)(this->m_pBuff + nIdx + 1), (const T *)(pTempData + nIdx), (this->m_nLen - nIdx) * sizeof(T));
    for (int i = 0; i < this->m_nLen - nIdx; i++)
    {
        this->m_pBuff[nIdx + 1 + i] = pTempData[nIdx + i];
    }
    //�������ݳ���
    this->m_nLen++;

    //�ͷ���ʱ�洢�ռ�
    delete[] pTempData;
    return true;
}

template<typename T>
inline bool CArray<T>::InsertHead(T &val)
{
    return Insert(0, val);
}

template<typename T>
inline bool CArray<T>::InsertTail(T &val)
{
    return Insert(this->m_nLen, val);
}

template<typename T>
inline bool CArray<T>::Delete(size_t nIdx)
{
    //���
    assert(nIdx < this->m_nLen);

    //����ԭ�ռ�����
    T *pTempData = nullptr;
    pTempData = new T[this->m_nLen];
    if (pTempData == nullptr)
    {
        return false;
    }
    //memcpy(pTempData, this->m_pBuff, this->m_nLen * sizeof(T));
    for (int i = 0; i < this->m_nLen; i++)
    {
        pTempData[i] = this->m_pBuff[i];
    }

    //���ԭ����
    memset(this->m_pBuff, 0, this->m_nLen * sizeof(T));

    //������
    //ָ��λ��ǰ������
    //memcpy(this->m_pBuff, pTempData, nIdx * sizeof(T));
    for (int i = 0; i < nIdx; i++)
    {
        this->m_pBuff[i] = pTempData[i];
    }
    //����ָ������
    //memcpy((T *)(this->m_pBuff + nIdx), (const T *)(pTempData + nIdx + 1), (this->m_nLen - nIdx - 1) * sizeof(T));
    for (int i = 0; i < this->m_nLen - nIdx - 1; i++)
    {
        this->m_pBuff[nIdx + i] = pTempData[nIdx + 1 + i];
    }
    //�������ݳ���
    this->m_nLen--;

    //�ͷ���ʱ�洢�ռ�
    delete[] pTempData;
    return true;
}

template<typename T>
inline bool CArray<T>::DeleteHead()
{
    return Delete(0);
}

template<typename T>
inline bool CArray<T>::DeleteTail()
{
    return  Delete(this->m_nLen - 1);
}

template<typename T>
inline int CArray<T>::Find(T val)
{
    for (size_t i = 0; i < this->m_nLen; i++)
    {
        if (*(this->m_pBuff) == val)
        {
            return i;
        }
    }
    return -1;
}

template<typename T>
inline T &CArray<T>::operator[](size_t nIdx)
{
    assert(nIdx < this->m_nLen);
    return this->m_pBuff[nIdx];
}

template<typename T>
inline size_t CArray<T>::GetLen()
{
    return this->m_nLen;
}

template<typename T>
inline bool CArray<T>::IsEmpty()
{
    return this->m_nLen == 0;
}

template<typename T>
inline void CArray<T>::Clear()
{
    if (this->m_pBuff == nullptr)
    {
        delete[] this->m_pBuff;
    }
    Init();
}

template<typename T>
inline void CArray<T>::Init()
{
    this->m_pBuff = nullptr;
    this->m_nBuffSize = 0;
    this->m_nLen = 0;
}
