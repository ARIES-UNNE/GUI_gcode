#include "MaterialSection.h"

/**
 * @brief Constructor de la clase MaterialSection.
 *
 * Inicializa el widget de la sección de materiales, configurando el título,
 * el spin box para el número de materiales y los layouts necesarios.
 *
 * @param parent El widget padre.
 */
MaterialSection::MaterialSection(QWidget *parent) : QWidget(parent) {
    // Crea el widget principal de la sección de materiales.
    QWidget *MaterialSectionWidget = new QWidget(this);
    MaterialSectionWidget->setObjectName("section4Widget");

    // Inicializa el spin box para seleccionar el número de materiales.
    numMaterialsSpinBox = new QSpinBox(this);
    numMaterialsSpinBox->setObjectName("numMaterialsSpinBox");
    numMaterialsSpinBox->setRange(0, 100);

    // Conecta el cambio en el spin box para actualizar los nombres de materiales.
    connect(numMaterialsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MaterialSection::updateMaterialNames);

    // Configura la etiqueta que muestra el título de la sección.
    sectionTitle = new QLabel(tr("Number of Materials"), this);
    sectionTitle->setObjectName("plateSizeLabel4");

    // Crea el layout vertical para la sección de materiales y añade los widgets.
    QVBoxLayout *section4Layout = new QVBoxLayout(MaterialSectionWidget);
    section4Layout->setAlignment(Qt::AlignTop);
    MaterialSectionWidget->setLayout(section4Layout);
    section4Layout->addWidget(sectionTitle);
    section4Layout->addWidget(numMaterialsSpinBox);

    // Crea el contenedor y el layout para los nombres de materiales.
    materialNamesContainer = new QWidget(this);
    materialNamesLayout = new QVBoxLayout(materialNamesContainer);
    materialNamesContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    // Crea el contenedor y layout para los encabezados de las columnas.
    QWidget *headerContainer = new QWidget(this);
    headerContainer->setObjectName("headerContainer");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerContainer);

    // Configura las etiquetas de encabezado para cada columna.
    headerLabel1 = new QLabel(tr("Tecnology"), this);
    headerLabel1->setObjectName("headerLabel");
    headerLabel2 = new QLabel(tr("Filament µm"), this);
    headerLabel2->setObjectName("headerLabel");
    headerLabel3 = new QLabel(tr("Nozzle"), this);
    headerLabel3->setObjectName("headerLabel");

    // Añade espaciadores y etiquetas al layout de encabezado.
    headerLayout->addItem(new QSpacerItem(50, 0, QSizePolicy::Minimum));
    headerLayout->addWidget(headerLabel1);
    headerLayout->addItem(new QSpacerItem(10, 0, QSizePolicy::Minimum));
    headerLayout->addWidget(headerLabel2);
    headerLayout->addItem(new QSpacerItem(50, 0, QSizePolicy::Minimum));
    headerLayout->addWidget(headerLabel3);
    headerLayout->addItem(new QSpacerItem(1, 0,  QSizePolicy::Minimum));

    // Añade el contenedor de encabezado al layout principal de la sección.
    section4Layout->addWidget(headerContainer);

    // Crea y configura un área de desplazamiento para los nombres de materiales.
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(materialNamesContainer);
    scrollArea->setObjectName("materialScrollArea");

    // Añade el área de desplazamiento al layout principal de la sección.
    section4Layout->addWidget(scrollArea);

    // Establece un tamaño fijo para el widget de la sección de materiales.
    MaterialSectionWidget->setFixedSize(570, 535);

    // Crea el layout principal y añade el widget de la sección de materiales.
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(MaterialSectionWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    setLayout(mainLayout);

    // Aplica los estilos (por defecto, claro).
    applyStyles(false);
}

/**
 * @brief Retraduce el texto de los elementos de la interfaz de usuario.
 *
 * Actualiza los textos de los encabezados y el título de la sección.
 */
void MaterialSection::retranslateUi() {
    sectionTitle->setText(tr("Number of Materials"));
    headerLabel1->setText(tr("Tecnology"));
    headerLabel2->setText(tr("Nozzle"));
    headerLabel3->setText(tr("Filament µm"));
}

/**
 * @brief Actualiza la lista de configuraciones de materiales según los combos seleccionados.
 *
 * Recorre los widgets de fila de material, obtiene los valores seleccionados de los combo boxes
 * y actualiza la lista de configuraciones.
 */
