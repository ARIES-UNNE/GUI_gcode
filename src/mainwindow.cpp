#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "section1.h"
#include "section2.h"
#include "section3.h"
#include "section4.h"
#include <QDir>
#include <QDebug>
#include "section5.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Crear un widget central 
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Configurar la ventana principal como no redimensionable
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedSize(300, 300);

    // Ocultar la esquina de redimensionamiento
    QSizeGrip *sizeGrip = findChild<QSizeGrip*>();
    if (sizeGrip)
        sizeGrip->setVisible(false);

    // Crear un diseño de formulario para organizar los widgets
    QFormLayout *formLayout = new QFormLayout(centralWidget);

    // Sección 1: Tamaño de la placa
    Section1 *section1Widget= new Section1(this);

    // Sección 2: Tamaño de la placa
    Section2 *section2Widget= new Section2(this);

    // Sección 3: Tamaño de la placa
    Section3 *section3Widget= new Section3(this);

    // Sección 4: Tamaño de la placa
    Section4 *section4Widget= new Section4(this);

    // Sección 5: Tamaño de la placa
    Section5 *section5Widget= new Section5(this);

    
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(section1Widget);
    stackedWidget->addWidget(section2Widget);
    stackedWidget->addWidget(section3Widget);
    stackedWidget->addWidget(section4Widget);
    stackedWidget->addWidget(section5Widget);


    formLayout->addWidget(stackedWidget);

    // Crear  el layout horizontal para los botones
    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    // Crear  el botón para pasar a la siguiente sección
    QPushButton *nextButton = new QPushButton("Next", this);
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::nextSection);
    buttonsLayout->addWidget(nextButton);

    QPushButton *prevButton = new QPushButton("Previous", this);
    connect(prevButton, &QPushButton::clicked, this, &MainWindow::previousSection);
    buttonsLayout->addWidget(prevButton);

    // Crear el botón para mostrar los valores de las Secciones
    showValuesButton = new QPushButton("Show Section Values", this);
    connect(showValuesButton, &QPushButton::clicked, this, [section1Widget, section2Widget, section3Widget, section4Widget, this]() {
        showSectionValues(section1Widget, section2Widget,section3Widget, section4Widget);
    });
    showValuesButton->setVisible(false);


    formLayout->addWidget(showValuesButton);

    // Crear y configurar el botón para cancelar
    QPushButton *cancelButton = new QPushButton("Cancel", this);
    connect(cancelButton, &QPushButton::clicked, this, &MainWindow::cancelConfirmation);
    buttonsLayout->addWidget(cancelButton);

    // Establecer el tamaño fijo de los botones
    prevButton->setFixedSize(80, 30);
    nextButton->setFixedSize(80, 30);
    cancelButton->setFixedSize(80, 30);
    buttonsLayout->setAlignment(cancelButton, Qt::AlignRight);



    formLayout->addRow(buttonsLayout);

    // Aplicar estilos
    applyStyles();
}


// work in progress
void MainWindow::applyStyles() {
    // Estilo general de la ventana
    setStyleSheet("MainWindow { background-color: #f0f0f0; }");

    // Estilo de los botones
    QString buttonStyle = "QPushButton { background-color: #4CAF50; color: white; border: none;"
                          "border-radius: 5px; padding: 10px 24px; font-size: 14px; }"
                          "QPushButton:hover { background-color: #45a049; }";
    showValuesButton->setStyleSheet(buttonStyle);
}


void MainWindow::adjustSectionSize(int sectionIndex) {
    // Ajustar el tamaño de la ventana según la sección actual
    switch (sectionIndex) {
    case 0:
        setFixedSize(300, 300);
        showValuesButton->setVisible(false);
        break;
    case 1:
        setFixedSize(300, 300);
        showValuesButton->setVisible(false);
        break;
    case 2:
        setFixedSize(300, 300);
        showValuesButton->setVisible(false);
        break;
    case 3:
        setFixedSize(430, 500);
        showValuesButton->setVisible(false);
        break;
    case 4:
        setFixedSize(300, 300);
        showValuesButton->setVisible(true);
        break;
    default:
        break;
    }
}

void MainWindow::cancelConfirmation() {
    // Mostrar una ventana de confirmación
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Cancel Confirmation", "Are you sure you want to cancel?",
                                  QMessageBox::Yes|QMessageBox::No);

    // Si el usuario elige "Yes", cerrar la aplicación
    if (reply == QMessageBox::Yes) {
        close();
    }
}

void MainWindow::previousSection() {
    // Mostrar la sección anterior
    int prevIndex = stackedWidget->currentIndex() - 1;
    if (prevIndex >= 0) {
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

void MainWindow::showSectionValues(Section1 *section1Widget, Section2 *section2Widget, Section3 *section3Widget, Section4 *section4Widget) {

    QFile file("section_values.txt");

    // Abrir el archivo en modo de escritura
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        qDebug() << QDir::currentPath();
        // Guardar los valores de la Section1 en el archivo
        out << "Section1 Values:\n";
        out << "Plate X Value: " << section1Widget->getPlateXSpinBox() << "\n";
        out << "Plate Y Value: " << section1Widget->getPlateYSpinBox() << "\n";
        out << "Center X Value: " << section1Widget->getcenterXSpinBox() << "\n";
        out << "Center Y Value: " << section1Widget->getcenterYSpinBox() << "\n\n";

        // Guardar los valores de la Section2 en el archivo
        out << "Section2 Values:\n";
        out << "Size 1: " << section2Widget->getSize1() << "\n";
        out << "Size 2: " << section2Widget->getSize2() << "\n";
        out << "Shape Index 1: " << section2Widget->getShapeIndex1() << "\n";
        out << "Shape Index 2: " << section2Widget->getShapeIndex2() << "\n\n";

        // Guardar los valores de la Section3 en el archivo
        out << "Section3 Values:\n";
        out << "Infill Value: " << section3Widget->getInfillValue() << "\n";
        out << "Shape Index: " << section3Widget->getShapeIndex() << "\n";
        out << "Strand Distance Value: " << section3Widget->getStrandDistanceValue() << "\n\n";

        // Guardar los valores de la Section4 en el archivo
        out << "Section4 Values:\n";
        QList<MaterialConfig> materialConfigs = section4Widget->getMaterialConfigs();
        for (const auto& materialConfig : materialConfigs) {
            out << "Material Name: " << materialConfig.name << "\n";
            out << "Nozzle Size: " << materialConfig.nozzleSize << "\n";
            out << "Filament Amount: " << materialConfig.filamentAmount << "\n\n";
        }

        file.close();

        QMessageBox::information(this, "File Saved", "Section values saved to section_values.txt");
    } else {
        
        QMessageBox::critical(this, "Error", "Failed to open section_values.txt for writing");
    }
}






