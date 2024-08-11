#include "opengl.h"
#include <QPainter>

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), fileCheckTimer(new QTimer(this)), filePath("Axo3_1mat.gcode"),
    zoomFactor(2.0f), offsetX(0.0f), offsetY(0.0f), minZoomFactor(0.5f), maxZoomFactor(3.7f),
    plateX(70.0f), plateY(70.0f) {
    connect(fileCheckTimer, &QTimer::timeout, this, &OpenGLWidget::checkFile);
    fileCheckTimer->start(1000);
    parseGCode(filePath);


    updateMovementLimits();
}

OpenGLWidget::~OpenGLWidget() {
    delete fileCheckTimer;
}

QPointF OpenGLWidget::getFigureCenter() const {
    if (vertices.empty()) {
        return QPointF(0.0f, 0.0f);
    }

    float sumX = 0.0f;
    float sumY = 0.0f;
    for (const auto &vertex : vertices) {
        sumX += vertex.x();
        sumY += vertex.y();
    }

    float centerX = sumX / vertices.size();
    float centerY = sumY / vertices.size();

    return QPointF(centerX, centerY);
}

QPointF OpenGLWidget::getViewCenter() const {
    return QPointF(-offsetX, -offsetY);
}

QPointF OpenGLWidget::getArrowDirection() const {
    QPointF figureCenter = getFigureCenter();
    QPointF viewCenter = getViewCenter();

    QPointF direction = figureCenter - viewCenter;
    qreal length = qSqrt(direction.x() * direction.x() + direction.y() * direction.y());

    if (length == 0.0) {
        return QPointF(0.0, 0.0); // Avoid division by zero
    }

    // Normalize the direction vector
    return QPointF(direction.x() / length, direction.y() / length);
}

void OpenGLWidget::setPlateSize(int plateX, int plateY) {
    this->plateX = plateX;
    this->plateY = plateY;
    update();
}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void OpenGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
    if (aspectRatio >= 1.0f) {
        glOrtho(-100.0f * aspectRatio, 100.0f * aspectRatio, -100.0f, 100.0f, -1.0f, 1.0f);
    } else {
        glOrtho(-100.0f, 100.0f, -100.0f / aspectRatio, 100.0f / aspectRatio, -1.0f, 1.0f);
    }

    glMatrixMode(GL_MODELVIEW);

    float xOffset = -1.0f * plateX;
    float yOffset = -1.0f * plateY;
    offsetX = xOffset;
    offsetY = yOffset;

    updateMovementLimits();
}

void OpenGLWidget::updateMovementLimits() {
    movementLimits.left = -150.0f * zoomFactor;
    movementLimits.right = 100.0f * zoomFactor;
    movementLimits.bottom = -150.0f * zoomFactor;
    movementLimits.top = 100.0f * zoomFactor;
}


void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(offsetX, offsetY, 0.0f);
    glScalef(zoomFactor, zoomFactor, zoomFactor);

    // Dibuja las líneas de la cuadrícula (guías) primero
    glColor3f(0.5f, 0.5f, 0.5f); // Color gris
    glBegin(GL_LINES);
    for (float x = 0.0f; x <= plateX; x += 10.0f) {
        glVertex3f(x, 0.0f, 0.0f);
        glVertex3f(x, plateY, 0.0f);
    }
    for (float y = 0.0f; y <= plateY; y += 10.0f) {
        glVertex3f(0.0f, y, 0.0f);
        glVertex3f(plateX, y, 0.0f);
    }
    glEnd();

    // Dibuja los ejes X e Y
    glColor3f(1.0f, 1.0f, 1.0f); // Color blanco
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(plateX, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, plateY, 0.0f);
    glEnd();

    // Dibuja las marcas de los ejes
    glColor3f(1.0f, 1.0f, 1.0f); // Color blanco
    glBegin(GL_LINES);
    for (float x = 0.0f; x <= plateX; x += 10.0f) {
        if (x != 0.0f) {
            glVertex3f(x, 2.0f, 0.0f);
            glVertex3f(x, -2.0f, 0.0f);
        }
    }
    for (float y = 0.0f; y <= plateY; y += 10.0f) {
        if (y != 0.0f) {
            glVertex3f(2.0f, y, 0.0f);
            glVertex3f(-2.0f, y, 0.0f);
        }
    }
    glEnd();

    // Dibuja el borde del plato (cuadrado)
    glColor3f(1.0f, 1.0f, 1.0f); // Color blanco
    glBegin(GL_LINE_LOOP);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(plateX, 0.0f, 0.0f);
    glVertex3f(plateX, plateY, 0.0f);
    glVertex3f(0.0f, plateY, 0.0f);
    glEnd();

    // Dibuja las líneas de datos después de la cuadrícula
    glColor3f(0.0f, 1.0f, 0.0f); // Color verde
    glBegin(GL_LINES);
    for (size_t i = 1; i < vertices.size(); ++i) {
        glVertex3f(vertices[i-1].x(), vertices[i-1].y(), vertices[i-1].z());
        glVertex3f(vertices[i].x(), vertices[i].y(), vertices[i].z());
    }
    glEnd();

    // Dibuja el texto del nivel de zoom usando QPainter
    QPainter painter(this);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 12));

    QString zoomText = QString("Zoom: %1").arg(zoomFactor, 0, 'f', 2); // 2 decimales
    painter.drawText(width() - 105, 25, zoomText);

    // Dibuja la flecha de dirección
    drawDirectionArrow(painter);
}

void OpenGLWidget::drawDirectionArrow(QPainter &painter) const {
    // Obtiene la dirección de la flecha
    QPointF arrowDirection = getArrowDirection();
    float arrowLength = 25.0f;

    // Configura el color y el pincel para la flecha
    painter.setPen(Qt::green);
    painter.setBrush(Qt::green);

    // Calcula el ángulo de rotación en radianes
    float angle = std::atan2(arrowDirection.y(), arrowDirection.x());

    // Posición de la flecha
    QPointF center(width() - 140, 30);

    // Guarda el estado del transformador
    painter.save();

    // Mueve el transformador al centro de la flecha
    painter.translate(center);

    // Rota el transformador por el ángulo calculado
    painter.rotate(-angle * 180.0 / M_PI);

    // Dibuja el palito de la flecha
    QLineF line(-arrowLength, 0, 0, 0);
    painter.drawLine(line);

    // Dibuja la punta de la flecha
    QPolygonF arrowHead;
    arrowHead << QPointF(-10, -5)
              << QPointF(0, 0)
              << QPointF(-10, 5);

    painter.drawPolygon(arrowHead);

    // Restaura el estado original del transformador
    painter.restore();
}


void OpenGLWidget::parseGCode(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    float x = 0, y = 0, z = 0;
    vertices.clear();
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
    float mouseX = (event->position().x() - width() / 2) / zoomFactor + offsetX;
    float mouseY = (height() / 2 - event->position().y()) / zoomFactor + offsetY;

    if (event->angleDelta().y() > 0) {
        zoomFactor *= 1.1f;
    } else {
        zoomFactor /= 1.1f;
    }

    if (zoomFactor < minZoomFactor) {
        zoomFactor = minZoomFactor;
    } else if (zoomFactor > maxZoomFactor) {
        zoomFactor = maxZoomFactor;
    }

    updateMovementLimits();
    offsetX = mouseX - (event->position().x() - width() / 2) / zoomFactor;
    offsetY = mouseY - (height() / 2 - event->position().y()) / zoomFactor;

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
        offsetX += delta.x() / 5.0f;
        offsetY -= delta.y() / 5.0f;

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
