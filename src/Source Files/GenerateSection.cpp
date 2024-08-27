#include "GenerateSection.h"

/**
 * @class GenerateSection
 * @brief Widget para generar y guardar archivos GCODE.
 *
 * Permite al usuario generar y guardar archivos GCODE mediante botones de interfaz.
 */
GenerateSection::GenerateSection(QWidget *parent) : QWidget(parent) {
    // Crear un layout vertical para organizar los widgets en la sección
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Crear y configurar la etiqueta de finalización de configuración
    completionLabel = new QLabel(tr("GCODE Configuration Completed"), this);
    completionLabel->setAlignment(Qt::AlignCenter);

    // Crear y configurar el botón para generar GCODE
    generateButton = new QPushButton(tr("CREATE SAVE CONFIG"), this);
    connect(generateButton, &QPushButton::clicked, this, [this]() {
        emit generateGcode();
    });
    generateButton->setFixedSize(200, 50);
    generateButton->setObjectName("generateButton");

    // Crear y configurar el botón para guardar GCODE
    saveGcodeButton = new QPushButton(tr("SAVE GCODE"), this);
    connect(saveGcodeButton, &QPushButton::clicked, this, &GenerateSection::saveGcode);
    saveGcodeButton->setFixedSize(200, 50);
    saveGcodeButton->setObjectName("saveGcodeButton");

    // Agregar un espaciador para reducir el espacio entre los botones
    QSpacerItem *spacer = new QSpacerItem(30, 30, QSizePolicy::Minimum, QSizePolicy::Fixed);

    // Agregar la etiqueta y los botones al layout
    layout->addWidget(completionLabel);
    layout->addItem(spacer);
    layout->addWidget(generateButton);
    layout->addWidget(saveGcodeButton);
    layout->addItem(spacer);

    // Centrar los widgets en el layout
    layout->setAlignment(completionLabel, Qt::AlignCenter);
    layout->setAlignment(generateButton, Qt::AlignCenter);
    layout->setAlignment(saveGcodeButton, Qt::AlignCenter);

    // Establecer el layout en el widget
    setLayout(layout);

    // Aplicar estilos iniciales (modo oscuro por defecto)
    applyStyles(false);
}

/**
 * @brief Actualiza el texto de la interfaz según el idioma actual.
 *
 * Actualiza los textos de la etiqueta y los botones para reflejar el idioma seleccionado.
 */
void GenerateSection::retranslateUi() {
    completionLabel->setText(tr("GCODE Configuration Completed"));
    generateButton->setText(tr("CREATE SAVE CONFIG"));
    saveGcodeButton->setText(tr("SAVE GCODE"));
}

/**
 * @brief Copia el archivo GCODE original a una ubicación especificada por el usuario.
 *
 * Abre un cuadro de diálogo para que el usuario seleccione la ubicación de guardado y
 * copia el archivo GCODE original a esa ubicación. Muestra mensajes de éxito o error según corresponda.
 */
void GenerateSection::saveGcode() {
    // Ruta del archivo GCODE original
    QString originalGcodePath = QCoreApplication::applicationDirPath() + "/Axo3_1mat.gcode";

    // Verificar si el archivo GCODE original existe
    if (!QFile::exists(originalGcodePath)) {
        QMessageBox::warning(this, tr("Error"), tr("Original GCODE file not found."));
        return;
    }

    // Abrir un diálogo para que el usuario elija la ubicación de guardado
    QString savePath = QFileDialog::getSaveFileName(this, tr("Save GCODE"), QDir::homePath(), tr("GCODE Files (*.gcode)"));

    // Verificar si se ha especificado una ruta de guardado
    if (!savePath.isEmpty()) {
        // Intentar copiar el archivo GCODE a la nueva ubicación
        if (QFile::copy(originalGcodePath, savePath)) {
            QMessageBox::information(this, tr("Success"), tr("GCODE file saved successfully."));
        } else {
            QMessageBox::warning(this, tr("Error"), tr("Failed to save GCODE file."));
        }
    }
}

/**
 * @brief Aplica estilos a los widgets de la sección.
 *
 * Configura los estilos visuales de los widgets según el modo oscuro.
 *
 * @param darkMode Indica si se debe aplicar el modo oscuro (true) o el modo claro (false).
 */
void GenerateSection::applyStyles(bool darkMode) {
    if (darkMode) {
        setStyleSheet(
            "QLabel {"
            "    font-size: 24px;"
            "    color: white;"
            "}"
            "QPushButton#generateButton, QPushButton#saveGcodeButton {"
            "    background-color: #173418;"
            "    color: white;"
            "    border: none;"
            "    border-radius: 5px;"
            "    padding: 10px 24px;"
            "    font-size: 14px;"
            "}"
            "QPushButton#generateButton:hover, QPushButton#saveGcodeButton:hover {"
            "    background-color: #1e4620;"
            "    border: 1px solid #4CAF50;"
            "}"
            "QWidget {"
            "    background-color: #1a1a1a;"
            "}"
            );
    } else {
        setStyleSheet(
            "QLabel {"
            "    font-size: 24px;"
            "    color: #333;"
            "}"
            "QPushButton#generateButton, QPushButton#saveGcodeButton {"
            "    background-color: #4CAF50;"
            "    color: white;"
            "    border: none;"
            "    border-radius: 5px;"
            "    padding: 10px 24px;"
            "    font-size: 14px;"
            "}"
            "QPushButton#generateButton:hover, QPushButton#saveGcodeButton:hover {"
            "    background-color: #45a049;"
            "    border: 1px solid #4CAF50;"
            "}"
            "QWidget {"
            "    background-color: #f9f9f9;"
            "}"
            );
    }
}
