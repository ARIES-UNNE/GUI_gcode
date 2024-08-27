#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

/**
 * @class SectionStart
 * @brief Widget de la página de inicio para la aplicación de impresión 3D.
 *
 * Esta clase representa la página de inicio de la aplicación, donde el usuario puede seleccionar el idioma
 * y empezar la configuración. Incluye un combo box para seleccionar el idioma, una etiqueta de bienvenida
 * y un botón para iniciar el proceso.
 */
class SectionStart : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor de SectionStart.
     *
     * Inicializa los widgets y configura el diseño del widget de la página de inicio.
     *
     * @param parent Widget padre, si lo hay.
     */
    explicit SectionStart(QWidget *parent = nullptr);

    /**
     * @brief Retraduce los textos de los widgets según el idioma actual.
     *
     * Actualiza el texto de la etiqueta de bienvenida y del botón para reflejar las traducciones adecuadas.
     */
    void retranslateUi();

    /**
     * @brief Aplica estilos a los widgets según el modo oscuro.
     *
     * Configura los estilos visuales de los widgets en función de si el modo oscuro está activado.
     *
     * @param darkMode Indica si se debe aplicar el modo oscuro (true) o el modo claro (false).
     */
    void applyStyles(bool darkMode);

signals:
    /**
     * @brief Señal emitida cuando el botón de inicio es clicado.
     */
    void startButtonClicked();

    /**
     * @brief Señal emitida cuando el idioma seleccionado cambia.
     *
     * @param language Código del idioma seleccionado.
     */
    void languageChanged(const QString &language);

private:
    QPushButton *startButton; ///< Botón para iniciar el proceso.
    QComboBox *languageComboBox; ///< Combo box para seleccionar el idioma.
    QLabel *welcomeLabel; ///< Etiqueta de bienvenida.
};

#endif // STARTPAGE_H
