#include "opengl.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QVector3D>

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), fileCheckTimer(new QTimer(this)), filePath("Axo3_1mat.gcode") {
    connect(fileCheckTimer, &QTimer::timeout, this, &OpenGLWidget::checkFile);
    fileCheckTimer->start(1000); // Check every second
    parseGCode(filePath);
}

OpenGLWidget::~OpenGLWidget() {
    delete fileCheckTimer;
}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
}

void OpenGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 100, 0, 100, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glColor3f(0.0f, 1.0f, 0.0f); // Green lines

    glBegin(GL_LINES);
    for (size_t i = 1; i < vertices.size(); ++i) {
        glVertex3f(vertices[i-1].x(), vertices[i-1].y(), vertices[i-1].z());
        glVertex3f(vertices[i].x(), vertices[i].y(), vertices[i].z());
    }
    glEnd();
}

void OpenGLWidget::parseGCode(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    float x = 0, y = 0, z = 0;
    vertices.clear(); // Clear existing vertices
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.startsWith("G1")) {
            QStringList tokens = line.split(' ');
            for (const QString &token : tokens) {
                if (token.startsWith('X')) {
                    x = token.mid(1).toFloat();
                } else if (token.startsWith('Y')) {
                    y = token.mid(1).toFloat();
                } else if (token.startsWith('Z')) {
                    z = token.mid(1).toFloat();
                }
            }
            vertices.emplace_back(x, y, z);
        }
    }
    file.close();
}

bool OpenGLWidget::fileModified() {
    QFile file(filePath);
    if (file.exists()) {
        QDateTime modifiedTime = file.fileTime(QFileDevice::FileModificationTime);
        if (modifiedTime != lastModifiedTime) {
            lastModifiedTime = modifiedTime;
            return true;
        }
    }
    return false;
}

void OpenGLWidget::checkFile() {
    if (fileModified()) {
        parseGCode(filePath);
        update(); // Update the OpenGL widget to reflect changes
    }
}
