#include "MaterialSection.h"

// Constructor for the MaterialSection class
MaterialSection::MaterialSection(QWidget *parent) : QWidget(parent) {
    // Create the main widget for the section
    QWidget *MaterialSectionWidget = new QWidget(this);
    MaterialSectionWidget->setObjectName("section4Widget");

    // Initialize the spin box to select the number of materials
    numMaterialsSpinBox = new QSpinBox(this);
    numMaterialsSpinBox->setObjectName("numMaterialsSpinBox");
    numMaterialsSpinBox->setRange(0, 100);

    // Create a label for the number of materials
    sectionTitle = new QLabel(tr("Number of Materials"), this);
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
    headerContainer->setObjectName("headerContainer");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerContainer);

    // Header Labels
    headerLabel1 = new QLabel(tr("Material"), this);
    headerLabel1->setObjectName("headerLabel");
    headerLabel2 = new QLabel(tr("Nozzle"), this);
    headerLabel2->setObjectName("headerLabel");
    headerLabel3 = new QLabel(tr("Filament mm"), this);
    headerLabel3->setObjectName("headerLabel");
    headerLabel4 = new QLabel(tr("Additional Field"), this);
    headerLabel4->setObjectName("headerLabel");

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
    applyStyles(false);
}

void MaterialSection::retranslateUi() {
    sectionTitle->setText(tr("Number of Materials"));
    headerLabel1->setText(tr("Material"));
    headerLabel2->setText(tr("Nozzle"));
    headerLabel3->setText(tr("Filament mm"));
    headerLabel4->setText(tr("Additional Field"));
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
        QLabel *materialLabel = new QLabel(tr("Material ") + QString::number(i), this);
        materialLabel->setObjectName("materialLabel");
        QComboBox *nozzleComboBox = new QComboBox(this);
        nozzleComboBox->setObjectName("nozzleComboBox");
        nozzleComboBox->addItem(tr("0.2 mm"));
        nozzleComboBox->addItem(tr("0.4 mm"));
        nozzleComboBox->addItem(tr("0.6 mm"));
        QLineEdit *filamentLineEdit = new QLineEdit(this);
        filamentLineEdit->setObjectName("filamentLineEdit");
        filamentLineEdit->setValidator(new QDoubleValidator(this));
        QLineEdit *additionalLineEdit = new QLineEdit(this);
        additionalLineEdit->setObjectName("additionalLineEdit");
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
void MaterialSection::applyStyles(bool darkMode) {
    if (darkMode) {
        setStyleSheet(
            "QLabel {"
            "    font-size: 14px;"
            "    color: white;"
            "    background-color: #212121;"
            "}"
            "QLabel#plateSizeLabel4 {"
            "    font-size: 18px;"
            "    color: #4CAF50;"
            "    font-weight: bold;"
            "    padding: 5px 0;"
            "    background-color: #212121;"
            "}"
            "QLabel#headerLabel {"
            "    font-size: 14px;"
            "    color: white;"
            "    background-color: #212121;"
            "}"
            "QLabel#materialLabel {"
            "    font-size: 14px;"
            "    color: white;"
            "    background-color: #212121;"
            "}"
            "QSpinBox#numMaterialsSpinBox, QComboBox#nozzleComboBox, QLineEdit#filamentLineEdit, QLineEdit#additionalLineEdit {"
            "    border: 1px solid #555555;"
            "    border-radius: 3px;"
            "    padding: 5px;"
            "    font-size: 14px;"
            "    background-color: #212121;"
            "    color: white;"
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
            "QWidget#section4Widget {"
            "    background-color: #212121;"
            "    padding: 10px;"
            "    border: 1px solid #555555;"
            "    border-radius: 10px;"
            "}"
            "QWidget#materialRowWidget {"
            "    background-color: #212121;"
            "    padding: 5px;"
            "    border: 1px solid #555555;"
            "    border-radius: 5px;"
            "}"
            "QScrollArea#materialScrollArea {"
            "    background-color: #1a1a1a;"
            "    border: 1px solid #555555;"
            "    border-radius: 5px;"
            "}"
            "QScrollBar:vertical {"
            "    border: none;"
            "    background: #1a1a1a;"
            "    width: 12px;"
            "    margin: 1px 2px 1px 2px;"
            "}"
            "QScrollBar::handle:vertical {"
            "    background: #212121;"
            "    min-height: 10px;"
            "    border-radius: 4px;"
            "}"
            "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
            "    height: 0px;"
            "}"
            "QWidget {"
            "    background-color: #1a1a1a;"
            "    color: white;"
            "}"
            "QWidget#headerContainer {"
            "    background-color: #212121;"
            "}"
            );
    } else {
        setStyleSheet(
            "QLabel {"
            "    font-size: 14px;"
            "    color: #333;"
            "    background-color: transparent;"
            "}"
            "QLabel#plateSizeLabel4 {"
            "    font-size: 18px;"
            "    color: #4CAF50;"
            "    font-weight: bold;"
            "    padding: 5px 0;"
            "    background-color: transparent;"
            "}"
            "QLabel#headerLabel {"
            "    font-size: 14px;"
            "    color: #333;"
            "    background-color: transparent;"
            "}"
            "QLabel#materialLabel {"
            "    font-size: 14px;"
            "    color: #333;"
            "    background-color: transparent;"
            "}"
            "QSpinBox#numMaterialsSpinBox, QComboBox#nozzleComboBox, QLineEdit#filamentLineEdit, QLineEdit#additionalLineEdit {"
            "    border: 1px solid #ddd;"
            "    border-radius: 3px;"
            "    padding: 5px;"
            "    font-size: 14px;"
            "    background-color: #ffffff;"
            "    color: #333;"
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
}
