#include "SaveSection.h"

/**
 * @brief Constructor de la clase SaveSection.
 *
 * Configura el área de desplazamiento, los botones y el diseño de la sección de guardado.
 *
 * @param parent El widget padre.
 */
SaveSection::SaveSection(QWidget *parent) : QWidget(parent) {
    // Crear un área de desplazamiento para contener el contenido desplazable
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollWidget = new QWidget(this);
    scrollWidget->setObjectName("scrollWidget");

    // Crear un diseño vertical para el widget de desplazamiento
    layout = new QVBoxLayout(scrollWidget);
    layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    // Configurar las dimensiones del área de desplazamiento
    scrollArea->setMaximumSize(240, 400);
    scrollArea->setMinimumSize(240, 400);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollWidget);

    // Crear un contenedor para el área de desplazamiento
    QWidget *scrollContainer = new QWidget(this);
    QHBoxLayout *scrollContainerLayout = new QHBoxLayout(scrollContainer);
    scrollContainerLayout->addWidget(scrollArea);

    // Leer la ruta del directorio de la configuración desde QSettings
    QSettings settings("Nebrija", "3Dprinting");
    directoryPath = settings.value("saveSection/directoryPath", QCoreApplication::applicationDirPath() + "/configurations").toString();
    qDebug() << "New directory path saved:" << directoryPath;

    // Emitir la señal con la ruta del archivo
    QMetaObject::invokeMethod(this, [this]() {
            emit fileRoute(directoryPath);
        }, Qt::QueuedConnection);

    // Crear y configurar el botón para cambiar el directorio
    changeDirectoryButton = new QPushButton(tr("Change Directory"), this);
    changeDirectoryButton->setFixedSize(140, 30);
    connect(changeDirectoryButton, &QPushButton::clicked, this, &SaveSection::changeDirectory);

    // Crear un diseño horizontal para el botón
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(changeDirectoryButton, 0, Qt::AlignLeft);

    // Crear el diseño principal y agregar el botón
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(buttonLayout);

    // Etiqueta de instrucciones
    QSpacerItem *topSpacer = new QSpacerItem(50, 50, QSizePolicy::Minimum, QSizePolicy::Expanding);
    mainLayout->addItem(topSpacer);

    instructionLabel = new QLabel(tr("Select Save Configuration"), this);
    instructionLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(instructionLabel);

    QSpacerItem *spacer = new QSpacerItem(30, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);
    mainLayout->addItem(spacer);

    // Agregar el contenedor de desplazamiento al diseño principal
    mainLayout->addWidget(scrollContainer);
    setLayout(mainLayout);

    // Crear botones de configuración
    createConfigurations();

    // Aplicar estilos según el modo inicial (modo oscuro por defecto)
    applyStyles(true);
}

/**
 * @brief Reaplicar las traducciones a los widgets de la sección.
 *
 * Actualiza los textos de los botones y etiquetas según el idioma actual.
 */
void SaveSection::retranslateUi() {
    changeDirectoryButton->setText(tr("Change Directory"));
    instructionLabel->setText(tr("Select Save Configuration"));
}

/**
 * @brief Crear botones para cada archivo de configuración en el directorio.
 *
 * Lee los archivos de configuración en el directorio especificado y crea un botón
 * para cada archivo. Los botones permiten al usuario seleccionar una configuración
 * para ser utilizada. Se limpia el diseño actual de botones antes de agregar los nuevos.
 */
void SaveSection::createConfigurations() {
    QDir directory(directoryPath);
    QStringList confFiles = directory.entryList(QStringList() << "*.conf", QDir::Files);

    // Limpiar el diseño actual de botones
    clearLayout(layout);

    // Crear un botón para cada archivo de configuración
    for (const QString &fileName : confFiles) {
        QPushButton *button = new QPushButton(fileName, this);
        button->setFixedSize(200, 50);
        button->setProperty("isConfButton", true);

        // Conectar la señal de clic del botón al manejador de clic
        connect(button, &QPushButton::clicked, this, [this, fileName]() {
            onConfButtonClicked(fileName);
        });

        // Agregar el botón al diseño de botones
        layout->addWidget(button);
    }
}

/**
 * @brief Abrir un cuadro de diálogo para seleccionar un nuevo directorio.
 *
 * Permite al usuario seleccionar un nuevo directorio para guardar configuraciones.
 * Actualiza la ruta del directorio en la variable `directoryPath` y guarda esta
 * información en QSettings para que no se pierda al cerrar la aplicación.
 */
void SaveSection::changeDirectory() {
    QString newDirectory = QFileDialog::getExistingDirectory(this, tr("Select Directory"), directoryPath);
    if (!newDirectory.isEmpty()) {
        directoryPath = newDirectory;

        // Guardar la nueva ruta del directorio en QSettings
        QSettings settings("Nebrija", "3Dprinting");
        settings.setValue("saveSection/directoryPath", directoryPath);

        QString savedPath = settings.value("saveSection/directoryPath").toString();
        qDebug() << "New directory path saved:" << savedPath;
        emit fileRoute(savedPath);

        // Actualizar la lista de configuraciones con los archivos en el nuevo directorio
        createConfigurations();
    } else {
        qDebug() << "No directory was selected.";
    }
}

