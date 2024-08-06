#include "mainwindow.h"
#include "startpage.h"
#include "SaveSection.h"
#include "DimensionSection.h"
#include "ShapeSection.h"
#include "infillSection.h"
#include "materialSection.h"
#include "GenerateSection.h"
#include <QFile>
#include <QAction>
#include <QMenuBar>
#include <QApplication>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), darkModeEnabled(false) {
    // Create a central widget for the main window
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Set size for all app
    setFixedSize(700, 600);
    darkModeEnabled = true;
    // Create the main horizontal layout
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Create layout for sections and buttons
    QVBoxLayout *sectionsLayout = new QVBoxLayout();

    // Start section
    SectionStart *sectionStartWidget = new SectionStart(this);
    connect(sectionStartWidget, &SectionStart::startButtonClicked, this, [this]() {
        nextSection();  // Move to the next section
    });
    connect(sectionStartWidget, &SectionStart::languageChanged, this, &MainWindow::changeLanguage);

    // Section 0: Save configuration
    SaveSection *SaveSectionWidget = new SaveSection(this);

    // Section 1: Dimension configuration
    DimensionSection *DimensionSectionWidget = new DimensionSection(this);

    // Section 2: Shape configuration
    ShapeSection *ShapeSectionWidget = new ShapeSection(this);

    // Section 3: Infill configuration
    InfillSection *infillSectionWidget = new InfillSection(this);

    // Section 4: Material configuration
    MaterialSection *materialSectionWidget = new MaterialSection(this);

    // Section 5: Generate GCODE
    GenerateSection *GenerateSectionWidget = new GenerateSection(this);

    connect(SaveSectionWidget, &SaveSection::section1ValuesRead, DimensionSectionWidget, &DimensionSection::updateValues);
    connect(SaveSectionWidget, &SaveSection::section2ValuesRead, ShapeSectionWidget, &ShapeSection::updateValues);
    connect(SaveSectionWidget, &SaveSection::section3ValuesRead, infillSectionWidget, &InfillSection::updateValues);
    connect(SaveSectionWidget, &SaveSection::section4ValuesRead, materialSectionWidget, &MaterialSection::updateMaterialSection);



    // Create QStackedWidget to manage the sections
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(sectionStartWidget);
    stackedWidget->addWidget(SaveSectionWidget);
    stackedWidget->addWidget(DimensionSectionWidget);
    stackedWidget->addWidget(ShapeSectionWidget);
    stackedWidget->addWidget(infillSectionWidget);
    stackedWidget->addWidget(materialSectionWidget);
    stackedWidget->addWidget(GenerateSectionWidget);

    sectionsLayout->addWidget(stackedWidget);

    // Add sectionsLayout to the main layout
    mainLayout->addLayout(sectionsLayout);

    // START BUTTONS
    // Create horizontal layout for buttons
    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    // Spacer to align buttons
    buttonsLayout->addItem(new QSpacerItem(9, 0, QSizePolicy::Minimum));
    // Create button to go to the previous section
    prevButton = new QPushButton(tr("Previous"), this);
    connect(prevButton, &QPushButton::clicked, this, &MainWindow::previousSection);
    buttonsLayout->addWidget(prevButton);

    // Create button to go to the next section
    nextButton = new QPushButton(tr("Next"), this);
    connect(nextButton, &QPushButton::clicked, this, [this, DimensionSectionWidget, ShapeSectionWidget, infillSectionWidget, materialSectionWidget]() {
        nextSection();
        // Update 3D visualicer on next-section action
        RealTimeGCODE(DimensionSectionWidget, ShapeSectionWidget, infillSectionWidget, materialSectionWidget);
    });
    buttonsLayout->addWidget(nextButton);

    // Create connection to generate GCODE
    connect(GenerateSectionWidget, &GenerateSection::generateGcode, this, [this, DimensionSectionWidget, ShapeSectionWidget, infillSectionWidget, materialSectionWidget]() {
        saveConfigurationToFile(DimensionSectionWidget, ShapeSectionWidget, infillSectionWidget, materialSectionWidget);
    });


    // Create button to toggle dark mode
    toggleDarkModeButton = new QPushButton(tr("Light Mode"), this);
    connect(toggleDarkModeButton, &QPushButton::clicked, this, &MainWindow::toggleDarkMode);
    buttonsLayout->addWidget(toggleDarkModeButton);

    // Create cancel button
    cancelButton = new QPushButton(tr("Cancel"), this);
    connect(cancelButton, &QPushButton::clicked, this, &MainWindow::cancelConfirmation);
    buttonsLayout->addWidget(cancelButton);

    // Spacer to align buttons
    buttonsLayout->addItem(new QSpacerItem(9, 0, QSizePolicy::Minimum));

    // Set fixed size for buttons
    prevButton->setFixedSize(80, 30);
    nextButton->setFixedSize(80, 30);
    cancelButton->setFixedSize(80, 30);
    toggleDarkModeButton->setFixedSize(80, 30);
    buttonsLayout->setAlignment(cancelButton, Qt::AlignRight);

    sectionsLayout->addLayout(buttonsLayout);
    // END BUTTONS

    // Create a container for the OpenGL widget with rounded corners
    QWidget *openGLContainer = new QWidget(this);
    QVBoxLayout *openGLLayout = new QVBoxLayout(openGLContainer);
    openGLWidget = new OpenGLWidget(this);

    // Set size for visualicer
    openGLWidget->setMinimumSize(600, 580);
    openGLLayout->addWidget(openGLWidget);

    mainLayout->addWidget(openGLContainer);

    // Connect signals of sections to RealTimeGCODE (all sections send signals to mainwindow every time any value changes on them)
    connect(DimensionSectionWidget, &DimensionSection::valueChanged, this, [this, DimensionSectionWidget, ShapeSectionWidget, infillSectionWidget, materialSectionWidget]() {
        RealTimeGCODE(DimensionSectionWidget, ShapeSectionWidget, infillSectionWidget, materialSectionWidget);
    });

    connect(ShapeSectionWidget, &ShapeSection::valueChanged, this, [this, DimensionSectionWidget, ShapeSectionWidget, infillSectionWidget, materialSectionWidget]() {
        RealTimeGCODE(DimensionSectionWidget, ShapeSectionWidget, infillSectionWidget, materialSectionWidget);
    });

    connect(infillSectionWidget, &InfillSection::valueChanged, this, [this, DimensionSectionWidget, ShapeSectionWidget, infillSectionWidget, materialSectionWidget]() {
        RealTimeGCODE(DimensionSectionWidget, ShapeSectionWidget, infillSectionWidget, materialSectionWidget);
    });

    // Initially hide the buttons and OpenGL widget
    nextButton->setVisible(false);
    prevButton->setVisible(false);
    cancelButton->setVisible(false);
    openGLWidget->setVisible(false);

    // Apply initial styles
    applyStyles(darkModeEnabled);
}

