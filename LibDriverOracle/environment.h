#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QProcessEnvironment>
#include <QLibrary>
#include <QScopedPointer>
#include <iostream>
#include <jni.h>
#include <QtGlobal>
#include <QDirIterator>
#include <QDir>
#include <QFile>
#include <QCoreApplication>
#include <QByteArray>

namespace driver {

class DbJ;

class Q_DECL_EXPORT Environment
{
    friend class driver::DbJ;

private:
    Environment();
    static JNIEnv* rootJniEnv();

protected:

    static void copyPath(QDir path,QString root,QDir *principal);
    static void createResourceDir(QString pathRoot);

public:
    static bool Initialize(std::string path = QCoreApplication::instance()->applicationDirPath().toStdString());

};

}

#include "dbj.h"

#endif // ENVIRONMENT_H
