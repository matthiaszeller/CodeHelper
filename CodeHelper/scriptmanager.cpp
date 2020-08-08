#include "scriptmanager.h"
#include <iostream>
ScriptManager::ScriptManager()
{

}

QString ScriptManager::exec_python(const QString &script_name, const QStringList &args, bool python3) {
    // Use a static method of QApplication to get the executable path
    QString scriptPath(QApplication::applicationDirPath());
    // Change the directory to target ../res/ relative to scriptPath
    int lastSlash(scriptPath.lastIndexOf("/"));
    scriptPath = scriptPath.mid(0, lastSlash) + "/res/" + script_name;
    // Determine version of python to execute
    QString progName(python3 ? "python3" : "python");
    // CHECKPOINT: check if the script does exist
    // QProcess returns no error if this is the case ???
    if(!QFile::exists(scriptPath))
        throw std::runtime_error("The script '"+scriptPath.toStdString()+"' does not exist !");

    // Initialize the process
    QProcess p;
    // Run the process, wait for finished
    QStringList progArgs(QStringList() << scriptPath << args);
    p.start(progName, progArgs);
    p.waitForFinished();
    std::cout << "Running " << scriptPath.toStdString() << std::endl;
    // Return the output
    return p.readAllStandardOutput();
}