void MainWindow::changeLanguage(const QString &languageCode) {
    QString qmPath = QCoreApplication::applicationDirPath() + "/translations/myapp_" + languageCode + ".qm";

    if (translator.load(qmPath)) {
        qApp->installTranslator(&translator);
        retranslateUi();
    } else {
        qDebug() << "Failed to load translation file: " << qmPath;
    }
}

void MainWindow::retranslateUi() {
    // Reaplicar las traducciones a todos los widgets visibles
    for (int i = 0; i < stackedWidget->count(); ++i) {
        QWidget *widget = stackedWidget->widget(i);
        retranslateSection(widget);
    }

    // Actualizar el texto de los botones
    prevButton->setText(tr("Previous"));
    nextButton->setText(tr("Next"));
    cancelButton->setText(tr("Cancel"));
    toggleDarkModeButton->setText(tr("Light Mode"));

    // Actualizar el título de la ventana principal
    setWindowTitle(tr("My Application"));
}

void MainWindow::retranslateSection(QWidget* section) {
    if (auto* startPage = qobject_cast<SectionStart*>(section)) {
        startPage->retranslateUi();
    } else if (auto* saveSection = qobject_cast<SaveSection*>(section)) {
        saveSection->retranslateUi();
    } else if (auto* dimensionSection = qobject_cast<DimensionSection*>(section)) {
        dimensionSection->retranslateUi();
    } else if (auto* shapeSection = qobject_cast<ShapeSection*>(section)) {
        shapeSection->retranslateUi();
    } else if (auto* infillSection = qobject_cast<InfillSection*>(section)) {
        infillSection->retranslateUi();
    } else if (auto* materialSection = qobject_cast<MaterialSection*>(section)) {
        materialSection->retranslateUi();
    } else if (auto* generateSection = qobject_cast<GenerateSection*>(section)) {
        generateSection->retranslateUi();
    }
}

