#include "startpage.h"

/**
 * @brief Constructor de SectionStart.
 *
 * Inicializa los widgets de la página de inicio y configura el diseño.
 * Crea un combo box para seleccionar el idioma, una etiqueta de bienvenida y un botón de inicio.
 * Conecta las señales de los widgets a sus slots correspondientes.
 *
 * @param parent Widget padre, si lo hay.
 */
SectionStart::SectionStart(QWidget *parent) : QWidget(parent) {
    // Crear un combo box para seleccionar el idioma
    languageComboBox = new QComboBox(this);
    languageComboBox->addItem("English", "en");
    languageComboBox->addItem("Español", "es");

    // Conectar el cambio de idioma a una señal que emitirá el código del idioma seleccionado
    connect(languageComboBox, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        QString languageCode = languageComboBox->currentData().toString();
        emit languageChanged(languageCode);
    });

    // Crear una etiqueta de bienvenida
    welcomeLabel = new QLabel(tr("3D Printing App for GCODE Generation"), this);
    welcomeLabel->setAlignment(Qt::AlignCenter);

    // Crear un botón de inicio
    startButton = new QPushButton(tr("Start"), this);

    // Conectar el clic del botón de inicio a su slot correspondiente
    connect(startButton, &QPushButton::clicked, this, &SectionStart::startButtonClicked);

    // Crear un layout horizontal para colocar el combo box del idioma y un espaciador
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(languageComboBox, 0, Qt::AlignLeft);
    topLayout->addStretch();

    // Crear un layout vertical para organizar el combo box, la etiqueta y el botón de inicio
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topLayout);
    mainLayout->addStretch();
    mainLayout->addWidget(welcomeLabel, 0, Qt::AlignCenter);

    // Agregar un espaciador para separar la etiqueta del botón de inicio
    mainLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // Agregar el botón de inicio al centro y agregar más espacio flexible en la parte inferior
    mainLayout->addWidget(startButton, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    // Configurar el layout del widget
    setLayout(mainLayout);
}

/**
 * @brief Retraduce los textos de los widgets según el idioma actual.
 *
 * Actualiza el texto de la etiqueta y el botón para reflejar los textos traducidos.
 */
void SectionStart::retranslateUi() {
    welcomeLabel->setText(tr("3D Printing App for GCODE Generation"));
    startButton->setText(tr("Start"));
}

/**
 * @brief Aplica estilos a los widgets del widget de la página de inicio.
 *
 * Configura los estilos según si el modo oscuro está activado o no.
 *
 * @param darkMode Indica si se debe aplicar el modo oscuro.
 */
void SectionStart::applyStyles(bool darkMode) {
    if (darkMode) {
        languageComboBox->setStyleSheet(
            "QComboBox {"
            "    border: 1px solid #555555;"
            "    border-radius: 5px;"
            "    padding: 5px;"
            "    font-size: 14px;"
            "    color: white;"
            "    background-color: #2d2d2d;"
            "}"
            "QComboBox::drop-down {"
            "    border: none;"
            "}"
            "QComboBox QAbstractItemView {"
            "    border: 1px solid #555555;"
            "    selection-background-color: #666666;"
            "    selection-color: white;"
            "    background-color: #2d2d2d;"
            "}"
            "QComboBox QAbstractItemView::item {"
            "    color: white;"
            "    background-color: #2d2d2d;"
            "}"
            "QComboBox QAbstractItemView::item:selected {"
            "    background-color: #666666;"
            "    color: white;"
            "}"
            );

        welcomeLabel->setStyleSheet("font-size: 24px; color: #ccc;");

        startButton->setStyleSheet(
            "QPushButton {"
            "    background-color: #173418;"
            "    color: white;"
            "    border: none;"
            "    border-radius: 5px;"
            "    padding: 10px 24px;"
            "    font-size: 16px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #1e4620;"
            "    border: 1px solid #4CAF50;"
            "}"
            );
    } else {
        languageComboBox->setStyleSheet(
            "QComboBox {"
            "    border: 1px solid #4CAF50;"
            "    border-radius: 5px;"
            "    padding: 5px;"
            "    font-size: 14px;"
            "}"
            "QComboBox::drop-down {"
            "    border: none;"
            "}"
            "QComboBox QAbstractItemView {"
            "    border: 1px solid #4CAF50;"
            "    selection-background-color: #45a049;"
            "    selection-color: white;"
            "}"
            );

        welcomeLabel->setStyleSheet("font-size: 24px; color: #333;");

        startButton->setStyleSheet(
            "QPushButton {"
            "    background-color: #4CAF50;"
            "    color: white;"
            "    border: none;"
            "    border-radius: 5px;"
            "    padding: 10px 24px;"
            "    font-size: 16px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #45a049;"
            "    border: 1px solid #4CAF50;"
            "}"
            );
    }
}
