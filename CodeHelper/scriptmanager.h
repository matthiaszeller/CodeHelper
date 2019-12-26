#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <QProcess>
#include <QApplication>
#include <QFile>

//! Run python scripts and get the output back using QProcess
//! By default, looks for the file in ../res/ relative the the executable
class ScriptManager
{
public:
    ScriptManager();

    static QString exec_python(const QString &script_name,
                               const QStringList &args = QStringList(),
                               bool python3 = true);
};

#endif // SCRIPTMANAGER_H
