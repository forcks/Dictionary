#include "config.h"
#include "QFile"
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QByteArray>

config::config()
{

}

bool config::isNotNullConfig(){
     if(readInConfig()!= nullptr)
         return true;
     else
         return false;
}

void config::createConfig(){
    QDir DirConfig;
    if(DirConfig.mkpath(QApplication::applicationDirPath() + "/config"))
        qDebug()<<"config folder created !!!";
    QFile config(QApplication::applicationDirPath() + "/config"+ "/config.txt");
    if(!config.exists()){
        config.open(QIODevice::WriteOnly);
        config.close();
    }
}

void config::writeInConfig(QString text){
    QFile config(QApplication::applicationDirPath() + "/config"+ "/config.txt");
    if(config.open(QIODevice::WriteOnly)){
        QTextStream writeStream(&config);
        writeStream<<text;
        config.close();
    }
}

QString config::readInConfig(){
    QFile config(QApplication::applicationDirPath() + "/config"+ "/config.txt");
    QByteArray data;
    if (!config.open(QIODevice::ReadOnly)){
        qDebug()<<"Config don't want open";
        return "";
    }
     data = config.readAll();
     return QString(data);
}











