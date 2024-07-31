#ifndef GENERATESECTION_H
#define GENERATESECTION_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

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

signals:
    void generateGcode();

public slots:
    void retranslateUi();
    void applyStyles(bool darkMode);

};

#endif // GENERATESECTION_H
