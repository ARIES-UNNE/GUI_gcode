#include "SaveSection.h"

SaveSection::SaveSection(QWidget *parent) : QWidget(parent) {
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollWidget = new QWidget(this);
    scrollWidget->setObjectName("scrollWidget");

    layout = new QVBoxLayout(scrollWidget);
    layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    scrollArea->setMaximumSize(240, 400);
    scrollArea->setMinimumSize(240, 400);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollWidget);

    QWidget *scrollContainer = new QWidget(this);
    QHBoxLayout *scrollContainerLayout = new QHBoxLayout(scrollContainer);
    scrollContainerLayout->addWidget(scrollArea);

    QSettings settings("Nebrija", "3Dprinting");
    directoryPath = settings.value("saveSection/directoryPath", QCoreApplication::applicationDirPath() + "/configurations").toString();
    qDebug() << "New directory path saved:" << directoryPath;

    QMetaObject::invokeMethod(this, [this]() {
            emit fileRoute(directoryPath);
        }, Qt::QueuedConnection);

    changeDirectoryButton = new QPushButton(tr("Change Directory"), this);
    changeDirectoryButton->setFixedSize(140, 30);
    connect(changeDirectoryButton, &QPushButton::clicked, this, &SaveSection::changeDirectory);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(changeDirectoryButton, 0, Qt::AlignLeft);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mainLayout->addLayout(buttonLayout);

    // LABEL
    // Add spacers on top of instruction text
    QSpacerItem *topSpacer = new QSpacerItem(50, 50, QSizePolicy::Minimum, QSizePolicy::Expanding);
    mainLayout->addItem(topSpacer);

    instructionLabel = new QLabel(tr("Select Save Configuration"), this);
    instructionLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(instructionLabel);

    QSpacerItem *spacer = new QSpacerItem(30, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);
    mainLayout->addItem(spacer);

    mainLayout->addWidget(scrollContainer);
    setLayout(mainLayout);

    createConfigurations();

    applyStyles(true);
}

void SaveSection::retranslateUi() {
    changeDirectoryButton->setText(tr("Change Directory"));
    instructionLabel->setText(tr("Select Save Configuration"));
}

void SaveSection::createConfigurations() {
    QDir directory(directoryPath);
    QStringList confFiles = directory.entryList(QStringList() << "*.conf", QDir::Files);

    clearLayout(layout);

    for (const QString &fileName : confFiles) {
        QPushButton *button = new QPushButton(fileName, this);
        button->setFixedSize(200, 50);
        button->setProperty("isConfButton", true);

        connect(button, &QPushButton::clicked, this, [this, fileName]() {
            onConfButtonClicked(fileName);
        });

        layout->addWidget(button);
    }

}
void SaveSection::changeDirectory() {
    QString newDirectory = QFileDialog::getExistingDirectory(this, tr("Select Directory"), directoryPath);
    if (!newDirectory.isEmpty()) {
        directoryPath = newDirectory;

        QSettings settings("Nebrija", "3Dprinting");
        settings.setValue("saveSection/directoryPath", directoryPath);

        QString savedPath = settings.value("saveSection/directoryPath").toString();
        qDebug() << "New directory path saved:" << savedPath;
        emit fileRoute(savedPath);

        createConfigurations();
    } else {
        qDebug() << "No directory was selected.";
    }
}


void SaveSection::clearLayout(QLayout *layout) {
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            delete child->widget();
        }
        delete child;
    }
}

void SaveSection::onConfButtonClicked(const QString &fileName) {
    QString confFilePath = directoryPath + "/" + fileName;
    copyConfiguration(confFilePath);
    emit fileSelected(fileName);

}

void SaveSection::copyConfiguration(const QString &confFilePath) {
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

    readSectionValues();
}

