#include "section3.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <qlineedit.h>

Section3::Section3(QWidget *parent) : QWidget(parent) {

    // Sección 3: Infill o Strand Distance
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


}




