#include "section2.h"

Section2::Section2(QWidget *parent) : QWidget(parent) {
    QWidget *section2Widget = new QWidget(this);
    shapeComboBox = new QComboBox(this);
    shapeComboBox->setPlaceholderText("Seleccione una Forma");
    shapeComboBox->addItem("Circle");
    shapeComboBox->addItem("Square");

    shapeComboBox2 = new QComboBox(this);
    shapeComboBox2->setPlaceholderText("Seleccione esto");
    shapeComboBox2->addItem("Numero de capas");
    shapeComboBox2->addItem("Altura en mm");

    int uniformSize = 200;

    QLabel *plateSizeLabel2 = new QLabel("Forma de la Figura", this);
    QLabel *text7 = new QLabel("Como quieres no me acuerdo de esto", this);

    plateSizeLabel2->setMinimumWidth(uniformSize);
    plateSizeLabel2->setMaximumWidth(uniformSize);
    plateSizeLabel2->setAlignment(Qt::AlignCenter);

    text7->setMinimumWidth(uniformSize);
    text7->setMaximumWidth(uniformSize);
    text7->setAlignment(Qt::AlignCenter);

    shapeComboBox->setMinimumWidth(uniformSize);
    shapeComboBox->setMaximumWidth(uniformSize);

    shapeComboBox2->setMinimumWidth(uniformSize);
    shapeComboBox2->setMaximumWidth(uniformSize);

    sizeLineEdit = new QLineEdit(this);
    sizeLineEdit->setPlaceholderText("Enter size in mm");
    sizeLineEdit->setVisible(false);
    sizeLineEdit->setMinimumWidth(uniformSize);
    sizeLineEdit->setMaximumWidth(uniformSize);

    QDoubleValidator* Validator = new QDoubleValidator(this);
    sizeLineEdit->setValidator(Validator);

    sizeLineEdit2 = new QLineEdit(this);
    sizeLineEdit2->setPlaceholderText("Enter");
    sizeLineEdit2->setVisible(false);
    sizeLineEdit2->setMinimumWidth(uniformSize);
    sizeLineEdit2->setMaximumWidth(uniformSize);

    sizeLineEdit2->setValidator(Validator);

    QVBoxLayout *section2Layout = new QVBoxLayout(section2Widget);
    section2Layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->setAlignment(Qt::AlignHCenter);
    horizontalLayout->addWidget(shapeComboBox);
    horizontalLayout->addWidget(sizeLineEdit);

    QHBoxLayout *horizontalLayout2 = new QHBoxLayout;
    horizontalLayout2->setAlignment(Qt::AlignHCenter);
    horizontalLayout2->addWidget(shapeComboBox2);
    horizontalLayout2->addWidget(sizeLineEdit2);

    QHBoxLayout *horizontalLayoutLabel = new QHBoxLayout;
    horizontalLayoutLabel->setAlignment(Qt::AlignHCenter);
    horizontalLayoutLabel->addWidget(plateSizeLabel2);

    QVBoxLayout *section2Layout2 = new QVBoxLayout;
    section2Layout2->setAlignment(Qt::AlignHCenter);
    section2Layout2->addWidget(text7);

    section2Layout->addItem(new QSpacerItem(20, 50, QSizePolicy::Minimum));
    section2Layout->addLayout(horizontalLayoutLabel);
    section2Layout->addLayout(horizontalLayout);
    section2Layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum));
    section2Layout->addLayout(section2Layout2);
    section2Layout->addLayout(horizontalLayout2);

    connect(shapeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Section2::handleShapeSelection);
    connect(shapeComboBox2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Section2::handleShapeSelection);

    connect(sizeLineEdit, &QLineEdit::textChanged, [this]() {
        emit valueChanged();
    });

    connect(sizeLineEdit2, &QLineEdit::textChanged, [this]() {
        emit valueChanged();
    });

    connect(shapeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        emit valueChanged();
    });

    connect(shapeComboBox2, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        emit valueChanged();
    });

    section2Widget->setFixedSize(570, 535);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(section2Widget);
    mainLayout->setAlignment(Qt::AlignCenter);
    setLayout(mainLayout);
    setStyleSheet(
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
        "QLineEdit {"
        "    border: 1px solid #ddd;"
        "    border-radius: 3px;"
        "    padding: 5px;"
        "    font-size: 14px;"
        "}"
        "QComboBox {"
        "    border: 1px solid #ddd;"
        "    border-radius: 3px;"
        "    padding: 5px;"
        "    font-size: 14px;"
        "}"
        "QComboBox QAbstractItemView {"
        "    border: 1px solid #ddd;"
        "    selection-background-color: #4CAF50;"
        "    selection-color: white;"
        "}"
        "QWidget {"
        "    background-color: #f9f9f9;"
        "    padding: 10px;"
        "    border: 1px solid #ccc;"
        "    border-radius: 10px;"
        "}"
        );
}

void Section2::handleShapeSelection(int index) {
    bool showSizeLineEdit = (shapeComboBox->currentText() == "Square" || shapeComboBox->currentText() == "Circle");
    sizeLineEdit->setVisible(showSizeLineEdit);

    bool showSizeLineEdit2 = (shapeComboBox2->currentText() == "Numero de capas" || shapeComboBox2->currentText() == "Altura en mm");
    sizeLineEdit2->setVisible(showSizeLineEdit2);

    if (shapeComboBox->currentText() == "Square") {
        sizeLineEdit->setPlaceholderText("Side in mm");
    } else if (shapeComboBox->currentText() == "Circle") {
        sizeLineEdit->setPlaceholderText("Diameter in mm");
    }
}

int Section2::getSize1() const {
    return sizeLineEdit->text().toInt();
}

int Section2::getSize2() const {
    return sizeLineEdit2->text().toInt();
}

int Section2::getShapeIndex1() const {
    return shapeComboBox->currentIndex();
}

int Section2::getShapeIndex2() const {
    return shapeComboBox2->currentIndex();
}
