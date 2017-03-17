#ifndef CIPHER_H
#define CIPHER_H

#include <QString>
#include <string>
#include <iostream>

using namespace std;

class Cipher
{
private:
    QString text;
    QString key;

public:
    Cipher(QString text = "", QString key = "");

    void putText(QString text);
    void putKey(QString keyWord);

    void encipher();
    void decipher();
    int toUpper(int i);
    int toLower(int i);

    QString getKey(QString keyWord, QString text);

    QString Result();
};

#endif // CIPHER_H

