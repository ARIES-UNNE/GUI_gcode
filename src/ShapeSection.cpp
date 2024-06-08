#include "ShapeSection.h"

ShapeSection::ShapeSection(QWidget *parent) : QWidget(parent) {
    // Create a main widget for the section
    QWidget *ShapeSectionWidget = new QWidget(this);

    QLabel *sectionTitle = new QLabel("Shape Configuration", this);
    sectionTitle->setObjectName("sectionTitle");

    // Set uniform size for widgets
    int uniformSize = 200;

    // Initialize combo boxes for shape selection
    shapeComboBox = new QComboBox(this);
    shapeComboBox->setPlaceholderText("Select a Shape");
    shapeComboBox->addItem("Circle");
    shapeComboBox->addItem("Square");

    shapeComboBox2 = new QComboBox(this);
    shapeComboBox2->setPlaceholderText("Select a Size");
    shapeComboBox2->addItem("Number of Layers");
    shapeComboBox2->addItem("Height in mm");

    // Initialize labels
    QLabel *shapeLabel = new QLabel("Shape Type", this);
    QLabel *sizeLabel = new QLabel("Size Type", this);

    // Set size and alignment for labels
    shapeLabel->setMinimumWidth(uniformSize);
    shapeLabel->setMaximumWidth(uniformSize);
    sizeLabel->setMinimumWidth(uniformSize);
    sizeLabel->setMaximumWidth(uniformSize);
    shapeLabel->setMinimumHeight(43);
    sizeLabel->setMinimumHeight(43);

    // Set size for combo boxes
    shapeComboBox->setMinimumWidth(uniformSize);
    shapeComboBox->setMaximumWidth(uniformSize);
    shapeComboBox2->setMinimumWidth(uniformSize);
    shapeComboBox2->setMaximumWidth(uniformSize);

    // Initialize line edits for size input
    sizeLineEdit = new QLineEdit(this);
    sizeLineEdit->setPlaceholderText("Enter size in mm");

    sizeLineEdit2 = new QLineEdit(this);
    sizeLineEdit2->setPlaceholderText("Enter");

    // Line edits configuration
    sizeLineEdit2->setVisible(false);
    sizeLineEdit->setVisible(false);
    sizeLineEdit2->setMinimumWidth(uniformSize);
    sizeLineEdit2->setMaximumWidth(uniformSize);
    sizeLineEdit->setMinimumWidth(uniformSize);
    sizeLineEdit->setMaximumWidth(uniformSize);

    // Set validators for line edits (only numbers)
    QDoubleValidator* validator = new QDoubleValidator(this);
    sizeLineEdit->setValidator(validator);
    sizeLineEdit2->setValidator(validator);

    // Create layout for the section
    QVBoxLayout *sectionLayout = new QVBoxLayout(ShapeSectionWidget);
    sectionLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    // Create horizontal layouts for combo boxes and line edits
    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->setAlignment(Qt::AlignHCenter);
    horizontalLayout->addWidget(shapeComboBox);
    horizontalLayout->addWidget(sizeLineEdit);

    QHBoxLayout *horizontalLayout2 = new QHBoxLayout;
    horizontalLayout2->setAlignment(Qt::AlignHCenter);
    horizontalLayout2->addWidget(shapeComboBox2);
    horizontalLayout2->addWidget(sizeLineEdit2);

    QHBoxLayout *horizontalLayoutLabel = new QHBoxLayout;
    horizontalLayoutLabel->setAlignment(Qt::AlignHCenter);
    horizontalLayoutLabel->addWidget(shapeLabel);

    QVBoxLayout *sectionLayout2 = new QVBoxLayout;
    sectionLayout2->setAlignment(Qt::AlignHCenter);
    sectionLayout2->addWidget(sizeLabel);

    // Add elements to the main layout
    sectionLayout->addItem(new QSpacerItem(20, 50, QSizePolicy::Minimum));
    sectionLayout->addLayout(horizontalLayoutLabel);
    sectionLayout->addLayout(horizontalLayout);
    sectionLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum));
    sectionLayout->addLayout(sectionLayout2);
    sectionLayout->addLayout(horizontalLayout2);

    // Connect signals and slots
    connect(shapeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ShapeSection::handleShapeSelection);
    connect(shapeComboBox2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ShapeSection::handleShapeSelection);

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

    // Set fixed size for the section widget
    ShapeSectionWidget->setFixedSize(570, 475);

    // Create main layout and add the section widget to it
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(sectionTitle);
    mainLayout->addWidget(ShapeSectionWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    setLayout(mainLayout);

    // Apply styles
    applyStyles();
}

// Slot to handle shape selection changes
void ShapeSection::handleShapeSelection(int index) {
    // Show or hide size line edit based on selected shape
    bool showSizeLineEdit = (shapeComboBox->currentText() == "Square" || shapeComboBox->currentText() == "Circle");
    sizeLineEdit->setVisible(showSizeLineEdit);

    // Show or hide additional size line edit based on second combo box selection
    bool showSizeLineEdit2 = (shapeComboBox2->currentText() == "Number of Layers" || shapeComboBox2->currentText() == "Height in mm");
    sizeLineEdit2->setVisible(showSizeLineEdit2);

    // Update placeholder text based on selected shape
    if (shapeComboBox->currentText() == "Square") {
        sizeLineEdit->setPlaceholderText("Side in mm");
    } else if (shapeComboBox->currentText() == "Circle") {
        sizeLineEdit->setPlaceholderText("Diameter in mm");
    }
}

// Getter for size 1
int ShapeSection::getSize1() const {
    return sizeLineEdit->text().toInt();
}

// Getter for size 2
int ShapeSection::getSize2() const {
    return sizeLineEdit2->text().toInt();
}

// Getter for shape index 1
int ShapeSection::getShapeIndex1() const {
    return shapeComboBox->currentIndex();
}

// Getter for shape index 2
int ShapeSection::getShapeIndex2() const {
    return shapeComboBox2->currentIndex();
}

// Apply styles to the section
void ShapeSection::applyStyles() {
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
        "QComboBox QAbstractItemView {"
        "    border: 1px solid #ddd;"
        "    selection-background-color: #4CAF50;"
        "    selection-color: white;"
        "}"
        "QWidget {"
        "    background-color: #f9f9f9;"
        "    padding: 10px;"
        "    border: 1px solid #ccc;"
        "    font-size: 14px;"
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
