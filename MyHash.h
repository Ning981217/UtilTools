#pragma once

#include <iostream>
#include <string>

#include "MyAVLTree.h"

// 哈希表最大范围
#define TABLE_LEN 64

template<typename Key1, typename Key2, typename Val>
class CMyHash
{
public:
    CMyHash();
    CMyHash(const CMyHash &hash);
    ~CMyHash();
    CMyHash &operator=(const CMyHash &hash);

    bool Insert(Key1 key1, Key2 key2, Val val);
    bool Delete(Key1 key1, Key2 key2);
    bool Find(Key1 key, Key2 key2);
    CBST<Key2, Val> &operator[](Key1 key);
    CBST<Key2, Val> &operator[](size_t nIdx);

private:
    CBST<Key2, Val> m_ary[TABLE_LEN];

    void Init();
    void Clear();
};

template<typename Key1, typename Key2, typename Val>
inline CMyHash<Key1, Key2, Val>::CMyHash()
{
}

template<typename Key1, typename Key2, typename Val>
inline CMyHash<Key1, Key2, Val>::CMyHash(const CMyHash &hash)
{
    *this = hash;
}

template<typename Key1, typename Key2, typename Val>
inline CMyHash<Key1, typename Key2, Val>::~CMyHash()
{
    Clear();
}

template<typename Key1, typename Key2, typename Val>
inline CMyHash<Key1, Key2, Val> &CMyHash<Key1, typename Key2, Val>::operator=(const CMyHash &hash)
{
    if (this == &hash)
    {
        return *this;
    }

    Clear();

    for (size_t i = 0; i < TABLE_LEN; i++)
    {
        this->m_ary[i] = hash.m_ary[i];
    }

    return *this;
}

template<typename Key1, typename Key2, typename Val>
inline bool CMyHash<Key1, Key2, Val>::Insert(Key1 key1, Key2 key2, Val val)
{
    //获取下标索引
    size_t nIdx = std::hash<Key1>{}(key1) % TABLE_LEN;

    bool isSuccess = this->m_ary[nIdx].Insert(key2, val);

    return isSuccess;
}

template<typename Key1, typename Key2, typename Val>
inline bool CMyHash<Key1, Key2, Val>::Delete(Key1 key1, Key2 key2)
{
    //获取下标索引
    size_t nIdx = std::hash<Key1>{}(key1) % TABLE_LEN;

    bool isSuccess = this->m_ary[nIdx].Delete(key2);

    return isSuccess;
}

template<typename Key1, typename Key2, typename Val>
inline bool CMyHash<Key1, Key2, Val>::Find(Key1 key1, Key2 key2)
{
    //获取下标索引
    size_t nIdx = std::hash<Key1>{}(key1) % TABLE_LEN;

    Val val;
    bool isSuccess = this->m_ary[nIdx].Find(key2, val);

    return isSuccess;
}

template<typename Key1, typename Key2, typename Val>
inline CBST<Key2, Val> &CMyHash<Key1, Key2, Val>::operator[](Key1 key1)
{
    //获取下标索引
    size_t nIdx = std::hash<Key1>{}(key1) % TABLE_LEN;

    return this->m_ary[nIdx];
}

template <typename Key1, typename Key2, typename Val>
inline CBST<Key2, Val> &CMyHash<Key1, Key2, Val>::operator[](size_t nIdx)
{
    return this->m_ary[nIdx];
}

template <typename Key1, typename Key2, typename Val>
inline void CMyHash<Key1, Key2, Val>::Init()
{
}

template <typename Key1, typename Key2, typename Val>
inline void CMyHash<Key1, Key2, Val>::Clear()
{
    for (size_t i = 0; i < TABLE_LEN; i++)
    {
        this->m_ary[i].Clear();
    }
}
