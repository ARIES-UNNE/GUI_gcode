#include "InfillSection.h"

/**
 * @brief Constructor de la clase InfillSection.
 *
 * Inicializa todos los elementos gráficos (widgets) de la sección de configuración de relleno.
 * Configura el diseño visual y establece las conexiones entre los widgets y las señales.
 *
 * @param parent Puntero al widget padre que contiene esta sección.
 */
InfillSection::InfillSection(QWidget *parent) : QWidget(parent) {
    // Inicializa el widget principal que contiene la sección
    QWidget *InfillSectionWidget = new QWidget(this);
    InfillSectionWidget->setObjectName("InfillSectionWidget");

    // Inicializa el título de la sección
    sectionTitle = new QLabel(tr("Infill Configuration"), this);
    sectionTitle->setObjectName("sectionTitle");

    // Define un tamaño uniforme para los widgets de entrada
    int uniformSize = 200;

    // Inicializa el spin box para el porcentaje de relleno
    infillSpinBox = new QSpinBox(this);
    infillSpinBox->setRange(0, 100);

    // Inicializa la línea de edición para la distancia entre hilos
    strandDistanceLineEdit = new QLineEdit(this);

    // Inicializa el combo box para la selección del método (Relleno o Distancia entre hilos)
    shapeComboBox = new QComboBox(this);
    shapeComboBox->setPlaceholderText(tr("Select"));
    shapeComboBox->addItem(tr("Infill"));
    shapeComboBox->addItem(tr("Strand Distance"));

    // Inicializa las etiquetas para los campos de entrada
    title = new QLabel(tr("Method Selection"), this);
    percentage = new QLabel("%: ", this);
    millimeters = new QLabel("mm: ", this);

    // Crea el layout vertical para la sección y lo alinea en la parte superior y centrado
    QVBoxLayout *section3Layout = new QVBoxLayout(InfillSectionWidget);
    section3Layout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    InfillSectionWidget->setLayout(section3Layout);

    // Configura el tamaño mínimo para los widgets de entrada y las etiquetas
    shapeComboBox->setMinimumWidth(uniformSize);
    infillSpinBox->setMinimumWidth(uniformSize);
    strandDistanceLineEdit->setMinimumWidth(uniformSize);
    strandDistanceLineEdit->setMaximumWidth(uniformSize);
    title->setMinimumWidth(uniformSize);
    title->setMinimumHeight(43);
    millimeters->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // Oculta inicialmente los widgets y etiquetas relacionados con el porcentaje y la distancia entre hilos
    percentage->setVisible(false);
    millimeters->setVisible(false);
    infillSpinBox->setVisible(false);
    strandDistanceLineEdit->setVisible(false);

    // Configura un validador para el campo de distancia entre hilos (solo permite valores numéricos válidos)
    QDoubleValidator* validator = new QDoubleValidator(this);
    strandDistanceLineEdit->setValidator(validator);

    // Crea layouts horizontales para organizar los widgets de entrada y sus etiquetas
    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    QHBoxLayout *horizontalLayout2 = new QHBoxLayout;

    horizontalLayout->addWidget(percentage);
    horizontalLayout->addWidget(infillSpinBox);
    horizontalLayout2->addWidget(millimeters);
    horizontalLayout2->addWidget(strandDistanceLineEdit);

    // Alinea los layouts horizontales al centro
    horizontalLayout->setAlignment(Qt::AlignCenter);
    horizontalLayout2->setAlignment(Qt::AlignCenter);

    // Crea un layout vertical para organizar el título y el combo box
    QVBoxLayout *titleAndComboBoxLayout = new QVBoxLayout;
    titleAndComboBoxLayout->setAlignment(Qt::AlignCenter);
    titleAndComboBoxLayout->addWidget(title);
    titleAndComboBoxLayout->addWidget(shapeComboBox);

    // Añade espaciadores y los layouts organizados al layout principal de la sección
    section3Layout->addItem(new QSpacerItem(20, 50, QSizePolicy::Minimum));
    section3Layout->addLayout(titleAndComboBoxLayout);
    section3Layout->addLayout(horizontalLayout);
    section3Layout->addLayout(horizontalLayout2);

    // Conecta señales y slots para manejar las interacciones del usuario
    connect(shapeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &InfillSection::handleMethodSelection);
    connect(infillSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &InfillSection::valueChanged);

    // Emite señal de cambio de valor cuando cambia el campo de entrada.
    connect(strandDistanceLineEdit, &QLineEdit::textChanged, [this]() {
        emit valueChanged();
    });
    connect(shapeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        emit valueChanged();
    });

    // Establece un tamaño fijo para el widget principal de la sección
    InfillSectionWidget->setFixedSize(570, 475);

    // Crea el layout principal del widget y lo alinea al centro
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(sectionTitle);
    mainLayout->addWidget(InfillSectionWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    setLayout(mainLayout);

    // Aplica los estilos iniciales (modo claro por defecto)
    applyStyles(false);
}

