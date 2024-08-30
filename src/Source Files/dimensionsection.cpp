#include "DimensionSection.h"

/**
 * @brief Constructor de DimensionSection.
 *
 * Inicializa los widgets de la sección de dimensiones y configura sus propiedades
 *
 * @param parent El widget padre para este widget.
 */
DimensionSection::DimensionSection(QWidget *parent) : QWidget(parent) {

    // Crear el widget contenedor para la sección
    QWidget *dimensionSectionWidget = new QWidget(this);

    // Crear el título de la sección
    sectionTitle = new QLabel(tr("Dimension Configuration"), this);
    sectionTitle->setObjectName("sectionTitle");

    // Inicializar los spin boxes para dimensiones y centro
    plateXSpinBox = new QSpinBox(this);
    plateYSpinBox = new QSpinBox(this);
    centerXSpinBox = new QSpinBox(this);
    centerYSpinBox = new QSpinBox(this);

    // Configurar rangos y valores iniciales para los spin boxes
    centerXSpinBox->setRange(0, 250);
    centerYSpinBox->setRange(0, 250);
    plateXSpinBox->setRange(20, 250);
    plateYSpinBox->setRange(20, 250);
    plateXSpinBox->setValue(70);
    plateYSpinBox->setValue(70);
    centerXSpinBox->setValue(50);
    centerYSpinBox->setValue(50);

    // Crear etiquetas para los spin boxes
    QLabel *labelX = new QLabel("X:", this);
    QLabel *labelY = new QLabel("Y:", this);
    labelTitle = new QLabel(tr("Introduce Plate Size:"), this);
    labelTitle->setAlignment(Qt::AlignCenter);

    // Etiquetas para coordenadas del centro
    QLabel *labelCenterX = new QLabel("X:", this);
    QLabel *labelCenterY = new QLabel("Y:", this);

    // Crear y configurar el layout vertical para la sección
    QVBoxLayout *sectionLayout = new QVBoxLayout(dimensionSectionWidget);
    sectionLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    dimensionSectionWidget->setLayout(sectionLayout);

    // Crear y configurar el layout horizontal para la dimensión X
    QHBoxLayout *horizontalLayoutX = new QHBoxLayout;
    horizontalLayoutX->setAlignment(Qt::AlignHCenter);
    plateXSpinBox->setMinimumWidth(200);
    horizontalLayoutX->addWidget(labelX);
    horizontalLayoutX->addWidget(plateXSpinBox);

    // Crear y configurar el layout horizontal para la dimensión Y
    QHBoxLayout *horizontalLayoutY = new QHBoxLayout;
    horizontalLayoutY->setAlignment(Qt::AlignHCenter);
    plateYSpinBox->setMinimumWidth(200);
    horizontalLayoutY->addWidget(labelY);
    horizontalLayoutY->addWidget(plateYSpinBox);

    // Añadir un espaciador y los elementos al layout de la sección
    sectionLayout->addItem(new QSpacerItem(20, 50, QSizePolicy::Minimum));
    sectionLayout->addWidget(labelTitle);
    sectionLayout->addLayout(horizontalLayoutX);
    sectionLayout->addLayout(horizontalLayoutY);

    // Configurar el tamaño fijo de los spin boxes y etiquetas
    plateXSpinBox->setMinimumWidth(200);
    centerXSpinBox->setMinimumWidth(200);
    plateYSpinBox->setMinimumWidth(200);
    centerYSpinBox->setMinimumWidth(200);
    labelTitle->setMinimumHeight(43);

    // Crear y configurar el layout horizontal para la coordenada X del centro
    QHBoxLayout *horizontalLayoutCenterX = new QHBoxLayout;
    horizontalLayoutCenterX->setAlignment(Qt::AlignHCenter);
    horizontalLayoutCenterX->addWidget(labelCenterX);
    horizontalLayoutCenterX->addWidget(centerXSpinBox);

    // Crear y configurar el layout horizontal para la coordenada Y del centro
    QHBoxLayout *horizontalLayoutCenterY = new QHBoxLayout;
    horizontalLayoutCenterY->setAlignment(Qt::AlignHCenter);
    horizontalLayoutCenterY->addWidget(labelCenterY);
    horizontalLayoutCenterY->addWidget(centerYSpinBox);

    // Crear botón para ingresar manualmente las coordenadas del centro
    manualCenterButton = new QPushButton(tr("Enter Center Manually"), this);
    connect(manualCenterButton, &QPushButton::clicked, [=]() {
        showManualCenterInput(labelCenterX, labelCenterY);
    });

    // Inicialmente ocultar los spin boxes y etiquetas para el centro
    centerXSpinBox->setVisible(false);
    centerYSpinBox->setVisible(false);
    labelCenterX->setVisible(false);
    labelCenterY->setVisible(false);

    // Conectar señales para los cambios en los valores de los spin boxes
    connect(plateXSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &DimensionSection::valueChanged);
    connect(plateYSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &DimensionSection::valueChanged);
    connect(centerXSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &DimensionSection::valueChanged);
    connect(centerYSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &DimensionSection::valueChanged);

    // Conectar señales específicas para cambios en las dimensiones de la placa
    connect(plateXSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &DimensionSection::onPlateXChanged);
    connect(plateYSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &DimensionSection::onPlateYChanged);

    // Añadir el botón y layouts para las coordenadas del centro al layout de la sección
    sectionLayout->addWidget(manualCenterButton);
    sectionLayout->addLayout(horizontalLayoutCenterX);
    sectionLayout->addLayout(horizontalLayoutCenterY);

    // Configurar el tamaño fijo del widget de la sección
    dimensionSectionWidget->setFixedSize(570, 475);

    // Crear el layout principal y añadir el widget de la sección
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(sectionTitle);
    mainLayout->addWidget(dimensionSectionWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    setLayout(mainLayout);

    // Aplicar estilos
    applyStyles(true);
}

/**
 * @brief Actualiza los textos de la interfaz según el idioma actual.
 *
 * Este método se usa para ajustar el texto de los widgets cuando se cambia el idioma
 * de la interfaz.
 */
void DimensionSection::retranslateUi() {
    sectionTitle->setText(tr("Dimension Configuration"));
    labelTitle->setText(tr("Introduce Plate Size:"));
    manualCenterButton->setText(tr("Enter Center Manually"));
}

/**
 * @brief Obtiene el valor de la dimensión X de la placa.
 *
 * @return El valor actual del spin box de la dimensión X.
 */
int DimensionSection::getPlateXSpinBox() const {
    return plateXSpinBox->value();
}

/**
 * @brief Obtiene el valor de la dimensión Y de la placa.
 *
 * @return El valor actual del spin box de la dimensión Y.
 */
int DimensionSection::getPlateYSpinBox() const {
    return plateYSpinBox->value();
}

/**
 * @brief Obtiene el valor de la coordenada X del centro.
 *
 * @return El valor actual del spin box de la coordenada X del centro.
 */
int DimensionSection::getcenterXSpinBox() const {
    return centerXSpinBox->value();
}

/**
 * @brief Obtiene el valor de la coordenada Y del centro.
 *
 * @return El valor actual del spin box de la coordenada Y del centro.
 */
int DimensionSection::getcenterYSpinBox() const {
    return centerYSpinBox->value();
}

/**
 * @brief Muestra u oculta los campos para ingresar manualmente las coordenadas del centro.
 *
 * Alterna la visibilidad de los spin boxes y etiquetas para las coordenadas del centro
 * cuando se presiona el botón correspondiente.
 *
 * @param labelX Etiqueta para la coordenada X del centro.
 * @param labelY Etiqueta para la coordenada Y del centro.
 */
void DimensionSection::showManualCenterInput(QLabel *labelX, QLabel *labelY) {
    bool isVisible = centerXSpinBox->isVisible();
    centerXSpinBox->setVisible(!isVisible);
    centerYSpinBox->setVisible(!isVisible);
    labelX->setVisible(!isVisible);
    labelY->setVisible(!isVisible);
}

/**
 * @brief Actualiza los valores de los spin boxes con los valores proporcionados.
 *
 * Establece los valores de las dimensiones de la placa y las coordenadas del centro
 * en los spin boxes. Pemite cargar los valores de la configuración de guardado
 *
 * @param plateX Valor de la dimensión X de la placa.
 * @param plateY Valor de la dimensión Y de la placa.
 * @param centerX Valor de la coordenada X del centro.
 * @param centerY Valor de la coordenada Y del centro.
 */
void DimensionSection::updateValues(int plateX, int plateY, int centerX, int centerY) {
    plateXSpinBox->setValue(plateX);
    plateYSpinBox->setValue(plateY);
    centerXSpinBox->setValue(centerX);
    centerYSpinBox->setValue(centerY);
}

/**
 * @brief Aplica los estilos de la interfaz.
 *
 * Configura los estilos de los widgets
 *
 * @param darkMode Indica si el modo oscuro está habilitado.
 */
void DimensionSection::applyStyles(bool darkMode) {
    if (darkMode) {
        setStyleSheet(
            "QLabel {"
            "    font-size: 14px;"
            "    color: white;"
            "}"
            "QPushButton {"
            "    background-color: #555555;"
            "    color: white;"
            "    border: none;"
            "    border-radius: 5px;"
            "    font-size: 14px;"
            "    padding: 5px 10px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #001900;"
            "    color: white;"
            "    border: 1px solid #4CAF50; "
            "}"
            "QSpinBox {"
            "    border: 1px solid #555555;"
            "    border-radius: 3px;"
            "    padding: 5px;"
            "    font-size: 14px;"
            "    background-color: #2d2d2d;"
            "    color: white;"
            "}"
            "QSpinBox::up-button {"
            "    width: 0px;"
            "}"
            "QSpinBox::down-button {"
            "    width: 0px;"
            "}"
            "QSpinBox::up-button:hover {"
            "    background-color: #666666;"
            "}"
            "QSpinBox::down-button:hover {"
            "    background-color: #666666;"
            "}"
            "QWidget {"
            "    background-color: #212121;"
            "    padding: 10px;"
            "    border: 1px solid #555555;"
            "    border-radius: 10px;"
            "}"
            "QLabel#sectionTitle {"
            "    font-size: 20px;"
            "    color: #4CAF50;"
            "    font-weight: bold;"
            "    border: 1px solid #555555;"
            "    padding: 10px 5px;"
            "    background-color: #212121;"
            "}"
            );
    } else {
        setStyleSheet(
            "QLabel {"
            "    font-size: 14px;"
            "    color: #333;"
            "}"
            "QPushButton {"
            "    background-color: #4CAF50;"
            "    color: black;"
            "    border: none;"
            "    border-radius: 5px;"
            "    font-size: 14px;"
            "    padding: 5px 10px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #e6ffe6;"
            "    color: black;"
            "    border: 1px solid #4CAF50; "
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
            "}"
            "QLabel#sectionTitle {"
            "    font-size: 20px;"
            "    color: #4CAF50;"
            "    font-weight: bold;"
            "    border: 1px solid #ccc;"
            "    padding: 10px 5px;"
            "}"
            );
    }
}