/**
 * @brief Limpiar los widgets de un diseño.
 *
 * Elimina todos los widgets y elementos del diseño proporcionado. Esto se utiliza
 * para limpiar el diseño de botones antes de agregar nuevos botones de configuración.
 *
 * @param layout El diseño cuyo contenido debe limpiarse.
 */
void SaveSection::clearLayout(QLayout *layout) {
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        // Eliminar el widget asociado si existe
        if (child->widget()) {
            delete child->widget();
        }
        // Eliminar el ítem del diseño
        delete child;
    }
}

/**
 * @brief Manejar el clic en un botón de configuración.
 *
 * Copia el archivo de configuración seleccionado al archivo de valores de sección
 * y emite una señal con el nombre del archivo seleccionado. Estas señales son
 * utilizadas en la ventana principal (MainWindow) para gestionar el sistema de
 * actualización de configuraciones de guardado.
 *
 * @param fileName El nombre del archivo de configuración seleccionado.
 */
void SaveSection::onConfButtonClicked(const QString &fileName) {
    QString confFilePath = directoryPath + "/" + fileName;
    copyConfiguration(confFilePath);
    emit fileSelected(fileName);
}

/**
 * @brief Copiar la configuración seleccionada y ejecutar un script de Python.
 *
 * Lee el archivo de configuración seleccionado y lo copia al archivo de valores
 * de sección (`section_values.txt`). Luego, ejecuta un script de Python para
 * procesar la configuración. Se copian los datos para que el Generador de
 * GCODES pueda proccesar correctamente
 *
 * @param confFilePath La ruta del archivo de configuración a copiar.
 */
void SaveSection::copyConfiguration(const QString &confFilePath) {
    // Abre la configuracion de guardado seleccionada por el usuario
    QFile confFile(confFilePath);
    if (!confFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open configuration file:" << confFilePath;
        return;
    }

    // Abre el archivo section_values.txt
    QString sectionValuesFilePath = QCoreApplication::applicationDirPath() + "/section_values.txt";
    QFile sectionValuesFile(sectionValuesFilePath);
    if (!sectionValuesFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open section values file:" << sectionValuesFilePath;
        return;
    }

    QTextStream confStream(&confFile);
    QTextStream sectionValuesStream(&sectionValuesFile);

    // Ejecutar el script de Python
    QProcess *process = new QProcess(this);  // Crear un nuevo proceso
    QString currentPath = QCoreApplication::applicationDirPath();  // Obtener la ruta del directorio actual
    QString exePath = currentPath + "/axolotl_1mat.exe";  // Ruta del archivo .exe
    process->start(exePath);  // Iniciar el proceso con el archivo .exe
    if (process->waitForStarted()) {
        qDebug() << "Executable started successfully.";
    } else {
        qDebug() << "Failed to start executable.";
    }

    // Copiar el contenido del archivo de configuración al archivo section_values.txt
    sectionValuesStream << confStream.readAll();
    sectionValuesStream << "\n\n";

    sectionValuesFile.close();
    confFile.close();

    // Leer los valores de sección del archivo copiado
    readSectionValues();
}

/**
 * @brief Leer los valores de configuración desde el archivo y emitir señales correspondientes.
 *
 * Lee los valores del archivo `section_values.txt` y emite señales con los valores leídos
 * para cada sección. Esto permite cargar los valores de la configuración de guardado
 * en los widgets correspondientes de cada sección.
 */
