#ifndef Statement_H
#define Statement_H

#include <QString>
#include "dbj.h"
#include <jni.h>

namespace driver {

class Q_DECL_EXPORT Statement
{
public:
    explicit Statement(driver::DbJ *db);
    ~Statement();

    inline bool exec(QString query);

protected:
    void init();
    void close();

private:
    jclass m_class;
    jmethodID m_executeUpdate;
    jmethodID m_close;
    jobject m_result;
    QString m_query;
    driver::DbJ *m_db;
};

}

#endif // Statement_H
