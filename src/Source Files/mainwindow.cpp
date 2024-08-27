#include "mainwindow.h"
#include "startpage.h"
#include "SaveSection.h"
#include "DimensionSection.h"
#include "ShapeSection.h"
#include "infillSection.h"
#include "materialSection.h"
#include "GenerateSection.h"
#include <QFile>
#include <QAction>
#include <QMenuBar>
#include <QApplication>

/**
 * @brief Constructor de la clase MainWindow.
 *
 * Inicializa la ventana principal y configura la interfaz de usuario, incluyendo los layouts,
 * secciones, botones y las conexiones entre señales y slots.
 *
 * @param parent Puntero al widget padre.
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), changeDetected(false)
{
    // Configura el widget central
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Configura el tamaño fijo de la ventana
    setFixedSize(700, 600);
    darkModeEnabled = true;

    // Layout principal horizontal
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Layout vertical para las secciones
    QVBoxLayout *sectionsLayout = new QVBoxLayout();

    // Sección de inicio
    SectionStart *sectionStartWidget = new SectionStart(this);
    connect(sectionStartWidget, &SectionStart::startButtonClicked, this, [this]() {
        nextSection();  // Mover a la siguiente sección
    });
    connect(sectionStartWidget, &SectionStart::languageChanged, this, &MainWindow::changeLanguage);

    // Sección 0: Cargar configuración
    SaveSection *SaveSectionWidget = new SaveSection(this);
    connect(SaveSectionWidget, &SaveSection::fileSelected, this, [this]() {
        nextSection();  // Mover a la siguiente sección
        changeDetected = false;
    });

    // Sección 1: Configuración de dimensiones de placa y posicion de figura
    DimensionSectionWidget = new DimensionSection(this);

    // Sección 2: Configuración de forma figura y su tamaño
    ShapeSectionWidget = new ShapeSection(this);

    // Sección 3: Configuración del relleno de la figura
    infillSectionWidget = new InfillSection(this);

    // Sección 4: Configuración de los materiales de la figura
    materialSectionWidget = new MaterialSection(this);

    // Sección 5: Generar GCODE y guardar configuración
    GenerateSection *GenerateSectionWidget = new GenerateSection(this);

    // Crear QStackedWidget para gestionar las secciones
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(sectionStartWidget);
    stackedWidget->addWidget(SaveSectionWidget);
    stackedWidget->addWidget(DimensionSectionWidget);
    stackedWidget->addWidget(ShapeSectionWidget);
    stackedWidget->addWidget(infillSectionWidget);
    stackedWidget->addWidget(materialSectionWidget);
    stackedWidget->addWidget(GenerateSectionWidget);

    // Añadir el layout de las secciones al layout principal
    sectionsLayout->addWidget(stackedWidget);
    mainLayout->addLayout(sectionsLayout);

    // BOTONES
    // Crear layout horizontal para los botones
    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    // Espaciador para alinear los botones
    buttonsLayout->addItem(new QSpacerItem(9, 0, QSizePolicy::Minimum));

    // Crear botón para ir a la sección anterior
    prevButton = new QPushButton(tr("Previous"), this);
    connect(prevButton, &QPushButton::clicked, this, &MainWindow::previousSection);
    buttonsLayout->addWidget(prevButton);

    // Crear botón para ir a la siguiente sección
    nextButton = new QPushButton(tr("Next"), this);
    connect(nextButton, &QPushButton::clicked, this, [this]() {
        nextSection();
        // Actualizar visualizador 3D al cambiar de sección
        RealTimeGCODE();
    });
    buttonsLayout->addWidget(nextButton);

    // Crear conexión para generar GCODE
    connect(GenerateSectionWidget, &GenerateSection::generateGcode, this, [this]() {
        saveConfigurationToFile();
    });

    // Crear botón para alternar el modo oscuro
    toggleDarkModeButton = new QPushButton(tr("Light Mode"), this);
    connect(toggleDarkModeButton, &QPushButton::clicked, this, &MainWindow::toggleDarkMode);
    buttonsLayout->addWidget(toggleDarkModeButton);

    // Crear botón de cancelación
    cancelButton = new QPushButton(tr("Cancel"), this);
    connect(cancelButton, &QPushButton::clicked, this, &MainWindow::cancelConfirmation);
    buttonsLayout->addWidget(cancelButton);

    // Espaciador para alinear los botones
    buttonsLayout->addItem(new QSpacerItem(9, 0, QSizePolicy::Minimum));

    // Establecer tamaño fijo para los botones
    prevButton->setFixedSize(80, 30);
    nextButton->setFixedSize(80, 30);
    cancelButton->setFixedSize(80, 30);
    toggleDarkModeButton->setFixedSize(80, 30);
    buttonsLayout->setAlignment(cancelButton, Qt::AlignRight);

    // Añadir el layout de los botones al layout de secciones
    sectionsLayout->addLayout(buttonsLayout);


    // Crear un contenedor para el widget OpenGL con esquinas redondeadas
    QWidget *openGLContainer = new QWidget(this);
    QVBoxLayout *openGLLayout = new QVBoxLayout(openGLContainer);
    openGLWidget = new OpenGLWidget(this);

    // Establecer tamaño mínimo para el visualizador
    openGLWidget->setMinimumSize(600, 580);
    openGLLayout->addWidget(openGLWidget);

    // Añadir el contenedor OpenGL al layout principal
    mainLayout->addWidget(openGLContainer);

    // Conectar señales de las secciones para actualizar GCODE en tiempo real (cuando se detecta un cambio señal:valueChanged)
    connect(DimensionSectionWidget, &DimensionSection::valueChanged, this, [this]() {
        RealTimeGCODE();
        changeDetected = true;
    });

    connect(ShapeSectionWidget, &ShapeSection::valueChanged, this, [this]() {
        RealTimeGCODE();
        changeDetected = true;
    });

    connect(infillSectionWidget, &InfillSection::valueChanged, this, [this]() {
        RealTimeGCODE();
        changeDetected = true;
    });

    connect(materialSectionWidget, &MaterialSection::valueChanged, this, [this]() {
        RealTimeGCODE();
        changeDetected = true;
    });

    // Conectar señales para obtener la ruta y el nombre del archivo cargado
    connect(SaveSectionWidget, &SaveSection::fileSelected, this, [this](const QString &fileName) {
        loadedConfigFileName = fileName;
    });

    connect(SaveSectionWidget, &SaveSection::fileRoute, this, [this](const QString &fileRoute) {
        loadedConfigFileRoute = fileRoute;
    });

    // Conectar señal con informacion del tamaño de la placa al widget de OpenGL.
    connect(DimensionSectionWidget, &DimensionSection::plateSizeChanged, this, &MainWindow::updatePlateSize);

    // Conectar señales de secciones para actualizar valores de widgets en otras secciones cuando se carga una configuración de guardado
    connect(SaveSectionWidget, &SaveSection::section1ValuesRead, DimensionSectionWidget, &DimensionSection::updateValues);
    connect(SaveSectionWidget, &SaveSection::section2ValuesRead, ShapeSectionWidget, &ShapeSection::updateValues);
    connect(SaveSectionWidget, &SaveSection::section3ValuesRead, infillSectionWidget, &InfillSection::updateValues);
    connect(SaveSectionWidget, &SaveSection::section4ValuesRead, materialSectionWidget, &MaterialSection::updateMaterialSection);


    // Inicialmente ocultar los botones y el widget OpenGL
    nextButton->setVisible(false);
    prevButton->setVisible(false);
    cancelButton->setVisible(false);
    openGLWidget->setVisible(false);

    // Aplicar estilos iniciales
    applyStyles(darkModeEnabled);
}
/**
 * @brief Cambiar el idioma de la aplicación.
 *
 * Esta función carga un archivo de traducción basado en el código de idioma proporcionado
 * y aplica las traducciones a la interfaz de usuario.
 *
 * @param languageCode El código del idioma a cargar (por ejemplo, "es" para español).
 */
