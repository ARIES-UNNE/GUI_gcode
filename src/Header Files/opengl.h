#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>
#include <QVector>
#include <QColor>
#include <QPointF>


/**
 * @struct LineSegment
 * @brief Representa un segmento de línea con inicio, fin, color y grosor.
 */
struct LineSegment {
    QPointF start;      ///< Punto de inicio
    QPointF end;        ///< Punto final
    QColor color;       ///< Color del segmento
    float thickness;    ///< Grosor del segmento
};

/**
 * @brief Clase que gestiona la visualización y manipulación de gráficos 2D en un widget OpenGL.
 *
 * Esta clase se encarga de inicializar OpenGL, gestionar eventos de entrada (ratón y rueda),
 * y renderizar gráficos basados en datos de un archivo GCODE. También maneja el zoom y
 * desplazamiento de la vista, así como la actualización dinámica del contenido gráfico.
 */
class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    /**
     * @brief Constructor de OpenGLWidget.
     * @param parent Widget padre.
     */
    explicit OpenGLWidget(QWidget *parent = nullptr);

    /**
     * @brief Destructor de OpenGLWidget.
     */
    ~OpenGLWidget() override;

    /**
     * @brief Establece el tamaño de la placa de impresión.
     * @param plateX Ancho de la placa.
     * @param plateY Alto de la placa.
     */
    void setPlateSize(int plateX, int plateY);

    /**
     * @brief Agrega un nuevo material.
     * @param code Código del material.
     * @param color Color del material.
     */
    void addMaterial(const QString &code, const QColor &color);

    /**
     * @brief Elimina un material.
     * @param code Código del material a eliminar.
     */
    void removeMaterial(const QString &code);

    /**
     * @brief Cambia el color de un material.
     * @param code Código del material.
     * @param color Nuevo color del material.
     */
    void setMaterialColor(const QString &code, const QColor &color);

    /**
     * @brief Selecciona el material actual.
     * @param code Código del material a seleccionar.
     */
    void setCurrentMaterial(const QString &code);

protected:
    /**
     * @brief Inicializa el contexto OpenGL.
     * Configura el entorno OpenGL cuando se crea el widget.
     */
    void initializeGL() override;

    /**
     * @brief Redimensiona el viewport OpenGL.
     * @param w Ancho del nuevo tamaño.
     * @param h Alto del nuevo tamaño.
     * Ajusta la proyección y el viewport de OpenGL al cambiar el tamaño del widget.
     */
    void resizeGL(int w, int h) override;

    /**
     * @brief Dibuja la escena OpenGL.
     * Renderiza la escena en el widget OpenGL.
     */
    void paintGL() override;

    /**
     * @brief Maneja eventos de rueda del ratón.
     * @param event Evento de rueda del ratón.
     * Ajusta el factor de zoom y el desplazamiento basado en la rueda del ratón.
     */
    void wheelEvent(QWheelEvent *event) override;

    /**
     * @brief Maneja eventos de presión del ratón.
     * @param event Evento de presión del ratón.
     * Inicia el arrastre de la vista cuando se presiona el botón izquierdo del ratón.
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief Maneja eventos de movimiento del ratón.
     * @param event Evento de movimiento del ratón.
     * Desplaza la vista cuando se arrastra el ratón mientras se mantiene presionado el botón izquierdo.
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @brief Maneja eventos de liberación del ratón.
     * @param event Evento de liberación del ratón.
     * Restaura el cursor cuando se libera el botón izquierdo del ratón.
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    /**
     * @brief Actualiza los límites de movimiento según el factor de zoom.
     * Ajusta los límites de movimiento del widget OpenGL basado en el factor de zoom actual.
     */
    void updateMovementLimits();

    /**
     * @brief Obtiene el centro de los vértices de la figura.
     * @return El centro de la figura en coordenadas de la vista.
     */
    QPointF getFigureCenter() const;

    /**
     * @brief Obtiene el centro de la vista.
     * @return El centro de la vista en coordenadas de la vista.
     */
    QPointF getViewCenter() const;

    /**
     * @brief Obtiene la dirección de la flecha de dirección.
     * @return La dirección de la flecha normalizada.
     */
    QPointF getArrowDirection() const;

    /**
     * @brief Dibuja la flecha de dirección.
     * @param painter El objeto QPainter utilizado para dibujar la flecha.
     * Dibuja una flecha en el widget que indica la dirección de la figura.
     */
    void drawDirectionArrow(QPainter &painter) const;

    /**
     * @brief Analiza el archivo GCODE y actualiza los segmentos de línea.
     * @param filePath La ruta del archivo GCODE a analizar.
     * Lee el archivo GCODE y actualiza los vértices y segmentos de línea.
     */
    void parseGCode(const QString &filePath);

    /**
     * @brief Verifica si el archivo GCODE ha sido modificado.
     * @return Verdadero si el archivo ha sido modificado, falso en caso contrario.
     */
    bool fileModified();

    /**
     * @brief Comprueba el archivo GCODE y actualiza si es necesario.
     * Llama a `parseGCode` si el archivo ha sido modificado.
     */
    void checkFile();

    QTimer *fileCheckTimer; ///< Temporizador para comprobar modificaciones del archivo GCODE.
    QString filePath; ///< Ruta del archivo GCODE.
    QDateTime lastModifiedTime; ///< Última modificación del archivo.

    float zoomFactor; ///< Factor de zoom actual.
    float offsetX; ///< Desplazamiento horizontal.
    float offsetY; ///< Desplazamiento vertical.
    float minZoomFactor; ///< Factor de zoom mínimo permitido.
    float maxZoomFactor; ///< Factor de zoom máximo permitido.

    float plateX; ///< Ancho de la placa.
    float plateY; ///< Alto de la placa.

    struct MovementLimits {
        float left; ///< Límite izquierdo de movimiento.
        float right; ///< Límite derecho de movimiento.
        float bottom; ///< Límite inferior de movimiento.
        float top; ///< Límite superior de movimiento.
    } movementLimits;

    QVector<QPointF> vertices; ///< Vértices de los segmentos de línea.
    QVector<LineSegment> lineSegments; ///< Segmentos de línea con color y grosor.

    QMap<QString, QColor> materialColors; ///< Mapa de colores para los materiales.
    QString currentMaterialCode; ///< Código del material actualmente seleccionado.

    QPoint lastMousePosition; ///< Última posición del ratón.
};

#endif // OPENGLWIDGET_H

