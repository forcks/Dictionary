#include "config.h"
#include "QFile"
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QByteArray>

#include "QtSql/QSqlDatabase"
#include "QSqlQuery"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>

config::config()
{

}


void config::createConfig(){
}

void config::writeInConfig(QString key,QString text,QString nameFile){
    QJsonDocument _jsonDoc;
    QJsonObject jsonObj;
    //jsonObj.insert(key,text);
    jsonObj[key]=text;
    _jsonDoc.setObject(jsonObj);

    QFile configFile(nameFile+".json");
    configFile.open(QFile::WriteOnly);
    configFile.write(_jsonDoc.toJson());
}

QString config::readInConfig(QString key,QString anotherValue,QString nameFile){
    QFile configFile(nameFile+".json");
    configFile.open(QFile::ReadOnly);
    QByteArray readValue = QJsonDocument().fromJson(configFile.readAll()).toJson();

    QString jsonStr = QString(readValue);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();
    QString searchValue = anotherValue;
    for (const QString& eachKey : jsonObj.keys())
    {
        if(eachKey == key)
            searchValue= jsonObj.value(eachKey).toString();
    }
    return searchValue;
}











