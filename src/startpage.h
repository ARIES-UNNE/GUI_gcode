#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

class SectionStart : public QWidget {
    Q_OBJECT

public:
    explicit SectionStart(QWidget *parent = nullptr);
    void retranslateUi();
   void applyStyles(bool darkMode);
signals:
    void startButtonClicked();
    void languageChanged(const QString &language);

private:
    QPushButton *startButton;
    QComboBox *languageComboBox;
    QLabel *welcomeLabel;
};

#endif // STARTPAGE_H