void MainWindow::changeLanguage(const QString &languageCode) {
    // Construir la ruta al archivo de traducción
    QString qmPath = QCoreApplication::applicationDirPath() + "/translations/myapp_" + languageCode + ".qm";

    // Cargar el archivo de traducción y aplicarlo
    if (translator.load(qmPath)) {
        qApp->installTranslator(&translator);  // Instalar el traductor en la aplicación
        retranslateUi();  // Reaplicar las traducciones a la interfaz de usuario
    } else {
        qDebug() << "Failed to load translation file: " << qmPath;  // Mostrar mensaje de error si no se puede cargar el archivo
    }
}

/**
 * @brief Reaplicar las traducciones a todos los widgets visibles.
 *
 * Esta función actualiza los textos de todos los widgets visibles en la interfaz de usuario
 * después de cambiar el idioma de la aplicación.
 */
void MainWindow::retranslateUi() {
    // Reaplicar las traducciones a todos los widgets en el stackedWidget
    for (int i = 0; i < stackedWidget->count(); ++i) {
        QWidget *widget = stackedWidget->widget(i);
        retranslateSection(widget);
    }

    // Actualizar el texto de los botones
    prevButton->setText(tr("Previous"));  // Traducir el texto del botón "Previous"
    nextButton->setText(tr("Next"));      // Traducir el texto del botón "Next"
    cancelButton->setText(tr("Cancel"));  // Traducir el texto del botón "Cancel"
    toggleDarkModeButton->setText(tr("Light Mode"));  // Traducir el texto del botón "Light Mode"

    // Actualizar el título de la ventana principal
    setWindowTitle(tr("My Application"));  // Traducir el título de la ventana principal
}

