#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QProcess>
#include <QDebug>
#include <QFileDialog>
#include <QMainWindow>
#include <QFormLayout>
#include <QStackedWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QCoreApplication>
#include <QPushButton>
#include <QMessageBox>
#include <qlabel.h>
#include <QLineEdit>
#include <qsizegrip.h>
#include "section1.h"
#include "section2.h"
#include "section3.h"
#include "opengl.h"
#include "section4.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    QWidget *centralWidget;
    QStackedWidget *stackedWidget;
    QPushButton *generateButton;
    QPushButton *showValuesButton;

    QPushButton *cancelButton;
    QPushButton *prevButton;
    QPushButton *nextButton;
    OpenGLWidget *openGLWidget;

    QVBoxLayout *centeredButtonLayout;

    void applyStyles();

    bool saveConfigurationToFile(const QString &fileName, Section1 *section1Widget, Section2 *section2Widget, Section3 *section3Widget, Section4 *section4Widget);

private slots:
    void nextSection();
    void previousSection();
    void adjustSectionSize(int sectionIndex);
    void cancelConfirmation();
    void showSectionValues(Section1 *section1Widget, Section2 *section2Widget,  Section3 *section3Widget, Section4 *section4Widget);
    void RealTimeGCODE(Section1 *section1Widget, Section2 *section2Widget,  Section3 *section3Widget, Section4 *section4Widget);
    void executePython();



};

#endif // MAINWINDOW_H
