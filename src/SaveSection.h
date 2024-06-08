#ifndef SAVESECTION_H
#define SAVESECTION_H

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

// The SaveSection class provides an interface for selecting and managing save configurations.
class SaveSection : public QWidget {
    Q_OBJECT

public:
    // Constructor
    explicit SaveSection(QWidget *parent = nullptr);

private:

    // Layout to arrange widgets vertically
    QVBoxLayout *layout;

    QPushButton *changeDirectoryButton; // Button to change the directory
    QString directoryPath; // Path to the current directory

    // create the list of configuration files in the current directory
    void createConfigurations();

    // Clears the provided layout of buttons (configurations)
    void clearLayout(QLayout *layout);

    //  Styles
    void applyStyles();

signals:
    // Signal emitted to load a configuration file
    void loadConfiguration(const QString &filePath);

private slots:
    // funtion to change the directory
    void changeDirectory();

    // funtion called when a configuration button is clicked
    void onConfButtonClicked(const QString &fileName);

public slots:
    // Copies the selected configuration file to the application's directory
    void copyConfiguration(const QString &confFilePath);
};

#endif // SAVESECTION_H
