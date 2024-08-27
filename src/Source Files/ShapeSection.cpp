#include "ShapeSection.h"

/**
 * @brief Constructor de la clase ShapeSection.
 *
 * Inicializa la interfaz de la sección de configuración de formas.
 *
 * @param parent El widget padre, por defecto es nullptr.
 */
ShapeSection::ShapeSection(QWidget *parent) : QWidget(parent) {
    // Crear un widget principal para contener todos los elementos de la sección.
    QWidget *ShapeSectionWidget = new QWidget(this);

    // Crear y configurar la etiqueta de título de la sección.
    sectionTitle = new QLabel(tr("Shape Configuration"), this);
    sectionTitle->setObjectName("sectionTitle");

    // Establecer un tamaño uniforme para varios widgets en la interfaz.
    int uniformSize = 200;

    // Crear y configurar el primer combo box para seleccionar el tipo de forma.
    shapeComboBox = new QComboBox(this);
    shapeComboBox->setPlaceholderText(tr("Select a Shape"));
    shapeComboBox->addItem(tr("Circle"));
    shapeComboBox->addItem(tr("Square"));

    // Crear y configurar el segundo combo box para seleccionar el tipo de tamaño.
    shapeComboBox2 = new QComboBox(this);
    shapeComboBox2->setPlaceholderText(tr("Select a Size"));
    shapeComboBox2->addItem(tr("Number of Layers"));
    shapeComboBox2->addItem(tr("Height"));

    // Crear y configurar las etiquetas descriptivas para los combo boxes.
    shapeLabel = new QLabel(tr("Shape Type"), this);
    sizeLabel = new QLabel(tr("Size Type"), this);

    // Crear y configurar el campo de entrada de texto para ingresar el tamaño de la forma.
    sizeLineEdit = new QLineEdit(this);
    sizeLineEdit->setPlaceholderText(tr("Enter size in mm"));
    sizeLineEdit2 = new QLineEdit(this);
    sizeLineEdit2->setPlaceholderText(tr("Enter"));

    // Crear y configurar etiquetas adicionales para unidades (como mm) que se mostrarán junto a los campos de texto.
    mmLabel = new QLabel(tr("mm:"), this);
    mmLabel->setMaximumWidth(55);
    mmLabel->setVisible(false);
    sizeUnitLabel = new QLabel(this);
    sizeUnitLabel->setMaximumWidth(55);
    sizeUnitLabel->setVisible(false);  //

    // Configuración de tamaño y alineación para las etiquetas de los combo boxes.
    shapeLabel->setMinimumWidth(uniformSize);
    shapeLabel->setMaximumWidth(uniformSize);
    sizeLabel->setMinimumWidth(uniformSize);
    sizeLabel->setMaximumWidth(uniformSize);
    shapeLabel->setMinimumHeight(43);
    sizeLabel->setMinimumHeight(43);

    // Configuración de tamaño para los combo boxes.
    shapeComboBox->setMinimumWidth(uniformSize);
    shapeComboBox->setMaximumWidth(uniformSize);
    shapeComboBox2->setMinimumWidth(uniformSize);
    shapeComboBox2->setMaximumWidth(uniformSize);

    // Configuración inicial para los campos de entrada de texto (inicialmente ocultos).
    sizeLineEdit2->setVisible(false);
    sizeLineEdit->setVisible(false);
    sizeLineEdit2->setMinimumWidth(uniformSize);
    sizeLineEdit2->setMaximumWidth(uniformSize);
    sizeLineEdit->setMinimumWidth(uniformSize);
    sizeLineEdit->setMaximumWidth(uniformSize);

    // Crear un validador numérico para asegurar que solo se ingresen números en los campos de texto.
    QDoubleValidator* validator = new QDoubleValidator(this);
    sizeLineEdit->setValidator(validator);  // Asignar el validador al primer campo de entrada.
    sizeLineEdit2->setValidator(validator);  // Asignar el validador al segundo campo de entrada.

    // Crear un layout vertical para organizar los elementos dentro del widget de la sección.
    QVBoxLayout *sectionLayout = new QVBoxLayout(ShapeSectionWidget);
    sectionLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    // Crear un layout horizontal para organizar los combo boxes y los campos de entrada en una sola línea.
    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->setAlignment(Qt::AlignHCenter);
    horizontalLayout->addWidget(shapeComboBox);
    horizontalLayout->addWidget(mmLabel);
    horizontalLayout->addWidget(sizeLineEdit);

    // Crear un segundo layout horizontal para el segundo combo box y su campo de entrada correspondiente.
    QHBoxLayout *horizontalLayout2 = new QHBoxLayout;
    horizontalLayout2->setAlignment(Qt::AlignHCenter);
    horizontalLayout2->addWidget(shapeComboBox2);
    horizontalLayout2->addWidget(sizeUnitLabel);
    horizontalLayout2->addWidget(sizeLineEdit2);

    // Crear un layout horizontal para la etiqueta de tipo de forma.
    QHBoxLayout *horizontalLayoutLabel = new QHBoxLayout;
    horizontalLayoutLabel->setAlignment(Qt::AlignHCenter);
    horizontalLayoutLabel->addWidget(shapeLabel);

    // Crear un layout vertical para la etiqueta de tipo de tamaño.
    QVBoxLayout *sectionLayout2 = new QVBoxLayout;
    sectionLayout2->setAlignment(Qt::AlignHCenter);
    sectionLayout2->addWidget(sizeLabel);

    // Añadir elementos al layout principal de la sección.
    sectionLayout->addItem(new QSpacerItem(20, 50, QSizePolicy::Minimum));
    sectionLayout->addLayout(horizontalLayoutLabel);
    sectionLayout->addLayout(horizontalLayout);
    sectionLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum));
    sectionLayout->addLayout(sectionLayout2);
    sectionLayout->addLayout(horizontalLayout2);

    // Conectar señales de los combo boxes y campos de texto con sus respectivos slots y funciones lambda.
    connect(shapeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ShapeSection::handleShapeSelection);
    connect(shapeComboBox2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ShapeSection::handleShapeSelection);

    // Emitir señal de cambio de valor cuando cambia el texto del campo de entrada.
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

    // Establecer un tamaño fijo para el widget de la sección para mantener la consistencia visual.
    ShapeSectionWidget->setFixedSize(570, 475);

    // Crear el layout principal para la sección y añadir el widget de la sección a él.
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(sectionTitle);
    mainLayout->addWidget(ShapeSectionWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    setLayout(mainLayout);

    // Aplicar los estilos iniciales (por defecto en modo claro).
    applyStyles(false);
}
/**
 * @brief Slot para manejar cambios en la selección de forma.
 *
 * Muestra u oculta los campos de entrada de tamaño según la forma seleccionada.
 *
 * @param index Índice de la forma seleccionada.
 */
