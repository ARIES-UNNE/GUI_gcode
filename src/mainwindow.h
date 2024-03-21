#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFormLayout>
#include <QStackedWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QMessageBox>
#include <qlabel.h>
#include <QLineEdit>
#include <qsizegrip.h>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    QWidget *centralWidget;
    QStackedWidget *stackedWidget;
    QPushButton *generateButton;


private slots:
    void nextSection();
    void generateGCode();
    void previousSection();
    void adjustSectionSize(int sectionIndex);
    void cancelConfirmation();

};

#endif // MAINWINDOW_H
