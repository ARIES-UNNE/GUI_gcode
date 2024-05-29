#ifndef SECTIONSTART_H
#define SECTIONSTART_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class SectionStart : public QWidget {
    Q_OBJECT

public:
    explicit SectionStart(QWidget *parent = nullptr);

signals:
    void startButtonClicked();

private:
    QPushButton *startButton;
};

#endif // SECTIONSTART_H
