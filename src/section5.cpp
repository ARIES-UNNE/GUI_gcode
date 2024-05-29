#include "section5.h"

Section5::Section5(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *completionLabel = new QLabel("Configuración de GCODE completada", this);
    completionLabel->setAlignment(Qt::AlignCenter);
    completionLabel->setStyleSheet("font-size: 24px; color: #333;");

    layout->addWidget(completionLabel);
    layout->setAlignment(completionLabel, Qt::AlignCenter);

    setLayout(layout);
}