void ShapeSection::handleShapeSelection(int index) {
    // Mostrar u ocultar el line edit de tamaño según la forma seleccionada
    bool showSizeLineEdit = (shapeComboBox->currentText() == tr("Square") || shapeComboBox->currentText() == tr("Circle"));
    sizeLineEdit->setVisible(showSizeLineEdit);

    mmLabel->setVisible(showSizeLineEdit && shapeComboBox->currentIndex() >= 0);

    // Mostrar u ocultar el segundo line edit de tamaño según la selección del segundo combo box
    bool showSizeLineEdit2 = (shapeComboBox2->currentText() == tr("Number of Layers") || shapeComboBox2->currentText() == tr("Height"));
    sizeLineEdit2->setVisible(showSizeLineEdit2);

    // Actualizar el texto del placeholder según la forma seleccionada
    if (shapeComboBox->currentText() == tr("Square")) {
        sizeLineEdit->setPlaceholderText(tr("Side"));
    } else if (shapeComboBox->currentText() == tr("Circle")) {
        sizeLineEdit->setPlaceholderText(tr("Diameter"));
    }

    sizeUnitLabel->setVisible(showSizeLineEdit2 && shapeComboBox2->currentIndex() >= 0);

    // Actualizar el texto de la etiqueta de unidad de tamaño según el tipo de tamaño seleccionado
    if (shapeComboBox2->currentText() == tr("Height")) {
        sizeUnitLabel->setText(tr("mm:"));
    } else if (shapeComboBox2->currentText() == tr("Number of Layers")) {
        sizeUnitLabel->setText(tr("N:"));
    }
}

