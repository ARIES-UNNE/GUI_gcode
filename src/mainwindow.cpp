
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // widget central
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QFormLayout *formLayout = new QFormLayout(centralWidget);

    // Secci�n 1: Tama�o de la placa
    QWidget *section1Widget = new QWidget(this);
    QSpinBox *plateXSpinBox = new QSpinBox(this);
    QSpinBox *plateYSpinBox = new QSpinBox(this);
    QLabel *plateSizeLabel = new QLabel("Tama�o de la Placa:", this);
    section1Widget->setLayout(new QVBoxLayout);
    section1Widget->layout()->addWidget(plateSizeLabel);
    section1Widget->layout()->addWidget(plateXSpinBox);
    section1Widget->layout()->addWidget(plateYSpinBox);


    // Secci�n 2: Forma
    QWidget *section2Widget = new QWidget(this);
    shapeComboBox = new QComboBox(this);
    shapeComboBox->addItem("Circle");
    shapeComboBox->addItem("Square");
    QLabel *plateSizeLabel2 = new QLabel("Forma", this);
    section2Widget->setLayout(new QVBoxLayout);
    section2Widget->layout()->addWidget(plateSizeLabel2);
    section2Widget->layout()->addWidget(shapeComboBox);

    // Secci�n 3: Infill o Strand Distance
    QWidget *section3Widget = new QWidget(this);
    infillSpinBox = new QSpinBox(this);
    infillSpinBox->setRange(0, 100);  // Establecer el rango de valores
    strandDistanceSpinBox = new QSpinBox(this);
    strandDistanceSpinBox->setRange(1, 10000);  // Establecer el rango de valores
    QLabel *plateSizeLabel3 = new QLabel("Infill", this);
    section3Widget->setLayout(new QVBoxLayout);
    section3Widget->layout()->addWidget(plateSizeLabel3);
    section3Widget->layout()->addWidget(infillSpinBox);
    section3Widget->layout()->addWidget(strandDistanceSpinBox);

    // Secci�n 4: N�mero de materiales
    QWidget *section4Widget = new QWidget(this);
    numMaterialsSpinBox = new QSpinBox(this);
    numMaterialsSpinBox->setRange(1, 100);  // Establecer el rango de valores
    QLabel *plateSizeLabel4 = new QLabel("Materials", this);
    section4Widget->setLayout(new QVBoxLayout);
    section4Widget->layout()->addWidget(plateSizeLabel4);
    section4Widget->layout()->addWidget(numMaterialsSpinBox);

    // Crear el bot�n de generaci�n de GCode
    generateButton = new QPushButton("Generate GCode", this);
    connect(generateButton, &QPushButton::clicked, this, &MainWindow::generateGCode);
    section4Widget->layout()->addWidget(generateButton);


    // Crear el bot�n para pasar a la siguiente secci�n
    QPushButton *nextButton = new QPushButton("Next Section", this);
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::nextSection);

    // Crear el bot�n para retroceder a la secci�n anterior
    QPushButton *prevButton = new QPushButton("Previous Section", this);
    connect(prevButton, &QPushButton::clicked, this, &MainWindow::previousSection);


    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(section1Widget);
    stackedWidget->addWidget(section2Widget);
    stackedWidget->addWidget(section3Widget);
    stackedWidget->addWidget(section4Widget);

    formLayout->addWidget(stackedWidget);
    formLayout->addRow(nextButton);
    formLayout->addRow(prevButton);

}


void MainWindow::previousSection() {
    // Mostrar la secci�n anterior
    int prevIndex = stackedWidget->currentIndex() - 1;
    if (prevIndex >= 0) {
        stackedWidget->setCurrentIndex(prevIndex);
    }
}


void MainWindow::nextSection() {
    // Mostrar la siguiente secci�n
    int nextIndex = stackedWidget->currentIndex() + 1;
    if (nextIndex < stackedWidget->count()) {
        stackedWidget->setCurrentIndex(nextIndex);
    }
}

void MainWindow::generateGCode() {

    QString message = QString("Plate Size: (%1, %2) mm\n"
                              "Shape: %3\n"
                              "Infill: %4%%\n"
                              "Strand Distance: %5 mm\n"
                              "Number of Materials: %6")
                          .arg(stackedWidget->widget(0)->findChild<QSpinBox*>("plateXSpinBox")->value())
                          .arg(stackedWidget->widget(0)->findChild<QSpinBox*>("plateYSpinBox")->value())
                          .arg(shapeComboBox->currentText())
                          .arg(infillSpinBox->value())
                          .arg(strandDistanceSpinBox->value())
                          .arg(numMaterialsSpinBox->value());

    QMessageBox::information(this, "GCode Generation", message);
}