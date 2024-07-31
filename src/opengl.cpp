#include "opengl.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), fileCheckTimer(new QTimer(this)), filePath("Axo3_1mat.gcode"),
    zoomFactor(2.0f), offsetX(0.0f), offsetY(0.0f), minZoomFactor(1.0f), maxZoomFactor(3.7f) {
    connect(fileCheckTimer, &QTimer::timeout, this, &OpenGLWidget::checkFile);
    fileCheckTimer->start(1000);
    parseGCode(filePath);


    updateMovementLimits();
}


OpenGLWidget::~OpenGLWidget() {
    delete fileCheckTimer;
}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void OpenGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Ajustar la proyección para que el origen esté en el centro del widget
    float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
    if (aspectRatio >= 1.0f) {
        glOrtho(-100.0f * aspectRatio, 100.0f * aspectRatio, -100.0f, 100.0f, -1.0f, 1.0f);
    } else {
        glOrtho(-100.0f, 100.0f, -100.0f / aspectRatio, 100.0f / aspectRatio, -1.0f, 1.0f);
    }

    glMatrixMode(GL_MODELVIEW);

    // Ajustar el desplazamiento inicial para centrar el origen
    offsetX = 0.0f;
    offsetY = 0.0f;

    // Calcular límites de movimiento
    updateMovementLimits();
}

void OpenGLWidget::updateMovementLimits() {
    // La longitud total de los ejes es de 400 unidades (200 hacia cada lado)
    float length = 160.0f * zoomFactor;

    // Definir los límites basados en la longitud de los ejes
    movementLimits.left = -length;
    movementLimits.right = length;
    movementLimits.bottom = -length;
    movementLimits.top = length;
}

void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(offsetX, offsetY, 0.0f);
    glScalef(zoomFactor, zoomFactor, zoomFactor);

    // Draw extended X and Y axes
    glColor3f(1.0f, 1.0f, 1.0f); // White for axes
    glBegin(GL_LINES);
    // Extended X axis in both directions
    glVertex3f(-160.0f, 0.0f, 0.0f);
    glVertex3f(160.0f, 0.0f, 0.0f);
    // Extended Y axis in both directions
    glVertex3f(0.0f, -160.0f, 0.0f);
    glVertex3f(0.0f, 160.0f, 0.0f);
    glEnd();

    // Draw axis ticks
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    // Ticks on the X axis
    for (float x = -160.0f; x <= 160.0f; x += 10.0f) {
        if (x != 0.0f) { // Avoid tick at the center
            if (x == 160.0f || x == -160.0f) {
                // Extended ticks to form a square guide
                glVertex3f(x, -160.0f, 0.0f);
                glVertex3f(x, 160.0f, 0.0f);
            } else {
                glVertex3f(x, 2.0f, 0.0f);
                glVertex3f(x, -2.0f, 0.0f);
            }
        }
    }
    // Ticks on the Y axis
    for (float y = -160.0f; y <= 160.0f; y += 10.0f) {
        if (y != 0.0f) { // Avoid tick at the center
            if (y == 160.0f || y == -160.0f) {
                // Extended ticks to form a square guide
                glVertex3f(-160.0f, y, 0.0f);
                glVertex3f(160.0f, y, 0.0f);
            } else {
                glVertex3f(2.0f, y, 0.0f);
                glVertex3f(-2.0f, y, 0.0f);
            }
        }
    }
    glEnd();

    // Draw data lines
    glColor3f(0.0f, 1.0f, 0.0f);
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
        update();
    }
}

void OpenGLWidget::wheelEvent(QWheelEvent *event) {
    // Convertir la posición del ratón en coordenadas del mundo
    float mouseX = (event->position().x() - width() / 2) / zoomFactor + offsetX;
    float mouseY = (height() / 2 - event->position().y()) / zoomFactor + offsetY;

    if (event->angleDelta().y() > 0) {
        zoomFactor *= 1.1f;
    } else {
        zoomFactor /= 1.1f;
    }

    // Limitar el zoom dentro del rango permitido
    if (zoomFactor < minZoomFactor) {
        zoomFactor = minZoomFactor;
    } else if (zoomFactor > maxZoomFactor) {
        zoomFactor = maxZoomFactor;
    }

    updateMovementLimits();
    // Ajustar el offset para mantener el punto bajo el ratón en la misma posición
    offsetX = mouseX - (event->position().x() - width() / 2) / zoomFactor;
    offsetY = mouseY - (height() / 2 - event->position().y()) / zoomFactor;

    // Restringir el movimiento dentro de los límites
    if (offsetX < movementLimits.left) {
        offsetX = movementLimits.left;
    } else if (offsetX > movementLimits.right) {
        offsetX = movementLimits.right;
    }

    if (offsetY < movementLimits.bottom) {
        offsetY = movementLimits.bottom;
    } else if (offsetY > movementLimits.top) {
        offsetY = movementLimits.top;
    }

    update();
}


void OpenGLWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastMousePosition = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        QPoint delta = event->pos() - lastMousePosition;
        offsetX += delta.x() /  5.0f;
        offsetY -= delta.y() /  5.0f;

        // Restringir el movimiento dentro de los límites
        if (offsetX < movementLimits.left) {
            offsetX = movementLimits.left;
        } else if (offsetX > movementLimits.right) {
            offsetX = movementLimits.right;
        }

        if (offsetY < movementLimits.bottom) {
            offsetY = movementLimits.bottom;
        } else if (offsetY > movementLimits.top) {
            offsetY = movementLimits.top;
        }

        lastMousePosition = event->pos();
        update();
    }
}


void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        setCursor(Qt::ArrowCursor);
    }
}
