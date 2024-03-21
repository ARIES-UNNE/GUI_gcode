#include "section2.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <qlineedit.h>

Section2::Section2(QWidget *parent) : QWidget(parent) {


    // Sección 2: Forma
    QWidget *section2Widget = new QWidget(this);
    shapeComboBox = new QComboBox(this);
    shapeComboBox->setPlaceholderText("Seleccione una Forma");
    shapeComboBox->addItem("Circle");
    shapeComboBox->addItem("Square");

    shapeComboBox2 = new QComboBox(this);
    shapeComboBox2->setPlaceholderText("Seleccione esto");
    shapeComboBox2->addItem("Numero de capas");
    shapeComboBox2->addItem("Altura en mm");

    QLabel *plateSizeLabel2 = new QLabel("Forma de la Figura", this);
    QLabel *text7 = new QLabel("Como quieres no me acuerdo de esto", this);

    QVBoxLayout *section2Layout = new QVBoxLayout(section2Widget);

    section2Layout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    section2Widget->setLayout(section2Layout);

    // Layout horizontal para los elementos debajo del QComboBox
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

    QVBoxLayout *section2Layout2 = new QVBoxLayout(section2Widget);
    section2Layout2->addWidget(text7);

    // Agregar el layout horizontal al layout vertical principal
    section2Layout->addWidget(plateSizeLabel2);
    section2Layout->addLayout(horizontalLayout);
    section2Layout->addLayout(section2Layout2);
    section2Layout->addLayout(horizontalLayout2);

    // Configurar el QLabel para que se expanda automáticamente
    shapeLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    shapeLabel->setFixedWidth(60);  // Ajusta el ancho según sea necesario

    // Conectar la señal de cambio de forma con la función handleShapeSelection
    connect(shapeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Section2::handleShapeSelection);
    connect(shapeComboBox2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Section2::handleShapeSelection);

    section2Widget->setFixedSize(280,200);


}


void Section2::handleShapeSelection(int index) {
    // Mostrar el QLineEdit si la forma seleccionada es "Square" o "Circle"
    bool showSizeLineEdit = (shapeComboBox->currentText() == "Square" || shapeComboBox->currentText() == "Circle");
    sizeLineEdit->setVisible(showSizeLineEdit);

    // Actualizar el texto del QLabel según la forma seleccionada
    QString labelText = (shapeComboBox->currentText() == "Square") ? "Lado:" : "Diámetro:";
    shapeLabel->setText(labelText);
    shapeLabel->setVisible(showSizeLineEdit);

    bool showSizeLineEdit2 = (shapeComboBox2->currentText() == "Numero de capas" || shapeComboBox2->currentText() == "Altura en mm");
    sizeLineEdit2->setVisible(showSizeLineEdit2);
}


