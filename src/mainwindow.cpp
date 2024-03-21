#include "mainwindow.h"
#include "section1.h"
#include "section2.h"
#include "section3.h"
#include "section4.h"
#include "section5.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Crear un widget central para la ventana principal
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Configurar la ventana principal como no redimensionable
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedSize(300, 300);  // Establecer el tamaño fijo

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

    // Crear QStackedWidget para manejar las secciones
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(section1Widget);
    stackedWidget->addWidget(section2Widget);
    stackedWidget->addWidget(section3Widget);
    stackedWidget->addWidget(section4Widget);
    stackedWidget->addWidget(section5Widget);

    // Añadir QStackedWidget al formulario
    formLayout->addWidget(stackedWidget);

    // Crear y configurar el layout horizontal para los botones
    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    // Crear y configurar el botón para pasar a la siguiente sección
    QPushButton *nextButton = new QPushButton("Next", this);
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::nextSection);
    buttonsLayout->addWidget(nextButton);

    QPushButton *prevButton = new QPushButton("Previous", this);
    connect(prevButton, &QPushButton::clicked, this, &MainWindow::previousSection);
    buttonsLayout->addWidget(prevButton);

    // Crear y configurar el botón para mostrar los valores de las Secciones
    showValuesButton = new QPushButton("Show Section Values", this);
    connect(showValuesButton, &QPushButton::clicked, this, [section1Widget, section2Widget, section3Widget, section4Widget, this]() {
        showSectionValues(section1Widget, section2Widget,section3Widget, section4Widget);
    });
    showValuesButton->setVisible(false);

    // Añadir el botón al formulario
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


    // Añadir el layout de botones al formulario
    formLayout->addRow(buttonsLayout);
}


void MainWindow::adjustSectionSize(int sectionIndex) {
    // Ajustar el tamaño de la ventana según la sección actual
    switch (sectionIndex) {
    case 0:
        setFixedSize(300, 300);
        break;
    case 1:
        setFixedSize(300, 300);
        break;
    case 2:
        setFixedSize(300, 300);
        break;
    case 3:
        setFixedSize(430, 500);
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
    // Mostrar los valores de la Section1
    int plateXValue = section1Widget->getPlateXSpinBox();
    int plateYValue = section1Widget->getPlateYSpinBox();
    int centerXValue = section1Widget->getcenterXSpinBox();
    int centerYValue = section1Widget->getcenterYSpinBox();
    QMessageBox::information(this, "Section1 Values", QString("Plate X Value: %1 \n Plate Y Value: %2 \n center X Value: %3 \n center Y Value: %4 \n\n ")
                                                          .arg(plateXValue).arg(plateYValue).arg(centerXValue).arg(centerYValue));
    // Mostrar los valores de la Section2
    int size1Value = section2Widget->getSize1();
    int size2Value = section2Widget->getSize2();
    int shapeIndex1 = section2Widget->getShapeIndex1();
    int shapeIndex2 = section2Widget->getShapeIndex2();
    QMessageBox::information(this, "Section2 Values", QString("Size 1: %1\nSize 2: %2\nShape Index 1: %3\nShape Index 2: %4")
                                                          .arg(size1Value).arg(size2Value).arg(shapeIndex1).arg(shapeIndex2));

    // Mostrar los valores de la Section3
    int infillValue = section3Widget->getInfillValue();
    int shapeIndex = section3Widget->getShapeIndex();
    double strandDistanceValue = section3Widget->getStrandDistanceValue();
    QMessageBox::information(this, "Section3 Values", QString("Infill Value: %1\nShape Index: %2\nStrand Distance Value: %3")
                                                          .arg(infillValue).arg(shapeIndex).arg(strandDistanceValue));

    // Mostrar los valores de la Section4
    QList<MaterialConfig> materialConfigs = section4Widget->getMaterialConfigs();
    QString materialConfigString;
    for (const auto& materialConfig : materialConfigs) {
        materialConfigString += QString("Material Name: %1\nNozzle Size: %2\nFilament Amount: %3\n\n")
                                    .arg(materialConfig.name)
                                    .arg(materialConfig.nozzleSize)
                                    .arg(materialConfig.filamentAmount);
    }
    QMessageBox::information(this, "Section4 Values", materialConfigString);
}





