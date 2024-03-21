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
    numMaterialsSpinBox->setRange(0, 100);  // Establecer el rango de valores

    QLabel *plateSizeLabel4 = new QLabel("Materials", this);

    // Crear un diseño vertical para organizar los widgets
    QVBoxLayout *section4Layout = new QVBoxLayout(section4Widget);
    section4Layout->setAlignment(Qt::AlignTop);
    section4Widget->setLayout(section4Layout);

    // Conectar la señal valueChanged del QSpinBox a una función para manejar cambios en el número de materiales
    connect(numMaterialsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &Section4::updateMaterialNames);

    // Agregar el widget principal al diseño vertical
    section4Layout->addWidget(plateSizeLabel4);
    section4Layout->addWidget(numMaterialsSpinBox);

    // Crear un contenedor para los nombres de los materiales y colocarlo dentro de un QScrollArea
    materialNamesContainer = new QWidget(this);
    materialNamesLayout = new QVBoxLayout(materialNamesContainer);

    // Configura el tamaño del contenido para que se ajuste al ancho del QScrollArea
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

void Section4::updateMaterialNames(int numMaterials) {
    // Limpiar cualquier widget anterior en el contenedor
    QLayoutItem *child;
    while ((child = materialNamesLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    // Limpiar la lista de configuraciones de materiales
    materialConfigs.clear();

    // Agregar nuevos widgets para los nombres de los materiales según el número seleccionado
    for (int i = 1; i <= numMaterials; ++i) {
        // Crear widgets para cada columna (Material, Nozzle, Filament)
        QWidget *materialRow = new QWidget(this);
        QHBoxLayout *materialRowLayout = new QHBoxLayout(materialRow);

        QLabel *materialLabel = new QLabel("Material " + QString::number(i), this);
        QComboBox *nozzleComboBox = new QComboBox(this);
        nozzleComboBox->addItem("0.2 mm");
        nozzleComboBox->addItem("0.4 mm");
        nozzleComboBox->addItem("0.6 mm");
        QLineEdit *filamentLineEdit = new QLineEdit(this);
        filamentLineEdit->setValidator(new QDoubleValidator(this));

        // Ajustar el tamaño mínimo y máximo de cada columna
        materialLabel->setMinimumWidth(70);
        materialLabel->setMaximumWidth(70);
        nozzleComboBox->setMinimumWidth(70);
        nozzleComboBox->setMaximumWidth(70);
        filamentLineEdit->setMinimumWidth(70);
        filamentLineEdit->setMaximumWidth(70);

        // Almacena la configuración del material en la lista
        MaterialConfig config;
        config.name = "Material " + QString::number(i);
        config.nozzleSize = nozzleComboBox->currentIndex();
        config.filamentAmount = filamentLineEdit->text().toDouble();

        // Agregar widgets al diseño de cada fila
        materialRowLayout->addWidget(materialLabel);
        materialRowLayout->addWidget(nozzleComboBox);
        materialRowLayout->addWidget(filamentLineEdit);


        materialRowLayout->setAlignment(materialLabel, Qt::AlignLeft);
        materialRowLayout->setAlignment(nozzleComboBox, Qt::AlignCenter);
        materialRowLayout->setAlignment(filamentLineEdit, Qt::AlignRight);


        materialNamesLayout->addWidget(materialRow);

        // Agregar la configuración del material a la lista
        materialConfigs.append(config);
    }

    // Asegurar que el contenedor se redibuje
    materialNamesContainer->update();
}

// Función para obtener la lista de configuraciones de materiales
QList<MaterialConfig> Section4::getMaterialConfigs() const {
    return materialConfigs;
}
