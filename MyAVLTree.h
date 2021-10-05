#pragma once

#include <iostream>
#include <queue>
#include <stack>

template<typename T_key, typename T_val>
class CBST
{
private:
    typedef struct tagNode
    {
        tagNode(T_key key, T_val val, tagNode *pParent = nullptr, tagNode *pLeft = nullptr, tagNode *pRight = nullptr);

        T_key m_key;                                            //�洢�ıȽ�ֵ
        T_val m_val;                                            //�洢����ֵ
        int m_nHeight;                                          //�ڵ�߶�
        tagNode *m_pParent;                                     //���ڵ�
        tagNode *m_pLeft;                                       //���ӽڵ�
        tagNode *m_pRight;                                      //���ӽڵ�
    }NODE, *PNODE;

public:
    CBST();
    CBST(const CBST &cbst);
    ~CBST();

    CBST &operator=(const CBST &cbst);

    bool Insert(T_key key, T_val val);                          //����
    bool Delete(T_key key);                                     //ɾ��
    bool Update(T_key OrgKey, T_val DstVal);                    //�޸�
    bool Find(T_key key, T_val &val);                           //��ѯ
    void Clear();                                               //���

    //void LMR(PNODE pNode);                                    //�������  
    std::queue<T_val> LMR_LOOP();                               //�������
    //void MLR(PNODE pNode);                                    //ǰ�����
    //void MLR_LOOP();                                          //ǰ�����
    //void LRM(PNODE pNode);                                    //�������
    //void LRM_LOOP();                                          //�������

    //void Level();                                             //�������
private:
    PNODE m_pRoot;                                              //�ڵ�

    PNODE FindNode(T_key key);                                  //��ѯ�ڵ�
    void DelLeaf(PNODE pLeaf);                                  //ɾ��Ҷ�ӽڵ�
    void DelSingle(PNODE pSingle);                              //ɾ������֧�ڵ�
    void DelDoubleBranch(PNODE pDblBranch);                     //ɾ��˫��֧�ڵ�

    bool DeleteLeaf(PNODE pNode);
    bool DeleteBranch(PNODE pNode);

    void AdjustHeight(PNODE pNode);                             //�����߶�
    int CalcHeight(PNODE pNode);                                //��ȡ���ҽڵ�����߶�ֵ
    int GetHeight(PNODE pNode);                                 //��ȡ�߶�

    void RotateLeft(PNODE pNode);                               //����
    void RotateRight(PNODE pNode);                              //����
};

template<typename T_key, typename T_val>
inline CBST<T_key, T_val>::tagNode::tagNode(T_key key, T_val val, tagNode *pParent, tagNode *pLeft, tagNode *pRight)
    :m_key(key), m_val(val), m_pParent(pParent), m_pLeft(pLeft), m_pRight(pRight)
{
}

template<typename T_key, typename T_val>
inline CBST<T_key, T_val>::CBST()
{
    this->m_pRoot = nullptr;
}

template<typename T_key, typename T_val>
inline CBST<T_key, T_val>::CBST(const CBST &cbst)
{
    if (cbst.m_pRoot != nullptr)
    {
        *this = cbst;
    }
}

template<typename T_key, typename T_val>
inline CBST<T_key, T_val>::~CBST()
{
    Clear();

    if (this->m_pRoot != nullptr)
    {
        delete this->m_pRoot;
    }
    this->m_pRoot = nullptr;
}

template<typename T_key, typename T_val>
inline CBST<T_key, T_val> &CBST<T_key, T_val>::operator=(const CBST &cbst)
{
    if (this == &cbst)
    {
        return *this;
    }

    Clear();

    if (cbst.m_pRoot == nullptr)
    {
        return *this;
    }

    std::queue<PNODE> queue;

    queue.push(cbst.m_pRoot);
    while (!queue.empty())
    {
        // ��ȡ��ǰ���node
        PNODE pCurNode = queue.front();
        queue.pop();

        Insert(pCurNode->m_key, pCurNode->m_val);

        if (pCurNode->m_pLeft != nullptr)
        {
            queue.push(pCurNode->m_pLeft);
        }
        if (pCurNode->m_pRight != nullptr)
        {
            queue.push(pCurNode->m_pRight);
        }
    }

    return *this;
}

