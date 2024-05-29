#include "section3.h"

Section3::Section3(QWidget *parent) : QWidget(parent) {

    QWidget *section3Widget = new QWidget(this);

    infillSpinBox = new QSpinBox(this);
    infillSpinBox->setRange(0, 100);

    strandDistanceLineEdit = new QLineEdit(this);

    infillSpinBox->setVisible(false);
    strandDistanceLineEdit->setVisible(false);

    QVBoxLayout *section3Layout = new QVBoxLayout(section3Widget);
    section3Layout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    section3Widget->setLayout(section3Layout);

    shapeComboBox = new QComboBox(this);
    shapeComboBox->setPlaceholderText("Seleccione un método");
    shapeComboBox->addItem("Infill");
    shapeComboBox->addItem("Strand Distance");

    shapeComboBox->setMinimumWidth(200);
    shapeComboBox->setMaximumWidth(200);
    infillSpinBox->setMinimumWidth(200);
    strandDistanceLineEdit->setMinimumWidth(200);
    strandDistanceLineEdit->setMaximumWidth(200);

    QLabel *title = new QLabel("Selección de método", this);
    porcentaje = new QLabel("%: ", this);
    milimetros = new QLabel("mm: ", this);

    title->setMinimumWidth(200);
    title->setMaximumWidth(200);

    porcentaje->setVisible(false);
    milimetros->setVisible(false);
    milimetros->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QDoubleValidator* validator = new QDoubleValidator(this);
    strandDistanceLineEdit->setValidator(validator);

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    QHBoxLayout *horizontalLayout2 = new QHBoxLayout;

    horizontalLayout->addWidget(porcentaje);
    horizontalLayout->addWidget(infillSpinBox);

    horizontalLayout2->addWidget(milimetros);
    horizontalLayout2->addWidget(strandDistanceLineEdit);

    horizontalLayout->setAlignment(Qt::AlignCenter);
    horizontalLayout2->setAlignment(Qt::AlignCenter);

    QVBoxLayout *titleAndComboBoxLayout = new QVBoxLayout;
    titleAndComboBoxLayout->setAlignment(Qt::AlignCenter);
    titleAndComboBoxLayout->addWidget(title);
    titleAndComboBoxLayout->addWidget(shapeComboBox);

    section3Layout->addItem(new QSpacerItem(20, 50, QSizePolicy::Minimum));
    section3Layout->addLayout(titleAndComboBoxLayout);
    section3Layout->addLayout(horizontalLayout);
    section3Layout->addLayout(horizontalLayout2);

    connect(shapeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Section3::handleMethodSelection);
    connect(infillSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &Section3::valueChanged);
    connect(strandDistanceLineEdit, &QLineEdit::textChanged, [this]() {
        emit valueChanged();
    });
    connect(shapeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        emit valueChanged();
    });

    section3Widget->setFixedSize(579, 544);


    setStyleSheet(
        "QSpinBox, QLineEdit {"
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
        "    margin-left: 9px;"
        "    margin-top: 9px;"
        "}"
        "QSpinBox::up-button {"
        "    width: 0px;"
        "}"
        "QSpinBox::down-button {"
        "    width: 0px;"
        "}"
        );
}

void Section3::handleMethodSelection(int index) {
    infillSpinBox->setVisible(false);
    strandDistanceLineEdit->setVisible(false);
    porcentaje->setVisible(false);
    milimetros->setVisible(false);

    if (index == 0) {
        infillSpinBox->setVisible(true);
        porcentaje->setVisible(true);
    } else if (index == 1) {
        strandDistanceLineEdit->setVisible(true);
        milimetros->setVisible(true);
    }
}

int Section3::getInfillValue() const {
    return infillSpinBox->value();
}

int Section3::getShapeIndex() const {
    return shapeComboBox->currentIndex();
}

double Section3::getStrandDistanceValue() const {
    return strandDistanceLineEdit->text().toDouble();
}
