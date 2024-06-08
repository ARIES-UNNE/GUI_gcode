#include "MaterialSection.h"


// Constructor for the MaterialSection class
MaterialSection::MaterialSection(QWidget *parent) : QWidget(parent) {
    // Create the main widget for the section
    QWidget *MaterialSectionWidget = new QWidget(this);
    MaterialSectionWidget->setObjectName("section4Widget");

    // Initialize the spin box to select the number of materials
    numMaterialsSpinBox = new QSpinBox(this);
    numMaterialsSpinBox->setRange(0, 100);

    // Create a label for the number of materials
    QLabel *sectionTitle = new QLabel("Number of Materials", this);
    sectionTitle->setObjectName("plateSizeLabel4");

    // Create and set layout for the section
    QVBoxLayout *section4Layout = new QVBoxLayout(MaterialSectionWidget);
    section4Layout->setAlignment(Qt::AlignTop);
    MaterialSectionWidget->setLayout(section4Layout);

    // Connect the spin box value change signal to the slot
    connect(numMaterialsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MaterialSection::updateMaterialNames);

    // Add the label and spin box to the layout
    section4Layout->addWidget(sectionTitle);
    section4Layout->addWidget(numMaterialsSpinBox);

    // Create the container and layout for material names
    materialNamesContainer = new QWidget(this);
    materialNamesLayout = new QVBoxLayout(materialNamesContainer);
    materialNamesContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    // Create a header container and layout for the material properties
    QWidget *headerContainer = new QWidget(this);
    QHBoxLayout *headerLayout = new QHBoxLayout(headerContainer);

    // Header Labels
    QLabel *headerLabel1 = new QLabel("Material", this);
    QLabel *headerLabel2 = new QLabel("Nozzle", this);
    QLabel *headerLabel3 = new QLabel("Filament mm", this);
    QLabel *headerLabel4 = new QLabel("Additional Field", this);

    // Add header labels to the layout
    headerLayout->addItem(new QSpacerItem(10, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    headerLayout->addWidget(headerLabel1);
    headerLayout->addItem(new QSpacerItem(10, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    headerLayout->addWidget(headerLabel2);
    headerLayout->addItem(new QSpacerItem(10, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    headerLayout->addWidget(headerLabel3);
    headerLayout->addItem(new QSpacerItem(10, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    headerLayout->addWidget(headerLabel4);
    headerLayout->addItem(new QSpacerItem(10, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // Add the header container to the layout
    section4Layout->addWidget(headerContainer);

    // Create a scroll area for the material names container
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(materialNamesContainer);
    scrollArea->setObjectName("materialScrollArea");

    // Add the scroll area to the layout
    section4Layout->addWidget(scrollArea);

    // Set fixed size for the section widget
    MaterialSectionWidget->setFixedSize(570, 535);

    // Create main layout and add the section widget to it
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(MaterialSectionWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    setLayout(mainLayout);

    // Apply styles
    applyStyles();
}

// Function to update the material configurations based on the input fields
void MaterialSection::updateMaterialConfigs() {
    // Clear the current material configurations list
    materialConfigs.clear();

    // Iterate through each item in the material names layout
    for (int i = 0; i < materialNamesLayout->count(); ++i) {
        // Get the widget at the current position
        QWidget *materialRowWidget = materialNamesLayout->itemAt(i)->widget();
        if (materialRowWidget) {
            // Get the layout of the material row widget
            QHBoxLayout *materialRowLayout = qobject_cast<QHBoxLayout*>(materialRowWidget->layout());
            if (materialRowLayout) {
                // Retrieve the child widgets from the material row layout
                QLabel *materialLabel = qobject_cast<QLabel*>(materialRowLayout->itemAt(0)->widget());
                QComboBox *nozzleComboBox = qobject_cast<QComboBox*>(materialRowLayout->itemAt(1)->widget());
                QLineEdit *filamentLineEdit = qobject_cast<QLineEdit*>(materialRowLayout->itemAt(2)->widget());
                QLineEdit *additionalLineEdit = qobject_cast<QLineEdit*>(materialRowLayout->itemAt(3)->widget());

                // If all the widgets are valid, create a new MaterialConfig object
                if (materialLabel && nozzleComboBox && filamentLineEdit && additionalLineEdit) {
                    MaterialConfig config;
                    config.name = materialLabel->text();  // Set the material name
                    config.nozzleSize = nozzleComboBox->currentIndex();  // Set the nozzle size (index of the combo box)
                    config.filamentAmount = filamentLineEdit->text().toDouble();  // Set the filament amount
                    config.algo = additionalLineEdit->text().toDouble();  // Set the additional field value
                    materialConfigs.append(config);  // Add the config to the list
                }
            }
        }
    }
}

// Function to update the material names and input fields based on the number of materials
void MaterialSection::updateMaterialNames(int numMaterials) {
    // Clear existing material rows
    QLayoutItem *child;
    while ((child = materialNamesLayout->takeAt(0)) != nullptr) {
        // Remove and delete the child widget from the layout
        delete child->widget();
        delete child;
    }

    // Add new material rows based on the number of materials specified
    for (int i = 1; i <= numMaterials; ++i) {
        // Create a new widget for each material row
        QWidget *materialRow = new QWidget(this);
        materialRow->setObjectName("materialRowWidget");
        QHBoxLayout *materialRowLayout = new QHBoxLayout(materialRow);

        // Create and configure the material label, combo box, and line edits
        QLabel *materialLabel = new QLabel("Material " + QString::number(i), this);
        QComboBox *nozzleComboBox = new QComboBox(this);
        nozzleComboBox->addItem("0.2 mm");
        nozzleComboBox->addItem("0.4 mm");
        nozzleComboBox->addItem("0.6 mm");
        QLineEdit *filamentLineEdit = new QLineEdit(this);
        filamentLineEdit->setValidator(new QDoubleValidator(this));
        QLineEdit *additionalLineEdit = new QLineEdit(this);
        additionalLineEdit->setValidator(new QDoubleValidator(this));

        // Set the minimum and maximum width for the widgets
        materialLabel->setMinimumWidth(70);
        materialLabel->setMaximumWidth(70);
        nozzleComboBox->setMinimumWidth(70);
        nozzleComboBox->setMaximumWidth(70);
        filamentLineEdit->setMinimumWidth(70);
        filamentLineEdit->setMaximumWidth(70);
        additionalLineEdit->setMinimumWidth(70);
        additionalLineEdit->setMaximumWidth(70);

        // Add the widgets to the material row layout
        materialRowLayout->addWidget(materialLabel);
        materialRowLayout->addWidget(nozzleComboBox);
        materialRowLayout->addWidget(filamentLineEdit);
        materialRowLayout->addWidget(additionalLineEdit);

        // Add the material row layout to the main material names layout
        materialNamesLayout->addWidget(materialRow);

        // Connect signals for value changes to the updateMaterialConfigs slot
        connect(nozzleComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MaterialSection::updateMaterialConfigs);
        connect(filamentLineEdit, &QLineEdit::textChanged, this, &MaterialSection::updateMaterialConfigs);
        connect(additionalLineEdit, &QLineEdit::textChanged, this, &MaterialSection::updateMaterialConfigs);
    }

    // Update the material names container
    materialNamesContainer->update();

    // Update the material configurations
    updateMaterialConfigs();
}

// Getter to retrieve the list of material configurations
QList<MaterialConfig> MaterialSection::getMaterialConfigs() const {
    return materialConfigs;
}

// Getter to retrieve the number of materials
int MaterialSection::getNumMaterials() const {
    return numMaterialsSpinBox->value();
}

// Apply styles to the section
void MaterialSection::applyStyles() {
    setStyleSheet(
        "QLabel {"
        "    font-size: 14px;"
        "    color: #333;"
        "}"
        "QLabel#plateSizeLabel4 {"
        "    font-size: 18px;"
        "    color: #4CAF50;"
        "    font-weight: bold;"
        "    padding: 5px 0;"
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
        "QSpinBox, QComboBox, QLineEdit {"
        "    border: 1px solid #ddd;"
        "    border-radius: 3px;"
        "    padding: 5px;"
        "    font-size: 14px;"
        "}"
        "QSpinBox::up-button, QSpinBox::down-button {"
        "    width: 0px;"
        "}"
        "QSpinBox::up-button:hover, QSpinBox::down-button:hover {"
        "    background-color: #ddd;"
        "}"
        "QComboBox::drop-down {"
        "    border: none;"
        "}"
        "QComboBox QAbstractItemView {"
        "    border: 1px solid #ddd;"
        "    selection-background-color: #4CAF50;"
        "    selection-color: white;"
        "}"
        "QWidget#section4Widget {"
        "    background-color: #f9f9f9;"
        "    padding: 10px;"
        "    border: 1px solid #ccc;"
        "    border-radius: 10px;"
        "}"
        "QWidget#materialRowWidget {"
        "    background-color: #ffffff;"
        "    padding: 5px;"
        "    border: 1px solid #ddd;"
        "    border-radius: 5px;"
        "}"
        "QScrollArea#materialScrollArea {"
        "    border: 1px solid #ccc;"
        "    border-radius: 10px;"
        "}"
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: #f1f1f1;"
        "    width: 12px;"
        "    margin: 1px 2px 1px 2px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #d3d3d3;"
        "    min-height: 10px;"
        "    border-radius: 4px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "}"
        );
}