void SaveSection::readSectionValues() {
    QFile file(QCoreApplication::applicationDirPath() + "/section_values.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    QString section;
    int plateX = 0, plateY = 0, centerX = 0, centerY = 0;
    int shapeIndex1 = 0, shapeIndex2 = 0, size1 = 0, size2 = 0;
    int methodIndex = 0, infillValue = 0;
    double strandDistance = 0.0;
    QList<QPair<QString, QString>> section4Materials;

    while (!in.atEnd()) {
        QString line = in.readLine();

        if (line.contains("Section1 Values:")) {
            section = "Section1";
        } else if (line.contains("Section2 Values:")) {
            section = "Section2";
        } else if (line.contains("Section3 Values:")) {
            section = "Section3";
        } else if (line.contains("Section4 Values:")) {
            section = "Section4";
        } else if (section == "Section1") {
            if (line.contains("Plate X Value:")) {
                plateX = line.split(":").last().trimmed().toInt();
            } else if (line.contains("Plate Y Value:")) {
                plateY = line.split(":").last().trimmed().toInt();
            } else if (line.contains("Center X Value:")) {
                centerX = line.split(":").last().trimmed().toInt();
            } else if (line.contains("Center Y Value:")) {
                centerY = line.split(":").last().trimmed().toInt();
            }
        } else if (section == "Section2") {
            if (line.contains("Shape Index 1:")) {
                shapeIndex1 = line.split(":").last().trimmed().toInt();
            } else if (line.contains("Shape Index 2:")) {
                shapeIndex2 = line.split(":").last().trimmed().toInt();
            } else if (line.contains("Size 1:")) {
                size1 = line.split(":").last().trimmed().toInt();
            } else if (line.contains("Size 2:")) {
                size2 = line.split(":").last().trimmed().toInt();
            }
        } else if (section == "Section3") {
            if (line.contains("Method Index:")) {
                methodIndex = line.split(":").last().trimmed().toInt();
            } else if (line.contains("Infill Value:")) {
                infillValue = line.split(":").last().trimmed().toInt();
            } else if (line.contains("Strand Distance:")) {
                strandDistance = line.split(":").last().trimmed().toDouble();
            }
        } else if (section == "Section4") {
            if (line.contains("Material Name:")) {
                QString materialName = line.split(":").last().trimmed();
                QString filament, nozzle;
                while (!in.atEnd()) {
                    line = in.readLine();
                    if (line.contains("Filament:")) {
                        filament = line.split(":").last().trimmed();
                    } else if (line.contains("Nozzle:")) {
                        nozzle = line.split(":").last().trimmed();
                    } else if (line.trimmed().isEmpty()) {
                        section4Materials.append(qMakePair(materialName, filament + " | " + nozzle));
                        break;
                    }
                }
            }
        }
    }

    file.close();

    emit section1ValuesRead(plateX, plateY, centerX, centerY);
    emit section2ValuesRead(shapeIndex1, shapeIndex2, size1, size2);
    emit section3ValuesRead(methodIndex, infillValue, strandDistance);
    emit section4ValuesRead(section4Materials);
}

void SaveSection::clearSettings() {
    QSettings settings;
    settings.remove("saveSection/directoryPath");
}


// Aplica estilos a los widgets de la sección
void SaveSection::applyStyles(bool darkMode) {
    if (darkMode) {
        setStyleSheet(
            "QScrollArea {"
            "    border: 0px solid #4CAF50;"
            "    border-radius: 5px;"
            "}"
            "QScrollBar:vertical {"
            "    border: none;"
            "    background: #3d3d3d;"
            "    width: 10px;"
            "    margin: 0px 0px 0px 0px;"
            "}"
            "QScrollBar::handle:vertical {"
            "    background: #555555;"
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
            "}"
            "QPushButton {"
            "    background-color: #2a2a2a;"
            "    color: white;"
            "    border: none;"
            "    border-radius: 5px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #001900;"
            "    border: 1px solid #4CAF50;"
            "    border-radius: 5px;"
            "}"
            "QLabel {"
            "    font-size: 20px;"
            "    color: #4CAF50;"
            "    font-weight: bold;"
            "    padding: 10px 5px;"
            "}"
            "QWidget#scrollWidget {"
            "    background-color: #1a1a1a;"
            "}"
            );
    } else {
        setStyleSheet(
            "QScrollArea {"
            "    border: 0px solid #4CAF50;"
            "    border-radius: 5px;"
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
            "}"
            "QPushButton:hover {"
            "    background-color: #e6ffe6;"
            "    border: 1px solid #4CAF50;"
            "    border-radius: 5px;"
            "}"
            "QLabel{"
            "    font-size: 20px;"
            "    color: #4CAF50;"
            "    font-weight: bold;"
            "    padding: 10px 5px;"
            "}"
            );
    }
}
