#ifndef GENERATESECTION_H
#define GENERATESECTION_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCoreApplication>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QDir>

/**
 * @class GenerateSection
 * @brief Widget para generar y guardar archivos GCODE.
 *
 * Este widget proporciona una interfaz para que el usuario guarde archivos GCODE y configuraciones de guardado . Incluye botones
 * para crear el archivo GCODE y para guardarlo en una ubicación seleccionada por el usuario.
 */
class GenerateSection : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor de GenerateSection.
     *
     * Inicializa los widgets y configura el diseño de la sección de generación.
     *
     * @param parent Widget padre, si lo hay.
     */
    explicit GenerateSection(QWidget *parent = nullptr);

signals:
    /**
     * @brief Señal emitida cuando el usuario solicita generar el archivo GCODE.
     *
     * Esta señal se emite cuando se hace clic en el botón "CREATE SAVE CONFIG".
     */
    void generateGcode();

public slots:
    /**
     * @brief Actualiza el texto de la interfaz según el idioma actual.
     *
     * Actualiza los textos de los botones y etiquetas para reflejar el idioma seleccionado.
     */
    void retranslateUi();

    /**
     * @brief Aplica estilos a los widgets de la sección.
     *
     * Configura los estilos visuales de los widgets según el modo oscuro.
     *
     * @param darkMode Indica si se debe aplicar el modo oscuro (true) o el modo claro (false).
     */
    void applyStyles(bool darkMode);

private slots:
    /**
     * @brief Copia el archivo GCODE original a una ubicación especificada por el usuario.
     *
     * Abre un cuadro de diálogo para que el usuario seleccione la ubicación de guardado y
     * copia el archivo GCODE original a esa ubicación. Muestra mensajes de éxito o error según corresponda.
     */
    void saveGcode();

private:
    QLabel *completionLabel; ///< Etiqueta que muestra el mensaje de configuración completada.
    QPushButton *generateButton; ///< Botón para generar el archivo GCODE.
    QPushButton *saveGcodeButton; ///< Botón para guardar el archivo GCODE.
};

#endif // GENERATESECTION_H
