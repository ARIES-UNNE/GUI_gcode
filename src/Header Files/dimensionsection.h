#ifndef DIMENSIONSECTION_H
#define DIMENSIONSECTION_H

#include <QWidget>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVector2D>

/**
 * @class DimensionSection
 * @brief Clase que proporciona la interfaz para la configuración de dimensiones y coordenadas del centro.
 *
 * Esta clase gestiona los widgets para introducir y mostrar las dimensiones de una placa y las
 * coordenadas del centro. Incluye métodos para actualizar y obtener los valores de estas dimensiones.
 */
class DimensionSection : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor de la clase DimensionSection.
     *
     * Inicializa los widgets de la sección y configura su disposición.
     *
     * @param parent El widget padre para este widget.
     */
    explicit DimensionSection(QWidget *parent = nullptr);

    /**
     * @brief Obtiene el valor actual del spin box de la dimensión X de la placa.
     *
     * @return El valor actual del spin box de la dimensión X.
     */
    int getPlateXSpinBox() const;

    /**
     * @brief Obtiene el valor actual del spin box de la dimensión Y de la placa.
     *
     * @return El valor actual del spin box de la dimensión Y.
     */
    int getPlateYSpinBox() const;

    /**
     * @brief Obtiene el valor actual del spin box de la coordenada X del centro.
     *
     * @return El valor actual del spin box de la coordenada X del centro.
     */
    int getcenterXSpinBox() const;

    /**
     * @brief Obtiene el valor actual del spin box de la coordenada Y del centro.
     *
     * @return El valor actual del spin box de la coordenada Y del centro.
     */
    int getcenterYSpinBox() const;

signals:
    /**
     * @brief Señal emitida cuando cualquier valor en la sección cambia.
     */
    void valueChanged();

    /**
     * @brief Señal emitida cuando el tamaño de la placa cambia.
     *
     * @param size Un QVector2D que representa el tamaño de la placa.
     */
    void plateSizeChanged(const QVector2D &size);

private slots:
    /**
     * @brief Muestra u oculta los campos para ingresar manualmente las coordenadas del centro.
     *
     * Alterna la visibilidad de los spin boxes y etiquetas para las coordenadas del centro.
     *
     * @param labelX Etiqueta para la coordenada X del centro.
     * @param labelY Etiqueta para la coordenada Y del centro.
     */
    void showManualCenterInput(QLabel *labelX, QLabel *labelY);

    /**
     * @brief Maneja el cambio en el valor del spin box de la dimensión X de la placa.
     *
     * Emite la señal plateSizeChanged con el nuevo tamaño de la placa.
     *
     * @param value El nuevo valor de la dimensión X.
     */
    void onPlateXChanged(int value) {
        emit plateSizeChanged(QVector2D(value, plateYSpinBox->value()));
    }

    /**
     * @brief Maneja el cambio en el valor del spin box de la dimensión Y de la placa.
     *
     * Emite la señal plateSizeChanged con el nuevo tamaño de la placa.
     *
     * @param value El nuevo valor de la dimensión Y.
     */
    void onPlateYChanged(int value) {
        emit plateSizeChanged(QVector2D(plateXSpinBox->value(), value));
    }


public slots:
    /**
     * @brief Actualiza los valores de los spin boxes con los valores proporcionados.
     *
     * Establece los valores de las dimensiones de la placa y las coordenadas del centro
     * en los spin boxes.
     *
     * @param plateX Valor de la dimensión X de la placa.
     * @param plateY Valor de la dimensión Y de la placa.
     * @param centerX Valor de la coordenada X del centro.
     * @param centerY Valor de la coordenada Y del centro.
     */
    void updateValues(int plateX, int plateY, int centerX, int centerY);

    /**
     * @brief Actualiza los textos de la interfaz según el idioma actual.
     *
     * Este método se usa para ajustar el texto de los widgets cuando se cambia el idioma
     * de la interfaz.
     */
    void retranslateUi();

    /**
     * @brief Aplica los estilos a los widgets de la sección.
     *
     * Configura los estilos de los widgets según el modo oscuro activado.
     *
     * @param darkMode Indica si el modo oscuro está habilitado.
     */
    void applyStyles(bool darkMode);

private:

    /**
     * @brief Spin box para las dimensiones de la placa.
     */
    QSpinBox *plateXSpinBox; ///< Spin box para la dimensión X de la placa.
    QSpinBox *plateYSpinBox; ///< Spin box para la dimensión Y de la placa.

    /**
     * @brief Spin box para la posción de la figura.
     */
    QSpinBox *centerXSpinBox; ///< Spin box para la coordenada X del centro.
    QSpinBox *centerYSpinBox; ///< Spin box para la coordenada Y del centro.

    /**
     * @brief Etiquetas para la interfaz de usuario.
     */
    QLabel *sectionTitle; ///< Título de la sección.
    QLabel *labelTitle; ///< Etiqueta para las dimensiones de la placa.
    QPushButton *manualCenterButton; ///< Botón para ingresar manualmente las coordenadas del centro.

};

#endif // DIMENSIONSECTION_H