/**
 * @brief Actualiza los textos de la interfaz para soportar la internacionalización.
 *
 * Esta función actualiza los textos mostrados en la interfaz de usuario para soportar
 * diferentes idiomas mediante la función tr(). Se debe llamar a esta función cuando
 * se cambie el idioma de la aplicación.
 */
void InfillSection::retranslateUi() {
    sectionTitle->setText(tr("Infill Configuration"));
    title->setText(tr("Method Selection"));
    shapeComboBox->setPlaceholderText(tr("Select"));
    shapeComboBox->setItemText(0, tr("Infill"));
    shapeComboBox->setItemText(1, tr("Strand Distance"));
}

/**
 * @brief Actualiza los valores mostrados en los widgets según el método seleccionado.
 *
 * Dependiendo del método seleccionado (relleno o distancia entre hilos), esta función
 * actualiza los valores mostrados en los widgets correspondientes (QSpinBox o QLineEdit).
 *
 * @param methodIndex Índice del método seleccionado (0 para Relleno, 1 para Distancia entre hilos).
 * @param infillValue Valor de relleno a mostrar en el QSpinBox.
 * @param strandDistance Valor de la distancia entre hilos a mostrar en el QLineEdit.
 */
void InfillSection::updateValues(int methodIndex, int infillValue, double strandDistance) {
    shapeComboBox->setCurrentIndex(methodIndex);
    if (methodIndex == 0) {
        infillSpinBox->setValue(infillValue);
    } else if (methodIndex == 1) {
        strandDistanceLineEdit->setText(QString::number(strandDistance));
    }
}

/**
 * @brief Maneja los cambios en la selección del método en el combo box.
 *
 * Esta función se ejecuta cuando el usuario cambia la selección en el combo box.
 * Muestra los widgets correspondientes al método seleccionado y oculta los no relevantes.
 *
 * @param index Índice del método seleccionado en el combo box.
 */
void InfillSection::handleMethodSelection(int index) {
    // Oculta inicialmente todos los widgets
    infillSpinBox->setVisible(false);
    strandDistanceLineEdit->setVisible(false);
    percentage->setVisible(false);
    millimeters->setVisible(false);

    // Muestra los widgets adecuados según el método seleccionado
    if (index == 0) {
        infillSpinBox->setVisible(true);
        percentage->setVisible(true);
    } else if (index == 1) {
        strandDistanceLineEdit->setVisible(true);
        millimeters->setVisible(true);
    }
}

/**
 * @brief Obtiene el valor actual del relleno seleccionado.
 * @return Valor del relleno en porcentaje.
 */
int InfillSection::getInfillValue() const {
    return infillSpinBox->value();
}

/**
 * @brief Obtiene el índice de la forma seleccionada en el combo box.
 * @return Índice de la forma seleccionada.
 */
int InfillSection::getShapeIndex() const {
    return shapeComboBox->currentIndex();
}

/**
 * @brief Obtiene el valor actual de la distancia entre hilos ingresada.
 * @return Valor de la distancia entre hilos en milímetros.
 */
double InfillSection::getStrandDistanceValue() const {
    return strandDistanceLineEdit->text().toDouble();
}

/**
 * @brief Aplica los estilos visuales al widget, soportando modos claro y oscuro.
 * @param darkMode Booleano que indica si se debe aplicar el modo oscuro.
 */
void InfillSection::applyStyles(bool darkMode) {
    if (darkMode) {
        setStyleSheet(
            "QSpinBox, QLineEdit {"
            "    border: 1px solid #555555;"
            "    border-radius: 3px;"
            "    font-size: 14px;"
            "    background-color: #333333;"
            "    color: white;"
            "}"
            "QComboBox {"
            "    border: 1px solid #555555;"
            "    border-radius: 3px;"
            "    padding: 5px;"
            "    font-size: 14px;"
            "    background-color: #333333;"
            "    color: white;"
            "}"
            "QComboBox QAbstractItemView {"
            "    border: 1px solid #555555;"
            "    selection-background-color: #666666;"
            "    selection-color: white;"
            "}"
            "QWidget {"
            "    background-color: #212121;"
            "    padding: 10px;"
            "    border: 1px solid #555555;"
            "    border-radius: 10px;"
            "    font-size: 14px;"
            "    color: white;"
            "}"
            "QSpinBox::up-button {"
            "    width: 0px;"
            "}"
            "QSpinBox::down-button {"
            "    width: 0px;"
            "}"
            "QWidget#InfillSectionWidget {"
            "    background-color: #212121;"
            "    padding: 10px;"
            "    border: 1px solid #555555;"
            "    border-radius: 10px;"
            "    font-size: 14px;"
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
            "QSpinBox, QLineEdit {"
            "    border: 1px solid #ddd;"
            "    border-radius: 3px;"
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
            "    font-size: 14px;"
            "}"
            "QSpinBox::up-button {"
            "    width: 0px;"

            "}"
            "QSpinBox::down-button {"
            "    width: 0px;"
            "}"
            "QWidget#InfillSectionWidget {"
            "    background-color: #f9f9f9;"
            "    padding: 10px;"
            "    border: 1px solid #ccc;"
            "    border-radius: 10px;"
            "    font-size: 14px;"
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
