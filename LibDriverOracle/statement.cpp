#include "statement.h"

namespace driver {

Statement::Statement(DbJ *db)
{
    auto env       = db->getEnv();
    this->m_class  = env->FindClass("java/sql/PreparedStatement");
    this->m_executeUpdate   = env->GetMethodID(this->m_class,"executeUpdate","()I");

}

Statement::~Statement()
{
    this->close();

    auto env = this->m_db->getEnv();

    if(env)
        env->DeleteLocalRef(this->m_result);
}

bool Statement::exec(QString query)
{
    this->m_query = query;

    std::cout<<"Query: " << query.toStdString() << std::endl;
    this->m_result = this->m_db->createPreparedStatement(query);

    bool status = this->m_result != nullptr;

    if(status){

        auto env = this->m_db->getEnv();
        status = env->CallIntMethod(this->m_class,this->m_executeUpdate) > 0;
        this->close();
    }

    return status;
}

void Statement::close()
{
    auto env = this->m_db->getEnv();

    if(!env) return;

    if(this->m_class == nullptr) {

        std::cerr << "ERROR: class not found !" << std::endl;
    }

    jmethodID id  = env->GetMethodID(this->m_class,"close","()V");

    env->CallVoidMethod(this->m_result,id);
}

}