template<typename T_key, typename T_val>
inline bool CBST<T_key, T_val>::Insert(T_key key, T_val val)
{
    //�սڵ�
    if (this->m_pRoot == nullptr)
    {
        this->m_pRoot = new NODE(key, val);
        if (this->m_pRoot != nullptr)
        {
            return true;
        }
    }

    //�жϲ���λ��
    PNODE pNodeParent = nullptr;                        //��¼���ڵ�
    PNODE *pNodeChild = &(this->m_pRoot);               //��¼�ӽڵ�ĵ�ַ
    bool isLeft = true;
    while (*pNodeChild != nullptr)
    {
        pNodeParent = *pNodeChild;

        if (key < (*pNodeChild)->m_key)
        {
            //���ӽڵ�
            pNodeChild = &((*pNodeChild)->m_pLeft);
            isLeft = true;
        }
        else
        {
            //���ӽڵ�
            pNodeChild = &((*pNodeChild)->m_pRight);
            isLeft = false;
        }
    }
    //��������
    *pNodeChild = new NODE(key, val, pNodeParent);
    if (*pNodeChild != nullptr)
    {
        //���¸��ڵ��е��ӽڵ�ָ��
        if (isLeft)
        {
            pNodeParent->m_pLeft = *pNodeChild;
        }
        else
        {
            pNodeParent->m_pRight = *pNodeChild;
        }

        //�����߶�
        AdjustHeight(*pNodeChild);

        return true;
    }

    return false;
}

//template<typename T_key, typename T_val>
//inline bool CBST<T_key, T_val>::Delete(T_key key)
//{
//    //��ѯҪɾ����ֵ
//    PNODE pNode = FindNode(key);
//    if (pNode == nullptr)
//    {
//        return false;
//    }
//
//    //Ҷ�ӽڵ�
//    if (pNode->m_pLeft == nullptr && pNode->m_pRight == nullptr)
//    {
//        DelLeaf(pNode);
//        return true;
//    }
//
//    //����֧
//    if (pNode->m_pLeft == nullptr || pNode->m_pRight == nullptr)
//    {
//        DelSingle(pNode);
//        return true;
//    }
//
//    //˫��֧
//    DelDoubleBranch(pNode);
//    return true;
//}

template<typename T_key, typename T_val>
inline bool CBST<T_key, T_val>::Delete(T_key key)
{
    // �ҵ���ǰ�ڵ�
    auto pNode = FindNode(key);
    if (!pNode)
    {
        return false;
    }

    // Ҷ�ڵ�
    if (pNode->m_pLeft == nullptr && pNode->m_pRight == nullptr)
    {
        return DeleteLeaf(pNode);
    }
    // ��֧
    if (pNode->m_pLeft != nullptr || pNode->m_pRight != nullptr)
    {
        return DeleteBranch(pNode);
    }

    return false;
}

template<typename T_key, typename T_val>
inline bool CBST<T_key, T_val>::Update(T_key OrgKey, T_val nDstVal)
{
    PNODE pNode = FindNode(OrgKey);
    if (pNode == nullptr)
    {
        return false;
    }
    pNode->m_val = nDstVal;
    return true;
}

//template<typename T_key, typename T_val>
//inline bool CBST<T_key, T_val>::Delete(T_key key)
//{
//    //��ѯҪɾ����ֵ
//    PNODE pNode = FindNode(key);
//    if (pNode == nullptr)
//    {
//        return false;
//    }
//
//    //Ҷ�ӽڵ�
//    if (pNode->m_pLeft == nullptr && pNode->m_pRight == nullptr)
//    {
//        DelLeaf(pNode);
//        return true;
//    }
//
//    //����֧
//    if (pNode->m_pLeft == nullptr || pNode->m_pRight == nullptr)
//    {
//        DelSingle(pNode);
//        return true;
//    }
//
//    //˫��֧
//    DelDoubleBranch(pNode);
//    return true;
//}

//template<typename T_key, typename T_val>
//inline bool CBST<T_key, T_val>::Update(T_key OrgKey, T_val nDstVal)
//{
//    PNODE pNode = FindNode(OrgKey);
//    if (pNode == nullptr)
//    {
//        return false;
//    }
//    pNode->m_val = nDstVal;
//    return true;
//}

