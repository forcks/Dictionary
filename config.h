#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

class config
{
public:
    config();
    void createConfig();
    void writeInConfig(QString key,QString text,QString nameFile);
    QString readInConfig(QString key,QString anotherValue,QString nameFile);
};

#endif // CONFIG_H
