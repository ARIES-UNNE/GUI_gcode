#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QDateTime>
#include <QFile>
#include <QVector3D>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QFont>
#include <QFontMetrics>
#include <QPainter>

struct MovementLimits {
    float left;
    float right;
    float bottom;
    float top;
};

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;



private slots:
    void checkFile();

public slots:
    void setPlateSize(int plateX, int plateY);

private:
    void parseGCode(const QString &filePath);
    bool fileModified();

    QTimer *fileCheckTimer;
    QString filePath;
    QDateTime lastModifiedTime;
    QVector<QVector3D> vertices;
    float zoomFactor;
    QPoint lastMousePosition;
    float offsetX;
    float offsetY;

    float minZoomFactor;
    float maxZoomFactor;

    void updateMovementLimits();

    MovementLimits movementLimits;

    QPointF getFigureCenter() const;
    QPointF getViewCenter() const;
    QPointF getArrowDirection() const;

    void drawDirectionArrow(QPainter &painter) const;



    float plateX;
    float plateY;

};

#endif // OPENGLWIDGET_H
