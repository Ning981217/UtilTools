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

        T_key m_key;                                            //存储的比较值
        T_val m_val;                                            //存储数据值
        int m_nHeight;                                          //节点高度
        tagNode *m_pParent;                                     //父节点
        tagNode *m_pLeft;                                       //左子节点
        tagNode *m_pRight;                                      //右子节点
    }NODE, *PNODE;

public:
    CBST();
    CBST(const CBST &cbst);
    ~CBST();

    CBST &operator=(const CBST &cbst);

    bool Insert(T_key key, T_val val);                          //插入
    bool Delete(T_key key);                                     //删除
    bool Update(T_key OrgKey, T_val DstVal);                    //修改
    bool Find(T_key key, T_val &val);                           //查询
    void Clear();                                               //清空

    //void LMR(PNODE pNode);                                    //中序遍历  
    std::queue<T_val> LMR_LOOP();                               //中序遍历
    //void MLR(PNODE pNode);                                    //前序遍历
    //void MLR_LOOP();                                          //前序遍历
    //void LRM(PNODE pNode);                                    //后序遍历
    //void LRM_LOOP();                                          //后序遍历

    //void Level();                                             //层序遍历
private:
    PNODE m_pRoot;                                              //节点

    PNODE FindNode(T_key key);                                  //查询节点
    void DelLeaf(PNODE pLeaf);                                  //删除叶子节点
    void DelSingle(PNODE pSingle);                              //删除单分支节点
    void DelDoubleBranch(PNODE pDblBranch);                     //删除双分支节点

    bool DeleteLeaf(PNODE pNode);
    bool DeleteBranch(PNODE pNode);

    void AdjustHeight(PNODE pNode);                             //调整高度
    int CalcHeight(PNODE pNode);                                //获取左右节点的最大高度值
    int GetHeight(PNODE pNode);                                 //获取高度

    void RotateLeft(PNODE pNode);                               //左旋
    void RotateRight(PNODE pNode);                              //右旋
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
        // 获取最前面的node
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
    //空节点
    if (this->m_pRoot == nullptr)
    {
        this->m_pRoot = new NODE(key, val);
        if (this->m_pRoot != nullptr)
        {
            return true;
        }
    }

    //判断插入位置
    PNODE pNodeParent = nullptr;                        //记录父节点
    PNODE *pNodeChild = &(this->m_pRoot);               //记录子节点的地址
    bool isLeft = true;
    while (*pNodeChild != nullptr)
    {
        pNodeParent = *pNodeChild;

        if (key < (*pNodeChild)->m_key)
        {
            //左子节点
            pNodeChild = &((*pNodeChild)->m_pLeft);
            isLeft = true;
        }
        else
        {
            //右子节点
            pNodeChild = &((*pNodeChild)->m_pRight);
            isLeft = false;
        }
    }
    //插入数据
    *pNodeChild = new NODE(key, val, pNodeParent);
    if (*pNodeChild != nullptr)
    {
        //更新父节点中的子节点指向
        if (isLeft)
        {
            pNodeParent->m_pLeft = *pNodeChild;
        }
        else
        {
            pNodeParent->m_pRight = *pNodeChild;
        }

        //调整高度
        AdjustHeight(*pNodeChild);

        return true;
    }

    return false;
}

//template<typename T_key, typename T_val>
//inline bool CBST<T_key, T_val>::Delete(T_key key)
//{
//    //查询要删除的值
//    PNODE pNode = FindNode(key);
//    if (pNode == nullptr)
//    {
//        return false;
//    }
//
//    //叶子节点
//    if (pNode->m_pLeft == nullptr && pNode->m_pRight == nullptr)
//    {
//        DelLeaf(pNode);
//        return true;
//    }
//
//    //单分支
//    if (pNode->m_pLeft == nullptr || pNode->m_pRight == nullptr)
//    {
//        DelSingle(pNode);
//        return true;
//    }
//
//    //双分支
//    DelDoubleBranch(pNode);
//    return true;
//}

