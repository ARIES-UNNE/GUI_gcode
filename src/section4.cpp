#include "section4.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <qlineedit.h>

Section4::Section4(QWidget *parent) : QWidget(parent) {

    // Sección 4: Número de materiales
    QWidget *section4Widget = new QWidget(this);
    numMaterialsSpinBox = new QSpinBox(this);
    numMaterialsSpinBox->setRange(1, 100);  // Establecer el rango de valores
    QLabel *plateSizeLabel4 = new QLabel("Materials", this);
    section4Widget->setLayout(new QVBoxLayout);
    section4Widget->layout()->addWidget(plateSizeLabel4);
    section4Widget->layout()->addWidget(numMaterialsSpinBox);


}