/**
 * @brief Actualiza los valores en los campos de entrada de la sección.
 *
 * @param shapeIndex1 Índice del primer combo box (forma).
 * @param shapeIndex2 Índice del segundo combo box (tipo de tamaño).
 * @param size1 Valor del primer tamaño (por ejemplo, diámetro o lado).
 * @param size2 Valor del segundo tamaño (altura o número de capas).
 */
void ShapeSection::updateValues(int shapeIndex1, int shapeIndex2, int size1, int size2) {
    shapeComboBox->setCurrentIndex(shapeIndex1);
    shapeComboBox2->setCurrentIndex(shapeIndex2);
    sizeLineEdit->setText(QString::number(size1));
    sizeLineEdit2->setText(QString::number(size2));
}

/**
 * @brief Retraduce la interfaz de usuario de la sección.
 *
 * Actualiza los textos de los widgets para reflejar un cambio de idioma.
 */
void ShapeSection::retranslateUi() {
    sectionTitle->setText(tr("Shape Configuration"));
    shapeLabel->setText(tr("Shape Type"));
    sizeLabel->setText(tr("Size Type"));
    shapeComboBox->setItemText(0, tr("Circle"));
    shapeComboBox->setItemText(1, tr("Square"));
    shapeComboBox2->setItemText(0, tr("Number of Layers"));
    shapeComboBox2->setItemText(1, tr("Height"));
    shapeComboBox->setPlaceholderText(tr("Select a Shape"));
    shapeComboBox2->setPlaceholderText(tr("Select a Size"));
    sizeLineEdit->setPlaceholderText(tr("Enter size in mm"));
    sizeLineEdit2->setPlaceholderText(tr("Enter"));
}

/**
 * @brief Obtiene el valor del primer tamaño (por ejemplo, diámetro o lado).
 *
 * @return El valor del primer tamaño como entero.
 */
int ShapeSection::getSize1() const {
    return sizeLineEdit->text().toInt();
}

/**
 * @brief Obtiene el valor del segundo tamaño (por ejemplo, altura o número de capas).
 *
 * @return El valor del segundo tamaño como entero.
 */
int ShapeSection::getSize2() const {
    return sizeLineEdit2->text().toInt();
}

/**
 * @brief Obtiene el índice de la forma seleccionada en el primer combo box.
 *
 * @return El índice de la forma seleccionada como entero.
 */
int ShapeSection::getShapeIndex1() const {
    return shapeComboBox->currentIndex();
}

/**
 * @brief Obtiene el índice del tipo de tamaño seleccionado en el segundo combo box.
 *
 * @return El índice del tipo de tamaño seleccionado como entero.
 */
int ShapeSection::getShapeIndex2() const {
    return shapeComboBox2->currentIndex();
}

/**
 * @brief Aplica estilos a la sección.
 *
 * Cambia los estilos de la interfaz según el modo oscuro o claro.
 *
 * @param darkMode Indica si el modo oscuro está activado.
 */
void ShapeSection::applyStyles(bool darkMode) {
    if (darkMode) {
        setStyleSheet(
            "QPushButton {"
            "    background-color: #555555;"
            "    color: white;"
            "    border: none;"
            "    border-radius: 5px;"
            "    font-size: 14px;"
            "    padding: 5px 10px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #666666;"
            "}"
            "QLineEdit {"
            "    border: 1px solid #555555;"
            "    border-radius: 3px;"
            "    padding: 5px;"
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
            "QWidget {"
            "    background-color: #212121;"
            "    padding: 10px;"
            "    border: 1px solid #555555;"
            "    font-size: 14px;"
            "    border-radius: 10px;"
            "    color: white;"
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
            "QWidget {"
            "    background-color: #f9f9f9;"
            "    padding: 10px;"
            "    border: 1px solid #ccc;"
            "    font-size: 14px;"
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
