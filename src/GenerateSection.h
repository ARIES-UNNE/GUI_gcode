#ifndef GENERATESECTION_H
#define GENERATESECTION_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCoreApplication>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QDir>


// The GenerateSection class for the fibak generation section of the UI
class GenerateSection : public QWidget
{
    Q_OBJECT

public:
    // Constructor
    explicit GenerateSection(QWidget *parent = nullptr);

private:
    QLabel *completionLabel;
    QPushButton *generateButton;
    QPushButton *saveGcodeButton;

signals:
    void generateGcode();

public slots:
    void retranslateUi();
    void applyStyles(bool darkMode);

private slots:
    void saveGcode();

};

#endif // GENERATESECTION_H