// Apply styles to the widgets based on the mode
void MainWindow::applyStyles(bool darkMode) {
    if (darkMode) {
        applyDarkMode();
    } else {
        applyLightMode();
    }

    // Apply styles to sections
    for (int i = 0; i < stackedWidget->count(); ++i) {
        QWidget *widget = stackedWidget->widget(i);
        if (auto* startPage = qobject_cast<SectionStart*>(widget)) {
            startPage->applyStyles(darkMode);
        } else if (auto* saveSection = qobject_cast<SaveSection*>(widget)) {
            saveSection->applyStyles(darkMode);
        } else if (auto* dimensionSection = qobject_cast<DimensionSection*>(widget)) {
            dimensionSection->applyStyles(darkMode);
        } else if (auto* shapeSection = qobject_cast<ShapeSection*>(widget)) {
            shapeSection->applyStyles(darkMode);
        } else if (auto* infillSection = qobject_cast<InfillSection*>(widget)) {
            infillSection->applyStyles(darkMode);
        } else if (auto* materialSection = qobject_cast<MaterialSection*>(widget)) {
            materialSection->applyStyles(darkMode);
        } else if (auto* generateSection = qobject_cast<GenerateSection*>(widget)) {
            generateSection->applyStyles(darkMode);
        }
    }
}

// Apply light mode styles
void MainWindow::applyLightMode() {
    setStyleSheet("MainWindow { background-color: #f0f0f0; }");

    QString navButtonStyle = "QPushButton {}"
                             "QPushButton:hover { background-color: #e6ffe6; border: 1px solid #4CAF50;border-radius: 5px; }";
    prevButton->setStyleSheet(navButtonStyle);
    nextButton->setStyleSheet(navButtonStyle);
    cancelButton->setStyleSheet(navButtonStyle);
    toggleDarkModeButton->setStyleSheet(navButtonStyle);
}

// Apply dark mode styles
void MainWindow::applyDarkMode() {
    setStyleSheet("MainWindow { background-color: #1a1a1a; }");

    QString navButtonStyle = "QPushButton { background-color: #2a2a2a; color: white;"
                             "border-radius: 5px; }"
                             "QPushButton:hover { background-color: #001900; border: 1px solid #4CAF50; }";
    prevButton->setStyleSheet(navButtonStyle);
    nextButton->setStyleSheet(navButtonStyle);
    cancelButton->setStyleSheet(navButtonStyle);
    toggleDarkModeButton->setStyleSheet(navButtonStyle);
}

// Toggle dark mode
void MainWindow::toggleDarkMode() {
    darkModeEnabled = !darkModeEnabled;
    applyStyles(darkModeEnabled);
    toggleDarkModeButton->setText(darkModeEnabled ? tr("Light Mode") : tr("Dark Mode"));
}

// Configure the sections based on the index
void MainWindow::sectionConfiguration(int sectionIndex) {
    // Adjust window size and visibility of elements based on the current section
    switch (sectionIndex) {
    case 1:
        setFixedSize(1200, 600);
        nextButton->setVisible(true);
        prevButton->setVisible(true);
        cancelButton->setVisible(true);
        openGLWidget->setVisible(true);
        toggleDarkModeButton->setVisible(false);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    default:
        break;
    }

}



// Display cancel confirmation dialog
void MainWindow::cancelConfirmation() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Cancel Confirmation", "Are you sure you want to cancel?",
                                  QMessageBox::Yes|QMessageBox::No);

    // Close the application if user confirms
    if (reply == QMessageBox::Yes) {
        close();
    }
}

// Show the previous section
void MainWindow::previousSection() {
    int prevIndex = stackedWidget->currentIndex() - 1;
    if (prevIndex >= 1) {
        sectionConfiguration(prevIndex);
        stackedWidget->setCurrentIndex(prevIndex);
    }
}

