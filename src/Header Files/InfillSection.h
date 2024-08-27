#ifndef INFILLSECTION_H
#define INFILLSECTION_H

#include <QWidget>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QVBoxLayout>
/**
 * @class InfillSection
 * @brief Widget para configurar el relleno en la interfaz.
 *
 * Permite seleccionar el método de relleno y ajustar el porcentaje o distancia entre hilos.
 */
class InfillSection : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor de InfillSection.
     *
     * Inicializa los widgets y configura el diseño del widget.
     *
     * @param parent Widget padre, si lo hay.
     */
    explicit InfillSection(QWidget *parent = nullptr);

    /**
     * @brief Maneja los cambios en el método seleccionado.
     *
     * @param index Índice del método en el combo box.
     */
    void handleMethodSelection(int index);

    /**
     * @brief Obtiene el valor del porcentaje de relleno.
     *
     * @return Valor actual del spin box.
     */
    int getInfillValue() const;

    /**
     * @brief Obtiene el índice del método seleccionado.
     *
     * @return Índice del método en el combo box.
     */
    int getShapeIndex() const;

    /**
     * @brief Obtiene el valor de la distancia entre hilos.
     *
     * @return Valor actual del line edit.
     */
    double getStrandDistanceValue() const;

signals:
    /**
     * @brief Señal emitida cuando cambia un valor.
     */
    void valueChanged();

public slots:
    /**
     * @brief Actualiza los valores de los widgets.
     *
     * @param methodIndex Índice del método.
     * @param infillValue Valor del porcentaje de relleno.
     * @param strandDistance Valor de la distancia entre hilos.
     */
    void updateValues(int methodIndex, int infillValue, double strandDistance);

    /**
     * @brief Actualiza los textos de los widgets para el idioma actual.
     */
    void retranslateUi();

    /**
     * @brief Aplica estilos visuales al widget.
     *
     * @param darkMode Indica si se debe aplicar el modo oscuro.
     */
    void applyStyles(bool darkMode);


private:
    /**
     * @brief Spin box para el porcentaje de relleno.
     */
    QSpinBox *infillSpinBox;

    /**
     * @brief Combo box para seleccionar la forma de relleno.
     */
    QComboBox *shapeComboBox;

    /**
     * @brief Line edit para la distancia entre hilos.
     */
    QLineEdit *strandDistanceLineEdit;

    /**
     * @brief Etiquetas para la interfaz de usuario.
     */
    QLabel *percentage; ///< Etiqueta para el porcentaje de relleno.
    QLabel *millimeters; ///< Etiqueta para las unidades de medida (milímetros).

    QLabel *sectionTitle; ///< Título de la sección.
    QLabel *title; ///< Etiqueta para el título de la sección.

};

#endif // INFILLSECTION_H
