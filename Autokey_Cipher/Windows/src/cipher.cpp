#include "Cipher.h"
#include <iostream>
#include <string>
#include <QString>
#include <QChar>

using namespace std;


// 构造函数
Cipher::Cipher(QString key, QString text)
{
    this->key = key;
    this->text = text;
}


// 获取码文
void Cipher::putText(QString text)
{
    this->text = text;
}


// 获取密钥
void Cipher::putKey(QString keyWord)
{
    this->key = getKey(keyWord, text);
}


// 小写转大写
int Cipher::toUpper(int i)
{
    int tmp;
    if (i > 'Z')
        tmp = i - ('a' - 'A');
    else
        tmp = i;
    return tmp;
}


// 大写转小写
int Cipher::toLower(int i)
{
    int tmp;
    if (i <= 'Z')
        tmp = i + ('a' - 'A');
    else
        tmp = i;
    return tmp;
}


// 得到密钥
QString Cipher::getKey(QString keyWord, QString text)
{
    size_t plainSize = text.size();
    size_t keyWordSize = keyWord.size();

    QString key;
    key += keyWord;
    for (unsigned i = 0; i < plainSize - keyWordSize; i++)
        key.push_back(text[i]);

    return key;
}


// 加密
void Cipher::encipher()
{
    string Skey = key.toStdString();
    string Stext = text.toStdString();
    string Sciphertext;

    for (string::iterator i = Skey.begin(), j = Stext.begin(); i != Skey.end(); i++, j++)
    {
        int tmp;
        tmp = toUpper(*i) + toUpper(*j) - 'A';
        if (tmp > 'Z')
            tmp -= 26;
        Sciphertext.push_back(tmp);
    }
    text = QString::fromStdString(Sciphertext);
    cout << Sciphertext << endl;
}


// 解密
void Cipher::decipher()
{
    string Skey = key.toStdString();
    string Stext = text.toStdString();
    string Splaintext;

    for (string::iterator i = Skey.begin(), j = Stext.begin(); i != Skey.end(); i++, j++)
    {
        int tmp;
        tmp = toUpper(*j) - toUpper(*i) + 'A';
        if (tmp < 'A')
            tmp += 26;
        Splaintext.push_back(toLower(tmp));
    }
    text = QString::fromStdString(Splaintext);
    cout << Splaintext << endl;
}


// 输出结果
QString Cipher::Result()
{
    return text;
}
