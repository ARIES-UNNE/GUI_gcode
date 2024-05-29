#ifndef PYTHONEXECUTOR_H
#define PYTHONEXECUTOR_H

#include <QObject>
#include <QProcess>
#include <QDir>
#include <QDebug>

class PythonExecutor {
public:
    PythonExecutor();

    void executePythonScript();
};

#endif // PYTHONEXECUTOR_H
