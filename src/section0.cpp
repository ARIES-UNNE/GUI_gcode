#include "section0.h"

Section0::Section0(QWidget *parent) : QWidget(parent) {
    QScrollArea *scrollArea = new QScrollArea(this);
    QWidget *scrollWidget = new QWidget(this);
    layout = new QVBoxLayout(scrollWidget);
    layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    directoryPath = QCoreApplication::applicationDirPath();
    refreshConfigurations();

    scrollArea->setMaximumSize(600, 400);
    scrollArea->setMinimumSize(550, 400);

    scrollArea->setStyleSheet("QScrollArea {"
                              "    border: 2px solid #4CAF50;"
                              "    border-radius: 5px;"
                              "    background: #f9f9f9;"
                              "}"
                              "QScrollBar:vertical {"
                              "    border: none;"
                              "    background: #f1f1f1;"
                              "    width: 10px;"
                              "    margin: 0px 0px 0px 0px;"
                              "}"
                              "QScrollBar::handle:vertical {"
                              "    background: #d3d3d3;"
                              "    min-height: 10px;"
                              "    border-radius: 4px;"
                              "}"
                              "QScrollBar::add-line:vertical {"
                              "    border: none;"
                              "    background: none;"
                              "}"
                              "QScrollBar::sub-line:vertical {"
                              "    border: none;"
                              "    background: none;"
                              "}");

    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    changeDirectoryButton = new QPushButton("Change Directory", this);
    changeDirectoryButton->setFixedSize(130, 30);
    changeDirectoryButton->setStyleSheet("QPushButton {"
                                         "    background-color: #4CAF50;"
                                         "    color: white;"
                                         "    border: none;"
                                         "    border-radius: 5px;"
                                         "    font-size: 14px;"
                                         "}"
                                         "QPushButton:hover {"
                                         "    background-color: #45a049;"
                                         "}");
    connect(changeDirectoryButton, &QPushButton::clicked, this, &Section0::changeDirectory);
    mainLayout->addWidget(changeDirectoryButton);

    QSpacerItem *topSpacer = new QSpacerItem(50, 50, QSizePolicy::Minimum, QSizePolicy::Expanding);
    mainLayout->addItem(topSpacer);

    QLabel *instructionLabel = new QLabel("Selecciona configuración de guardado", this);
    instructionLabel->setAlignment(Qt::AlignCenter);
    instructionLabel->setStyleSheet("font-size: 20px; color: #333;");
    mainLayout->addWidget(instructionLabel);

    QSpacerItem *spacer = new QSpacerItem(30, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);
    mainLayout->addItem(spacer);

    mainLayout->addWidget(scrollArea);
    setLayout(mainLayout);
}

void Section0::refreshConfigurations() {
    QDir directory(directoryPath);
    QStringList confFiles = directory.entryList(QStringList() << "*.conf", QDir::Files);

    clearLayout(layout);

    for (const QString &fileName : confFiles) {
        QPushButton *button = new QPushButton(fileName, this);
        button->setFixedSize(200, 50);
        button->setProperty("isConfButton", true);
        button->setStyleSheet("QPushButton {"
                              "    background-color: #4CAF50;"
                              "    color: white;"
                              "    padding: 10px 24px;"
                              "    border: none;"
                              "    border-radius: 5px;"
                              "    font-size: 14px;"
                              "}"
                              "QPushButton:hover {"
                              "    background-color: #45a049;"
                              "}");
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
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            delete child->widget();
        }
        delete child;
    }
}

void Section0::onConfButtonClicked(const QString &fileName) {
    QString confFilePath = directoryPath + "/" + fileName;
    copyConfiguration(confFilePath);
    QMessageBox::information(this, "Success", "GCODE GENERATED.");
}

void Section0::copyConfiguration(const QString &confFilePath) {
    QFile confFile(confFilePath);
    if (!confFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QString sectionValuesFilePath = QCoreApplication::applicationDirPath() + "/section_values.txt";
    QFile sectionValuesFile(sectionValuesFilePath);
    if (!sectionValuesFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream confStream(&confFile);
    QTextStream sectionValuesStream(&sectionValuesFile);

    QProcess *process = new QProcess(this);
    QString currentPath = QDir::currentPath();
    QString pythonScriptPath = currentPath + "/axolotl_1mat.py";
    process->start("py", QStringList() << pythonScriptPath);
    if (process->waitForStarted()) {
        qDebug() << "Python script started successfully.";
    } else {
        qDebug() << "Failed to start Python script.";
    }

    sectionValuesStream << confStream.readAll();
    sectionValuesStream << "\n\n";

    sectionValuesFile.close();
    confFile.close();
}
