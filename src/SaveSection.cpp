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

    //LABEL
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

// Creates and refreshes the list of configuration files in the current directory
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

// Changes the directory to a new path selected by the user
void SaveSection::changeDirectory() {
    // Open a file dialog to select a new directory
    QString newDirectory = QFileDialog::getExistingDirectory(this, tr("Select Directory"), directoryPath);
    // If a new directory is selected, update the directory path and refresh the configurations
    if (!newDirectory.isEmpty()) {
        directoryPath = newDirectory;
        createConfigurations();
    }
}

// Clears the provided layout by removing all its child widgets
void SaveSection::clearLayout(QLayout *layout) {
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            delete child->widget();
        }
        delete child;
    }
}
// Mesagge called when a configuration button is clicked
void SaveSection::onConfButtonClicked(const QString &fileName) {
    QString confFilePath = directoryPath + "/" + fileName;
    // Copy the configuration file to the application's directory
    copyConfiguration(confFilePath);
    QMessageBox::information(this, tr("Success"), tr("GCODE GENERATED."));
}

// Copies the selected configuration file "section_values.txt"
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
}
