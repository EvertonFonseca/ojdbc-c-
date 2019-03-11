#include "resultset.h"

namespace driver {

ResultSet::ResultSet(driver::DbJ *db):m_db(db)
{
    auto env = this->m_db->getEnv();

    this->m_class  = env->FindClass("java/sql/ResultSet");
    this->m_next   = env->GetMethodID(this->m_class,"next","()Z");

}

ResultSet::~ResultSet()
{
   this->close();

    auto env = this->m_db->getEnv();

    if(env)
        env->DeleteLocalRef(this->m_result);
}

bool ResultSet::exec(QString query)
{
    this->m_query = query;

    std::cout<<"Query: " << query.toStdString() << std::endl;
    this->m_result = this->m_db->createResultSet(query);

    return this->m_result != nullptr;

}

bool ResultSet::next()
{
    auto env = this->m_db->getEnv();

    if(this->m_class == nullptr) {

        std::cerr << "ERROR: class not found !" << std::endl;
    }
    return env->CallBooleanMethod(this->m_result,this->m_next);
}

void ResultSet::close()
{
    auto env = this->m_db->getEnv();

    if(!env) return;

    if(this->m_class == nullptr) {

        std::cerr << "ERROR: class not found !" << std::endl;
    }

    jmethodID id  = env->GetMethodID(this->m_class,"close","()V");

    env->CallVoidMethod(this->m_result,id);
}

QString ResultSet::query() const
{
    return m_query;
}

QString ResultSet::valueString(int column)
{
    auto env = this->m_db->getEnv();

    if(this->m_class == nullptr) {

        std::cerr << "ERROR: class not found !" << std::endl;
    }

    jmethodID id  = env->GetMethodID(this->m_class,"getString","(I)Ljava/lang/String;");

    auto data = jstring(env->CallObjectMethod(this->m_result,id,column));

    return QString(data != nullptr ? env->GetStringUTFChars(data,nullptr) : "");

}

QString ResultSet::valueString(QString column)
{
    auto env = this->m_db->getEnv();

    if(this->m_class == nullptr) {

        std::cerr << "ERROR: class not found !" << std::endl;
    }

    jmethodID id  = env->GetMethodID(this->m_class,"getString","(Ljava/lang/String;)Ljava/lang/String;");

    auto data = jstring(env->CallObjectMethod(this->m_result,id,env->NewStringUTF(QByteArray("").append(column).data())));

    return QString(data != nullptr ? env->GetStringUTFChars(data,nullptr) : "");
}

int ResultSet::valueInt(int column)
{
    auto env = this->m_db->getEnv();

    if(this->m_class == nullptr) {

        std::cerr << "ERROR: class not found !" << std::endl;
    }

    jmethodID id  = env->GetMethodID(this->m_class,"getInt","(I)I");

    return  env->CallIntMethod(this->m_result,id,column);
}

int ResultSet::valueInt(QString column)
{
    auto env = this->m_db->getEnv();

    if(this->m_class == nullptr) {

        std::cerr << "ERROR: class not found !" << std::endl;
    }

    jmethodID id  = env->GetMethodID(this->m_class,"getInt","(Ljava/lang/String;)I");

    return  env->CallIntMethod(this->m_result,id,env->NewStringUTF(QByteArray().append(column).data()));
}

long ResultSet::valueLong(int column)
{
    auto env = this->m_db->getEnv();

    if(this->m_class == nullptr) {

        std::cerr << "ERROR: class not found !" << std::endl;
    }

    jmethodID id  = env->GetMethodID(this->m_class,"getLong","(I)J");

    return long(env->CallLongMethod(this->m_result,id,column));
}

long ResultSet::valueLong(QString column)
{
    auto env = this->m_db->getEnv();

    if(this->m_class == nullptr) {

        std::cerr << "ERROR: class not found !" << std::endl;
    }

    jmethodID id  = env->GetMethodID(this->m_class,"getLong","(Ljava/lang/String;)J");

    return long(env->CallLongMethod(this->m_result,id,env->NewStringUTF(QByteArray().append(column).data())));
}


}
