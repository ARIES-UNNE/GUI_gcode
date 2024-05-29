#include "section1.h"

Section1::Section1(QWidget *parent) : QWidget(parent) {
    QWidget *section1Widget = new QWidget(this);
    plateXSpinBox = new QSpinBox(this);
    plateYSpinBox = new QSpinBox(this);
    centerXSpinBox = new QSpinBox(this);
    centerYSpinBox = new QSpinBox(this);

    centerXSpinBox->setVisible(false);
    centerYSpinBox->setVisible(false);

    QLabel *labelX = new QLabel("X:", this);
    QLabel *labelY = new QLabel("Y:", this);
    QLabel *labelTitle = new QLabel("Tamaño de la placa:", this);

    QVBoxLayout *section1Layout = new QVBoxLayout(section1Widget);
    section1Layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    section1Widget->setLayout(section1Layout);

    QHBoxLayout *horizontalLayoutS1 = new QHBoxLayout;
    horizontalLayoutS1->setAlignment(Qt::AlignHCenter);

    plateXSpinBox->setMinimumWidth(200);
    centerXSpinBox->setMinimumWidth(200);

    horizontalLayoutS1->addWidget(labelX);
    horizontalLayoutS1->addWidget(plateXSpinBox);

    QHBoxLayout *horizontalLayout2S1 = new QHBoxLayout;
    horizontalLayout2S1->setAlignment(Qt::AlignHCenter);

    plateYSpinBox->setMinimumWidth(200);
    centerYSpinBox->setMinimumWidth(200);

    horizontalLayout2S1->addWidget(labelY);
    horizontalLayout2S1->addWidget(plateYSpinBox);

    QSpacerItem *verticalSpacer = new QSpacerItem(20, 50, QSizePolicy::Minimum);

    section1Layout->addItem(verticalSpacer);
    section1Layout->addWidget(labelTitle);
    section1Layout->addLayout(horizontalLayoutS1);
    section1Layout->addLayout(horizontalLayout2S1);

    QLabel *labelCenterX = new QLabel("X:", this);
    QLabel *labelCenterY = new QLabel("Y:", this);

    labelCenterX->setVisible(false);
    labelCenterY->setVisible(false);

    QHBoxLayout *horizontalLayoutCenterX = new QHBoxLayout;
    horizontalLayoutCenterX->setAlignment(Qt::AlignHCenter);

    horizontalLayoutCenterX->addWidget(labelCenterX);
    horizontalLayoutCenterX->addWidget(centerXSpinBox);

    QHBoxLayout *horizontalLayoutCenterY = new QHBoxLayout;
    horizontalLayoutCenterY->setAlignment(Qt::AlignHCenter);

    horizontalLayoutCenterY->addWidget(labelCenterY);
    horizontalLayoutCenterY->addWidget(centerYSpinBox);

    QPushButton *manualCenterButton = new QPushButton("Introducir Manualmente el centro", this);

    connect(manualCenterButton, &QPushButton::clicked, [=]() {
        showManualCenterInput(labelCenterX, labelCenterY);
    });

    connect(plateXSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &Section1::valueChanged);
    connect(plateYSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &Section1::valueChanged);
    connect(centerXSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &Section1::valueChanged);
    connect(centerYSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &Section1::valueChanged);

    section1Layout->addWidget(manualCenterButton);
    section1Layout->addLayout(horizontalLayoutCenterX);
    section1Layout->addLayout(horizontalLayoutCenterY);

    section1Widget->setFixedSize(570, 535);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(section1Widget);
    mainLayout->setAlignment(Qt::AlignCenter);
    setLayout(mainLayout);
    setStyleSheet("QLabel {"
                  "    font-size: 14px;"
                  "    color: #333;"
                  "}"
                  "QPushButton {"
                  "    background-color: #4CAF50;"
                  "    color: #333;"
                  "    border: none;"
                  "    border-radius: 5px;"
                  "    font-size: 14px;"
                  "    padding: 5px 10px;"
                  "}"
                  "QPushButton:hover {"
                  "    background-color: #45a049;"
                  "    color: white;"
                  "}"
                  "QSpinBox {"
                  "    border: 1px solid #ddd;"
                  "    border-radius: 3px;"
                  "    padding: 5px;"
                  "    font-size: 14px;"
                  "}"
                  "QSpinBox::up-button {"
                  "    width: 0px;"
                  "}"
                  "QSpinBox::down-button {"
                  "    width: 0px;"
                  "}"
                  "QSpinBox::up-button:hover {"
                  "    background-color: #ddd;"
                  "}"
                  "QSpinBox::down-button:hover {"
                  "    background-color: #ddd;"
                  "}"
                  "QWidget {"
                  "    background-color: #f9f9f9;"
                  "    padding: 10px;"
                  "    border: 1px solid #ccc;"
                  "    border-radius: 10px;"
                  "}");
}
int Section1::getPlateXSpinBox() const {
    return plateXSpinBox->value();
}

int Section1::getPlateYSpinBox() const {
    return plateYSpinBox->value();
}

int Section1::getcenterXSpinBox() const {
    return centerXSpinBox->value();
}

int Section1::getcenterYSpinBox() const {
    return centerYSpinBox->value();
}

void Section1::showManualCenterInput(QLabel *labelX, QLabel *labelY) {
    bool isVisible = centerXSpinBox->isVisible();
    centerXSpinBox->setVisible(!isVisible);
    centerYSpinBox->setVisible(!isVisible);
    labelX->setVisible(!isVisible);
    labelY->setVisible(!isVisible);
}
