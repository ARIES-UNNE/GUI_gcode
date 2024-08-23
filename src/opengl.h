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

// Estructura para almacenar la información de un segmento de línea
struct LineSegment {
    QPointF start;  // Punto de inicio del segmento
    QPointF end;    // Punto final del segmento
    QColor color;   // Color del segmento
    float thickness; // Nueva propiedad para el grosor
};

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget() override;

    void setPlateSize(int plateX, int plateY);

    // Método para agregar un nuevo material
    void addMaterial(const QString &code, const QColor &color);

    // Método para eliminar un material
    void removeMaterial(const QString &code);

    // Método para cambiar el color de un material
    void setMaterialColor(const QString &code, const QColor &color);

    // Método para seleccionar el material actual
    void setCurrentMaterial(const QString &code);


protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:

        QColor currentLayerColor;


    void updateMovementLimits();
    QPointF getFigureCenter() const;
    QPointF getViewCenter() const;
    QPointF getArrowDirection() const;
    void drawDirectionArrow(QPainter &painter) const;
    void parseGCode(const QString &filePath);
    bool fileModified();
    void checkFile();

    QTimer *fileCheckTimer;
    QString filePath;
    QDateTime lastModifiedTime;

    float zoomFactor;
    float offsetX;
    float offsetY;
    float minZoomFactor;
    float maxZoomFactor;

    float plateX;
    float plateY;

    struct MovementLimits {
        float left;
        float right;
        float bottom;
        float top;
    } movementLimits;

    QVector<QPointF> vertices;  // Almacena los vértices de las líneas (2D)
    QVector<LineSegment> lineSegments; // Almacena los segmentos de línea con colores

    QMap<QString, QColor> materialColors; // Mapa de materiales
    QString currentMaterialCode; // Código del material actual

    QPoint lastMousePosition;
};

#endif // OPENGLWIDGET_H
