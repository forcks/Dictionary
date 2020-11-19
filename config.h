#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

class config
{
public:
    config();
    bool isNotNullConfig();
    void createConfig();
    void writeInConfig(QString text);
    QString readInConfig();
};

#endif // CONFIG_H
