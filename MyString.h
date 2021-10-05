#pragma once
class CMyString
{
public:
    //初始化
    CMyString();
    CMyString(const char *sz);
    CMyString(const CMyString &obj);

    //反初始化
    ~CMyString();

    //属性
    size_t GetLen() const;
    size_t GetBuffSize() const;
    const char *GetStr() const;

    //拼接\追加
    CMyString &Append(const char *sz);
    CMyString &Append(const CMyString &sz);
    CMyString &Append(char ch);
    CMyString &Append(int n);
    CMyString &Append(float f);
    CMyString &Append(double dbl);

    //查询
    int Find(const char *sz) const; //-1 找不到
    int Find(const CMyString &obj) const;
    int ReverseFind(const char *sz) const;
    int ReverseFind(const CMyString &obj) const;

    //拷贝
    CMyString &Copy(const char *sz);
    CMyString &Copy(const CMyString &obj);

    //比较
    int Compare(const char *sz) const;
    int Compare(const CMyString &obj) const;

    //字串提取
    CMyString Left(size_t nCount); //从字符串左侧提出nCount个字符
    CMyString Right(size_t nCount);
    CMyString Mid(int nIdx, size_t nCount);//从索引nIdx提出nCount个字符

    //字符串删除(nBeg:起始下标   \   nCount:删除个数，为负数时向左边删除)
    CMyString &Delete(int nBeg, int nCount);

    static CMyString ValueOf(bool b);
    static CMyString ValueOf(char c);
    static CMyString ValueOf(double d);
    static CMyString ValueOf(float f);
    static CMyString ValueOf(int i);
    static CMyString ValueOf(long i);
    static CMyString ValueOf(short i);
    static CMyString ValueOf(const char *data);
    static CMyString CopyValueOf(const char *data, int offset, int count);
    static CMyString StringFromFormat(const char *format, ...);

    //运算符重载
    CMyString &operator=(const CMyString &obj);
    CMyString operator+(const CMyString &obj);
    char &operator[](int nIndex);
    bool operator==(const CMyString &obj);
    bool operator!=(const CMyString &obj);
    bool operator>(const CMyString &obj);

private:
    void Init();
    void SetStr(const char *sz);
    void SetCount();
    void DeepCopy(const CMyString &obj);

private:
    char *m_pBuff;      //存储字符串的缓冲区
    size_t m_nLen;      //字符串的长度
    size_t m_nBuffSize; //缓冲区的大小

    size_t *m_pCount;    //引用计数

    static char *m_pStr;
};

