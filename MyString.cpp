#include <string>
#include <cassert>
#include <stdarg.h>

#include "MyString.h"

#pragma warning(disable: 4996)

#pragma region 构造函数
CMyString::CMyString()
{
    Init();
}

CMyString::CMyString(const char *sz)
{
    Init();
    SetStr(sz);
}

CMyString::CMyString(const CMyString &obj)
{
    DeepCopy(obj);
}

CMyString::~CMyString()
{
    if (this->m_pBuff != nullptr && this->m_pCount != nullptr)
    {
        --(*(this->m_pCount));
        if (*(this->m_pCount) <= 0)
        {
            delete[] this->m_pBuff;
            delete this->m_pCount;
        }
    }
    Init();
}
#pragma endregion

#pragma region GetLen()
size_t CMyString::GetLen() const
{
    return this->m_nLen;
}
size_t CMyString::GetBuffSize() const
{
    return this->m_nBuffSize;
}
#pragma endregion

#pragma region GetStr()
const char *CMyString::GetStr() const
{
    return this->m_pBuff;
}
#pragma endregion

#pragma region Append
CMyString &CMyString::Append(const char *sz)
{

    if (sz == nullptr)
    {
        return *this;
    }

    SetCount();

    if (strlen(sz) + this->m_nLen < this->m_nBuffSize)
    {
        strcat(this->m_pBuff, sz);
        this->m_nLen += strlen(sz);
        return *this;
    }
    else
    {
        char *pTemp = new char[this->m_nLen + 1];
        if (pTemp == nullptr)
        {
            return *this;
        }

        strcpy(pTemp, this->m_pBuff);
        delete[] this->m_pBuff;
        this->m_pBuff = nullptr;
        this->m_pBuff = new char[this->m_nLen + strlen(sz) + 1];
        if (this->m_pBuff == nullptr)
        {
            Init();
            return *this;
        }

        memset(this->m_pBuff, '\0', this->m_nLen + strlen(sz) + 1);
        strcpy(this->m_pBuff, pTemp);
        strcat(this->m_pBuff, sz);
        this->m_nLen = this->m_nLen + strlen(sz);
        this->m_nBuffSize = this->m_nLen + 1;

        delete[] pTemp;
        pTemp = nullptr;

        return *this;
    }
}

CMyString &CMyString::Append(const CMyString &sz)
{
    return Append(sz.m_pBuff);
}

CMyString &CMyString::Append(char ch)
{
    char szTemp[2] = { ch , '\0' };
    return Append(szTemp);
}

CMyString &CMyString::Append(int n)
{
    char szTemp[11] = { '\0' };
    sprintf(szTemp, "%d", n);
    return Append(szTemp);
}

CMyString &CMyString::Append(float f)
{
    char szTemp[128] = { '\0' };
    sprintf(szTemp, "%0.6f", f);
    return Append(szTemp);
}

CMyString &CMyString::Append(double dbl)
{
    char szTemp[0x1000] = { '\0' };
    sprintf(szTemp, "%0.6lf", dbl);
    return Append(szTemp);
}
#pragma endregion

#pragma region Find
int CMyString::Find(const char *sz) const
{
    if (sz == nullptr)
    {
        return -1;
    }

    if (this->m_pBuff == nullptr)
    {
        return -1;
    }

    char *pTemp = nullptr;

    pTemp = strstr(this->m_pBuff, sz);

    if (pTemp != NULL)
    {
        return (int)pTemp - (int)(this->m_pBuff);
    }

    return -1;
}

int CMyString::Find(const CMyString &obj) const
{
    return Find(obj.m_pBuff);
}

int CMyString::ReverseFind(const char *sz) const
{
    if (sz == nullptr)
    {
        return -1;
    }

    if (this->m_pBuff == nullptr)
    {
        return -1;
    }

    int szLen = strlen(sz);
    char *pTemp = nullptr;
    pTemp = this->m_pBuff;
    pTemp = pTemp + this->m_nLen - szLen;

    while (pTemp != this->m_pBuff)
    {
        if (strstr(pTemp, sz))
        {
            return (int)pTemp - (int)(this->m_pBuff);
        }
        pTemp--;
    }

    return -1;
}

int CMyString::ReverseFind(const CMyString &obj) const
{
    return ReverseFind(obj.m_pBuff);
}
#pragma endregion

