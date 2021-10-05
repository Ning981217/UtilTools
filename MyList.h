#pragma once

#include <assert.h>

template<typename T>
class CList
{
private:
    //����Ľڵ�
    typedef struct tagNode
    {
        //����
        tagNode();
        tagNode(T val);
        //~tagNode();

        //T *m_pVal;                            //�ڵ㱣�������
        T m_val;                            //�ڵ㱣�������
        tagNode *m_pPre;                    //ǰ���ڵ��ָ��
        tagNode *m_pNext;                   //��̽ڵ��ָ��
    }NODE, *PNODE;

public:
    //���������
    class Iterator
    {
    public:
        //����
        Iterator(PNODE pPos) :m_pPos(pPos)
        {
        }

        //���������
        virtual Iterator &operator++()
        {
            //����ƶ����ܳ���β�ڱ�
            assert(this->m_pPos->m_pNext != nullptr);

            //λ�ú���,ָ���Լ��ĺ��
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
            //��ǰ�ƶ����ܳ���ͷԪ��
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
            //β�ڱ�����ȡ����
            assert(this->m_pPos->m_pNext != nullptr);

            return this->m_pPos->m_val;
        }
        T *operator->()
        {
            assert(this->m_pPos->m_pNext != nullptr);

            return &(this->m_pPos->m_val);
        }

        PNODE m_pPos;                       //�ڵ��ָ��

    };

    //���������
    class reverse_Iterator :
        public Iterator
    {
    public:
        //����
        reverse_Iterator(PNODE pPos) :Iterator(pPos)
        {
        }

        //���������
        virtual Iterator &operator++()
        {
            //����++��������--
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
            //����--��������++
            assert(this->m_pPos->m_pNext->m_pNext != nullptr);

            //λ�ú���,ָ���Լ��ĺ��
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

    //����
    CList();
    CList(const CList &obj);
    ~CList();

    //���������
    CList &operator=(const CList &obj);

    //����
    Iterator Insert(Iterator pNode, T val);       //��ָ���ڵ���������
    Iterator InsertHead(T val);
    Iterator InsertTail(T val);

    //ɾ��
    bool Delete(Iterator pNode);               //ɾ��ָ���ڵ�
    bool DeleteHead();
    bool DeleteTail();

    //����
    Iterator Find(T val) const;                //���ң�ʧ��Ϊnullptr

    //�޸�
    T &operator[](size_t nIdx);

    //�������
    void Clear();

    //��ȡԪ�ظ���
    size_t GetLen();

    //�Ƿ�Ϊ��
    bool IsEmpty();

    Iterator begin();
    Iterator end();

    reverse_Iterator rbegin();
    reverse_Iterator rend();

private:
    PNODE m_pHeadSentinel;                  //�ڱ�ͷ
    PNODE m_pTailSentinel;                  //�ڱ�β
    size_t m_nLen;                          //������Ԫ�صĸ���

    void Init();                            //��ʼ��
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
    //��ֹ �Լ� = �Լ�
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
    //���
    if (pNode == nullptr)
    {
        return nullptr;
    }

    //�½ڵ�����
    PNODE pNow = new NODE(val);
    if (pNow == nullptr)
    {
        return nullptr;
    }

    //��һ�ڵ�
    PNODE pPre = pNode.m_pPos;
    //��һ�ڵ�
    PNODE pNext = pNode.m_pPos->m_pNext;

    //�½ڵ�ǰ��ָ����һ�ڵ�
    pNow->m_pPre = pPre;
    //�½ڵ���ָ����һ�ڵ�
    pNow->m_pNext = pNext;

    //��һ�ڵ��ǰ��ָ���½ڵ�
    pNext->m_pPre = pNow;
    //��һ�ڵ�ĺ��ָ���½ڵ�
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
    //��һ�ڵ�
    PNODE pPre = pNode.m_pPos->m_pPre;
    //��һ�ڵ�
    PNODE pNext = pNode.m_pPos->m_pNext;

    //��һ�ڵ�ĺ��ָ����һ�ڵ�
    pPre->m_pNext = pNext;
    //��һ�ڵ��ǰ��ָ����һ�ڵ�
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
    //����Ϊ��ʱ��ֻ����ͷβ�ڱ�
    //ͷ�ڱ����ָ��β�ڱ���β�ڱ�ǰ��ָ��ͷ�ڱ�
    this->m_pHeadSentinel = new NODE;
    this->m_pTailSentinel = new NODE;

    this->m_pHeadSentinel->m_pNext = this->m_pTailSentinel;
    this->m_pTailSentinel->m_pPre = this->m_pHeadSentinel;
    this->m_nLen = 0;
}