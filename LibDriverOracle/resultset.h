#ifndef RESULTSET_H
#define RESULTSET_H

#include <QString>
#include "dbj.h"
#include <jni.h>

namespace driver {

class Q_DECL_EXPORT ResultSet
{
public:
    explicit ResultSet(driver::DbJ *db);
    ~ResultSet();

    inline bool exec(QString query);
    inline bool next();
    inline QString valueString(int column);
    inline QString valueString(QString column);
    inline int valueInt(int column);
    inline int valueInt(QString column);
    inline long valueLong(int column);
    inline long valueLong(QString column);
    void close();
    QString query() const;

private:
    jclass m_class;
    jmethodID m_next;
    jobject m_result;
    QString m_query;
    driver::DbJ *m_db;

};


}


#endif // RESULTSET_H
