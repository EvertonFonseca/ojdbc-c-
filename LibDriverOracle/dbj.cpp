#include "dbj.h"

namespace driver{

DbJ::DbJ(QString url, QString user, QString pass,QString schema)
    :m_url(url)
    ,m_user(user)
    ,m_pass(pass)
    ,m_schema(schema.isEmpty() ? "DEFAULT": schema)
    ,m_flag(true)
{
    this->init();
}

DbJ::~DbJ()
{
    this->close();

    auto env = Environment::rootJniEnv();

    if(env)
        env->DeleteLocalRef(this->m_construtor);

}

bool DbJ::open()
{
    bool status = false;
    try {

        auto env = Environment::rootJniEnv();

        if(!this->m_flag)
            this->m_schema = "";

        jmethodID connection = env->GetMethodID(this->m_class,"connect","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z");
        status = env->CallBooleanMethod(this->m_construtor,connection,
                                        env->NewStringUTF(
                                            QByteArray().append(this->m_url).data()),
                                        env->NewStringUTF(
                                            QByteArray().append(this->m_user).data()),
                                        env->NewStringUTF(
                                            QByteArray().append(this->m_pass).data()),
                                        env->NewStringUTF(
                                            QByteArray().append(this->m_schema).data()));

    } catch (std::exception &e) {

        std::cout << "Error: open connection "<<e.what() << std::endl;
        status = false;
    }
    return status;
}

bool DbJ::isOpen()
{
    auto env = Environment::rootJniEnv();

    jmethodID idFun = env->GetMethodID(this->m_class,"isOpen","()Z");

    if(idFun == nullptr)
        return false;

    bool status = env->CallBooleanMethod(this->m_construtor,idFun);

    return status;
}

void DbJ::close()
{
    auto env = Environment::rootJniEnv();

    if(!env) return;

    jmethodID idFun = env->GetMethodID(this->m_class,"close","()V");

    if(idFun == nullptr)
        return;

    env->CallBooleanMethod(this->m_construtor,idFun);
}

void DbJ::activeSchema(bool flag)
{
    this->m_flag = flag;
}

void DbJ::init()
{
    auto env = Environment::rootJniEnv();

    if(env == nullptr){

        std::cout << "ERROR: JniENv is null !" << std::endl;
        return;
    }

    this->m_class = env->FindClass("com/m7/ai/Database");

    if(this->m_class == nullptr) {

        std::cout << "ERROR: class not found !" << std::endl;
    }

    jmethodID constructor = env->GetMethodID(this->m_class,"<init>", "()V");

    this->m_construtor = env->NewObject(this->m_class, constructor);

}

jobject DbJ::createResultSet(QString query)
{
    auto env = Environment::rootJniEnv();

    jmethodID idSql = env->GetMethodID(this->m_class,"sql","(Ljava/lang/String;)Ljava/sql/ResultSet;");

    jobject result = env->CallObjectMethod(this->m_construtor,idSql,env->NewStringUTF(QByteArray().append(query).data()));

    return result;
}

jobject DbJ::createPreparedStatement(QString query)
{
    auto env = Environment::rootJniEnv();

    jmethodID idSql = env->GetMethodID(this->m_class,"sql","(Ljava/lang/String;)Ljava/sql/PreparedStatement;");

    jobject st = env->CallObjectMethod(this->m_construtor,idSql,env->NewStringUTF(QByteArray().append(query).data()));

    return st;
}

JNIEnv *DbJ::getEnv()
{
    return Environment::rootJniEnv();
}

QString DbJ::schema() const
{
    return m_schema;
}

void DbJ::setSchema(const QString &schema)
{
    m_schema = schema;
}

QString DbJ::pass() const
{
    return m_pass;
}

void DbJ::setPass(const QString &pass)
{
    m_pass = pass;
}

QString DbJ::url() const
{
    return m_url;
}

void DbJ::setUrl(const QString &url)
{
    m_url = url;
}

QString DbJ::user() const
{
    return m_user;
}

void DbJ::setUser(const QString &user)
{
    m_user = user;
}

}
