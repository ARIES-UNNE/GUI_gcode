#include "pythonexecutor.h"

PythonExecutor::PythonExecutor() {}

void PythonExecutor::executePythonScript() {
    QProcess *process = new QProcess();
    QString currentPath = QDir::currentPath();
    QString pythonScriptPath = currentPath + "/axolotl_1mat.py";
    process->start("py", QStringList() << pythonScriptPath);
    if (process->waitForStarted()) {
        qDebug() << "Python script started successfully.";
    } else {
        qDebug() << "Failed to start Python script.";
    }
}