// Show the next section
void MainWindow::nextSection() {
    int nextIndex = stackedWidget->currentIndex() + 1;
    if (nextIndex < stackedWidget->count()) {
        sectionConfiguration(nextIndex);
        stackedWidget->setCurrentIndex(nextIndex);
    }
}

// Execute the Python script
void MainWindow::executePython() {
    QProcess *process = new QProcess(this);
    QString currentPath = QDir::currentPath();
    QString pythonScriptPath = currentPath + "/axolotl_1mat.py";
    process->start("py", QStringList() << pythonScriptPath);
    if (process->waitForStarted()) {
        qDebug() << "Python script started successfully.";
    } else {
        qDebug() << "Failed to start Python script.";
    }
}

// Save the configuration to a file
void MainWindow::saveConfigurationToFile(DimensionSection *section1Widget, ShapeSection *section2Widget, InfillSection *section3Widget, MaterialSection *section4Widget) {
    if (writeConfigurationToFile("section_values.txt", section1Widget, section2Widget, section3Widget, section4Widget)) {
        QMessageBox::information(this, "Configuration Saved", "Configuration saved to section_values.txt.");
    } else {
        QMessageBox::critical(this, "Error", "Failed to save configuration to section_values.txt.");
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Save Configuration", "Do you want to save this configuration?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QString fileName = QFileDialog::getSaveFileName(this, "Save Configuration", QDir::currentPath(), "Configuration Files (*.conf)");
        if (!fileName.isEmpty()) {
            if (writeConfigurationToFile(fileName, section1Widget, section2Widget, section3Widget, section4Widget)) {
                QMessageBox::information(this, "Configuration Saved", "Configuration saved successfully.");
            } else {
                QMessageBox::critical(this, "Error", "Failed to save configuration to file.");
            }
        }
    }

    executePython();
}

// Write the configuration to a file
bool MainWindow::writeConfigurationToFile(const QString &fileName, DimensionSection *section1Widget, ShapeSection *section2Widget, InfillSection *section3Widget, MaterialSection *section4Widget) {
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);

        // Section1 values
        out << "Section1 Values:\n";
        out << "Plate X Value: " << section1Widget->getPlateXSpinBox() << "\n";
        out << "Plate Y Value: " << section1Widget->getPlateYSpinBox() << "\n";
        out << "Center X Value: " << section1Widget->getcenterXSpinBox() << "\n";
        out << "Center Y Value: " << section1Widget->getcenterYSpinBox() << "\n\n";

        // Section2 values
        out << "Section2 Values:\n";
        out << "Size 1: " << section2Widget->getSize1() << "\n";
        out << "Size 2: " << section2Widget->getSize2() << "\n";
        out << "Shape Index 1: " << section2Widget->getShapeIndex1() << "\n";
        out << "Shape Index 2: " << section2Widget->getShapeIndex2() << "\n\n";

        // Section3 values
        out << "Section3 Values:\n";
        out << "Infill Value: " << section3Widget->getInfillValue() << "\n";
        out << "Shape Index: " << section3Widget->getShapeIndex() << "\n";
        out << "Strand Distance Value: " << section3Widget->getStrandDistanceValue() << "\n\n";

        // Section4 values
        out << "Section4 Values:\n";
        QList<MaterialConfig> materialConfigs = section4Widget->getMaterialConfigs();
        for (const auto& materialConfig : materialConfigs) {
            out << "Material Name: " << materialConfig.name << "\n";
            out << "Filament: " << materialConfig.filament << "\n";
            out << "Nozzle: " << materialConfig.nozzle << "\n" << "\n";
        }

        file.close();
        return true;
    } else {
        return false;
    }
}

// Update GCODE in real-time
void MainWindow::RealTimeGCODE(DimensionSection *DimensionSectionWidget, ShapeSection *ShapeSectionWidget, InfillSection *InfillSectionWidget, MaterialSection *MaterialSectionWidget) {
    writeConfigurationToFile("section_values.txt", DimensionSectionWidget, ShapeSectionWidget, InfillSectionWidget, MaterialSectionWidget);
    executePython();
}

