#include "mainwindow.h"
#include "startpage.h"
#include "section0.h"
#include "section1.h"
#include "section2.h"
#include "section3.h"
#include "section4.h"
#include "section5.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Crear un widget central para la ventana principal
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    setFixedSize(600, 600);

    // Crear un layout principal horizontal
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Sección de inicio
    SectionStart *sectionStartWidget = new SectionStart(this);
    connect(sectionStartWidget, &SectionStart::startButtonClicked, this, [this]() {
        nextSection();  // Para avanzar a la siguiente sección
    });

    // Crear un layout para las secciones y los botones
    QVBoxLayout *sectionsLayout = new QVBoxLayout();

    // Sección 0: Tamaño de la placa
    Section0 *section0Widget = new Section0(this);

    // Sección 1: Tamaño de la placa
    Section1 *section1Widget = new Section1(this);

    // Sección 2: Tamaño de la placa
    Section2 *section2Widget = new Section2(this);

    // Sección 3: Tamaño de la placa
    Section3 *section3Widget = new Section3(this);

    // Sección 4: Tamaño de la placa
    Section4 *section4Widget = new Section4(this);

    // Sección 5: Tamaño de la placa
    Section5 *section5Widget = new Section5(this);

    // Crear QStackedWidget para manejar las secciones
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(sectionStartWidget);
    stackedWidget->addWidget(section0Widget);
    stackedWidget->addWidget(section1Widget);
    stackedWidget->addWidget(section2Widget);
    stackedWidget->addWidget(section3Widget);
    stackedWidget->addWidget(section4Widget);
    stackedWidget->addWidget(section5Widget);

    sectionsLayout->addWidget(stackedWidget);

    // Crear el layout horizontal para los botones
    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    // Crear el botón para pasar a la siguiente sección
    nextButton = new QPushButton("Next", this);
    connect(nextButton, &QPushButton::clicked, this, [this, section1Widget, section2Widget, section3Widget, section4Widget]() {
        nextSection();
        RealTimeGCODE(section1Widget, section2Widget, section3Widget, section4Widget);
    });
    buttonsLayout->addWidget(nextButton);

    // Crear el botón para pasar a la sección anterior
    prevButton = new QPushButton("Previous", this);
    connect(prevButton, &QPushButton::clicked, this, &MainWindow::previousSection);
    buttonsLayout->addWidget(prevButton);

    // Crear el botón para mostrar los valores de las Secciones
    showValuesButton = new QPushButton("GENERATE GCODE", this);
    showValuesButton->setFixedSize(200, 50);
    connect(showValuesButton, &QPushButton::clicked, this, [section1Widget, section2Widget, section3Widget, section4Widget, this]() {
        showSectionValues(section1Widget, section2Widget, section3Widget, section4Widget);
    });

    // Layout para centrar el botón
    centeredButtonLayout = new QVBoxLayout();
    centeredButtonLayout->addStretch(); // Espaciador superior
    QHBoxLayout *hCenterLayout = new QHBoxLayout();
    hCenterLayout->addStretch();
    hCenterLayout->addWidget(showValuesButton);
    hCenterLayout->addStretch();
    centeredButtonLayout->addLayout(hCenterLayout);
    centeredButtonLayout->addStretch(); // Espaciador inferior
    sectionsLayout->addLayout(centeredButtonLayout);

    // Crear el botón para cancelar
    cancelButton = new QPushButton("Cancel", this);
    connect(cancelButton, &QPushButton::clicked, this, &MainWindow::cancelConfirmation);
    buttonsLayout->addWidget(cancelButton);

    // Tamaño fijo botones
    prevButton->setFixedSize(80, 30);
    nextButton->setFixedSize(80, 30);
    cancelButton->setFixedSize(80, 30);
    buttonsLayout->setAlignment(cancelButton, Qt::AlignRight);

    sectionsLayout->addLayout(buttonsLayout);

    // Añadir sectionsLayout al layout principal
    mainLayout->addLayout(sectionsLayout);

    // Crear un contenedor para el widget OpenGL con bordes redondeados
    QWidget *openGLContainer = new QWidget(this);
    QVBoxLayout *openGLLayout = new QVBoxLayout(openGLContainer);
    openGLLayout->setContentsMargins(0, 0, 0, 0); // Eliminar márgenes del layout
    openGLWidget = new OpenGLWidget(this);
    openGLWidget->setMinimumSize(590, 400);
    openGLLayout->addWidget(openGLWidget);
    openGLContainer->setStyleSheet(
        "QWidget#openGLContainer {"
        "    border: 1px solid #ccc;"
        "    border-radius: 15px;"
        "    background-color: #ffffff;"
        "}"
        );
    openGLContainer->setObjectName("openGLContainer");

    mainLayout->addWidget(openGLContainer);
    // Conectar signals de las secciones a RealTimeGCODE
    connect(section1Widget, &Section1::valueChanged, this, [this, section1Widget, section2Widget, section3Widget, section4Widget]() {
        RealTimeGCODE(section1Widget, section2Widget, section3Widget, section4Widget);
    });

    connect(section2Widget, &Section2::valueChanged, this, [this, section1Widget, section2Widget, section3Widget, section4Widget]() {
        RealTimeGCODE(section1Widget, section2Widget, section3Widget, section4Widget);
    });

    connect(section3Widget, &Section3::valueChanged, this, [this, section1Widget, section2Widget, section3Widget, section4Widget]() {
        RealTimeGCODE(section1Widget, section2Widget, section3Widget, section4Widget);
    });

    // False para inicio
    showValuesButton->setVisible(false);
    nextButton->setVisible(false);
    prevButton->setVisible(false);
    showValuesButton->setVisible(false);
    cancelButton->setVisible(false);
    openGLWidget->setVisible(false);

    // Estilo
    applyStyles();
}

