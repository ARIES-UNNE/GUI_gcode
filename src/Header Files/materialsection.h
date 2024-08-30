#ifndef MATERIALSECTION_H
#define MATERIALSECTION_H

#include <QWidget>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QList>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QComboBox>
#include <QDoubleValidator>

/**
 * @struct MaterialConfig
 * @brief Almacena detalles de configuración de material.
 *
 * Incluye el nombre del material, el tipo de filamento y la boquilla. y sus configuraciones de guardado
 */
struct MaterialConfig {
    QString name; ///< Nombre del material.
    QString filament; ///< Tipo de filamento.
    QString nozzle; ///< Tipo de boquilla.
    QString filamentSave; ///< Tipo de filamento.
    QString nozzleSave; ///< Tipo de boquilla.
};


/**
 * @class MaterialSection
 * @brief Widget para gestionar configuraciones de materiales.
 *
 * Permite seleccionar y gestionar múltiples configuraciones de materiales,
 * incluyendo el nombre, tipo de filamento y boquilla.
 */
class MaterialSection : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor de MaterialSection.
     *
     * Inicializa los widgets y configura la disposición del widget.
     *
     * @param parent Widget padre, si lo hay.
     */
    explicit MaterialSection(QWidget *parent = nullptr);

    /**
     * @brief Obtiene la lista de configuraciones de material.
     *
     * @return Lista de configuraciones de material.
     */
    QList<MaterialConfig> getMaterialConfigs() const;

    /**
     * @brief Obtiene el número de materiales configurados.
     *
     * @return Número de materiales.
     */
    int getNumMaterials() const;

signals:
    /**
     * @brief Señal emitida cuando cualquier valor cambia.
     */
    void valueChanged();

private slots:
    /**
     * @brief Crea los widgets de cada material en función del número de materiales.
     *
     * @param numMaterials Número de materiales a configurar.
     */
    void updateMaterialNames(int numMaterials);

    /**
     * @brief Actualiza las configuraciones de los materiales.
     */
    void updateMaterialConfigs();

    /**
     * @brief Actualiza los valores del material en los combo boxes según el preset seleccionado.
     *
     * @param materialComboBox Combo box para seleccionar el material.
     * @param filamentComboBox Combo box para seleccionar el filamento.
     * @param nozzleComboBox Combo box para seleccionar la boquilla.
     */
    void updateMaterialPreset(QComboBox *materialComboBox, QComboBox *filamentComboBox, QComboBox *nozzleComboBox);

    /**
     * @brief Restringe las opciones de boquilla en el combo box a las opciones válidas.
     *
     * @param nozzleComboBox Combo box para seleccionar la boquilla.
     * @param validOptions Lista de opciones válidas para la boquilla.
     */
    void restrictNozzleOptions(QComboBox *nozzleComboBox, const QStringList &validOptions);

public slots:
    /**
     * @brief Carga la informacion de la configuración de guardado en caso de que este activa.
     *
     * @param materials Lista de pares de nombre y tipo de material.
     */
    void updateMaterialSection(const QList<QPair<QString, QString>> &materials);

    /**
     * @brief Retraduce la interfaz de usuario.
     *
     * Ajusta los textos de los widgets según el idioma actual.
     */
    void retranslateUi();

    /**
     * @brief Aplica los estilos a la interfaz.
     *
     * Configura los estilos de los widgets según el modo oscuro.
     *
     * @param darkMode Indica si se debe aplicar el modo oscuro.
     */
    void applyStyles(bool darkMode);

private:
    /**
     * @brief Spin box para seleccionar el número de materiales.
     */
    QSpinBox *numMaterialsSpinBox;

    /**
     * @brief Widget contenedor para los nombres de los materiales.
     */
    QWidget *materialNamesContainer;

    /**
     * @brief Layout para organizar los nombres de los materiales.
     */
    QVBoxLayout *materialNamesLayout;

    /**
     * @brief Lista para almacenar las configuraciones de los materiales.
     */
    QList<MaterialConfig> materialConfigs;

    /**
     * @brief Aplica los estilos a los widgets de la sección.
     */
    void applyStyles();

    /**
     * @brief Etiquetas para la interfaz de usuario.
     */
    QLabel *sectionTitle; ///< Título de la sección.
    QLabel *headerLabel1; ///< Primera etiqueta del encabezado.
    QLabel *headerLabel2; ///< Segunda etiqueta del encabezado.
    QLabel *headerLabel3; ///< Tercera etiqueta del encabezado.
    QLabel *headerLabel4; ///< Cuarta etiqueta del encabezado.
};

#endif // MATERIALSECTION_H
