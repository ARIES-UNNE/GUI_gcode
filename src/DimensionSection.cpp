#include "DimensionSection.h"

DimensionSection::DimensionSection(QWidget *parent) : QWidget(parent) {

    QWidget *dimensionSectionWidget = new QWidget(this);

    QLabel *sectionTitle = new QLabel("Dimension Configuration", this);
    sectionTitle->setObjectName("sectionTitle");

    // Initialize spin boxes
    plateXSpinBox = new QSpinBox(this);
    plateYSpinBox = new QSpinBox(this);
    centerXSpinBox = new QSpinBox(this);
    centerYSpinBox = new QSpinBox(this);


    // Create labels
    QLabel *labelX = new QLabel("X:", this);
    QLabel *labelY = new QLabel("Y:", this);
    QLabel *labelTitle = new QLabel("Introduce Plate Size:", this);
    labelTitle->setAlignment(Qt::AlignCenter);

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

    // Create labels for center coordinates
    QLabel *labelCenterX = new QLabel("X:", this);
    QLabel *labelCenterY = new QLabel("Y:", this);

    // Initially hide the center spin boxes
    centerXSpinBox->setVisible(false);
    centerYSpinBox->setVisible(false);
    labelCenterX->setVisible(false);
    labelCenterY->setVisible(false);

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
    QPushButton *manualCenterButton = new QPushButton("Enter Center Manually", this);
    connect(manualCenterButton, &QPushButton::clicked, [=]() {
        showManualCenterInput(labelCenterX, labelCenterY);
    });

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

    // Create main layout and add the section widget to it
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(sectionTitle);
    mainLayout->addWidget(dimensionSectionWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    setLayout(mainLayout);

    // Apply styles
    applyStyles();
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

// Apply styles to the section
void DimensionSection::applyStyles() {
    setStyleSheet(
        "QLabel {"
        "    font-size: 14px;"
        "    color: #333;"
        "}"
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

