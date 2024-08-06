#include "GenerateSection.h"


GenerateSection::GenerateSection(QWidget *parent) : QWidget(parent) {
    // Create a vertical layout for the section
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Create a label to display the completion message
    completionLabel = new QLabel(tr("GCODE Configuration Completed"), this);
    completionLabel->setAlignment(Qt::AlignCenter);

    // Create button to generate GCODE
    generateButton = new QPushButton(tr("SAVE GCODE"), this);
    connect(generateButton, &QPushButton::clicked, this, [this]() {
        emit generateGcode();
    });
    generateButton->setFixedSize(200, 50);
    generateButton->setObjectName("generateButton");

    // Add the label and button to the layout
    layout->addWidget(completionLabel);
    layout->addWidget(generateButton);
    layout->setAlignment(completionLabel, Qt::AlignCenter);
    layout->setAlignment(generateButton, Qt::AlignCenter);

    setLayout(layout);

    // Apply initial styles
    applyStyles(false);
}

void GenerateSection::retranslateUi() {
    completionLabel->setText(tr("GCODE Configuration Completed"));
    generateButton->setText(tr("GENERATE GCODE"));
}

void GenerateSection::applyStyles(bool darkMode) {
    if (darkMode) {
        setStyleSheet(
            "QLabel {"
            "    font-size: 24px;"
            "    color: white;"
            "}"
            "QPushButton#generateButton {"
            "    background-color: #173418;"
            "    color: white;"
            "    border: none;"
            "    border-radius: 5px;"
            "    padding: 10px 24px;"
            "    font-size: 14px;"
            "}"
            "QPushButton#generateButton:hover {"
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
            "QPushButton#generateButton {"
            "    background-color: #4CAF50;"
            "    color: white;"
            "    border: none;"
            "    border-radius: 5px;"
            "    padding: 10px 24px;"
            "    font-size: 14px;"
            "}"
            "QPushButton#generateButton:hover {"
            "    background-color: #45a049;"
            "    border: 1px solid #4CAF50;"
            "}"
            "QWidget {"
            "    background-color: #f9f9f9;"
            "}"
            );
    }
}
