#include "section1.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

Section1::Section1(QWidget *parent) : QWidget(parent) {


    // Sección 1: Tamaño de la placa
    QWidget *section1Widget = new QWidget(this);
    plateXSpinBox = new QSpinBox(this);
    plateYSpinBox = new QSpinBox(this);
    centerXSpinBox = new QSpinBox(this);
    centerYSpinBox = new QSpinBox(this);

    centerXSpinBox->setVisible(false);
    centerYSpinBox->setVisible(false);

    // Etiquetas para X e Y
    QLabel *labelX = new QLabel("X:", this);
    QLabel *labelY = new QLabel("Y:", this);
    QLabel *labelTitle = new QLabel("Tamaño de la placa:", this);

    QVBoxLayout *section1Layout = new QVBoxLayout(section1Widget);
    section1Layout->setAlignment(Qt::AlignTop | Qt::AlignCenter);

    section1Widget->setLayout(section1Layout);


    // Diseño horizontal para colocar las etiquetas y spinboxes
    QHBoxLayout *horizontalLayoutS1 = new QHBoxLayout;


    // Ajustar el tamaño mínimo de los spinboxes
    plateXSpinBox->setMinimumWidth(200);  // Ajusta el valor según sea necesario
    centerXSpinBox->setMinimumWidth(200);

    horizontalLayoutS1->addWidget(labelX);
    horizontalLayoutS1->addWidget(plateXSpinBox);

    QHBoxLayout *horizontalLayout2S1 = new QHBoxLayout;


    // Ajustar el tamaño mínimo de los spinboxes
    plateYSpinBox->setMinimumWidth(200);  
    centerYSpinBox->setMinimumWidth(200);

    horizontalLayout2S1->addWidget(labelY);
    horizontalLayout2S1->addWidget(plateYSpinBox);

    // Agregar los diseños horizontales al diseño vertical principal
    section1Layout->addWidget(labelTitle);
    section1Layout->addLayout(horizontalLayoutS1);
    section1Layout->addLayout(horizontalLayout2S1);

    // Etiquetas para las coordenadas X e Y
    QLabel *labelCenterX = new QLabel("X:", this);
    QLabel *labelCenterY = new QLabel("Y:", this);

    labelCenterX->setVisible(false);
    labelCenterY->setVisible(false);

    // Diseño horizontal para colocar las etiquetas y spinboxes del centro
    QHBoxLayout *horizontalLayoutCenterX = new QHBoxLayout;

    horizontalLayoutCenterX->addWidget(labelCenterX);
    horizontalLayoutCenterX->addWidget(centerXSpinBox);

    QHBoxLayout *horizontalLayoutCenterY = new QHBoxLayout;

    horizontalLayoutCenterY->addWidget(labelCenterY);
    horizontalLayoutCenterY->addWidget(centerYSpinBox);

    // Botón para introducir manualmente el centro de la placa
    QPushButton *manualCenterButton = new QPushButton("Introducir Manualmente el centro", this);

    // Conectar el botón con una función lambda que llama a showManualCenterInput
    connect(manualCenterButton, &QPushButton::clicked, [=]() {
        showManualCenterInput(labelCenterX, labelCenterY);
    });

    // Añadir los widgets al diseño vertical principal
    section1Layout->addWidget(manualCenterButton);
    section1Layout->addLayout(horizontalLayoutCenterX);
    section1Layout->addLayout(horizontalLayoutCenterY);

    // Ajustar márgenes del diseño vertical principal
    section1Widget->setFixedSize(280,200);
}


QSpinBox *Section1::getPlateXSpinBox() const {
    return plateXSpinBox;
}

QSpinBox *Section1::getPlateYSpinBox() const {
    return plateYSpinBox;
}

void Section1::showManualCenterInput(QLabel *labelX, QLabel *labelY) {
    // Alternar la visibilidad de los spinboxes y etiquetas del centro
    bool isVisible = centerXSpinBox->isVisible();
    centerXSpinBox->setVisible(!isVisible);
    centerYSpinBox->setVisible(!isVisible);
    labelX->setVisible(!isVisible);
    labelY->setVisible(!isVisible);
}