template<typename T_key, typename T_val>
inline bool CBST<T_key, T_val>::Delete(T_key key)
{
    // 找到当前节点
    auto pNode = FindNode(key);
    if (!pNode)
    {
        return false;
    }

    // 叶节点
    if (pNode->m_pLeft == nullptr && pNode->m_pRight == nullptr)
    {
        return DeleteLeaf(pNode);
    }
    // 分支
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
//    //查询要删除的值
//    PNODE pNode = FindNode(key);
//    if (pNode == nullptr)
//    {
//        return false;
//    }
//
//    //叶子节点
//    if (pNode->m_pLeft == nullptr && pNode->m_pRight == nullptr)
//    {
//        DelLeaf(pNode);
//        return true;
//    }
//
//    //单分支
//    if (pNode->m_pLeft == nullptr || pNode->m_pRight == nullptr)
//    {
//        DelSingle(pNode);
//        return true;
//    }
//
//    //双分支
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
    //空节点
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
    //父节点
    PNODE pNodeParent = pLeaf->m_pParent;

    //更新父节点
    if (pNodeParent != nullptr)
    {
        if (pLeaf->m_key < pNodeParent->m_key)
        {
            //左子节点
            pNodeParent->m_pLeft = nullptr;
        }
        else
        {
            //右子节点
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
        //调整高度
        AdjustHeight(pNodeParent);
    }
}

template<typename T_key, typename T_val>
inline void CBST<T_key, T_val>::DelSingle(PNODE pSingle)
{
    //父节点
    PNODE pNodeParent = pSingle->m_pParent;
    //子节点
    PNODE pNodeChild = (pSingle->m_pLeft != nullptr) ? pSingle->m_pLeft : pSingle->m_pRight;

    //更新子节点的指向
    if (pNodeParent != nullptr)
    {
        pNodeChild->m_pParent = pNodeParent;
    }

    //更新父节点的指向
    if (pNodeParent != nullptr)
    {
        if (pSingle->m_key < pNodeParent->m_key)
        {
            //左子节点
            pNodeParent->m_pLeft = pNodeChild;
        }
        else
        {
            //右子节点
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
    //父节点
    PNODE pNodeParent = pDblBranch->m_pParent;
    //获取该节点的右子节点
    PNODE pNodeRight = pDblBranch->m_pRight;

    //右节点中没有左节点
    if (pNodeRight->m_pLeft == nullptr)
    {
        //更新该节点
        pDblBranch->m_key = pNodeRight->m_key;
        pDblBranch->m_val = pNodeRight->m_val;

        //删除右节点
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

    //寻找右子节点中最左节点的值
    PNODE pNodeLeft = pNodeRight->m_pLeft;
    while (pNodeLeft->m_pLeft != nullptr)
    {
        pNodeLeft = pNodeLeft->m_pLeft;
    }
    //更新该节点
    pDblBranch->m_key = pNodeLeft->m_key;
    pDblBranch->m_val = pNodeLeft->m_val;

    //删除左节点
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
            // 单分支出现两个分支都不存在的话 就非内部原因
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
    //沿着父节点到根节点，调整所有的节点高度
    while (pNode != nullptr)
    {
        //修改节点的高度值
        pNode->m_nHeight = CalcHeight(pNode);
        int nLeftHeight = GetHeight(pNode->m_pLeft);
        int nRightHeight = GetHeight(pNode->m_pRight);

        //判断树是否平衡
        if (nLeftHeight - nRightHeight > 1)
        {
            //判断是一次单旋，还是两次单旋
            if (GetHeight(pNode->m_pLeft->m_pLeft) >= GetHeight(pNode->m_pLeft->m_pRight))
            {
                //右旋
                RotateRight(pNode);
            }
            else
            {
                //两次旋转，左右旋
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
                //左旋
                RotateLeft(pNode);
            }
            else
            {
                //两次旋转
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

    //调整高度
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

    //调整高度
    pNodeNow->m_nHeight = CalcHeight(pNodeNow);
    pNodeLeft->m_nHeight = CalcHeight(pNodeLeft);
}