void SaveSection::readSectionValues() {
    // Abrir archivo section_values.txt con la informacion a enviar
    QFile file(QCoreApplication::applicationDirPath() + "/section_values.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open section values file.";
        return;
    }

    QTextStream in(&file);
    QString section;
    // Variables temporales para almacenar información a enviar
    int plateX = 0, plateY = 0, centerX = 0, centerY = 0;
    int shapeIndex1 = 0, shapeIndex2 = 0, size1 = 0, size2 = 0;
    int methodIndex = 0, infillValue = 0;
    double strandDistance = 0.0;
    QList<QPair<QString, QString>> section4Materials;

    // Leer línea por línea para extraer valores específicos
    while (!in.atEnd()) {
        QString line = in.readLine();

        // Identificar la sección actual y extraer los valores correspondientes
        if (line.contains("Section1 Values:")) {
            section = "Section1";
        } else if (line.contains("Section2 Values:")) {
            section = "Section2";
        } else if (line.contains("Section3 Values:")) {
            section = "Section3";
        } else if (line.contains("Section4 Values:")) {
            section = "Section4";
        } else if (section == "Section1") {
            // Extraer valores de la Sección 1 (Dimensiones)
            if (line.contains("Plate X Value:")) {
                plateX = line.split(":").last().trimmed().toInt();
            } else if (line.contains("Plate Y Value:")) {
                plateY = line.split(":").last().trimmed().toInt();
            } else if (line.contains("Center X Value:")) {
                centerX = line.split(":").last().trimmed().toInt();
            } else if (line.contains("Center Y Value:")) {
                centerY = line.split(":").last().trimmed().toInt();
            }
        } else if (section == "Section2") {
            // Extraer valores de la Sección 2 (Forma)
            if (line.contains("Shape Index 1:")) {
                shapeIndex1 = line.split(":").last().trimmed().toInt();
            } else if (line.contains("Shape Index 2:")) {
                shapeIndex2 = line.split(":").last().trimmed().toInt();
            } else if (line.contains("Size 1:")) {
                size1 = line.split(":").last().trimmed().toInt();
            } else if (line.contains("Size 2:")) {
                size2 = line.split(":").last().trimmed().toInt();
            }
        } else if (section == "Section3") {
            // Extraer valores de la Sección 3 (Infill)
            if (line.contains("Method Index:")) {
                methodIndex = line.split(":").last().trimmed().toInt();
            } else if (line.contains("Infill Value:")) {
                infillValue = line.split(":").last().trimmed().toInt();
            } else if (line.contains("Strand Distance:")) {
                strandDistance = line.split(":").last().trimmed().toDouble();
            }
        } else if (section == "Section4") {
            // Extraer valores de la Sección 4 (Material)
            if (line.contains("Material Name:")) {
                QString materialName = line.split(":").last().trimmed();
                QString filament, nozzle;
                while (!in.atEnd()) {
                    line = in.readLine();
                    if (line.contains("Filament:")) {
                        filament = line.split(":").last().trimmed();
                    } else if (line.contains("Nozzle:")) {
                        nozzle = line.split(":").last().trimmed();
                    } else if (line.trimmed().isEmpty()) {
                        // Guardar el material y sus propiedades
                        section4Materials.append(qMakePair(materialName, filament + " | " + nozzle));
                        break;
                    }
                }
            }
        }
    }

    file.close();

    // Emitir señales con los valores leídos
    emit section1ValuesRead(plateX, plateY, centerX, centerY);
    emit section2ValuesRead(shapeIndex1, shapeIndex2, size1, size2);
    emit section3ValuesRead(methodIndex, infillValue, strandDistance);
    emit section4ValuesRead(section4Materials);
}

/**
 * @brief Aplicar estilos a los widgets de la sección.
 *
 * Configura los estilos visuales de los widgets basados en el modo especificado.
 *
 * @param darkMode Indica si se debe aplicar el modo oscuro (true) o el modo claro (false).
 */
void SaveSection::applyStyles(bool darkMode) {
    if (darkMode) {
        setStyleSheet(
            "QScrollArea {"
            "    border: 0px solid #4CAF50;"
            "    border-radius: 5px;"
            "}"
            "QScrollBar:vertical {"
            "    border: none;"
            "    background: #3d3d3d;"
            "    width: 10px;"
            "    margin: 0px 0px 0px 0px;"
            "}"
            "QScrollBar::handle:vertical {"
            "    background: #555555;"
            "    min-height: 10px;"
            "    border-radius: 4px;"
            "}"
            "QScrollBar::add-line:vertical {"
            "    border: none;"
            "    background: none;"
            "}"
            "QScrollBar::sub-line:vertical {"
            "    border: none;"
            "    background: none;"
            "}"
            "QPushButton {"
            "    background-color: #2a2a2a;"
            "    color: white;"
            "    border: none;"
            "    border-radius: 5px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #001900;"
            "    border: 1px solid #4CAF50;"
            "    border-radius: 5px;"
            "}"
            "QLabel {"
            "    font-size: 20px;"
            "    color: #4CAF50;"
            "    font-weight: bold;"
            "    padding: 10px 5px;"
            "}"
            "QWidget#scrollWidget {"
            "    background-color: #1a1a1a;"
            "}"
            );
    } else {
        setStyleSheet(
            "QScrollArea {"
            "    border: 0px solid #4CAF50;"
            "    border-radius: 5px;"
            "}"
            "QScrollBar:vertical {"
            "    border: none;"
            "    background: #f1f1f1;"
            "    width: 10px;"
            "    margin: 0px 0px 0px 0px;"
            "}"
            "QScrollBar::handle:vertical {"
            "    background: #d3d3d3;"
            "    min-height: 10px;"
            "    border-radius: 4px;"
            "}"
            "QScrollBar::add-line:vertical {"
            "    border: none;"
            "    background: none;"
            "}"
            "QScrollBar::sub-line:vertical {"
            "    border: none;"
            "    background: none;"
            "}"
            "QPushButton:hover {"
            "    background-color: #e6ffe6;"
            "    border: 1px solid #4CAF50;"
            "    border-radius: 5px;"
            "}"
            "QLabel{"
            "    font-size: 20px;"
            "    color: #4CAF50;"
            "    font-weight: bold;"
            "    padding: 10px 5px;"
            "}"
            );
    }
}