/**
 * @brief Reaplicar las traducciones a una sección específica.
 *
 * Esta función reaplica las traducciones a un widget de sección específico, dependiendo
 * del tipo de widget.
 *
 * @param section El widget de sección a traducir.
 */
void MainWindow::retranslateSection(QWidget* section) {
    if (auto* startPage = qobject_cast<SectionStart*>(section)) {
        startPage->retranslateUi();  // Reaplicar traducciones en la página de inicio
    } else if (auto* saveSection = qobject_cast<SaveSection*>(section)) {
        saveSection->retranslateUi();  // Reaplicar traducciones en la sección de guardado
    } else if (auto* dimensionSection = qobject_cast<DimensionSection*>(section)) {
        dimensionSection->retranslateUi();  // Reaplicar traducciones en la sección de dimensiones
    } else if (auto* shapeSection = qobject_cast<ShapeSection*>(section)) {
        shapeSection->retranslateUi();  // Reaplicar traducciones en la sección de forma
    } else if (auto* infillSection = qobject_cast<InfillSection*>(section)) {
        infillSection->retranslateUi();  // Reaplicar traducciones en la sección de relleno
    } else if (auto* materialSection = qobject_cast<MaterialSection*>(section)) {
        materialSection->retranslateUi();  // Reaplicar traducciones en la sección de materiales
    } else if (auto* generateSection = qobject_cast<GenerateSection*>(section)) {
        generateSection->retranslateUi();  // Reaplicar traducciones en la sección de generación
    }
}

/**
 * @brief Aplicar estilos a los widgets en función del modo actual.
 *
 * Esta función aplica los estilos de modo oscuro o claro a todos los widgets
 * del resto de las secciones en función del valor del parámetro darkMode.
 *
 * @param darkMode Indica si el modo oscuro está habilitado (true) o no (false).
 */
void MainWindow::applyStyles(bool darkMode) {
    if (darkMode) {
        applyDarkMode();  // Aplicar estilos del modo oscuro
    } else {
        applyLightMode();  // Aplicar estilos del modo claro
    }

    // Aplicar estilos a las secciones del widget apilado
    for (int i = 0; i < stackedWidget->count(); ++i) {
        QWidget *widget = stackedWidget->widget(i);
        if (auto* startPage = qobject_cast<SectionStart*>(widget)) {
            startPage->applyStyles(darkMode);  // Aplicar estilos a la página de inicio
        } else if (auto* saveSection = qobject_cast<SaveSection*>(widget)) {
            saveSection->applyStyles(darkMode);  // Aplicar estilos a la sección de guardado
        } else if (auto* dimensionSection = qobject_cast<DimensionSection*>(widget)) {
            dimensionSection->applyStyles(darkMode);  // Aplicar estilos a la sección de dimensiones
        } else if (auto* shapeSection = qobject_cast<ShapeSection*>(widget)) {
            shapeSection->applyStyles(darkMode);  // Aplicar estilos a la sección de forma
        } else if (auto* infillSection = qobject_cast<InfillSection*>(widget)) {
            infillSection->applyStyles(darkMode);  // Aplicar estilos a la sección de relleno
        } else if (auto* materialSection = qobject_cast<MaterialSection*>(widget)) {
            materialSection->applyStyles(darkMode);  // Aplicar estilos a la sección de materiales
        } else if (auto* generateSection = qobject_cast<GenerateSection*>(widget)) {
            generateSection->applyStyles(darkMode);  // Aplicar estilos a la sección de generación
        }
    }
}