template<typename T_key, typename T_val>
inline bool CBST<T_key, T_val>::Find(T_key key, T_val &val)
{
    /*CStack<PNODE> stk;

    auto pNode = this->m_pRoot;
    while (true)
    {
        while (pNode != nullptr)
        {
            stk.Push(pNode);
            pNode = pNode->m_pLeft;
        }

        if (stk.IsEmpty())
        {
            break;
        }

        pNode = stk.Top();

        if (pNode->m_key == key)
        {
            val = pNode->m_val;
            return true;
        }

        stk.Pop();

        pNode = pNode->m_pRight;
    }*/

    PNODE pNode = FindNode(key);
    if (pNode == nullptr)
    {
        return false;
    }
    val = pNode->m_val;
    return true;
}

template<typename T_key, typename T_val>
inline void CBST<T_key, T_val>::Clear()
{
    while (this->m_pRoot != nullptr)
    {
        Delete(this->m_pRoot->m_key);
    }
}

/*
template<typename T_key, typename T_val>
inline void CBST<T_key, T_val>::LMR(PNODE pNode)
{
    if (pNode != nullptr)
    {
        LMR(pNode->m_pLeft);
        std::cout << pNode->m_val << "  ";
        LMR(pNode->m_pRight);
    }
}*/

template<typename T_key, typename T_val>
inline std::queue<T_val> CBST<T_key, T_val>::LMR_LOOP()
{
    std::stack<PNODE> stk;
    std::queue<T_val> qu;

    auto pNode = this->m_pRoot;
    while (true)
    {
        while (pNode != nullptr)
        {
            stk.push(pNode);
            pNode = pNode->m_pLeft;
        }

        if (stk.empty())
        {
            break;
        }

        pNode = stk.top();

        qu.push(pNode->m_val);

        stk.pop();

        pNode = pNode->m_pRight;
    }

    return qu;
}

template<typename T_key, typename T_val>
typename inline CBST<T_key, T_val>::PNODE CBST<T_key, T_val>::FindNode(T_key key)
{
    //�սڵ�
    if (this->m_pRoot == nullptr)
    {
        return nullptr;
    }

    PNODE pNode = this->m_pRoot;
    while (pNode != nullptr)
    {
        if (key == pNode->m_key)
        {
            return pNode;
        }
        else if (key < pNode->m_key)
        {
            pNode = pNode->m_pLeft;
        }
        else
        {
            pNode = pNode->m_pRight;
        }
    }

    return nullptr;
}

template<typename T_key, typename T_val>
inline void CBST<T_key, T_val>::DelLeaf(PNODE pLeaf)
{
    //���ڵ�
    PNODE pNodeParent = pLeaf->m_pParent;

    //���¸��ڵ�
    if (pNodeParent != nullptr)
    {
        if (pLeaf->m_key < pNodeParent->m_key)
        {
            //���ӽڵ�
            pNodeParent->m_pLeft = nullptr;
        }
        else
        {
            //���ӽڵ�
            pNodeParent->m_pRight = nullptr;
        }
    }

    delete pLeaf;

    if (pNodeParent == nullptr)
    {
        this->m_pRoot = nullptr;
    }
    else
    {
        //�����߶�
        AdjustHeight(pNodeParent);
    }
}

template<typename T_key, typename T_val>
inline void CBST<T_key, T_val>::DelSingle(PNODE pSingle)
{
    //���ڵ�
    PNODE pNodeParent = pSingle->m_pParent;
    //�ӽڵ�
    PNODE pNodeChild = (pSingle->m_pLeft != nullptr) ? pSingle->m_pLeft : pSingle->m_pRight;

    //�����ӽڵ��ָ��
    if (pNodeParent != nullptr)
    {
        pNodeChild->m_pParent = pNodeParent;
    }

    //���¸��ڵ��ָ��
    if (pNodeParent != nullptr)
    {
        if (pSingle->m_key < pNodeParent->m_key)
        {
            //���ӽڵ�
            pNodeParent->m_pLeft = pNodeChild;
        }
        else
        {
            //���ӽڵ�
            pNodeParent->m_pRight = pNodeChild;
        }

    }
    delete pSingle;

    if (pNodeParent == nullptr)
    {
        this->m_pRoot = pNodeChild;
        this->m_pRoot->m_pParent = nullptr;
    }
    else
    {
        AdjustHeight(pNodeParent);
    }
}

