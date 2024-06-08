#include "InfillSection.h"

InfillSection::InfillSection(QWidget *parent) : QWidget(parent) {

    // Create a main widget for the section
    QWidget *InfillSectionWidget = new QWidget(this);
    InfillSectionWidget->setObjectName("InfillSectionWidget");

    QLabel *sectionTitle = new QLabel("Infill Configuration", this);
    sectionTitle->setObjectName("sectionTitle");

    // Set uniform size
    int uniformSize = 200;

    // Initialize infill spin box and strand distance line edit
    infillSpinBox = new QSpinBox(this);
    infillSpinBox->setRange(0, 100);

    strandDistanceLineEdit = new QLineEdit(this);

    // Create layout for the section
    QVBoxLayout *section3Layout = new QVBoxLayout(InfillSectionWidget);
    section3Layout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    InfillSectionWidget->setLayout(section3Layout);

    // Initialize combo box for method selection
    shapeComboBox = new QComboBox(this);
    shapeComboBox->setPlaceholderText("Select");
    shapeComboBox->addItem("Infill");
    shapeComboBox->addItem("Strand Distance");

    // Initialize labels
    QLabel *title = new QLabel("Method Selection", this);
    percentage = new QLabel("%: ", this);
    millimeters = new QLabel("mm: ", this);

    // Size configuration
    shapeComboBox->setMinimumWidth(uniformSize);
    shapeComboBox->setMaximumWidth(uniformSize);
    infillSpinBox->setMinimumWidth(uniformSize);
    strandDistanceLineEdit->setMinimumWidth(uniformSize);
    strandDistanceLineEdit->setMaximumWidth(uniformSize);
    title->setMinimumWidth(uniformSize);
    title->setMaximumWidth(uniformSize);
    title->setMinimumHeight(43);
    millimeters->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // Initially hide labels
    percentage->setVisible(false);
    millimeters->setVisible(false);
    infillSpinBox->setVisible(false);
    strandDistanceLineEdit->setVisible(false);

    // Set validator for strand distance line edit
    QDoubleValidator* validator = new QDoubleValidator(this);
    strandDistanceLineEdit->setValidator(validator);

    // Create horizontal layouts for widgets
    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    QHBoxLayout *horizontalLayout2 = new QHBoxLayout;

    horizontalLayout->addWidget(percentage);
    horizontalLayout->addWidget(infillSpinBox);

    horizontalLayout2->addWidget(millimeters);
    horizontalLayout2->addWidget(strandDistanceLineEdit);

    horizontalLayout->setAlignment(Qt::AlignCenter);
    horizontalLayout2->setAlignment(Qt::AlignCenter);

    // Create layout for title and combo box
    QVBoxLayout *titleAndComboBoxLayout = new QVBoxLayout;
    titleAndComboBoxLayout->setAlignment(Qt::AlignCenter);
    titleAndComboBoxLayout->addWidget(title);
    titleAndComboBoxLayout->addWidget(shapeComboBox);

    // Add elements to the main layout
    section3Layout->addItem(new QSpacerItem(20, 50, QSizePolicy::Minimum));
    section3Layout->addLayout(titleAndComboBoxLayout);
    section3Layout->addLayout(horizontalLayout);
    section3Layout->addLayout(horizontalLayout2);

    // Connect signals and slots
    connect(shapeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &InfillSection::handleMethodSelection);
    connect(infillSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &InfillSection::valueChanged);
    connect(strandDistanceLineEdit, &QLineEdit::textChanged, [this]() {
        emit valueChanged();
    });
    connect(shapeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        emit valueChanged();
    });

    // Set fixed size for the section widget
    InfillSectionWidget->setFixedSize(570, 475);

    // Create main layout and add the section widget to it
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(sectionTitle);
    mainLayout->addWidget(InfillSectionWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    setLayout(mainLayout);

    // Apply styles
    applyStyles();
}

// Slot to handle method selection changes
void InfillSection::handleMethodSelection(int index) {
    // Hide all widgets initially
    infillSpinBox->setVisible(false);
    strandDistanceLineEdit->setVisible(false);
    percentage->setVisible(false);
    millimeters->setVisible(false);

    // Show the appropriate widgets based on the selected method
    if (index == 0) {
        infillSpinBox->setVisible(true);
        percentage->setVisible(true);
    } else if (index == 1) {
        strandDistanceLineEdit->setVisible(true);
        millimeters->setVisible(true);
    }
}

// Getter for infill value
int InfillSection::getInfillValue() const {
    return infillSpinBox->value();
}

// Getter for selected shape index
int InfillSection::getShapeIndex() const {
    return shapeComboBox->currentIndex();
}

// Getter for strand distance value
double InfillSection::getStrandDistanceValue() const {
    return strandDistanceLineEdit->text().toDouble();
}

// Apply styles to the section
void InfillSection::applyStyles() {
    setStyleSheet(
        "QSpinBox, QLineEdit {"
        "    border: 1px solid #ddd;"
        "    border-radius: 3px;"
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
        "    border-radius: 10px;"
        "    font-size: 14px;"
        "}"
        "QSpinBox::up-button {"
        "    width: 0px;"
        "}"
        "QSpinBox::down-button {"
        "    width: 0px;"
        "}"
        "QWidget#InfillSectionWidget {"
        "    background-color: #f9f9f9;"
        "    padding: 10px;"
        "    border: 1px solid #ccc;"
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