void MainWindow::applyStyles() {
    setStyleSheet("MainWindow { background-color: #f0f0f0; }");

    QString buttonStyle = "QPushButton { background-color: #4CAF50; color: white; border: none;"
                          "border-radius: 5px; padding: 10px 24px; font-size: 14px; }"
                          "QPushButton:hover { background-color: #45a049; }";
    showValuesButton->setStyleSheet(buttonStyle);
}


void MainWindow::adjustSectionSize(int sectionIndex) {
    // Ajustar el tamaño de la ventana según la sección actual
    switch (sectionIndex) {
    case 1:
        setFixedSize(1200, 600);
        showValuesButton->setVisible(false);
        nextButton->setVisible(true);
        prevButton->setVisible(true);
        cancelButton->setVisible(true);
        openGLWidget->setVisible(true);
        break;
    case 2:
        setFixedSize(1200, 600);
        showValuesButton->setVisible(false);
        break;
    case 3:
        setFixedSize(1200, 600);
        showValuesButton->setVisible(false);
        break;
    case 4:
        setFixedSize(1200, 600);
        showValuesButton->setVisible(false);
        break;
    case 5:
        setFixedSize(1200, 600);
        showValuesButton->setVisible(false);
        break;
    case 6:
        setFixedSize(1200, 600);
        showValuesButton->setVisible(true);
        // Añadir un espaciador al final para la sección 6
        if (centeredButtonLayout->itemAt(centeredButtonLayout->count() - 1) == nullptr) {
            centeredButtonLayout->addStretch();
        }
        centeredButtonLayout->addSpacing(150); // Espaciador
        break;
    default:
        break;
    }

    // Eliminar el espaciador adicional en las otras secciones
    if (sectionIndex != 6) {
        QLayoutItem* item = centeredButtonLayout->itemAt(centeredButtonLayout->count() - 1);
        if (item != nullptr && item->spacerItem() != nullptr) {
            centeredButtonLayout->removeItem(item);
            delete item;
        }
    }
}


void MainWindow::cancelConfirmation() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Cancel Confirmation", "Are you sure you want to cancel?",
                                  QMessageBox::Yes|QMessageBox::No);

    // Si el usuario elige Yes cerrar la aplicación
    if (reply == QMessageBox::Yes) {
        close();
    }
}

