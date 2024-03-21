#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Crear un widget central 
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QFormLayout *formLayout = new QFormLayout(centralWidget);

    // Sección 1: Tamaño de la placa
    QWidget *section1Widget = new QWidget(this);
    QSpinBox *plateXSpinBox = new QSpinBox(this);
    QSpinBox *plateYSpinBox = new QSpinBox(this);
    QLabel *plateSizeLabel = new QLabel("Tamaño de la Placa:", this);
    section1Widget->setLayout(new QVBoxLayout);
    section1Widget->layout()->addWidget(plateSizeLabel);
    section1Widget->layout()->addWidget(plateXSpinBox);
    section1Widget->layout()->addWidget(plateYSpinBox);

    // Sección 2: Forma
    QWidget *section2Widget = new QWidget(this);
    shapeComboBox = new QComboBox(this);
    shapeComboBox->setPlaceholderText("Seleccione una Forma");
    shapeComboBox->addItem("Circle");
    shapeComboBox->addItem("Square");
    shapeComboBox->setCurrentText("jwifhnawi");

    shapeComboBox2 = new QComboBox(this);
    shapeComboBox2->setPlaceholderText("Seleccione esto");
    shapeComboBox2->addItem("Numero de capas");
    shapeComboBox2->addItem("Altura en mm");

    QLabel *plateSizeLabel2 = new QLabel("Forma de la Figura", this);

    QLabel *text7 = new QLabel("Como quieres no me acuerdo de esto", this);

    QVBoxLayout *section2Layout = new QVBoxLayout(section2Widget);
    QVBoxLayout *section2Layout2 = new QVBoxLayout(section2Widget);
    section2Widget->setLayout(section2Layout);

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    QHBoxLayout *horizontalLayout2 = new QHBoxLayout;

    // Agregar QLabel para el texto adicional
    shapeLabel = new QLabel(this);
    horizontalLayout->addWidget(shapeComboBox);
    horizontalLayout->addWidget(shapeLabel);
    horizontalLayout2->addWidget(shapeComboBox2);

    // Agregar QLineEdit para el tamaño
    sizeLineEdit = new QLineEdit(this);
    sizeLineEdit->setPlaceholderText("Enter size in mm");
    sizeLineEdit->setVisible(false);
    horizontalLayout->addWidget(sizeLineEdit);

    QDoubleValidator* Validator = new QDoubleValidator(this);
    sizeLineEdit->setValidator(Validator);

    sizeLineEdit2 = new QLineEdit(this);
    sizeLineEdit2->setPlaceholderText("Enter");
    sizeLineEdit2->setVisible(false);
    horizontalLayout2->addWidget(sizeLineEdit2);

    sizeLineEdit2->setValidator(Validator);
    section2Layout2->addWidget(text7);

    section2Layout->addWidget(plateSizeLabel2);
    section2Layout->addLayout(horizontalLayout);
    section2Layout->addLayout(section2Layout2);
    section2Layout->addLayout(horizontalLayout2);


    shapeLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    shapeLabel->setFixedWidth(60);  /

 
    connect(shapeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::handleShapeSelection);
    connect(shapeComboBox2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::handleShapeSelection);




    // Sección 3: Infill o Strand Distance
    QWidget *section3Widget = new QWidget(this);
    infillSpinBox = new QSpinBox(this);
    infillSpinBox->setRange(0, 100);  
    strandDistanceSpinBox = new QSpinBox(this);
    strandDistanceSpinBox->setRange(1, 10000);  
    QLabel *plateSizeLabel3 = new QLabel("Infill", this);
    section3Widget->setLayout(new QVBoxLayout);
    section3Widget->layout()->addWidget(plateSizeLabel3);
    section3Widget->layout()->addWidget(infillSpinBox);
    section3Widget->layout()->addWidget(strandDistanceSpinBox);

    // Sección 4: Número de materiales
    QWidget *section4Widget = new QWidget(this);
    numMaterialsSpinBox = new QSpinBox(this);
    numMaterialsSpinBox->setRange(1, 100);  
    QLabel *plateSizeLabel4 = new QLabel("Materials", this);
    section4Widget->setLayout(new QVBoxLayout);
    section4Widget->layout()->addWidget(plateSizeLabel4);
    section4Widget->layout()->addWidget(numMaterialsSpinBox);
    // Crear el botón de generación de GCode 
    generateButton = new QPushButton("Generate GCode", this);
    connect(generateButton, &QPushButton::clicked, this, &MainWindow::generateGCode);
    section4Widget->layout()->addWidget(generateButton);


    // Crear el botón para pasar a la siguiente sección
    QPushButton *nextButton = new QPushButton("Next Section", this);
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::nextSection);

    // Crear el botón para retroceder a la sección anterior
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

void MainWindow::handleShapeSelection(int index) {
    //  "Square" o "Circle"
    bool showSizeLineEdit = (shapeComboBox->currentText() == "Square" || shapeComboBox->currentText() == "Circle");
    sizeLineEdit->setVisible(showSizeLineEdit);

    QString labelText = (shapeComboBox->currentText() == "Square") ? "Lado:" : "Diámetro:";
    shapeLabel->setText(labelText);
    shapeLabel->setVisible(showSizeLineEdit);

    bool showSizeLineEdit2 = (shapeComboBox2->currentText() == "Numero de capas" || shapeComboBox2->currentText() == "Altura en mm");
    sizeLineEdit2->setVisible(showSizeLineEdit2);
}


void MainWindow::previousSection() {
    // Mostrar la sección anterior
    int prevIndex = stackedWidget->currentIndex() - 1;
    if (prevIndex >= 0) {
        stackedWidget->setCurrentIndex(prevIndex);
    }
}


void MainWindow::nextSection() {
    // Mostrar la siguiente sección
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
