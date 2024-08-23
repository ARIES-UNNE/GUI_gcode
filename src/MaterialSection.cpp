#include "MaterialSection.h"

MaterialSection::MaterialSection(QWidget *parent) : QWidget(parent) {
    QWidget *MaterialSectionWidget = new QWidget(this);
    MaterialSectionWidget->setObjectName("section4Widget");

    numMaterialsSpinBox = new QSpinBox(this);
    numMaterialsSpinBox->setObjectName("numMaterialsSpinBox");
    numMaterialsSpinBox->setRange(0, 100);

    connect(numMaterialsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MaterialSection::updateMaterialNames);

    sectionTitle = new QLabel(tr("Number of Materials"), this);
    sectionTitle->setObjectName("plateSizeLabel4");

    QVBoxLayout *section4Layout = new QVBoxLayout(MaterialSectionWidget);
    section4Layout->setAlignment(Qt::AlignTop);
    MaterialSectionWidget->setLayout(section4Layout);

    section4Layout->addWidget(sectionTitle);
    section4Layout->addWidget(numMaterialsSpinBox);

    materialNamesContainer = new QWidget(this);
    materialNamesLayout = new QVBoxLayout(materialNamesContainer);
    materialNamesContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    QWidget *headerContainer = new QWidget(this);
    headerContainer->setObjectName("headerContainer");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerContainer);

    headerLabel1 = new QLabel(tr("Tecnology"), this);
    headerLabel1->setObjectName("headerLabel");
    headerLabel2 = new QLabel(tr("Filament µm"), this);
    headerLabel2->setObjectName("headerLabel");
    headerLabel3 = new QLabel(tr("Nozzle"), this);
    headerLabel3->setObjectName("headerLabel");

    headerLayout->addItem(new QSpacerItem(10, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    headerLayout->addWidget(headerLabel1);
    headerLayout->addItem(new QSpacerItem(10, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    headerLayout->addWidget(headerLabel2);
    headerLayout->addItem(new QSpacerItem(10, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    headerLayout->addWidget(headerLabel3);
    headerLayout->addItem(new QSpacerItem(10, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    section4Layout->addWidget(headerContainer);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(materialNamesContainer);
    scrollArea->setObjectName("materialScrollArea");

    section4Layout->addWidget(scrollArea);

    MaterialSectionWidget->setFixedSize(570, 535);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(MaterialSectionWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    setLayout(mainLayout);

    applyStyles(false);
}

void MaterialSection::retranslateUi() {
    sectionTitle->setText(tr("Number of Materials"));
    headerLabel1->setText(tr("Material"));
    headerLabel2->setText(tr("Nozzle"));
    headerLabel3->setText(tr("Filament µm"));
}

void MaterialSection::updateMaterialConfigs() {
    materialConfigs.clear();

    for (int i = 0; i < materialNamesLayout->count(); ++i) {
        QWidget *materialRowWidget = materialNamesLayout->itemAt(i)->widget();
        if (materialRowWidget) {
            QHBoxLayout *materialRowLayout = qobject_cast<QHBoxLayout*>(materialRowWidget->layout());
            if (materialRowLayout) {
                QComboBox *materialComboBox = qobject_cast<QComboBox*>(materialRowLayout->itemAt(0)->widget());
                QComboBox *filamentComboBox = qobject_cast<QComboBox*>(materialRowLayout->itemAt(1)->widget());
                QComboBox *nozzleComboBox = qobject_cast<QComboBox*>(materialRowLayout->itemAt(2)->widget());

                if (materialComboBox && nozzleComboBox && filamentComboBox) {
                    MaterialConfig config;
                    config.name = materialComboBox->currentText();  // Set the material name
                    config.filament = filamentComboBox->currentText(); // Set the filament amount
                    config.nozzle = nozzleComboBox->currentText(); // Set the nozzle size
                    materialConfigs.append(config);  // Add the config to the list
                }
            }
        }
    }
}

void MaterialSection::updateMaterialNames(int numMaterials) {
    QLayoutItem *child;
    while ((child = materialNamesLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    for (int i = 1; i <= numMaterials; ++i) {
        QWidget *materialRow = new QWidget(this);
        materialRow->setObjectName("materialRowWidget");
        QHBoxLayout *materialRowLayout = new QHBoxLayout(materialRow);

        QComboBox *materialComboBox = new QComboBox(this);
        materialComboBox->setObjectName("materialComboBox");
        materialComboBox->addItem(tr("Custom"));
        materialComboBox->addItem(tr("FDM - Granza"));
        materialComboBox->addItem(tr("FDM - Cartridge"));
        materialComboBox->addItem(tr("Extrusión de Pasta"));
        materialComboBox->addItem(tr("Extrusión de Pasta Calefactada"));
        materialComboBox->addItem(tr("MEW (Melt Electrowriting)"));

        QComboBox *filamentComboBox = new QComboBox(this);
        filamentComboBox->setObjectName("filamentComboBox");
        filamentComboBox->addItem(tr("Filamento (1.75 mm)"));
        filamentComboBox->addItem(tr("Arquímedes (1 mm)"));
        filamentComboBox->addItem(tr("Cartucho 3 cc (9.6 mm)"));
        filamentComboBox->addItem(tr("Cartucho 5 cc (12.7 mm)"));
        filamentComboBox->addItem(tr("Cartucho 10 cc (15.8 mm)"));
        filamentComboBox->addItem(tr("Jeringa 3 mL (8.7 mm)"));
        filamentComboBox->addItem(tr("Jeringa 5 mL (12.1 mm)"));

        QComboBox *nozzleComboBox = new QComboBox(this);
        nozzleComboBox->setObjectName("nozzleComboBox");
        nozzleComboBox->addItem(tr("100 µm"));
        nozzleComboBox->addItem(tr("200 µm"));
        nozzleComboBox->addItem(tr("300 µm"));
        nozzleComboBox->addItem(tr("400 µm"));
        nozzleComboBox->addItem(tr("840 µm"));
        nozzleComboBox->addItem(tr("10G (3000 µm, light green)"));
        nozzleComboBox->addItem(tr("11G (2500 µm, light orange)"));
        nozzleComboBox->addItem(tr("13G (2000 µm, light blue)"));
        nozzleComboBox->addItem(tr("14G (1550 µm, olive)"));
        nozzleComboBox->addItem(tr("15G (1360 µm, amber)"));
        nozzleComboBox->addItem(tr("16G (1200 µm, grey)"));
        nozzleComboBox->addItem(tr("18G (840 µm, green)"));
        nozzleComboBox->addItem(tr("20G (600 µm, pink)"));
        nozzleComboBox->addItem(tr("21G (510 µm, purple)"));
        nozzleComboBox->addItem(tr("22G (410 µm, blue)"));
        nozzleComboBox->addItem(tr("23G (340 µm, orange)"));
        nozzleComboBox->addItem(tr("25G (260 µm, red)"));
        nozzleComboBox->addItem(tr("27G (210 µm, transparent/black)"));
        nozzleComboBox->addItem(tr("30G (160 µm, transparent)"));

        materialComboBox->setMinimumWidth(150);
        filamentComboBox->setMinimumWidth(150);
        nozzleComboBox->setMinimumWidth(200);

        materialRowLayout->addWidget(materialComboBox);
        materialRowLayout->addWidget(filamentComboBox);
        materialRowLayout->addWidget(nozzleComboBox);

        materialNamesLayout->addWidget(materialRow);

        connect(materialComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MaterialSection::updateMaterialConfigs);
        connect(filamentComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MaterialSection::updateMaterialConfigs);
        connect(nozzleComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MaterialSection::updateMaterialConfigs);

        connect(materialComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=]() {
            updateMaterialPreset(materialComboBox, filamentComboBox, nozzleComboBox);
            emit valueChanged();  // Emitir señal cuando se cambia el material
        });

        connect(filamentComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=]() {
            emit valueChanged();  // Emitir señal cuando se cambia el filamento
        });

        connect(nozzleComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=]() {
            emit valueChanged();  // Emitir señal cuando se cambia la boquilla
        });
    }

    materialNamesContainer->update();

    updateMaterialConfigs();
    emit valueChanged();  // Emitir señal cuando se cambia el número de materiales
}

void MaterialSection::updateMaterialPreset(QComboBox *materialComboBox, QComboBox *filamentComboBox, QComboBox *nozzleComboBox) {
    QString selectedMaterial = materialComboBox->currentText();

    if (selectedMaterial == tr("FDM - Granza")) {
        filamentComboBox->setCurrentText(tr("Arquímedes (1 mm)"));
        nozzleComboBox->setCurrentText(tr("400 µm"));
        restrictNozzleOptions(nozzleComboBox, {"100 µm", "200 µm", "300 µm", "400 µm", "840 µm"});
    } else if (selectedMaterial == tr("FDM - Cartridge")) {
        filamentComboBox->setCurrentText(tr("Cartucho 3 cc (9.6 mm)"));
        nozzleComboBox->setCurrentText(tr("400 µm"));
        restrictNozzleOptions(nozzleComboBox, {"100 µm", "200 µm", "300 µm", "400 µm", "840 µm"});
    } else if (selectedMaterial == tr("Extrusión de Pasta")) {
        filamentComboBox->setCurrentText(tr("Cartucho 3 cc (9.6 mm)"));
        nozzleComboBox->setCurrentText(tr("22G (410 µm, blue)"));
        restrictNozzleOptions(nozzleComboBox, {
                                                  "10G (3000 µm, light green)", "11G (2500 µm, light orange)", "13G (2000 µm, light blue)",
                                                  "14G (1550 µm, olive)", "15G (1360 µm, amber)", "16G (1200 µm, grey)", "18G (840 µm, green)",
                                                  "20G (600 µm, pink)", "21G (510 µm, purple)", "22G (410 µm, blue)", "23G (340 µm, orange)",
                                                  "25G (260 µm, red)", "27G (210 µm, transparent/black)", "30G (160 µm, transparent)"
                                              });
    } else if (selectedMaterial == tr("Extrusión de Pasta Calefactada")) {
        filamentComboBox->setCurrentText(tr("Cartucho 3 cc (9.6 mm)"));
        nozzleComboBox->setCurrentText(tr("22G (410 µm, blue)"));
        restrictNozzleOptions(nozzleComboBox, {
                                                  "10G (3000 µm, light green)", "11G (2500 µm, light orange)", "13G (2000 µm, light blue)",
                                                  "14G (1550 µm, olive)", "15G (1360 µm, amber)", "16G (1200 µm, grey)", "18G (840 µm, green)",
                                                  "20G (600 µm, pink)", "21G (510 µm, purple)", "22G (410 µm, blue)", "23G (340 µm, orange)",
                                                  "25G (260 µm, red)", "27G (210 µm, transparent/black)", "30G (160 µm, transparent)"
                                              });
    } else if (selectedMaterial == tr("MEW (Melt Electrowriting)")) {
        filamentComboBox->setCurrentText(tr("Cartucho 3 cc (9.6 mm)"));
        nozzleComboBox->setCurrentText(tr("400 µm"));
        restrictNozzleOptions(nozzleComboBox, {"100 µm", "200 µm", "300 µm", "400 µm", "840 µm"});
    } else {
        restrictNozzleOptions(nozzleComboBox, {
                                                  "100 µm", "200 µm", "300 µm", "400 µm", "840 µm",
                                                  "10G (3000 µm, light green)", "11G (2500 µm, light orange)", "13G (2000 µm, light blue)",
                                                  "14G (1550 µm, olive)", "15G (1360 µm, amber)", "16G (1200 µm, grey)", "18G (840 µm, green)",
                                                  "20G (600 µm, pink)", "21G (510 µm, purple)", "22G (410 µm, blue)", "23G (340 µm, orange)",
                                                  "25G (260 µm, red)", "27G (210 µm, transparent/black)", "30G (160 µm, transparent)"
                                              });
    }
}

void MaterialSection::restrictNozzleOptions(QComboBox *nozzleComboBox, const QStringList &validOptions) {
    QString currentSelection = nozzleComboBox->currentText();

    nozzleComboBox->clear();

    for (const QString &option : validOptions) {
        nozzleComboBox->addItem(option);
    }

    int index = nozzleComboBox->findText(currentSelection);
    if (index != -1) {
        nozzleComboBox->setCurrentIndex(index);
    }
}


void MaterialSection::updateMaterialSection(const QList<QPair<QString, QString>> &materials) {
    numMaterialsSpinBox->setValue(materials.size());

    QLayoutItem *child;
    while ((child = materialNamesLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    for (const auto &material : materials) {
        QWidget *materialRow = new QWidget(this);
        materialRow->setObjectName("materialRowWidget");
        QHBoxLayout *materialRowLayout = new QHBoxLayout(materialRow);

        QComboBox *materialComboBox = new QComboBox(this);
        materialComboBox->setObjectName("materialComboBox");
        materialComboBox->addItem(tr("Custom"));
        materialComboBox->addItem(tr("FDM - Granza"));
        materialComboBox->addItem(tr("FDM - Cartridge"));
        materialComboBox->addItem(tr("Extrusión de Pasta"));
        materialComboBox->addItem(tr("Extrusión de Pasta Calefactada"));
        materialComboBox->addItem(tr("MEW (Melt Electrowriting)"));

        QComboBox *filamentComboBox = new QComboBox(this);
        filamentComboBox->setObjectName("filamentComboBox");
        filamentComboBox->addItem(tr("Filamento (1.75 mm)"));
        filamentComboBox->addItem(tr("Arquímedes (1 mm)"));
        filamentComboBox->addItem(tr("Cartucho 3 cc (9.6 mm)"));
        filamentComboBox->addItem(tr("Cartucho 5 cc (12.7 mm)"));
        filamentComboBox->addItem(tr("Cartucho 10 cc (15.8 mm)"));
        filamentComboBox->addItem(tr("Jeringa 3 mL (8.7 mm)"));
        filamentComboBox->addItem(tr("Jeringa 5 mL (12.1 mm)"));

        QComboBox *nozzleComboBox = new QComboBox(this);
        nozzleComboBox->setObjectName("nozzleComboBox");
        nozzleComboBox->addItem(tr("100 µm"));
        nozzleComboBox->addItem(tr("200 µm"));
        nozzleComboBox->addItem(tr("300 µm"));
        nozzleComboBox->addItem(tr("400 µm"));
        nozzleComboBox->addItem(tr("840 µm"));
        nozzleComboBox->addItem(tr("10G (3000 µm, light green)"));
        nozzleComboBox->addItem(tr("11G (2500 µm, light orange)"));
        nozzleComboBox->addItem(tr("13G (2000 µm, light blue)"));
        nozzleComboBox->addItem(tr("14G (1550 µm, olive)"));
        nozzleComboBox->addItem(tr("15G (1360 µm, amber)"));
        nozzleComboBox->addItem(tr("16G (1200 µm, grey)"));
        nozzleComboBox->addItem(tr("18G (840 µm, green)"));
        nozzleComboBox->addItem(tr("20G (600 µm, pink)"));
        nozzleComboBox->addItem(tr("21G (510 µm, purple)"));
        nozzleComboBox->addItem(tr("22G (410 µm, blue)"));
        nozzleComboBox->addItem(tr("23G (340 µm, orange)"));
        nozzleComboBox->addItem(tr("25G (260 µm, red)"));
        nozzleComboBox->addItem(tr("27G (210 µm, transparent/black)"));
        nozzleComboBox->addItem(tr("30G (160 µm, transparent)"));

        materialComboBox->setMinimumWidth(150);
        filamentComboBox->setMinimumWidth(150);
        nozzleComboBox->setMinimumWidth(200);

        QStringList parts = material.second.split(" | ");
        if (parts.size() == 2) {
            QString materialText = material.first;
            QString filamentText = parts.at(0);
            QString nozzleText = parts.at(1);

            int materialIndex = materialComboBox->findText(materialText);
            if (materialIndex != -1) {
                materialComboBox->setCurrentIndex(materialIndex);
            } else {
                materialComboBox->setCurrentIndex(0);
            }

            int filamentIndex = filamentComboBox->findText(filamentText);
            if (filamentIndex != -1) {
                filamentComboBox->setCurrentIndex(filamentIndex);
            }

            int nozzleIndex = nozzleComboBox->findText(nozzleText);
            if (nozzleIndex != -1) {
                nozzleComboBox->setCurrentIndex(nozzleIndex);
            }
        }

        materialRowLayout->addWidget(materialComboBox);
        materialRowLayout->addWidget(filamentComboBox);
        materialRowLayout->addWidget(nozzleComboBox);
        materialNamesLayout->addWidget(materialRow);

        connect(materialComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MaterialSection::updateMaterialConfigs);
        connect(filamentComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MaterialSection::updateMaterialConfigs);
        connect(nozzleComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MaterialSection::updateMaterialConfigs);

        connect(materialComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=]() {
            updateMaterialPreset(materialComboBox, filamentComboBox, nozzleComboBox);
            emit valueChanged();
        });

        connect(filamentComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=]() {
            emit valueChanged();
        });

        connect(nozzleComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=]() {
            emit valueChanged();
        });

        updateMaterialPreset(materialComboBox, filamentComboBox, nozzleComboBox);
    }

    materialNamesContainer->update();
}


QList<MaterialConfig> MaterialSection::getMaterialConfigs() const {
    return materialConfigs;
}

int MaterialSection::getNumMaterials() const {
    return numMaterialsSpinBox->value();
}

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
            "QSpinBox#numMaterialsSpinBox, QComboBox#nozzleComboBox, QComboBox#filamentComboBox,  QComboBox#materialComboBox {"
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
            "QSpinBox#numMaterialsSpinBox, QComboBox#nozzleComboBox, QComboBox#filamentComboBox, QComboBox#materialComboBox {"
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