/**
 * @brief Aplicar estilos del modo claro a la ventana principal.
 *
 * Esta función establece el estilo de la hoja para la ventana principal y sus botones de navegación
 * para utilizar los colores del modo claro.
 */
void MainWindow::applyLightMode() {
    // Establecer el color de fondo para la ventana principal
    setStyleSheet("MainWindow { background-color: #f0f0f0; }");

    // Definir el estilo para los botones de navegación en modo claro
    QString navButtonStyle = "QPushButton {}"
                             "QPushButton:hover { background-color: #e6ffe6; border: 1px solid #4CAF50; border-radius: 5px; }";
    prevButton->setStyleSheet(navButtonStyle);
    nextButton->setStyleSheet(navButtonStyle);
    cancelButton->setStyleSheet(navButtonStyle);
    toggleDarkModeButton->setStyleSheet(navButtonStyle);
}

/**
 * @brief Aplicar estilos del modo oscuro a la ventana principal.
 *
 * Esta función establece el estilo de la hoja para la ventana principal y sus botones de navegación
 * para utilizar los colores del modo oscuro.
 */
void MainWindow::applyDarkMode() {
    // Establecer estilos para la ventana principal y los widgets en modo oscuro
    setStyleSheet(
        "MainWindow { background-color: #1a1a1a; }"
        "QPushButton { border: 1px solid #2a2a2a;"
        "border-radius: 5px; "
        "padding: 6px; }"
        "QPushButton:hover { background-color: #001900; border: 1px solid #4CAF50; }"
        "QWidget { background-color: #1a1a1a; color: #ffffff; }"
        );

    // Definir el estilo para los botones de navegación en modo oscuro
    QString navButtonStyle = "QPushButton { background-color: #2a2a2a; color: white;"
                             "border-radius: 5px; }"
                             "QPushButton:hover { background-color: #001900; border: 1px solid #4CAF50; }";
    prevButton->setStyleSheet(navButtonStyle);
    nextButton->setStyleSheet(navButtonStyle);
    cancelButton->setStyleSheet(navButtonStyle);
    toggleDarkModeButton->setStyleSheet(navButtonStyle);
}

/**
 * @brief Alternar entre el modo oscuro y el modo claro.
 *
 * Esta función cambia la aplicación entre el modo oscuro y el modo claro,
 * y actualiza el texto del botón en consecuencia.
 */
void MainWindow::toggleDarkMode() {
    darkModeEnabled = !darkModeEnabled;  // Alternar el estado del modo oscuro
    applyStyles(darkModeEnabled);        // Aplicar los estilos correspondientes
    // Actualizar el texto del botón según el modo actual
    toggleDarkModeButton->setText(darkModeEnabled ? tr("Light Mode") : tr("Dark Mode"));
}

/**
 * @brief Configurar los elementos de la interfaz de usuario según el índice de sección actual.
 *
 * Esta función permite ajustar paramteros individuales para cada seccion, como tamaño o
 * visibilidad de determinados widgets.
 *
 * @param sectionIndex El índice de la sección a configurar.
 */
void MainWindow::sectionConfiguration(int sectionIndex) {
    // Ajustar el tamaño de la ventana y la visibilidad de los elementos según la sección actual
    switch (sectionIndex) {
    case 1:
        setFixedSize(1200, 600);  // Establecer tamaño fijo de la ventana
        nextButton->setVisible(true);  // Hacer visible el botón siguiente
        prevButton->setVisible(true);  // Hacer visible el botón previo
        cancelButton->setVisible(true);  // Hacer visible el botón cancelar
        openGLWidget->setVisible(true);  // Hacer visible el widget OpenGL
        toggleDarkModeButton->setVisible(false);  // Ocultar el botón de alternar modo oscuro
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    default:
        break;
    }
}

/**
 * @brief Mostrar un cuadro de diálogo de confirmación al intentar cancelar.
 *
 * Esta función muestra un QMessageBox para confirmar si el usuario desea cancelar.
 * Si se confirma, la aplicación se cerrará.
 */
void MainWindow::cancelConfirmation() {
    QMessageBox::StandardButton reply;
    // Preguntar al usuario si está seguro de que desea cancelar
    reply = QMessageBox::question(this, tr("Cancel Confirmation"), tr("Are you sure you want to cancel?"),
                                  QMessageBox::Yes|QMessageBox::No);

    // Cerrar la aplicación si el usuario confirma
    if (reply == QMessageBox::Yes) {
        close();
    }
}

