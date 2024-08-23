#include "GenerateSection.h"


GenerateSection::GenerateSection(QWidget *parent) : QWidget(parent) {
    // Crear un layout vertical para la sección
    QVBoxLayout *layout = new QVBoxLayout(this);

    completionLabel = new QLabel(tr("GCODE Configuration Completed"), this);
    completionLabel->setAlignment(Qt::AlignCenter);

    // Botón para generar GCODE
    generateButton = new QPushButton(tr("CREATE SAVE CONFIG"), this);
    connect(generateButton, &QPushButton::clicked, this, [this]() {
        emit generateGcode();
    });
    generateButton->setFixedSize(200, 50);
    generateButton->setObjectName("generateButton");

    // Botón para guardar GCODE
    saveGcodeButton = new QPushButton(tr("SAVE GCODE"), this);
    connect(saveGcodeButton, &QPushButton::clicked, this, &GenerateSection::saveGcode);
    saveGcodeButton->setFixedSize(200, 50);
    saveGcodeButton->setObjectName("saveGcodeButton");

    // Add a small spacer item to reduce the space between buttons
    QSpacerItem *spacer = new QSpacerItem(30, 30, QSizePolicy::Minimum, QSizePolicy::Fixed);

    // Agregar la etiqueta y los botones al layout
    layout->addWidget(completionLabel);
    layout->addItem(spacer);
    layout->addWidget(generateButton);
    layout->addWidget(saveGcodeButton);
    layout->addItem(spacer);
    layout->setAlignment(completionLabel, Qt::AlignCenter);
    layout->setAlignment(generateButton, Qt::AlignCenter);
    layout->setAlignment(saveGcodeButton, Qt::AlignCenter);

    setLayout(layout);

    // Aplicar estilos iniciales
    applyStyles(false);
}

void GenerateSection::retranslateUi() {
    completionLabel->setText(tr("GCODE Configuration Completed"));
    generateButton->setText(tr("CREATE SAVE CONFIG"));
    saveGcodeButton->setText(tr("SAVE GCODE"));
}

// Método para copiar GCODE
void GenerateSection::saveGcode() {
    QString originalGcodePath = QCoreApplication::applicationDirPath() + "/Axo3_1mat.gcode";

    if (!QFile::exists(originalGcodePath)) {
        QMessageBox::warning(this, tr("Error"), tr("Original GCODE file not found."));
        return;
    }

    QString savePath = QFileDialog::getSaveFileName(this, tr("Save GCODE"), QDir::homePath(), tr("GCODE Files (*.gcode)"));

    if (!savePath.isEmpty()) {
        if (QFile::copy(originalGcodePath, savePath)) {
            QMessageBox::information(this, tr("Success"), tr("GCODE file saved successfully."));
        } else {
            QMessageBox::warning(this, tr("Error"), tr("Failed to save GCODE file."));
        }
    }
}

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