void MainWindow::previousSection() {
    // Mostrar la sección anterior
    int prevIndex = stackedWidget->currentIndex() - 1;
    if (prevIndex >= 1) {
        adjustSectionSize(prevIndex);
        stackedWidget->setCurrentIndex(prevIndex);
    }
}

void MainWindow::nextSection() {
    // Mostrar la siguiente sección
    int nextIndex = stackedWidget->currentIndex() + 1;
    if (nextIndex < stackedWidget->count()) {
        adjustSectionSize(nextIndex);
        stackedWidget->setCurrentIndex(nextIndex);
    }
}



void MainWindow::executePython() {
    // Ejecutar archivo python
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



void MainWindow::showSectionValues(Section1 *section1Widget, Section2 *section2Widget, Section3 *section3Widget, Section4 *section4Widget) {
    // Crear el archivo section_values.txt
    if (saveConfigurationToFile("section_values.txt", section1Widget, section2Widget, section3Widget, section4Widget)) {
        QMessageBox::information(this, "Configuration Saved", "Configuration saved to section_values.txt.");
    } else {
        QMessageBox::critical(this, "Error", "Failed to save configuration to section_values.txt.");
    }

    // Mostrar una ventana de confirmación para guardar la configuración
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Save Configuration", "Do you want to save this configuration?",
                                  QMessageBox::Yes|QMessageBox::No);


    if (reply == QMessageBox::Yes) {
        QString fileName = QFileDialog::getSaveFileName(this, "Save Configuration", QDir::currentPath(), "Configuration Files (*.conf)");
        if (!fileName.isEmpty()) {
            // Guardar la configuración
            if (saveConfigurationToFile(fileName, section1Widget, section2Widget, section3Widget, section4Widget)) {
                QMessageBox::information(this, "Configuration Saved", "Configuration saved successfully.");
            } else {
                QMessageBox::critical(this, "Error", "Failed to save configuration to file.");
            }
        }
    }

    executePython();

}


bool MainWindow::saveConfigurationToFile(const QString &fileName, Section1 *section1Widget, Section2 *section2Widget, Section3 *section3Widget, Section4 *section4Widget) {
    // Escribir archivo de texto
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);

        // valores de la Section1
        out << "Section1 Values:\n";
        out << "Plate X Value: " << section1Widget->getPlateXSpinBox() << "\n";
        out << "Plate Y Value: " << section1Widget->getPlateYSpinBox() << "\n";
        out << "Center X Value: " << section1Widget->getcenterXSpinBox() << "\n";
        out << "Center Y Value: " << section1Widget->getcenterYSpinBox() << "\n\n";

        // valores de la Section2
        out << "Section2 Values:\n";
        out << "Size 1: " << section2Widget->getSize1() << "\n";
        out << "Size 2: " << section2Widget->getSize2() << "\n";
        out << "Shape Index 1: " << section2Widget->getShapeIndex1() << "\n";
        out << "Shape Index 2: " << section2Widget->getShapeIndex2() << "\n\n";

        //  valores de la Section3
        out << "Section3 Values:\n";
        out << "Infill Value: " << section3Widget->getInfillValue() << "\n";
        out << "Shape Index: " << section3Widget->getShapeIndex() << "\n";
        out << "Strand Distance Value: " << section3Widget->getStrandDistanceValue() << "\n\n";

        // valores de la Section4
        out << "Section4 Values:\n";
        QList<MaterialConfig> materialConfigs = section4Widget->getMaterialConfigs();
        for (const auto& materialConfig : materialConfigs) {
            out << "Material Name: " << materialConfig.name << "\n";
            out << "Nozzle Size: " << materialConfig.nozzleSize << "\n";
            out << "Filament Amount: " << materialConfig.filamentAmount << "\n";
            out << "Amount: " << materialConfig.algo << "\n\n";
        }


        file.close();
        return true;
    } else {
        return false;
    }
}

void MainWindow::RealTimeGCODE(Section1 *section1Widget, Section2 *section2Widget, Section3 *section3Widget, Section4 *section4Widget) {
    saveConfigurationToFile("section_values.txt", section1Widget, section2Widget, section3Widget, section4Widget);

    executePython();
}