template<typename T_key, typename T_val>
inline void CBST<T_key, T_val>::DelDoubleBranch(PNODE pDblBranch)
{
    //���ڵ�
    PNODE pNodeParent = pDblBranch->m_pParent;
    //��ȡ�ýڵ�����ӽڵ�
    PNODE pNodeRight = pDblBranch->m_pRight;

    //�ҽڵ���û����ڵ�
    if (pNodeRight->m_pLeft == nullptr)
    {
        //���¸ýڵ�
        pDblBranch->m_key = pNodeRight->m_key;
        pDblBranch->m_val = pNodeRight->m_val;

        //ɾ���ҽڵ�
        if (pNodeRight->m_pLeft != nullptr || pNodeRight->m_pRight != nullptr)
        {
            DelSingle(pNodeRight);
        }
        else
        {
            DelLeaf(pNodeRight);
        }

        return;
    }

    //Ѱ�����ӽڵ�������ڵ��ֵ
    PNODE pNodeLeft = pNodeRight->m_pLeft;
    while (pNodeLeft->m_pLeft != nullptr)
    {
        pNodeLeft = pNodeLeft->m_pLeft;
    }
    //���¸ýڵ�
    pDblBranch->m_key = pNodeLeft->m_key;
    pDblBranch->m_val = pNodeLeft->m_val;

    //ɾ����ڵ�
    if (pNodeLeft->m_pRight != nullptr)
    {
        DelSingle(pNodeLeft);
    }
    else
    {
        DelLeaf(pNodeLeft);
    }
}

template<typename T_key, typename T_val>
inline bool CBST<T_key, T_val>::DeleteLeaf(PNODE pNode)
{
    auto pParent = pNode->m_pParent;

    if (pParent == nullptr)
    {
        m_pRoot = nullptr;
        delete pNode;
        return true;
    }

    if (pParent->m_key > pNode->m_key)
    {
        pParent->m_pLeft = nullptr;
    }
    else
    {
        pParent->m_pRight = nullptr;
    }
    delete pNode;

    if (pParent)
    {
        AdjustHeight(pParent);
    }

    return true;
}

template<typename T_key, typename T_val>
inline bool CBST<T_key, T_val>::DeleteBranch(PNODE pNode)
{
    if (pNode == nullptr)
    {
        return false;
    }

    bool isLeft = false;
    auto pTmpNode = pNode;
    auto pParent = pNode->m_pParent;

    if (pTmpNode->m_pRight == nullptr)
    {
        isLeft = true;
        pTmpNode = pTmpNode->m_pLeft;

        while (true)
        {
            // ����֧����������֧�������ڵĻ� �ͷ��ڲ�ԭ��
            if (pTmpNode == nullptr)
            {
                return false;
            }
            if (pTmpNode->m_pRight == nullptr)
            {
                break;
            }
            pTmpNode = pTmpNode->m_pRight;
            isLeft = false;
        }
        if (pTmpNode->m_pLeft != nullptr)
        {
            if (isLeft)
            {
                pTmpNode->m_pParent->m_pLeft = pTmpNode->m_pLeft;
            }
            else
            {
                pTmpNode->m_pParent->m_pRight = pTmpNode->m_pLeft;
            }
            pTmpNode->m_pLeft->m_pParent = pTmpNode->m_pParent;
        }
        else
        {
            if (isLeft)
            {
                pTmpNode->m_pParent->m_pLeft = nullptr;
            }
            else
            {
                pTmpNode->m_pParent->m_pRight = nullptr;
            }
        }
    }
    else
    {
        pTmpNode = pTmpNode->m_pRight;
        while (true)
        {
            if (pTmpNode->m_pLeft == nullptr)
            {
                break;
            }
            pTmpNode = pTmpNode->m_pLeft;
            isLeft = true;
        }

        if (pTmpNode->m_pRight != nullptr)
        {
            if (isLeft)
            {
                pTmpNode->m_pParent->m_pLeft = pTmpNode->m_pRight;
            }
            else
            {
                pTmpNode->m_pParent->m_pRight = pTmpNode->m_pRight;
            }
            pTmpNode->m_pRight->m_pParent = pTmpNode->m_pParent;
        }
        else
        {
            if (isLeft)
            {
                pTmpNode->m_pParent->m_pLeft = nullptr;
            }
            else
            {
                pTmpNode->m_pParent->m_pRight = nullptr;
            }
        }
    }

    pNode->m_key = pTmpNode->m_key;
    pNode->m_val = pTmpNode->m_val;
    delete pTmpNode;

    if (pParent)
    {
        AdjustHeight(pParent);
    }

    return true;
}

