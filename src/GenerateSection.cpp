#include "GenerateSection.h"

GenerateSection::GenerateSection(QWidget *parent) : QWidget(parent) {
    // Create a vertical layout for the section
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Create a label to display the completion message
    QLabel *completionLabel = new QLabel("GCODE Configuration Completed", this);
    completionLabel->setAlignment(Qt::AlignCenter);
    completionLabel->setStyleSheet("font-size: 24px; color: #333;");  // Apply styling

    // Add the label to the layout
    layout->addWidget(completionLabel);
    layout->setAlignment(completionLabel, Qt::AlignCenter);

    setLayout(layout);
}
