#include "section3.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>

Section3::Section3(QWidget *parent) : QWidget(parent) {
    // Sección 3: Infill o Strand Distance
    QWidget *section3Widget = new QWidget(this);

    // Crear widgets para Infill y Strand Distance
    infillSpinBox = new QSpinBox(this);
    infillSpinBox->setRange(0, 100);

    strandDistanceLineEdit = new QLineEdit(this);

    // Inicialmente, ocultar ambos widgets
    infillSpinBox->setVisible(false);
    strandDistanceLineEdit->setVisible(false);

    // Crear un diseño vertical para organizar los widgets
    QVBoxLayout *section3Layout = new QVBoxLayout(section3Widget);
    section3Layout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    section3Widget->setLayout(section3Layout);

    // Crear un QComboBox
    shapeComboBox = new QComboBox(this);
    shapeComboBox->setPlaceholderText("Seleccione un método");
    shapeComboBox->addItem("Infill");
    shapeComboBox->addItem("Strand Distance");

    shapeComboBox->setMinimumWidth(250);
    infillSpinBox->setMinimumWidth(200);
    strandDistanceLineEdit->setMinimumWidth(200);
    strandDistanceLineEdit->setMaximumWidth(200);

    // Etiqueta para el título
    QLabel *title = new QLabel("Selección de método", this);
    porcentaje = new QLabel("%: ", this);
    milimetros = new QLabel("mm: ", this);

    // Inicialmente, ocultar ambos widgets
    porcentaje->setVisible(false);
    milimetros->setVisible(false);
    milimetros->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QDoubleValidator* Validator = new QDoubleValidator(this);
    strandDistanceLineEdit->setValidator(Validator);

    // Layout horizontal
    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    QHBoxLayout *horizontalLayout2 = new QHBoxLayout;

    horizontalLayout->addWidget(porcentaje);
    horizontalLayout->addWidget(infillSpinBox);

    horizontalLayout2->addWidget(milimetros);
    horizontalLayout2->addWidget(strandDistanceLineEdit);

    // Configurar alineación y espaciado
    horizontalLayout->setAlignment(Qt::AlignCenter);
    horizontalLayout2->setAlignment(Qt::AlignCenter);

    // Agregar los widgets al diseño vertical principal
    section3Layout->addWidget(title);
    section3Layout->addWidget(shapeComboBox);
    section3Layout->addLayout(horizontalLayout);
    section3Layout->addLayout(horizontalLayout2);

    // Conectar la señal currentIndexChanged a la función correspondiente
    connect(shapeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Section3::handleMethodSelection);

    section3Widget->setFixedSize(280, 200);
}

void Section3::handleMethodSelection(int index) {
    // Ocultar ambos widgets al principio
    infillSpinBox->setVisible(false);
    strandDistanceLineEdit->setVisible(false);
    porcentaje->setVisible(false);
    milimetros->setVisible(false);

    // Mostrar el widget correspondiente según la selección
    if (index == 0) {
        infillSpinBox->setVisible(true);
        porcentaje->setVisible(true);
    } else if (index == 1) {
        strandDistanceLineEdit->setVisible(true);
        milimetros->setVisible(true);
    }
}
