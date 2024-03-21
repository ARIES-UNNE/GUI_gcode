#include "section0.h"
#include <QDir>
#include <QCoreApplication>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>

Section0::Section0(QWidget *parent) : QWidget(parent) {
    layout = new QVBoxLayout(this);

    changeDirectoryButton = new QPushButton("Change Directory", this);
    connect(changeDirectoryButton, &QPushButton::clicked, this, &Section0::changeDirectory);
    layout->addWidget(changeDirectoryButton);

    directoryPath = QCoreApplication::applicationDirPath();
    refreshConfigurations();

    setLayout(layout);
}

void Section0::refreshConfigurations() {
    QDir directory(directoryPath);
    QStringList confFiles = directory.entryList(QStringList() << "*.conf", QDir::Files);

    clearLayout(layout);

    for (const QString &fileName : confFiles) {
        QPushButton *button = new QPushButton(fileName, this);
        button->setProperty("isConfButton", true);
        connect(button, &QPushButton::clicked, this, [this, fileName]() {
            onConfButtonClicked(fileName);
        });
        layout->addWidget(button);
    }
}

void Section0::changeDirectory() {
    QString newDirectory = QFileDialog::getExistingDirectory(this, "Select Directory", directoryPath);
    if (!newDirectory.isEmpty()) {
        directoryPath = newDirectory;
        refreshConfigurations();
    }
}

void Section0::clearLayout(QLayout *layout) {
    int itemCount = layout->count();
    for (int i = itemCount - 1; i >= 0; --i) {
        QLayoutItem *item = layout->takeAt(i);
        QWidget *widget = item->widget();
        if (widget && widget != changeDirectoryButton && widget->property("isConfButton").toBool()) {
            delete widget;
        }
        delete item;
    }
}

void Section0::onConfButtonClicked(const QString &fileName) {
    QString confFilePath = directoryPath + "/" + fileName;
    copyConfiguration(confFilePath);
    QMessageBox::information(this, "Success", "Configuration copied successfully.");
}

void Section0::copyConfiguration(const QString &confFilePath) {
    QFile confFile(confFilePath);
    if (!confFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Error al abrir el archivo .conf
        return;
    }

    QString sectionValuesFilePath = QCoreApplication::applicationDirPath() + "/section_values.txt";
    QFile sectionValuesFile(sectionValuesFilePath);
    if (!sectionValuesFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // Error al abrir o crear el archivo section_values.txt
        return;
    }

    QTextStream confStream(&confFile);
    QTextStream sectionValuesStream(&sectionValuesFile);

    sectionValuesStream << confStream.readAll();
    sectionValuesStream << "\n\n";

    sectionValuesFile.close();
    confFile.close();
}

