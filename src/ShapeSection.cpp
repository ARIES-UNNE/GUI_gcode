#include "ShapeSection.h"

ShapeSection::ShapeSection(QWidget *parent) : QWidget(parent) {
    // Create a main widget for the section
    QWidget *ShapeSectionWidget = new QWidget(this);

    sectionTitle = new QLabel(tr("Shape Configuration"), this);
    sectionTitle->setObjectName("sectionTitle");

    // Set uniform size for widgets
    int uniformSize = 200;

    // Initialize combo boxes for shape selection
    shapeComboBox = new QComboBox(this);
    shapeComboBox->setPlaceholderText(tr("Select a Shape"));
    shapeComboBox->addItem(tr("Circle"));
    shapeComboBox->addItem(tr("Square"));

    shapeComboBox2 = new QComboBox(this);
    shapeComboBox2->setPlaceholderText(tr("Select a Size"));
    shapeComboBox2->addItem(tr("Number of Layers"));
    shapeComboBox2->addItem(tr("Height"));

    // Initialize labels
    shapeLabel = new QLabel(tr("Shape Type"), this);
    sizeLabel = new QLabel(tr("Size Type"), this);

    // Initialize line edits for size input
    sizeLineEdit = new QLineEdit(this);
    sizeLineEdit->setPlaceholderText(tr("Enter size in mm"));

    sizeLineEdit2 = new QLineEdit(this);
    sizeLineEdit2->setPlaceholderText(tr("Enter"));

    mmLabel = new QLabel(tr("mm:"), this);
    mmLabel->setMaximumWidth(55);
    mmLabel->setVisible(false);

    sizeUnitLabel = new QLabel(this);
    sizeUnitLabel->setMaximumWidth(55);
    sizeUnitLabel->setVisible(false);

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
    horizontalLayout->addWidget(mmLabel);
    horizontalLayout->addWidget(sizeLineEdit);

    QHBoxLayout *horizontalLayout2 = new QHBoxLayout;
    horizontalLayout2->setAlignment(Qt::AlignHCenter);
    horizontalLayout2->addWidget(shapeComboBox2);
    horizontalLayout2->addWidget(sizeUnitLabel);
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
    applyStyles(false);
}

// Slot to handle shape selection changes
void ShapeSection::handleShapeSelection(int index) {
    // Show or hide size line edit based on selected shape
    bool showSizeLineEdit = (shapeComboBox->currentText() == tr("Square") || shapeComboBox->currentText() == tr("Circle"));
    sizeLineEdit->setVisible(showSizeLineEdit);


    mmLabel->setVisible(showSizeLineEdit && shapeComboBox->currentIndex() >= 0);

    // Show or hide additional size line edit based on second combo box selection
    bool showSizeLineEdit2 = (shapeComboBox2->currentText() == tr("Number of Layers") || shapeComboBox2->currentText() == tr("Height"));
    sizeLineEdit2->setVisible(showSizeLineEdit2);

    // Update placeholder text based on selected shape
    if (shapeComboBox->currentText() == tr("Square")) {
        sizeLineEdit->setPlaceholderText(tr("Side"));
    } else if (shapeComboBox->currentText() == tr("Circle")) {
        sizeLineEdit->setPlaceholderText(tr("Diameter"));
    }

    sizeUnitLabel->setVisible(showSizeLineEdit2 && shapeComboBox2->currentIndex() >= 0);

    // Update size unit label based on selected size type
    if (shapeComboBox2->currentText() == tr("Height")) {
        sizeUnitLabel->setText(tr("mm:"));
    } else if (shapeComboBox2->currentText() == tr("Number of Layers")) {
        sizeUnitLabel->setText(tr("N:"));
    }
}

void ShapeSection::updateValues(int shapeIndex1, int shapeIndex2, int size1, int size2) {
    shapeComboBox->setCurrentIndex(shapeIndex1);
    shapeComboBox2->setCurrentIndex(shapeIndex2);
    sizeLineEdit->setText(QString::number(size1));
    sizeLineEdit2->setText(QString::number(size2));
}


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
