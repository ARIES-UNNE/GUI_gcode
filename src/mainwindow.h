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

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *stackedWidget;
    QComboBox *shapeComboBox;
    QComboBox *shapeComboBox2;
    QSpinBox *infillSpinBox;
    QSpinBox *strandDistanceSpinBox;
    QSpinBox *numMaterialsSpinBox;
    QPushButton *generateButton;
    QLineEdit *sizeLineEdit;
    QLineEdit *sizeLineEdit2;
    QLabel *shapeLabel;

private slots:
    void nextSection();
    void generateGCode();
    void previousSection();
    void handleShapeSelection(int index);

};

#endif // MAINWINDOW_H
