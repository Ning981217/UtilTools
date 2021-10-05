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

    bool Insert(size_t nIdx, T &val);    //指定位置插入
    bool InsertHead(T &val);             //头部插入
    bool InsertTail(T &val);             //尾部插入

    bool Delete(size_t nIdx);            //删除指定位置
    bool DeleteHead();                   //删除头部
    bool DeleteTail();                   //删除尾部

    int Find(T val);                     //查找

    T &operator[](size_t nIdx);          //修改

    size_t GetLen();                     //获取元素的个数

    bool IsEmpty();                      //数组是否为空

    void Clear();                        //清空数组

private:
    T *m_pBuff;                          //指向数据的缓冲区
    size_t m_nBuffSize;                  //缓冲区的大小
    size_t m_nLen;                       //数组中元素的个数

    void Init();                         //初始化
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
    //检查
    assert(nIdx <= this->m_nLen);

    //空指针判断
    if (this->m_pBuff == nullptr)
    {
        this->m_pBuff = new T[1];
        this->m_nBuffSize = 1;
    }

    //保存原空间数据
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

    //判断空间是否足够
    if ((this->m_nLen + 1) > (this->m_nBuffSize * 0.75))
    {
        //空间不足，申请新空间
        delete[] this->m_pBuff;
        size_t nSize = ceil(this->m_nBuffSize * 1.25);
        this->m_pBuff = new T[nSize];
        this->m_nBuffSize = nSize;
    }

    //新数据
    //指定位置前的数据
    //memcpy(this->m_pBuff, pTempData, nIdx * sizeof(T));
    for (int i = 0; i < nIdx; i++)
    {
        this->m_pBuff[i] = pTempData[i];
    }
    //指定位置数据
    //memcpy((T *)(this->m_pBuff + nIdx), &val, sizeof(T));
    this->m_pBuff[nIdx] = val;
    //插入后的剩余数据
    //memcpy((T *)(this->m_pBuff + nIdx + 1), (const T *)(pTempData + nIdx), (this->m_nLen - nIdx) * sizeof(T));
    for (int i = 0; i < this->m_nLen - nIdx; i++)
    {
        this->m_pBuff[nIdx + 1 + i] = pTempData[nIdx + i];
    }
    //更新数据长度
    this->m_nLen++;

    //释放临时存储空间
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
    //检查
    assert(nIdx < this->m_nLen);

    //保存原空间数据
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

    //清空原数据
    memset(this->m_pBuff, 0, this->m_nLen * sizeof(T));

    //新数据
    //指定位置前的数据
    //memcpy(this->m_pBuff, pTempData, nIdx * sizeof(T));
    for (int i = 0; i < nIdx; i++)
    {
        this->m_pBuff[i] = pTempData[i];
    }
    //跳过指定数据
    //memcpy((T *)(this->m_pBuff + nIdx), (const T *)(pTempData + nIdx + 1), (this->m_nLen - nIdx - 1) * sizeof(T));
    for (int i = 0; i < this->m_nLen - nIdx - 1; i++)
    {
        this->m_pBuff[nIdx + i] = pTempData[nIdx + 1 + i];
    }
    //更新数据长度
    this->m_nLen--;

    //释放临时存储空间
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
