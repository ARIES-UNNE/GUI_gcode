#include "section4.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QComboBox>

Section4::Section4(QWidget *parent) : QWidget(parent) {
    // Sección 4: Número de materiales
    QWidget *section4Widget = new QWidget(this);
    numMaterialsSpinBox = new QSpinBox(this);
    numMaterialsSpinBox->setRange(0, 100);

    QLabel *plateSizeLabel4 = new QLabel("Materials", this);

    // Crear un diseño vertical
    QVBoxLayout *section4Layout = new QVBoxLayout(section4Widget);
    section4Layout->setAlignment(Qt::AlignTop);
    section4Widget->setLayout(section4Layout);


    connect(numMaterialsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &Section4::updateMaterialNames);


    section4Layout->addWidget(plateSizeLabel4);
    section4Layout->addWidget(numMaterialsSpinBox);


    materialNamesContainer = new QWidget(this);
    materialNamesLayout = new QVBoxLayout(materialNamesContainer);

    // Configura el tamaño
    materialNamesContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    // Crear un contenedor para el encabezado
    QWidget *headerContainer = new QWidget(this);
    QHBoxLayout *headerLayout = new QHBoxLayout(headerContainer);

    QLabel *headerLabel1 = new QLabel("Material", this);
    QLabel *headerLabel2 = new QLabel("Nozzle", this);
    QLabel *headerLabel3 = new QLabel("Filament mm", this);

    headerLayout->addWidget(headerLabel1);
    headerLayout->addWidget(headerLabel2);
    headerLayout->addWidget(headerLabel3);

    headerLayout->setAlignment(headerLabel1, Qt::AlignLeft);
    headerLayout->setAlignment(headerLabel2, Qt::AlignCenter);
    headerLayout->setAlignment(headerLabel3, Qt::AlignRight);

    // Agregar el encabezado al principio del materialNamesLayout
    section4Layout->addWidget(headerContainer);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(materialNamesContainer);

    section4Layout->addWidget(scrollArea);

    section4Widget->setFixedSize(400, 400);
}

void Section4::updateMaterialConfigs() {
    // Limpiar la lista
    materialConfigs.clear();


    for (int i = 0; i < materialNamesLayout->count(); ++i) {
        QWidget *materialRowWidget = materialNamesLayout->itemAt(i)->widget();
        if (materialRowWidget) {
            QHBoxLayout *materialRowLayout = qobject_cast<QHBoxLayout*>(materialRowWidget->layout());
            if (materialRowLayout) {
                QLabel *materialLabel = qobject_cast<QLabel*>(materialRowLayout->itemAt(0)->widget());
                QComboBox *nozzleComboBox = qobject_cast<QComboBox*>(materialRowLayout->itemAt(1)->widget());
                QLineEdit *filamentLineEdit = qobject_cast<QLineEdit*>(materialRowLayout->itemAt(2)->widget());

                if (materialLabel && nozzleComboBox && filamentLineEdit) {
                    MaterialConfig config;
                    config.name = materialLabel->text();
                    config.nozzleSize = nozzleComboBox->currentIndex();
                    config.filamentAmount = filamentLineEdit->text().toDouble();
                    materialConfigs.append(config);
                }
            }
        }
    }
}

void Section4::updateMaterialNames(int numMaterials) {
    // Limpiar
    QLayoutItem *child;
    while ((child = materialNamesLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    // Agregar nuevos widgets
    for (int i = 1; i <= numMaterials; ++i) {

        // Crear widgets para cada columna
        QWidget *materialRow = new QWidget(this);
        QHBoxLayout *materialRowLayout = new QHBoxLayout(materialRow);

        QLabel *materialLabel = new QLabel("Material " + QString::number(i), this);
        QComboBox *nozzleComboBox = new QComboBox(this);
        nozzleComboBox->addItem("0.2 mm");
        nozzleComboBox->addItem("0.4 mm");
        nozzleComboBox->addItem("0.6 mm");
        QLineEdit *filamentLineEdit = new QLineEdit(this);
        filamentLineEdit->setValidator(new QDoubleValidator(this));

        // tamaño mínimo y máximo
        materialLabel->setMinimumWidth(70);
        materialLabel->setMaximumWidth(70);
        nozzleComboBox->setMinimumWidth(70);
        nozzleComboBox->setMaximumWidth(70);
        filamentLineEdit->setMinimumWidth(70);
        filamentLineEdit->setMaximumWidth(70);


        materialRowLayout->addWidget(materialLabel);
        materialRowLayout->addWidget(nozzleComboBox);
        materialRowLayout->addWidget(filamentLineEdit);

        materialNamesLayout->addWidget(materialRow);

        // Conectar las señales
        connect(nozzleComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Section4::updateMaterialConfigs);
        connect(filamentLineEdit, &QLineEdit::textChanged, this, &Section4::updateMaterialConfigs);
    }

    // Asegurar que el contenedor se redibuje
    materialNamesContainer->update();


    updateMaterialConfigs();
}



QList<MaterialConfig> Section4::getMaterialConfigs() const {
    return materialConfigs;
}


int Section4::getNumMaterials() const {
    return numMaterialsSpinBox->value();
}
