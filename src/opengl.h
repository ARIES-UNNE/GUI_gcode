#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QDateTime>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QTimer *fileCheckTimer;
    QString filePath;
    QDateTime lastModifiedTime;

    QVector<QVector3D> vertices;

    void parseGCode(const QString &filePath);
    bool fileModified();

private slots:
    void checkFile();
};

#endif // OPENGLWIDGET_H
