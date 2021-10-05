#pragma once

#include <assert.h>

template<typename T>
class CList
{
private:
    //链表的节点
    typedef struct tagNode
    {
        //构造
        tagNode();
        tagNode(T val);
        //~tagNode();

        //T *m_pVal;                            //节点保存的数据
        T m_val;                            //节点保存的数据
        tagNode *m_pPre;                    //前驱节点的指针
        tagNode *m_pNext;                   //后继节点的指针
    }NODE, *PNODE;

public:
    //正向迭代器
    class Iterator
    {
    public:
        //构造
        Iterator(PNODE pPos) :m_pPos(pPos)
        {
        }

        //运算符重载
        virtual Iterator &operator++()
        {
            //向后移动不能超过尾哨兵
            assert(this->m_pPos->m_pNext != nullptr);

            //位置后移,指向自己的后继
            this->m_pPos = this->m_pPos->m_pNext;
            return *this;
        }
        virtual Iterator operator++(int)
        {
            assert(this->m_pPos->m_pNext != nullptr);

            Iterator itrTemp(this->m_pPos);
            ++(*this);
            return itrTemp;
        }

        virtual Iterator &operator--()
        {
            //向前移动不能超过头元素
            assert(this->m_pPos->m_pPre->m_pPre != nullptr);

            this->m_pPos = this->m_pPos->m_pPre;
            return *this;
        }
        virtual Iterator operator--(int)
        {
            assert(this->m_pPos->m_pPre->m_pPre != nullptr);

            Iterator itrTemp(this->m_pPos);
            --(*this);
            return itrTemp;
        }

        bool operator==(const Iterator &obj)
        {
            return this->m_pPos == obj.m_pPos;
        }
        bool operator!=(const Iterator &obj)
        {
            return !(*this == obj);
        }

        T &operator*()
        {
            //尾哨兵不能取内容
            assert(this->m_pPos->m_pNext != nullptr);

            return this->m_pPos->m_val;
        }
        T *operator->()
        {
            assert(this->m_pPos->m_pNext != nullptr);

            return &(this->m_pPos->m_val);
        }

        PNODE m_pPos;                       //节点的指针

    };

    //反向迭代器
    class reverse_Iterator :
        public Iterator
    {
    public:
        //构造
        reverse_Iterator(PNODE pPos) :Iterator(pPos)
        {
        }

        //运算符重载
        virtual Iterator &operator++()
        {
            //反向++等于正向--
            assert(this->m_pPos->m_pPre != nullptr);

            this->m_pPos = this->m_pPos->m_pPre;
            return *this;
        }
        virtual Iterator operator++(int)
        {
            assert(this->m_pPos->m_pPre != nullptr);

            Iterator itrTemp(this->m_pPos);
            --(*this);
            return itrTemp;
        }

        virtual Iterator &operator--()
        {
            //反向--等于正向++
            assert(this->m_pPos->m_pNext->m_pNext != nullptr);

            //位置后移,指向自己的后继
            this->m_pPos = this->m_pPos->m_pNext;
            return *this;
        }
        virtual Iterator operator--(int)
        {
            assert(this->m_pPos->m_pNext->m_pNext != nullptr);

            Iterator itrTemp(this->m_pPos);
            ++(*this);
            return itrTemp;
        }
    };

    //构造
    CList();
    CList(const CList &obj);
    ~CList();

    //运算符重载
    CList &operator=(const CList &obj);

    //插入
    Iterator Insert(Iterator pNode, T val);       //在指定节点后插入数据
    Iterator InsertHead(T val);
    Iterator InsertTail(T val);

    //删除
    bool Delete(Iterator pNode);               //删除指定节点
    bool DeleteHead();
    bool DeleteTail();

    //查找
    Iterator Find(T val) const;                //查找，失败为nullptr

    //修改
    T &operator[](size_t nIdx);

    //清空数据
    void Clear();

    //获取元素个数
    size_t GetLen();

    //是否为空
    bool IsEmpty();

    Iterator begin();
    Iterator end();

    reverse_Iterator rbegin();
    reverse_Iterator rend();

private:
    PNODE m_pHeadSentinel;                  //哨兵头
    PNODE m_pTailSentinel;                  //哨兵尾
    size_t m_nLen;                          //链表中元素的个数

    void Init();                            //初始化
};

template<typename T>
inline CList<T>::tagNode::tagNode() :
    m_pPre(nullptr), m_pNext(nullptr)
{
}

template<typename T>
inline CList<T>::tagNode::tagNode(T val) :
    m_val(val), m_pPre(nullptr), m_pNext(nullptr)
{
}

//template<typename T>
//inline CList<T>::tagNode::~tagNode()
//{
//    if (m_val != nullptr)
//    {
//        delete m_val;
//    }
//    m_val = nullptr;
//    m_pPre = nullptr;
//    m_pNext = nullptr;
//}

template<typename T>
inline CList<T>::CList()
{
    Init();
}

template<typename T>
inline CList<T>::CList(const CList &obj)
{
    Init();

    *this = obj;
}

template<typename T>
inline CList<T>::~CList()
{
    Clear();

    if (this->m_pHeadSentinel != nullptr)
    {
        delete this->m_pHeadSentinel;
    }
    if (this->m_pTailSentinel != nullptr)
    {
        delete this->m_pTailSentinel;
    }

    this->m_pHeadSentinel = nullptr;
    this->m_pTailSentinel = nullptr;
}