#pragma region Copy
CMyString &CMyString::Copy(const char *sz)
{

    if (sz == nullptr)
    {
        return *this;
    }


    if (this->m_pBuff == nullptr)
    {
        Init();
        //return *this;
    }

    SetCount();

    size_t szLen = strlen(sz);
    if (szLen >= this->m_nBuffSize)
    {
        delete[] this->m_pBuff;
        this->m_pBuff = nullptr;
        this->m_pBuff = new char[strlen(sz) + 1];
        if (m_pBuff == nullptr)
        {
            Init();
            return *this;
        }
        this->m_nBuffSize = strlen(sz) + 1;
    }

    memset(this->m_pBuff, '\0', strlen(sz) + 1);
    strcpy(this->m_pBuff, sz);
    this->m_nLen = strlen(sz);

    return *this;
}

CMyString &CMyString::Copy(const CMyString &obj)
{
    return Copy(obj.m_pBuff);
}
#pragma endregion

#pragma region MyRegion
int CMyString::Compare(const char *sz) const
{
    if (sz == nullptr)
    {
        //throw "空指针";
        assert(sz != nullptr);
    }

    if (this->m_pBuff == nullptr)
    {
        //throw "空指针";
        assert(this->m_pBuff != nullptr);
    }
    return strcmp(this->m_pBuff, sz);
}

int CMyString::Compare(const CMyString &obj) const
{
    return Compare(obj.m_pBuff);
}
#pragma endregion

#pragma region 字串提取
CMyString CMyString::Left(size_t nCount)
{
    if (this->m_pBuff == nullptr)
    {
        return nullptr;
    }

    char *pTemp = nullptr;
    pTemp = new char[nCount + 1];
    if (pTemp == nullptr)
    {
        return nullptr;
    }
    memset(pTemp, '\0', nCount + 1);

    if (nCount > this->m_nLen)
    {
        nCount = this->m_nLen;
    }

    memcpy(pTemp, this->m_pBuff, nCount);

    CMyString leftString(pTemp);

    delete[] pTemp;
    pTemp = nullptr;

    return leftString;
}

CMyString CMyString::Right(size_t nCount)
{
    if (this->m_pBuff == nullptr)
    {
        return nullptr;
    }

    char *pTemp = nullptr;
    pTemp = new char[nCount + 1];
    if (pTemp == nullptr)
    {
        return nullptr;
    }
    memset(pTemp, '\0', nCount + 1);

    if (nCount > this->m_nLen)
    {
        nCount = this->m_nLen;
    }

    char *pBuffTemp = nullptr;
    pBuffTemp = this->m_pBuff;
    pBuffTemp = pBuffTemp + this->m_nLen - nCount;
    memcpy(pTemp, pBuffTemp, nCount);

    CMyString rightString(pTemp);

    delete[] pTemp;
    pTemp = nullptr;

    return rightString;
}

CMyString CMyString::Mid(int nIdx, size_t nCount)
{
    if (nIdx > this->m_nLen)
    {
        return nullptr;
    }

    if (this->m_pBuff == nullptr)
    {
        return nullptr;
    }

    char *pTemp = nullptr;
    pTemp = new char[nCount + 1];
    if (pTemp == nullptr)
    {
        return nullptr;
    }
    memset(pTemp, '\0', nCount + 1);

    if (nCount > this->m_nLen)
    {
        nCount = this->m_nLen;
    }

    char *pBuffTemp = nullptr;
    pBuffTemp = this->m_pBuff;

    if (nIdx + nCount > this->m_nLen)
    {
        return nullptr;
    }

    pBuffTemp = pBuffTemp + nIdx;
    memcpy(pTemp, pBuffTemp, nCount);

    CMyString midString(pTemp);

    delete[] pTemp;

    return midString;
}
#pragma endregion

CMyString &CMyString::Delete(int nBeg, int nCount)
{
    //记录初始位置
    char *pInitIndex = this->m_pBuff;

    if (nCount > 0)
    {
        pInitIndex += nBeg;
    }
    else
    {
        pInitIndex += (nBeg + nCount);
        nCount = ~nCount + 1;
    }
    memset(pInitIndex, '\0', nCount);
    return *this;
}

CMyString CMyString::ValueOf(bool b)
{
    if (b)
    {
        return CopyValueOf("true", 0, strlen("true"));
    }
    else
    {
        return CopyValueOf("false", 0, strlen("false"));
    }
}

CMyString CMyString::ValueOf(char c)
{
    char szTemp[2] = { c ,'\0' };
    return CopyValueOf(szTemp, 0, strlen(szTemp));
}

