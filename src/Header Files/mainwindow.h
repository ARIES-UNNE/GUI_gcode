#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QProcess>
#include <QDebug>
#include <QTranslator>
#include <QFileDialog>
#include <QMainWindow>
#include <QFormLayout>
#include <QStackedWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QCoreApplication>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSizeGrip>
#include <QVector2D>

#include "DimensionSection.h"
#include "ShapeSection.h"
#include "InfillSection.h"
#include "MaterialSection.h"
#include "OpenGL.h"

/**
 * @brief Clase principal de la aplicación, que hereda de QMainWindow.
 *
 * Esta clase maneja la interfaz principal de la aplicación, incluyendo la
 * gestión de secciones, modos de visualización, y traducción.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Constructor para la clase MainWindow.
     *
     * @param parent Widget padre opcional.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Aplicar estilos a los widgets basados en el modo actual.
     *
     * @param darkMode Indica si el modo oscuro está habilitado (true) o no (false).
     */
    void applyStyles(bool darkMode);

    /**
     * @brief Cambiar el idioma de la aplicación.
     *
     * @param languageCode El código del idioma a cargar (por ejemplo, "es" para español).
     */
    void changeLanguage(const QString &languageCode);

    /**
     * @brief Actualizar el tamaño de la placa en el widget OpenGL.
     *
     * @param size El nuevo tamaño de la placa como un objeto QVector2D.
     */
    void updatePlateSize(const QVector2D &size);

protected:
    /**
     * @brief Manejar el evento de cierre de la ventana principal.
     *
     * @param event El objeto QCloseEvent que representa el evento de cierre.
     */
    void closeEvent(QCloseEvent *event) override;

private:
    /**
     * @brief Puntero al widget central de la ventana principal.
     */
    QWidget *centralWidget;

    /**
     * @brief Puntero al QStackedWidget que gestiona las secciones.
     */
    QStackedWidget *stackedWidget;

    /**
     * @brief Puntero al widget OpenGL.
     */
    OpenGLWidget *openGLWidget;

    /**
     * @brief Puntero al widget de la sección de dimensiones.
     */
    DimensionSection *DimensionSectionWidget;

    /**
     * @brief Puntero al widget de la sección de formas.
     */
    ShapeSection *ShapeSectionWidget;

    /**
     * @brief Puntero al widget de la sección de relleno.
     */
    InfillSection *infillSectionWidget;

    /**
     * @brief Puntero al widget de la sección de materiales.
     */
    MaterialSection *materialSectionWidget;

    /**
     * @brief Puntero al botón de cancelar.
     */
    QPushButton *cancelButton;

    /**
     * @brief Puntero al botón para ir a la sección anterior.
     */
    QPushButton *prevButton;

    /**
     * @brief Puntero al botón para ir a la siguiente sección.
     */
    QPushButton *nextButton;

    /**
     * @brief Puntero al botón para alternar el modo oscuro.
     */
    QPushButton *toggleDarkModeButton;

    /**
     * @brief Traductor para gestionar los cambios de idioma.
     */
    QTranslator translator;

    /**
     * @brief Indica si el modo oscuro está habilitado.
     */
    bool darkModeEnabled;

    /**
     * @brief Nombre del archivo de configuración cargado.
     */
    QString loadedConfigFileName;

    /**
     * @brief Ruta del archivo de configuración cargado.
     */
    QString loadedConfigFileRoute;

    /**
     * @brief Indica si se han detectado cambios en la configuración.
     */
    bool changeDetected;


    /**
     * @brief Aplicar los estilos del modo oscuro.
     */
    void applyDarkMode();

    /**
     * @brief Aplicar los estilos del modo claro.
     */
    void applyLightMode();

    /**
     * @brief Reaplicar las traducciones a todos los widgets visibles.
     */
    void retranslateUi();

    /**
     * @brief Reaplicar las traducciones a una sección específica.
     *
     * @param section El widget de sección a traducir.
     */
    void retranslateSection(QWidget* section);

    /**
     * @brief Escribir la configuración actual en un archivo.
     *
     * @param filePath Ruta al directorio del archivo.
     * @param fileName Nombre del archivo.
     * @return true Si la configuración se guardó correctamente.
     * @return false Si ocurrió un error al guardar la configuración.
     */
    bool writeConfigurationToFile(const QString &filePath, const QString &fileName);

    bool errorShown;

private slots:
    /**
     * @brief Slot para pasar a la siguiente sección.
     */
    void nextSection();

    /**
     * @brief Slot para volver a la sección anterior.
     */
    void previousSection();

    /**
     * @brief Slot para mostrar un cuadro de diálogo de confirmación de cancelación.
     */
    void cancelConfirmation();

    /**
     * @brief Slot para alternar entre el modo oscuro y el modo claro.
     */
    void toggleDarkMode();

    /**
     * @brief Slot para ajustar la configuración de la sección.
     *
     * @param sectionIndex Índice de la sección a configurar.
     */
    void sectionConfiguration(int sectionIndex);

    /**
     * @brief Slot para guardar la configuración en un archivo.
     */
    void saveConfigurationToFile();

    /**
     * @brief Slot para actualizar el GCODE en tiempo real.
     */
    void RealTimeGCODE();

    /**
     * @brief Slot para ejecutar un script de Python.
     */
    void executePython();
};

#endif // MAINWINDOW_H
