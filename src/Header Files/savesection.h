#ifndef SAVESECTION_H
#define SAVESECTION_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QDir>
#include <QLabel>
#include <QProcess>
#include <QCoreApplication>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>

/**
 * @brief La clase SaveSection proporciona una interfaz para seleccionar y gestionar configuraciones de guardado.
 *
 * Esta clase permite al usuario seleccionar configuraciones de guardado de un directorio custom y cargarlas
 * en los diferentes widgets de cada sección.
 */
class SaveSection : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor de la clase SaveSection.
     *
     * Inicializa la interfaz de usuario, configura los botones y el diseño, y carga la ruta del directorio desde QSettings.
     *
     * @param parent El widget padre (opcional).
     */
    explicit SaveSection(QWidget *parent = nullptr);

private:
    /**
     * @brief Layout para organizar los widgets verticalmente.
     */
    QVBoxLayout *layout;

    /**
     * @brief Etiqueta que proporciona instrucciones al usuario.
     */
    QLabel *instructionLabel;

    /**
     * @brief Botón para cambiar el directorio donde se guardan las configuraciones.
     */
    QPushButton *changeDirectoryButton;

    /**
     * @brief Ruta del directorio actual donde se almacenan las configuraciones.
     */
    QString directoryPath;

    /**
     * @brief Widget de desplazamiento que contiene la lista de botones de configuración.
     */
    QWidget *scrollWidget;

    /**
     * @brief Crear una lista de archivos de configuración en el directorio actual.
     *
     * Lee los archivos `.conf` del directorio y crea un botón para cada archivo, añadiéndolos al diseño.
     */
    void createConfigurations();

    /**
     * @brief Limpiar el diseño de botones proporcionado.
     *
     * Elimina todos los widgets y elementos del diseño especificado.
     *
     * @param layout El diseño cuyo contenido debe limpiarse.
     */
    void clearLayout(QLayout *layout);

    /**
     * @brief Leer los valores de configuración desde el archivo `section_values.txt`.
     *
     * Lee los valores de configuración del archivo y emite señales con los valores leídos para cada sección.
     */
    void readSectionValues();

    /**
     * @brief Limpiar las configuraciones guardadas en QSettings.
     *
     * Elimina las configuraciones almacenadas en QSettings.
     */
    void clearSettings();

    /**
     * @brief Indica si el modo oscuro está activado.
     */
    bool darkMode;

    // Estilos

signals:
    /**
     * @brief Señal emitida para cargar un archivo de configuración.
     *
     * Se emite cuando se selecciona un archivo de configuración.
     *
     * @param filePath La ruta del archivo de configuración.
     */
    void loadConfiguration(const QString &filePath);

    /**
     * @brief Señal emitida con los valores leídos para la Sección 1.
     *
     * @param plateX Valor del ancho de la placa.
     * @param plateY Valor del largo de la placa.
     * @param centerX Valor de la coordenada X del centro.
     * @param centerY Valor de la coordenada Y del centro.
     */
    void section1ValuesRead(int plateX, int plateY, int centerX, int centerY);

    /**
     * @brief Señal emitida con los valores leídos para la Sección 2.
     *
     * @param size1 Tamaño del primer elemento.
     * @param size2 Tamaño del segundo elemento.
     * @param shapeIndex1 Índice de la primera forma.
     * @param shapeIndex2 Índice de la segunda forma.
     */
    void section2ValuesRead(int size1, int size2, int shapeIndex1, int shapeIndex2);

    /**
     * @brief Señal emitida con los valores leídos para la Sección 3.
     *
     * @param methodIndex Índice del método de relleno.
     * @param infillValue Valor del relleno.
     * @param strandDistance Distancia entre hilos.
     */
    void section3ValuesRead(int methodIndex, int infillValue, double strandDistance);

    /**
     * @brief Señal emitida con los valores leídos para la Sección 4.
     *
     * @param materials Lista de materiales, donde cada material es un par de nombre y propiedades.
     */
    void section4ValuesRead(const QList<QPair<QString, QString>> &materials);

    /**
     * @brief Señal emitida cuando se selecciona un archivo de configuración.
     *
     * @param fileName El nombre del archivo de configuración seleccionado.
     */
    void fileSelected(const QString &fileName);

    /**
     * @brief Señal emitida para notificar la nueva ruta del directorio.
     *
     * @param fileRoute La nueva ruta del directorio.
     */
    void fileRoute(const QString &fileRoute);

private slots:
    /**
     * @brief Cambiar el directorio donde se guardan las configuraciones.
     *
     * Abre un cuadro de diálogo para seleccionar un nuevo directorio y actualiza la ruta en `QSettings`.
     */
    void changeDirectory();

    /**
     * @brief Manejar el clic en un botón de configuración.
     *
     * Se llama cuando el usuario hace clic en un botón de configuración. Copia el archivo de configuración
     * y emite una señal con el nombre del archivo.
     *
     * @param fileName El nombre del archivo de configuración seleccionado.
     */
    void onConfButtonClicked(const QString &fileName);

public slots:
    /**
     * @brief Copiar el archivo de configuración seleccionado.
     *
     * Copia el archivo de configuración al archivo `section_values.txt` y ejecuta un script de Python
     * para procesar la configuración.
     *
     * @param confFilePath La ruta del archivo de configuración a copiar.
     */
    void copyConfiguration(const QString &confFilePath);

    /**
     * @brief Retraducir la interfaz de usuario.
     *
     * Se utiliza para actualizar las cadenas de texto en la interfaz de usuario cuando se cambian los idiomas.
     */
    void retranslateUi();

    /**
     * @brief Aplicar estilos a la interfaz.
     *
     * Cambia el estilo de la interfaz de usuario en función del modo oscuro.
     *
     * @param darkMode Indica si el modo oscuro está activado.
     */
    void applyStyles(bool darkMode);
};

#endif // SAVESECTION_H