CMyString CMyString::ValueOf(double d)
{
    char szTemp[0x1000] = { '\0' };
    sprintf(szTemp, "%lf", d);
    return CopyValueOf(szTemp, 0, strlen(szTemp));
}
CMyString CMyString::ValueOf(float f)
{
    char szTemp[128] = { '\0' };
    sprintf(szTemp, "%f", f);
    return CopyValueOf(szTemp, 0, strlen(szTemp));
}
CMyString CMyString::ValueOf(int i)
{
    char szTemp[11] = { '\0' };
    sprintf(szTemp, "%d", i);
    return CopyValueOf(szTemp, 0, strlen(szTemp));
}
CMyString CMyString::ValueOf(long i)
{
    char szTemp[11] = { '\0' };
    sprintf(szTemp, "%ld", i);
    return CopyValueOf(szTemp, 0, strlen(szTemp));
}
CMyString CMyString::ValueOf(short i)
{
    char szTemp[6] = { '\0' };
    sprintf(szTemp, "%d", i);
    return CopyValueOf(szTemp, 0, strlen(szTemp));
}
CMyString CMyString::ValueOf(const char *data)
{
    return CopyValueOf(data, 0, strlen(data));
}
CMyString CMyString::CopyValueOf(const char *data, int offset, int count)
{
    if (data == nullptr)
    {
        return nullptr;
    }

    if (m_pStr != nullptr)
    {
        delete[] m_pStr;
    }
    m_pStr = new char[count + 1];

    char *pTemp = nullptr;
    pTemp = (char *)data;
    pTemp += offset;
    memset(m_pStr, '\0', count + 1);
    memcpy(m_pStr, pTemp, count);
    //delete[] pTemp;

    return CMyString(m_pStr);
}
CMyString CMyString::StringFromFormat(const char *format, ...)
{
    int temp = 0x78563412;
    char szTemp[16000 + 1] = { '\0' };

    va_list args;
    va_start(args, format);
    vsnprintf(szTemp, 16000 + 1, format, args);
    va_end(args);

    return CopyValueOf(szTemp, 0, strlen(szTemp));
}

CMyString &CMyString::operator=(const CMyString &obj)
{
    DeepCopy(obj);
    return *this;
}

CMyString CMyString::operator+(const CMyString &obj)
{
    return Append(obj);
}

char &CMyString::operator[](int nIndex)
{
    if (nIndex >= this->m_nLen)
    {
        throw "下标越界";
    }
    return m_pBuff[nIndex];
}

bool CMyString::operator==(const CMyString &obj)
{
    return Compare(obj) == 0 ? true : false;
}

bool CMyString::operator!=(const CMyString &obj)
{
    return !(*this == obj);
}

bool CMyString::operator>(const CMyString &obj)
{
    return Compare(obj) > 0 ? true : false;;
}


#pragma region Init
void CMyString::Init()
{
    this->m_pBuff = nullptr;
    this->m_nLen = 0;
    this->m_nBuffSize = 0;
    this->m_pCount = nullptr;
}
#pragma endregion

#pragma region SetStr
void CMyString::SetStr(const char *sz)
{
    if (sz == nullptr)
    {
        Init();
        return;
    }

    size_t nLen = strlen(sz);
    this->m_pBuff = new char[nLen + 1];
    if (this->m_pBuff == nullptr)
    {
        Init();
        throw "申请内存失败";
        return;
    }
    memset(this->m_pBuff, '\0', nLen + 1);
    strcpy(this->m_pBuff, sz);
    this->m_nLen = nLen;
    this->m_nBuffSize = nLen + 1;

    this->m_pCount = new size_t;
    *(this->m_pCount) = 1;
}
#pragma endregion

void CMyString::SetCount()
{
    //拷贝数据
    char *pTemp = new char[this->m_nLen + 1];
    memset(pTemp, '\0', this->m_nLen);

    if (this->m_pBuff != nullptr)
    {
        strcpy(pTemp, this->m_pBuff);
    }


    //释放
    if (this->m_pCount != nullptr)
    {
        if (*(this->m_pCount) >= 1)
        {
            //计数减少
            --(*(this->m_pCount));
        }
        if (*(this->m_pCount) == 0)
        {
            delete[] this->m_pBuff;
            delete this->m_pCount;
            Init();
        }
    }

    //新数据
    SetStr(pTemp);

    delete[] pTemp;

}

void CMyString::DeepCopy(const CMyString &obj)
{
    if (obj.m_pBuff == nullptr)
    {
        Init();
    }
    else
    {
        this->m_pBuff = obj.m_pBuff;
        this->m_nLen = obj.m_nLen;
        this->m_nBuffSize = obj.m_nBuffSize;
        this->m_pCount = obj.m_pCount;

        ++(*(this->m_pCount));
    }
}

char *CMyString::m_pStr;