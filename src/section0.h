#ifndef SECTION0_H
#define SECTION0_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QDir>
#include <QLabel>
#include <QProcess>
#include <QCoreApplication>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>

class Section0 : public QWidget {
    Q_OBJECT

public:
    explicit Section0(QWidget *parent = nullptr);

private:
    QVBoxLayout *layout;
    QPushButton *changeDirectoryButton;
    QString directoryPath;

    void refreshConfigurations();
    void clearLayout(QLayout *layout);

signals:
    void loadConfiguration(const QString &filePath);

private slots:
    void changeDirectory();
    void onConfButtonClicked(const QString &fileName);
public slots:
    void copyConfiguration(const QString &confFilePath);


};

#endif // SECTION0_H