/**
 * @brief Mostrar la sección anterior en el widget apilado.
 *
 * Esta función navega a la sección anterior si está permitido. También
 * previene la navegación a la sección de guardado si un archivo de configuración está cargado.
 */
void MainWindow::previousSection() {
    int prevIndex = stackedWidget->currentIndex() - 1;

    // Prevenir la navegación hacia atrás a la sección 1 si un archivo de configuración está cargado
    if (!loadedConfigFileName.isEmpty() && prevIndex == 1) {
        QMessageBox::warning(this, tr("Action Restricted"), tr("You cannot go back to this section as a configuration file is loaded."));
        return;
    }

    if (prevIndex >= 1) {
        sectionConfiguration(prevIndex);  // Configurar la sección anterior
        stackedWidget->setCurrentIndex(prevIndex);  // Cambiar a la sección anterior
    }
}

/**
 * @brief Mostrar la siguiente sección en el widget apilado.
 *
 * Esta función navega a la siguiente sección si está permitido. También verifica
 * si se deben guardar los cambios antes de continuar, en caso de que se haya cargado
 * previamente una configuracion de guardado
 *
 */
void MainWindow::nextSection() {

    // Preguntar al usuario si desea actualizar la configuración si: se han realizado cambios, se ha detectado una CdG, no esta en la seccion de guardado
    if (changeDetected && stackedWidget->currentIndex() != 1 && !loadedConfigFileName.isEmpty()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Update Configuration"), tr("You have made a change. Do you want to update it?"),
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            writeConfigurationToFile(loadedConfigFileRoute, loadedConfigFileName);  // Guardar la configuración
        }
        changeDetected = false;  // Restablecer el indicador de cambios
    }

    int nextIndex = stackedWidget->currentIndex() + 1;
    if (nextIndex < stackedWidget->count()) {
        sectionConfiguration(nextIndex);  // Configurar la siguiente sección
        stackedWidget->setCurrentIndex(nextIndex);  // Cambiar a la siguiente sección
    }
}

/**
 * @brief Ejecutar un script de Python usando QProcess.
 *
 * Esta función inicia el .exe que contiene el script de Python con el Generador de GCODES.
 */
void MainWindow::executePython() {
    QProcess *process = new QProcess(this);  // Crear un nuevo proceso
    QString currentPath = QCoreApplication::applicationDirPath();  // Obtener la ruta del directorio actual
    QString exePath = currentPath + "/axolotl_1mat.exe";  // Ruta del archivo .exe
    process->start(exePath);  // Iniciar el proceso con el archivo .exe
    if (process->waitForStarted()) {
        qDebug() << "Executable started successfully.";
    } else {
        qDebug() << "Failed to start executable.";
    }
}

/**
 * @brief Guardar la configuración actual en un archivo.
 *
 * Esta función solicita al usuario que guarde la configuración actual en un archivo.
 * Si se confirma, se escribe la configuración en el archivo especificado y
 * se ejecuta un script de Python.
 */
void MainWindow::saveConfigurationToFile() {
    QString directoryPath = QCoreApplication::applicationDirPath();  // Obtener la ruta del directorio de la aplicación

    QMessageBox::StandardButton reply;
    // Preguntar al usuario si desea guardar la configuración
    reply = QMessageBox::question(this, tr("Save Configuration"), tr("Do you want to save this configuration?"),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Mostrar el diálogo para guardar el archivo de configuración
        QString fullFilePath = QFileDialog::getSaveFileName(this, "Save Configuration", loadedConfigFileRoute, "Configuration Files (*.conf)");

        if (!fullFilePath.isEmpty()) {
            QFileInfo fileInfo(fullFilePath);  // Obtener información del archivo
            QString saveDirectory = fileInfo.absolutePath();  // Ruta al directorio
            QString fileName = fileInfo.fileName();  // Solo el nombre del archivo

            if (writeConfigurationToFile(saveDirectory, fileName)) {
                QMessageBox::information(this, tr("Configuration Saved"), tr("Configuration saved successfully."));
            } else {
                QMessageBox::critical(this, tr("Error"), tr("Failed to save configuration to file."));
            }
        }
    }

    executePython();  // Ejecutar el script de Python
}