template<typename T>
inline CList<T> &CList<T>::operator=(const CList &obj)
{
    //防止 自己 = 自己
    if (this == &obj)
    {
        return *this;
    }

    Clear();

    PNODE pNow = obj.m_pHeadSentinel->m_pNext;
    for (size_t i = 0; i < obj.m_nLen; i++)
    {
        InsertTail(pNow->m_val);
        pNow = pNow->m_pNext;
    }
    this->m_nLen = obj.m_nLen;

    return *this;
}

template<typename T>
typename inline CList<T>::Iterator CList<T>::Insert(Iterator pNode, T val)
{
    //检查
    if (pNode == nullptr)
    {
        return nullptr;
    }

    //新节点数据
    PNODE pNow = new NODE(val);
    if (pNow == nullptr)
    {
        return nullptr;
    }

    //上一节点
    PNODE pPre = pNode.m_pPos;
    //下一节点
    PNODE pNext = pNode.m_pPos->m_pNext;

    //新节点前驱指向上一节点
    pNow->m_pPre = pPre;
    //新节点后继指向下一节点
    pNow->m_pNext = pNext;

    //下一节点的前驱指向新节点
    pNext->m_pPre = pNow;
    //上一节点的后继指向新节点
    pPre->m_pNext = pNow;

    this->m_nLen++;
    return Iterator(pNow);
}

template<typename T>
typename inline CList<T>::Iterator CList<T>::InsertHead(T val)
{
    return Insert(this->m_pHeadSentinel, val);
}

template<typename T>
typename inline CList<T>::Iterator CList<T>::InsertTail(T val)
{
    return Insert(this->m_pTailSentinel->m_pPre, val);
}

template<typename T>
typename inline bool CList<T>::Delete(Iterator pNode)
{
    if (pNode == nullptr)
    {
        return false;
    }
    //上一节点
    PNODE pPre = pNode.m_pPos->m_pPre;
    //下一节点
    PNODE pNext = pNode.m_pPos->m_pNext;

    //上一节点的后继指向下一节点
    pPre->m_pNext = pNext;
    //下一节点的前驱指向上一节点
    pNext->m_pPre = pPre;

    delete pNode.m_pPos;
    this->m_nLen--;

    return true;
}

template<typename T>
inline bool CList<T>::DeleteHead()
{
    return Delete(this->m_pHeadSentinel->m_pNext);
}

template<typename T>
inline bool CList<T>::DeleteTail()
{
    return Delete(this->m_pTailSentinel->m_pPre);
}

template<typename T>
typename inline CList<T>::Iterator CList<T>::Find(T val) const
{
    PNODE pFirst = this->m_pHeadSentinel->m_pNext;
    for (PNODE item = pFirst; item != this->m_pTailSentinel; item = item->m_pNext)
    {
        if (item->m_val == val)
        {
            return Iterator(item);
        }
    }
    return nullptr;
}

template<typename T>
inline T &CList<T>::operator[](size_t nIdx)
{
    assert(nIdx < this->m_nLen);

    size_t nCount = 0;
    if (this->m_nLen / 2 >= nIdx)
    {
        for (Iterator itr = begin(); itr != end(); itr++)
        {
            if (nCount == nIdx)
            {
                return itr.m_pPos->m_val;
            }
            nCount++;
        }
    }
    else
    {
        for (reverse_Iterator rItr = rbegin(); rItr != rend(); rItr++)
        {
            nCount = this->m_nLen - 1;
            if (nCount == nIdx)
            {
                return rItr.m_pPos->m_val;
            }
            nCount--;
        }
    }
}

template<typename T>
inline void CList<T>::Clear()
{
    PNODE pFirst = this->m_pHeadSentinel->m_pNext;
    PNODE pNext = nullptr;
    for (PNODE item = pFirst; item != this->m_pTailSentinel; item = pNext)
    {
        if (item != nullptr)
        {
            pNext = item->m_pNext;
            Delete(item);
        }
    }
    this->m_nLen = 0;
}

template<typename T>
inline size_t CList<T>::GetLen()
{
    return this->m_nLen;
}

template<typename T>
inline bool CList<T>::IsEmpty()
{
    return this->m_nLen == 0;
}

template<typename T>
typename inline CList<T>::Iterator CList<T>::begin()
{
    return Iterator(this->m_pHeadSentinel->m_pNext);
}

template<typename T>
typename inline CList<T>::Iterator CList<T>::end()
{
    return Iterator(this->m_pTailSentinel);
}

template<typename T>
typename inline CList<T>::reverse_Iterator CList<T>::rbegin()
{
    return reverse_Iterator(this->m_pTailSentinel->m_pPre);
}

template<typename T>
typename inline CList<T>::reverse_Iterator CList<T>::rend()
{
    return reverse_Iterator(this->m_pHeadSentinel);
}

template<typename T>
inline void CList<T>::Init()
{
    //链表为空时，只存在头尾哨兵
    //头哨兵后继指向尾哨兵，尾哨兵前驱指向头哨兵
    this->m_pHeadSentinel = new NODE;
    this->m_pTailSentinel = new NODE;

    this->m_pHeadSentinel->m_pNext = this->m_pTailSentinel;
    this->m_pTailSentinel->m_pPre = this->m_pHeadSentinel;
    this->m_nLen = 0;
}