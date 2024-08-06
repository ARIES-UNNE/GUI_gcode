#include "SaveSection.h"

SaveSection::SaveSection(QWidget *parent) : QWidget(parent) {
    // Initialize the scroll area and the widget that will contain the layout
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollWidget = new QWidget(this);
    scrollWidget->setObjectName("scrollWidget");

    // Create a vertical layout for the scroll widget and align it to the top and center horizontally
    layout = new QVBoxLayout(scrollWidget);
    layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    // Configure the scroll area
    scrollArea->setMaximumSize(240, 400);
    scrollArea->setMinimumSize(240, 400);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollWidget);

    // Create a container widget for the scroll area and add horizontal layout for centering
    QWidget *scrollContainer = new QWidget(this);
    QHBoxLayout *scrollContainerLayout = new QHBoxLayout(scrollContainer);
    scrollContainerLayout->addWidget(scrollArea);

    // Set the initial directory path to the application directory
    directoryPath = QCoreApplication::applicationDirPath();

    // Create and configure the change directory button
    changeDirectoryButton = new QPushButton(tr("Change Directory"), this);
    changeDirectoryButton->setFixedSize(140, 30);
    connect(changeDirectoryButton, &QPushButton::clicked, this, &SaveSection::changeDirectory);

    // Add the change directory button to the main layout aligned to the left
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(changeDirectoryButton, 0, Qt::AlignLeft);

    // Create the main layout for the section
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mainLayout->addLayout(buttonLayout);

    // LABEL
    // Add spacers on top of instruction text
    QSpacerItem *topSpacer = new QSpacerItem(50, 50, QSizePolicy::Minimum, QSizePolicy::Expanding);
    mainLayout->addItem(topSpacer);

    // Instruction text and configuration
    instructionLabel = new QLabel(tr("Select Save Configuration"), this);
    instructionLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(instructionLabel);

    // Add spacers under instruction text
    QSpacerItem *spacer = new QSpacerItem(30, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);
    mainLayout->addItem(spacer);

    mainLayout->addWidget(scrollContainer);
    setLayout(mainLayout);

    // Create buttons
    createConfigurations();

    // Apply styles
    applyStyles(false);
}

void SaveSection::retranslateUi() {
    changeDirectoryButton->setText(tr("Change Directory"));
    instructionLabel->setText(tr("Select Save Configuration"));
}

void SaveSection::createConfigurations() {
    QDir directory(directoryPath);
    // Get the list of .conf files in the directory
    QStringList confFiles = directory.entryList(QStringList() << "*.conf", QDir::Files);

    // Clear the existing layout to remove any old configuration buttons
    clearLayout(layout);

    // Loop through each configuration to create a new button for each configuration file
    for (const QString &fileName : confFiles) {
        QPushButton *button = new QPushButton(fileName, this);
        button->setFixedSize(200, 50);
        button->setProperty("isConfButton", true);

        // Connect the button's clicked signal to a lambda function to handle the click event
        connect(button, &QPushButton::clicked, this, [this, fileName]() {
            onConfButtonClicked(fileName);
        });

        // Add the button to the layout
        layout->addWidget(button);
    }

    applyStyles(false);
}

void SaveSection::changeDirectory() {
    // Open a file dialog to select a new directory
    QString newDirectory = QFileDialog::getExistingDirectory(this, tr("Select Directory"), directoryPath);
    // If a new directory is selected, update the directory path and refresh the configurations
    if (!newDirectory.isEmpty()) {
        directoryPath = newDirectory;
        createConfigurations();
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
    // Copy the configuration file to the application's directory
    copyConfiguration(confFilePath);
    QMessageBox::information(this, tr("Success"), tr("GCODE GENERATED."));
}

void SaveSection::copyConfiguration(const QString &confFilePath) {
    // Open the configuration file for reading
    QFile confFile(confFilePath);
    if (!confFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    // Define the path to the target file where the configuration will be copied
    QString sectionValuesFilePath = QCoreApplication::applicationDirPath() + "/section_values.txt";
    // Open the target file for writing
    QFile sectionValuesFile(sectionValuesFilePath);
    if (!sectionValuesFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    // Create text streams for reading and writing
    QTextStream confStream(&confFile);
    QTextStream sectionValuesStream(&sectionValuesFile);

    // Start a Python process to handle GCODE generation
    QProcess *process = new QProcess(this);
    QString currentPath = QDir::currentPath();
    QString pythonScriptPath = currentPath + "/axolotl_1mat.py";
    process->start("py", QStringList() << pythonScriptPath);
    if (process->waitForStarted()) {
        qDebug() << "Python script started successfully.";
    } else {
        qDebug() << "Failed to start Python script.";
    }

    // Copy the contents of the configuration file to the target file
    sectionValuesStream << confStream.readAll();
    sectionValuesStream << "\n\n";

    // Close both files
    sectionValuesFile.close();
    confFile.close();

    // Read Section values from the file and emit the signal
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