/**
 * @brief Escribir la configuración actual en un archivo.
 *
 * Esta función guarda los valores actuales de configuración en un archivo de texto.
 *
 * @param filePath Ruta al directorio del archivo.
 * @param fileName Nombre del archivo.
 * @return true Si la configuración se guardó correctamente.
 * @return false Si ocurrió un error al guardar la configuración.
 */
bool MainWindow::writeConfigurationToFile(const QString &filePath, const QString &fileName) {
    QString fullPath = filePath + "/" + fileName;  // Ruta completa del archivo

    QFile file(fullPath);  // Crear el archivo
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);  // Crear un flujo de texto para escribir en el archivo

        // Valores de la sección 1 DimensionSection
        out << "Section1 Values:\n";
        out << "Plate X Value: " << DimensionSectionWidget->getPlateXSpinBox() << "\n";
        out << "Plate Y Value: " << DimensionSectionWidget->getPlateYSpinBox() << "\n";
        out << "Center X Value: " << DimensionSectionWidget->getcenterXSpinBox() << "\n";
        out << "Center Y Value: " << DimensionSectionWidget->getcenterYSpinBox() << "\n\n";

        // Valores de la sección 2 ShapeSection
        out << "Section2 Values:\n";
        out << "Size 1: " << ShapeSectionWidget->getSize1() << "\n";
        out << "Size 2: " << ShapeSectionWidget->getSize2() << "\n";
        out << "Shape Index 1: " << ShapeSectionWidget->getShapeIndex1() << "\n";
        out << "Shape Index 2: " << ShapeSectionWidget->getShapeIndex2() << "\n\n";

        // Valores de la sección 3 InfillSection
        out << "Section3 Values:\n";
        out << "Infill Value: " << infillSectionWidget->getInfillValue() << "\n";
        out << "Shape Index: " << infillSectionWidget->getShapeIndex() << "\n";
        out << "Strand Distance Value: " << infillSectionWidget->getStrandDistanceValue() << "\n\n";

        // Valores de la sección 4 MaterialSection
        out << "Section4 Values:\n";
        QList<MaterialConfig> materialConfigs = materialSectionWidget->getMaterialConfigs();
        for (const auto& materialConfig : materialConfigs) {
            out << "Material Name: " << materialConfig.name << "\n";
            out << "Filament: " << materialConfig.filament << "\n";
            out << "Nozzle: " << materialConfig.nozzle << "\n" << "\n";
        }

        file.close();  // Cerrar el archivo
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Actualizar el archivo GCODE en tiempo real y ejecutar el script de Python.
 *
 * Esta función escribe la configuración actual en un archivo y luego ejecuta
 * el script de Python. Simula una actualizacion del GCODE en tiempo real.
 */
void MainWindow::RealTimeGCODE() {
    QString directoryPath = QCoreApplication::applicationDirPath();  // Obtener la ruta del directorio de la aplicación
    writeConfigurationToFile(directoryPath, "section_values.txt");  // Guardar la configuración en un archivo
    executePython();  // Ejecutar el script de Python
}

/**
 * @brief Actualizar el tamaño de la placa en el widget OpenGL.
 *
 * Esta función actualiza el tamaño de la placa en el widget OpenGL basado en
 * el tamaño proporcionado. Permite conectar el tamaño de placa de "DimensionSection" con la placa de OpenGL
 *
 * @param size El nuevo tamaño de la placa como un objeto QVector2D.
 */
void MainWindow::updatePlateSize(const QVector2D &size) {
    if (openGLWidget) {
        openGLWidget->setPlateSize(static_cast<int>(size.x()), static_cast<int>(size.y()));  // Actualizar el tamaño de la placa
    }
}

/**
 * @brief Manejar el evento de cierre de la ventana principal.
 *
 * Esta función se activa cuando se cierra la ventana principal. Asegura
 * de que el archivo GCODE sea eliminado si existe para resetearlo.
 *
 * @param event El objeto QCloseEvent que representa el evento de cierre.
 */
void MainWindow::closeEvent(QCloseEvent *event) {
    // Ruta al archivo GCODE que se debe eliminar
    QString directoryPath = QCoreApplication::applicationDirPath();
    QString gcodeFilePath = directoryPath + "/Axo3_1mat.gcode";

    // Comprobar si el archivo existe y eliminarlo
    if (QFile::exists(gcodeFilePath)) {
        QFile::remove(gcodeFilePath);
    }

    QMainWindow::closeEvent(event);  // Llamar al manejador de eventos de cierre de la ventana principal
}
