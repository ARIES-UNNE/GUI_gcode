#ifndef SHAPESECTION_H
#define SHAPESECTION_H

#include <QWidget>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QDoubleValidator>

/**
 * @brief Clase para la configuración de formas y tamaños geométricos.
 */
class ShapeSection : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor que inicializa la interfaz de usuario.
     * @param parent Widget padre.
     */
    explicit ShapeSection(QWidget *parent = nullptr);

    /**
     * @brief Maneja cambios en la selección de forma.
     * @param index Índice seleccionado.
     */
    void handleShapeSelection(int index);

    /**
     * @brief Obtiene el valor del primer tamaño.
     */
    int getSize1() const;

    /**
     * @brief Obtiene el valor del segundo tamaño.
     */
    int getSize2() const;

    /**
     * @brief Obtiene el índice de la primera forma.
     */
    int getShapeIndex1() const;

    /**
     * @brief Obtiene el índice de la segunda forma.
     */
    int getShapeIndex2() const;

signals:
    /**
     * @brief Señal emitida al cambiar un valor.
     */
    void valueChanged();


public slots:
    /**
     * @brief Actualiza los valores de forma y tamaño.
     */
    void updateValues(int size1, int size2, int shapeIndex1, int shapeIndex2);

    /**
     * @brief Retraduce la interfaz de usuario.
     */
    void retranslateUi();

    /**
     * @brief Aplica estilos a la interfaz.
     * @param darkMode Activa el modo oscuro si es true.
     */
    void applyStyles(bool darkMode);

private:
    /**
     * @brief Line edit widgets para entradas de tamaño.
     */
    QLineEdit *ShapeLineEdit; ///< Line edit para el tamaño 1.
    QLineEdit *HeightLineEdit; ///< Line edit para el tamaño 2.

    /**
     * @brief Combo box widgets para selección de forma.
     */
    QComboBox *shapeComboBox; ///< Combo box para la forma 1.
    QComboBox *HeightComboBox; ///< Combo box para la forma 2.

    /**
     * @brief Etiquetas para la interfaz de usuario.
     */
    QLabel *sectionTitle; ///< Título de la sección.
    QLabel *shapeLabel; ///< Etiqueta para la selección de forma.
    QLabel *sizeLabel; ///< Etiqueta para las entradas de tamaño.
    QLabel *mmLabel; ///< Etiqueta para la unidad de medida (milímetros).
    QLabel *mmNLabel; ///< Etiqueta adicional para la unidad de medida (milímetros).
    QLabel *sizeUnitLabel; ///< Etiqueta para la unidad de tamaño.


};

#endif // SHAPESECTION_H
