#include "startpage.h"

SectionStart::SectionStart(QWidget *parent) : QWidget(parent) {
    // Crear el combo box para la selección de idioma
    languageComboBox = new QComboBox(this);
    languageComboBox->addItem("English", "en");
    languageComboBox->addItem("Español", "es");

    // Conectar la señal de cambio de texto del combo box
    connect(languageComboBox, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        QString languageCode = languageComboBox->currentData().toString();
        emit languageChanged(languageCode);
    });

    // Crear una etiqueta de bienvenida
    welcomeLabel = new QLabel(tr("3D Printing App for GCODE Generation"), this);
    welcomeLabel->setAlignment(Qt::AlignCenter);

    // Crear un botón de inicio
    startButton = new QPushButton(tr("Start"), this);

    // Conectar la señal de clic del botón de inicio
    connect(startButton, &QPushButton::clicked, this, &SectionStart::startButtonClicked);

    // Crear un layout horizontal para colocar el combo box en la parte superior izquierda
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(languageComboBox, 0, Qt::AlignLeft);
    topLayout->addStretch();  // Añadir un estiramiento para empujar el combo box hacia la izquierda

    // Crear un layout vertical principal
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topLayout);
    mainLayout->addStretch();
    mainLayout->addWidget(welcomeLabel, 0, Qt::AlignCenter);

    // Añadir un espaciador entre el welcomeLabel y el startButton
    mainLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    mainLayout->addWidget(startButton, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    setLayout(mainLayout);  // Establecer el layout principal para el widget
}

void SectionStart::retranslateUi() {
    welcomeLabel->setText(tr("3D Printing App for GCODE Generation"));
    startButton->setText(tr("Start"));
}

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