void MaterialSection::updateMaterialConfigs() {
    // Limpia la lista de configuraciones de materiales actual.
    materialConfigs.clear();

    // Recorre cada fila de material en el layout.
    for (int i = 0; i < materialNamesLayout->count(); ++i) {
        QWidget *materialRowWidget = materialNamesLayout->itemAt(i)->widget();
        if (materialRowWidget) {
            QHBoxLayout *materialRowLayout = qobject_cast<QHBoxLayout*>(materialRowWidget->layout());
            if (materialRowLayout) {
                // Obtiene los combo boxes de material, filamento y boquilla de la fila.
                QComboBox *materialComboBox = qobject_cast<QComboBox*>(materialRowLayout->itemAt(0)->widget());
                QComboBox *filamentComboBox = qobject_cast<QComboBox*>(materialRowLayout->itemAt(1)->widget());
                QComboBox *nozzleComboBox = qobject_cast<QComboBox*>(materialRowLayout->itemAt(2)->widget());

                // Si todos los combo boxes están presentes, guarda los valores seleccionados.
                if (materialComboBox && nozzleComboBox && filamentComboBox) {
                    MaterialConfig config;
                    config.name = materialComboBox->currentText();  // Establece el nombre del material.
                    config.filament = filamentComboBox->currentText(); // Establece el filamento seleccionado.
                    config.nozzle = nozzleComboBox->currentText(); // Establece el tamaño de la boquilla.
                    materialConfigs.append(config);  // Añade la configuración a la lista.
                }
            }
        }
    }
}

/**
 * @brief Crea los widgets de cada material en función del número de materiales.
 *
 * Elimina los widgets antiguos y crea nuevos widgets de fila para cada material.
 * Configura los combo boxes con opciones y conecta las señales para actualizar configuraciones.
 *
 * @param numMaterials Número de materiales a mostrar.
 */
void MaterialSection::updateMaterialNames(int numMaterials) {
    // Elimina todos los widgets actuales del layout de nombres de materiales.
    QLayoutItem *child;
    while ((child = materialNamesLayout->takeAt(0)) != nullptr) {
        delete child->widget(); // Elimina el widget.
        delete child;          // Elimina el contenedor del layout.
    }

    // Crea widgets de fila para el número especificado de materiales.
    for (int i = 1; i <= numMaterials; ++i) {
        // Crea un nuevo widget para la fila del material.
        QWidget *materialRow = new QWidget(this);
        materialRow->setObjectName("materialRowWidget");
        QHBoxLayout *materialRowLayout = new QHBoxLayout(materialRow);

        // Crea y configura los combo boxes para material, filamento y boquilla.
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

        // Ajusta el ancho mínimo de los combo boxes.
        materialComboBox->setMinimumWidth(150);
        filamentComboBox->setMinimumWidth(150);
        nozzleComboBox->setMinimumWidth(200);

        // Añade los combo boxes al layout de la fila.
        materialRowLayout->addWidget(materialComboBox);
        materialRowLayout->addWidget(filamentComboBox);
        materialRowLayout->addWidget(nozzleComboBox);

        // Añade la fila al layout de nombres de materiales.
        materialNamesLayout->addWidget(materialRow);

        // Conecta las señales de cambio en los combo boxes para actualizar configuraciones.
        connect(materialComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MaterialSection::updateMaterialConfigs);
        connect(filamentComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MaterialSection::updateMaterialConfigs);
        connect(nozzleComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MaterialSection::updateMaterialConfigs);

        // Conecta las señales de cambio para emitir una señal cuando cambien los valores.
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

    // Actualiza el contenedor de nombres de materiales después de añadir los widgets.
    materialNamesContainer->update();

    // Actualiza la lista de configuraciones de materiales y emite una señal de cambio.
    updateMaterialConfigs();
    emit valueChanged();  // Emitir señal cuando se cambia el número de materiales
}


/**
 * @brief Actualiza los valores predeterminados de los combo boxes en función del material seleccionado.
 *
 * Configura el combo box de filamento y boquilla según el material seleccionado y restringe las opciones válidas
 * para la boquilla en función del material.
 *
 * @param materialComboBox ComboBox que contiene los tipos de materiales.
 * @param filamentComboBox ComboBox que contiene los tipos de filamentos.
 * @param nozzleComboBox ComboBox que contiene los tamaños de boquillas.
 */
void MaterialSection::updateMaterialPreset(QComboBox *materialComboBox, QComboBox *filamentComboBox, QComboBox *nozzleComboBox) {
    QString selectedMaterial = materialComboBox->currentText();

    // Dependiendo del material seleccionado, establece valores predeterminados para filamento y boquilla
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
        // Si el material seleccionado no coincide con ninguno de los anteriores, establece todas las opciones.
        restrictNozzleOptions(nozzleComboBox, {
                                                  "100 µm", "200 µm", "300 µm", "400 µm", "840 µm",
                                                  "10G (3000 µm, light green)", "11G (2500 µm, light orange)", "13G (2000 µm, light blue)",
                                                  "14G (1550 µm, olive)", "15G (1360 µm, amber)", "16G (1200 µm, grey)", "18G (840 µm, green)",
                                                  "20G (600 µm, pink)", "21G (510 µm, purple)", "22G (410 µm, blue)", "23G (340 µm, orange)",
                                                  "25G (260 µm, red)", "27G (210 µm, transparent/black)", "30G (160 µm, transparent)"
                                              });
    }
}

