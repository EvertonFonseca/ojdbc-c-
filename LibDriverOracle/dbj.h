#ifndef DBJ_H
#define DBJ_H

#include <jni.h>
#include <QString>
#include <QDebug>
#include <iostream>

namespace driver {

class ResultSet;
class Statement;

class Q_DECL_EXPORT DbJ
{
    friend class driver::ResultSet;
    friend class driver::Statement;
public:
    explicit DbJ(QString url,QString user,QString pass,QString schema);
    ~DbJ();
    bool open();
    bool isOpen();
    void close();
    void activeSchema(bool flag);
    QString user() const;
    void setUser(const QString &user);

    QString url() const;
    void setUrl(const QString &url);

    QString pass() const;
    void setPass(const QString &pass);

    QString schema() const;
    void setSchema(const QString &schema);

protected:
    void init();
    jobject createResultSet(QString query);
    jobject createPreparedStatement(QString query);
    JNIEnv* getEnv();


private:
    jclass m_class;
    QString m_user;
    QString m_url;
    QString m_pass;
    QString m_schema;
    jobject m_construtor;
    bool m_flag;
};

}
#include "statement.h"
#include "resultset.h"
#include "environment.h"

#endif // DBJ_H
