#pragma once
class CMyString
{
public:
    //��ʼ��
    CMyString();
    CMyString(const char *sz);
    CMyString(const CMyString &obj);

    //����ʼ��
    ~CMyString();

    //����
    size_t GetLen() const;
    size_t GetBuffSize() const;
    const char *GetStr() const;

    //ƴ��\׷��
    CMyString &Append(const char *sz);
    CMyString &Append(const CMyString &sz);
    CMyString &Append(char ch);
    CMyString &Append(int n);
    CMyString &Append(float f);
    CMyString &Append(double dbl);

    //��ѯ
    int Find(const char *sz) const; //-1 �Ҳ���
    int Find(const CMyString &obj) const;
    int ReverseFind(const char *sz) const;
    int ReverseFind(const CMyString &obj) const;

    //����
    CMyString &Copy(const char *sz);
    CMyString &Copy(const CMyString &obj);

    //�Ƚ�
    int Compare(const char *sz) const;
    int Compare(const CMyString &obj) const;

    //�ִ���ȡ
    CMyString Left(size_t nCount); //���ַ���������nCount���ַ�
    CMyString Right(size_t nCount);
    CMyString Mid(int nIdx, size_t nCount);//������nIdx���nCount���ַ�

    //�ַ���ɾ��(nBeg:��ʼ�±�   \   nCount:ɾ��������Ϊ����ʱ�����ɾ��)
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

    //���������
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
    char *m_pBuff;      //�洢�ַ����Ļ�����
    size_t m_nLen;      //�ַ����ĳ���
    size_t m_nBuffSize; //�������Ĵ�С

    size_t *m_pCount;    //���ü���

    static char *m_pStr;
};

