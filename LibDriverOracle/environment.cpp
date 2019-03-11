#include "environment.h"

static QSharedPointer<JNIEnv>   jniEnv;
static QScopedPointer<QLibrary> jniJVM;

#define Environment_JVM  "JAVA_HOME"

namespace driver {

JNIEnv *Environment::rootJniEnv()
{
    JNIEnv* env;
    JavaVM* jvm;
    jniEnv.data()->GetJavaVM(&jvm);
    jvm->AttachCurrentThread((void**)&env,nullptr);

    return env;
}

void Environment::copyPath(QDir path, QString root, QDir *principal)
{
    QDir directory = QDir(path);

    QString nomeDir = root+"/"+path.path().replace(":/","");
    QDir pathRoot(nomeDir);

    if(!pathRoot.exists())
    {
        principal->mkdir(pathRoot.path());

    }
    QStringList dir = directory.entryList(QDir::Dirs);
    QStringList files = directory.entryList(QDir::Files);

    //cp all files desta pasta
    for (int i = 0; i < files.size(); i++) {

        QFile f (directory.path()+"/"+files.at(i));
        QString toRoot = nomeDir+"/"+files.at(i);

        if(QFile(toRoot).exists())
            QFile(toRoot).remove();

        QFile::copy(f.fileName(),toRoot);
        QFile::setPermissions(toRoot,QFile::ReadUser | QFile::WriteUser);

    }

    //tem diretorio
    for (int i = 0; i < dir.size(); i++) {

        QDir d (directory.path()+"/"+dir.at(i));

        QDir rootPath = QDir(nomeDir+"/"+dir.at(i));

        if(!rootPath.exists()){

            principal->mkdir(rootPath.path());
        }

        copyPath(d,root,principal);
    }
}

void Environment::createResourceDir(QString pathRoot){

    QString path = pathRoot.isEmpty() ? QCoreApplication::instance()->applicationDirPath() : pathRoot;
    QDir pathDir(path);

    copyPath(QDir(":/"),path,&pathDir);
}

bool Environment::Initialize(std::string path)
{
    if(!jniEnv.isNull()){

        std::cout << "Environment already was Initialize!" << std::endl;
        return false;
    }

    auto systemEnv = QProcessEnvironment::systemEnvironment();

    if(!systemEnv.keys().contains(Environment_JVM)){

        std::cout << "Environment variable JAVA_HOME wasn't found!" << std::endl;
        return false;
    }

    QByteArray javaEnv;
    QDirIterator it(systemEnv.value(Environment_JVM), QDirIterator::Subdirectories);

    while (it.hasNext()){

#ifdef Q_OS_WIN
        if(it.fileName() == "jvm.dll"){
            javaEnv.append(it.filePath());
            break;
        }
#endif

#ifdef Q_OS_LINUX
        if(it.fileName() == "libjvm.so"){
            javaEnv.append(it.filePath());
            break;
        }
#endif
        it.next();

    }// end while

    jniJVM.reset(new QLibrary(javaEnv));

    if(!jniJVM.data()->load()){

        std::cout << "JVM wasn't loaded!" << std::endl;
        return false;
    }

    Environment::createResourceDir(QString::fromStdString(path));

    typedef jint  (*JNI_GetCreatedJavaVMs) (JavaVM **pvm,jsize, jsize *);

    JNI_GetCreatedJavaVMs funJVM = (JNI_GetCreatedJavaVMs) jniJVM.data()->resolve("JNI_GetCreatedJavaVMs");

    int gc = 0;
    JavaVM *jvm = nullptr;                      // Pointer to the JVM (Java Virtual Machine)
    JNIEnv *env = nullptr;                      // Pointer to native interface
    //================== prepare loading of Java VM ============================
    JavaVMInitArgs args;                        // Initialization arguments
    JavaVMOption* options = new JavaVMOption[1];   // JVM invocation options
    options[0].optionString = (char*)"-Djava.class.path=lib/DriverDB.jar";   // where to find java .class
    args.version = JNI_VERSION_1_8;             // minimum Java version
    args.nOptions = 1;                          // number of options
    args.options = options;
    args.ignoreUnrecognized = true;     // invalid options make the JVM init fail

    int n;
    jint status = funJVM(&jvm,1, (jsize*) &n);
    // try to get and attach to existing VM first

    if (status == JNI_OK && n == 1) {

        status = jvm->AttachCurrentThread((void**)&env, &args);

    }
    // if not successful, create a new VM
    if (status != JNI_OK || n != 1) {

        typedef jint  (*JNI_CreateJavaVM) (JavaVM **pvm, void **penv, void *args);

        JNI_CreateJavaVM functionJVM = (JNI_CreateJavaVM) jniJVM.data()->resolve("JNI_CreateJavaVM");

        status = functionJVM(&jvm, (void**)&env, &args);

        if(status != JNI_OK){

            std::cout << "Error JVM wasn't created!" << std::endl;
            jniEnv.reset(nullptr);
            jniJVM.reset(nullptr);
            return false;
        }

        gc = 1;
    }
    delete[] options;

    jniEnv.reset(env);
    std::cout << "Environment was loaded succeeded!" << std::endl;
    return true;
}


}