/**
 * @brief Restringe las opciones de boquilla disponibles en el combo box.
 *
 * Elimina las opciones actuales y añade solo las opciones válidas proporcionadas. Establece la selección actual
 * en función de la selección anterior si es válida.
 *
 * @param nozzleComboBox ComboBox que contiene los tamaños de boquilla.
 * @param validOptions Lista de opciones válidas para el combo box de boquilla.
 */
void MaterialSection::restrictNozzleOptions(QComboBox *nozzleComboBox, const QStringList &validOptions) {
    QString currentSelection = nozzleComboBox->currentText();

    nozzleComboBox->clear();  // Elimina todas las opciones actuales del combo box.

    // Añade las opciones válidas al combo box.
    for (const QString &option : validOptions) {
        nozzleComboBox->addItem(option);
    }

    // Establece la selección actual si la opción estaba disponible.
    int index = nozzleComboBox->findText(currentSelection);
    if (index != -1) {
        nozzleComboBox->setCurrentIndex(index);
    }
}

/**
 * @brief Carga dentro del scroll area la informacion de la configuración de guardado en caso de que este activa.
 *
 * Limpia los widgets existentes, crea y configura nuevos widgets de fila para cada material en la lista proporcionada.
 * Conecta las señales para cada combo box para que las actualizaciones en uno de ellos actualicen los otros.
 *
 * @param materials Lista de pares de materiales donde cada par contiene el nombre del material y sus propiedades.
 */
void MaterialSection::updateMaterialSection(const QList<QPair<QString, QString>> &materials) {
    numMaterialsSpinBox->setValue(materials.size());  // Establece el número de materiales en el spin box.

    // Elimina los widgets existentes del layout de nombres de materiales.
    QLayoutItem *child;
    while ((child = materialNamesLayout->takeAt(0)) != nullptr) {
        delete child->widget();  // Elimina el widget.
        delete child;          // Elimina el contenedor del layout.
    }

    // Crea un nuevo widget para cada material en la lista.
    for (const auto &material : materials) {
        QWidget *materialRow = new QWidget(this);
        materialRow->setObjectName("materialRowWidget");
        QHBoxLayout *materialRowLayout = new QHBoxLayout(materialRow);

        // Crea y configura los combo boxes para material, filamento y boquilla.
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

        // Ajusta el ancho mínimo de los combo boxes.
        materialComboBox->setMinimumWidth(150);
        filamentComboBox->setMinimumWidth(150);
        nozzleComboBox->setMinimumWidth(200);

        // Configura los valores de los combo boxes en función de los datos proporcionados.
        QStringList parts = material.second.split(" | ");
        if (parts.size() == 2) {
            QString materialText = material.first;
            QString filamentText = parts.at(0);
            QString nozzleText = parts.at(1);

            int materialIndex = materialComboBox->findText(materialText);
            if (materialIndex != -1) {
                materialComboBox->setCurrentIndex(materialIndex);
            } else {
                materialComboBox->setCurrentIndex(0);  // Establece "Custom" si el material no se encuentra.
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

        // Añade los combo boxes al layout de la fila y la fila al layout de nombres de materiales.
        materialRowLayout->addWidget(materialComboBox);
        materialRowLayout->addWidget(filamentComboBox);
        materialRowLayout->addWidget(nozzleComboBox);
        materialNamesLayout->addWidget(materialRow);

        // Conecta las señales de cambio de los combo boxes para actualizar las configuraciones.
        connect(materialComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MaterialSection::updateMaterialConfigs);
        connect(filamentComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MaterialSection::updateMaterialConfigs);
        connect(nozzleComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MaterialSection::updateMaterialConfigs);

        // Conecta las señales de cambio para emitir una señal cuando cambien los valores.
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

        // Actualiza la selección predeterminada de boquilla en función del material.
        updateMaterialPreset(materialComboBox, filamentComboBox, nozzleComboBox);
    }

    // Actualiza el contenedor de nombres de materiales después de añadir los widgets.
    materialNamesContainer->update();
}

/**
 * @brief Obtiene la lista de configuraciones de materiales actuales.
 *
 * @return Lista de configuraciones de materiales.
 */
QList<MaterialConfig> MaterialSection::getMaterialConfigs() const {
    return materialConfigs;
}

/**
 * @brief Obtiene el número actual de materiales configurados.
 *
 * @return Número de materiales.
 */
int MaterialSection::getNumMaterials() const {
    return numMaterialsSpinBox->value();
}

/**
 * @brief Aplica estilos a la sección de materiales en función del modo de visualización (modo oscuro o claro).
 *
 * Este método es una base para aplicar estilos adicionales según sea necesario, como cambiar colores o temas.
 *
 * @param darkMode Indica si el modo oscuro está activado.
 */
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