template<typename T_key, typename T_val>
inline void CBST<T_key, T_val>::AdjustHeight(PNODE pNode)
{
    //���Ÿ��ڵ㵽���ڵ㣬�������еĽڵ�߶�
    while (pNode != nullptr)
    {
        //�޸Ľڵ�ĸ߶�ֵ
        pNode->m_nHeight = CalcHeight(pNode);
        int nLeftHeight = GetHeight(pNode->m_pLeft);
        int nRightHeight = GetHeight(pNode->m_pRight);

        //�ж����Ƿ�ƽ��
        if (nLeftHeight - nRightHeight > 1)
        {
            //�ж���һ�ε������������ε���
            if (GetHeight(pNode->m_pLeft->m_pLeft) >= GetHeight(pNode->m_pLeft->m_pRight))
            {
                //����
                RotateRight(pNode);
            }
            else
            {
                //������ת��������
                auto pTemp = pNode;
                auto pLeft = pNode->m_pLeft;
                RotateLeft(pLeft);
                RotateRight(pTemp);
            }
        }
        else if (nRightHeight - nLeftHeight > 1)
        {
            if (GetHeight(pNode->m_pRight->m_pRight) >= GetHeight(pNode->m_pRight->m_pLeft))
            {
                //����
                RotateLeft(pNode);
            }
            else
            {
                //������ת
                auto pTemp = pNode;
                auto pRight = pNode->m_pRight;
                RotateRight(pTemp);
                RotateLeft(pRight);
            }
        }

        pNode = pNode->m_pParent;
    }
}

template<typename T_key, typename T_val>
inline int CBST<T_key, T_val>::CalcHeight(PNODE pNode)
{
    int nLeft = GetHeight(pNode->m_pLeft);
    int nRight = GetHeight(pNode->m_pRight);

    return nLeft > nRight ? nLeft + 1 : nRight + 1;
}

template<typename T_key, typename T_val>
inline int CBST<T_key, T_val>::GetHeight(PNODE pNode)
{
    return pNode == nullptr ? 0 : pNode->m_nHeight;
}

template<typename T_key, typename T_val>
inline void CBST<T_key, T_val>::RotateLeft(PNODE pNode)
{
    auto pNodeNow = pNode;
    auto pNodeParent = pNodeNow->m_pParent;
    auto pNodeRight = pNodeNow->m_pRight;
    auto pNodeLeft = pNodeRight->m_pLeft;

    if (pNodeParent == nullptr)
    {
        this->m_pRoot = pNodeRight;
    }
    else
    {
        if (pNodeParent->m_pLeft == pNodeNow)
        {
            pNodeParent->m_pLeft = pNodeRight;
        }
        else
        {
            pNodeParent->m_pRight = pNodeRight;
        }
    }

    pNodeNow->m_pParent = pNodeRight;
    pNodeNow->m_pRight = pNodeLeft;

    pNodeRight->m_pParent = pNodeParent;
    pNodeRight->m_pLeft = pNodeNow;

    if (pNodeLeft != nullptr)
    {
        pNodeLeft->m_pParent = pNodeNow;
    }

    //�����߶�
    pNodeNow->m_nHeight = CalcHeight(pNodeNow);
    pNodeRight->m_nHeight = CalcHeight(pNodeRight);
}

template<typename T_key, typename T_val>
inline void CBST<T_key, T_val>::RotateRight(PNODE pNode)
{
    auto pNodeNow = pNode;
    auto pNodeParent = pNodeNow->m_pParent;
    auto pNodeLeft = pNodeNow->m_pLeft;
    auto pNodeRight = pNodeLeft->m_pRight;

    if (pNodeParent == nullptr)
    {
        this->m_pRoot = pNodeLeft;
    }
    else
    {
        if (pNodeNow == pNodeParent->m_pLeft)
        {
            pNodeParent->m_pLeft = pNodeLeft;
        }
        else
        {
            pNodeParent->m_pRight = pNodeLeft;
        }
    }

    pNodeNow->m_pParent = pNodeLeft;
    pNodeNow->m_pLeft = pNodeRight;

    pNodeLeft->m_pParent = pNodeParent;
    pNodeLeft->m_pRight = pNodeNow;

    if (pNodeRight != nullptr)
    {
        pNodeRight->m_pParent = pNodeNow;
    }

    //�����߶�
    pNodeNow->m_nHeight = CalcHeight(pNodeNow);
    pNodeLeft->m_nHeight = CalcHeight(pNodeLeft);
}