#include "DimensionSection.h"

DimensionSection::DimensionSection(QWidget *parent) : QWidget(parent) {

    QWidget *dimensionSectionWidget = new QWidget(this);

    sectionTitle = new QLabel(tr("Dimension Configuration"), this);
    sectionTitle->setObjectName("sectionTitle");

    // Initialize spin boxes
    plateXSpinBox = new QSpinBox(this);
    plateYSpinBox = new QSpinBox(this);
    centerXSpinBox = new QSpinBox(this);
    centerYSpinBox = new QSpinBox(this);

    centerXSpinBox->setRange(-150, 150);
    centerYSpinBox->setRange(-150, 150);

    // Create labels
    QLabel *labelX = new QLabel("X:", this);
    QLabel *labelY = new QLabel("Y:", this);
    labelTitle = new QLabel(tr("Introduce Plate Size:"), this);
    labelTitle->setAlignment(Qt::AlignCenter);

    // Create labels for center coordinates
    QLabel *labelCenterX = new QLabel("X:", this);
    QLabel *labelCenterY = new QLabel("Y:", this);

    // Create and set layout for the section
    QVBoxLayout *sectionLayout = new QVBoxLayout(dimensionSectionWidget);
    sectionLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    dimensionSectionWidget->setLayout(sectionLayout);

    // Create horizontal layout for the X dimension
    QHBoxLayout *horizontalLayoutX = new QHBoxLayout;
    horizontalLayoutX->setAlignment(Qt::AlignHCenter);
    plateXSpinBox->setMinimumWidth(200);
    horizontalLayoutX->addWidget(labelX);
    horizontalLayoutX->addWidget(plateXSpinBox);

    // Create horizontal layout for the Y dimension
    QHBoxLayout *horizontalLayoutY = new QHBoxLayout;
    horizontalLayoutY->setAlignment(Qt::AlignHCenter);
    plateYSpinBox->setMinimumWidth(200);
    horizontalLayoutY->addWidget(labelY);
    horizontalLayoutY->addWidget(plateYSpinBox);

    // Add vertical spacer and elements to the section layout
    sectionLayout->addItem(new QSpacerItem(20, 50, QSizePolicy::Minimum));
    sectionLayout->addWidget(labelTitle);
    sectionLayout->addLayout(horizontalLayoutX);
    sectionLayout->addLayout(horizontalLayoutY);

    // Set fixed size
    plateXSpinBox->setMinimumWidth(200);
    centerXSpinBox->setMinimumWidth(200);
    plateYSpinBox->setMinimumWidth(200);
    centerYSpinBox->setMinimumWidth(200);
    labelTitle->setMinimumHeight(43);

    // Create horizontal layout for the center X coordinate
    QHBoxLayout *horizontalLayoutCenterX = new QHBoxLayout;
    horizontalLayoutCenterX->setAlignment(Qt::AlignHCenter);
    horizontalLayoutCenterX->addWidget(labelCenterX);
    horizontalLayoutCenterX->addWidget(centerXSpinBox);

    // Create horizontal layout for the center Y coordinate
    QHBoxLayout *horizontalLayoutCenterY = new QHBoxLayout;
    horizontalLayoutCenterY->setAlignment(Qt::AlignHCenter);
    horizontalLayoutCenterY->addWidget(labelCenterY);
    horizontalLayoutCenterY->addWidget(centerYSpinBox);

    // Create button to manually input the center coordinates
    manualCenterButton = new QPushButton(tr("Enter Center Manually"), this);
    connect(manualCenterButton, &QPushButton::clicked, [=]() {
        showManualCenterInput(labelCenterX, labelCenterY);
    });

    // Initially hide the center spin boxes
    centerXSpinBox->setVisible(false);
    centerYSpinBox->setVisible(false);
    labelCenterX->setVisible(false);
    labelCenterY->setVisible(false);

    // Connect signals for value changes
    connect(plateXSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &DimensionSection::valueChanged);
    connect(plateYSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &DimensionSection::valueChanged);
    connect(centerXSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &DimensionSection::valueChanged);
    connect(centerYSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &DimensionSection::valueChanged);

    // Add elements to the section layout
    sectionLayout->addWidget(manualCenterButton);
    sectionLayout->addLayout(horizontalLayoutCenterX);
    sectionLayout->addLayout(horizontalLayoutCenterY);

    // Set fixed size for the section widget
    dimensionSectionWidget->setFixedSize(570, 475);

    // Create main layout and add the section widget to itS
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(sectionTitle);
    mainLayout->addWidget(dimensionSectionWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    setLayout(mainLayout);

    // Apply styles
    applyStyles(true);
}

void DimensionSection::retranslateUi() {
    sectionTitle->setText(tr("Dimension Configuration"));
    labelTitle->setText(tr("Introduce Plate Size:"));
    manualCenterButton->setText(tr("Enter Center Manually"));
}

// Getter method for plate X dimension
int DimensionSection::getPlateXSpinBox() const {
    return plateXSpinBox->value();
}

// Getter method for plate Y dimension
int DimensionSection::getPlateYSpinBox() const {
    return plateYSpinBox->value();
}

// Getter method for center X coordinate
int DimensionSection::getcenterXSpinBox() const {
    return centerXSpinBox->value();
}

// Getter method for center Y coordinate
int DimensionSection::getcenterYSpinBox() const {
    return centerYSpinBox->value();
}

// Show or hide the manual center input fields (when button pressed)
void DimensionSection::showManualCenterInput(QLabel *labelX, QLabel *labelY) {
    bool isVisible = centerXSpinBox->isVisible();
    centerXSpinBox->setVisible(!isVisible);
    centerYSpinBox->setVisible(!isVisible);
    labelX->setVisible(!isVisible);
    labelY->setVisible(!isVisible);
}

void DimensionSection::updateValues(int plateX, int plateY, int centerX, int centerY) {
    plateXSpinBox->setValue(plateX);
    plateYSpinBox->setValue(plateY);
    centerXSpinBox->setValue(centerX);
    centerYSpinBox->setValue(centerY);
}


// Apply styles to the section
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
