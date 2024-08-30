#include "opengl.h"
#include <QPainter>

/**
     * @brief Constructor de OpenGLWidget.
     *
     * Inicializa el temporizador para comprobar el archivo GCODE periódicamente.
     * Establece los parámetros iniciales para el zoom, el desplazamiento y las dimensiones del plato.
     * Conecta la señal del temporizador a la función de verificación del archivo.
     */
OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), fileCheckTimer(new QTimer(this)), filePath("Axo3_1mat.gcode"),
    zoomFactor(2.0f), offsetX(0.0f), offsetY(0.0f), minZoomFactor(0.5f), maxZoomFactor(3.7f),
    plateX(70.0f), plateY(70.0f) {

    connect(fileCheckTimer, &QTimer::timeout, this, &OpenGLWidget::checkFile);
    fileCheckTimer->start(1000); // Comprobar cada 1000 ms

    parseGCode(filePath); // Analiza el archivo GCODE inicial

    updateMovementLimits(); // Actualiza los límites de movimiento de la vista
}

/**
     * @brief Destructor de OpenGLWidget.
     *
     * Libera el temporizador al destruir el widget.
     */
OpenGLWidget::~OpenGLWidget() {

    delete fileCheckTimer;
}

/**
     * @brief Calcula el centro de la figura basada en los vértices.
     *
     * Suma las coordenadas de todos los vértices y promedia las coordenadas x e y.
     * Si no hay vértices, retorna el punto (0, 0).
     *
     * @return El punto central de la figura.
     */
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

/**
     * @brief Obtiene el centro de la vista.
     *
     * Calcula el centro de la vista basado en el desplazamiento actual.
     *
     * @return El punto central de la vista.
     */
QPointF OpenGLWidget::getViewCenter() const {

    return QPointF(-offsetX, -offsetY);
}

/**
     * @brief Calcula la dirección de la flecha en la vista.
     *
     * Calcula la dirección desde el centro de la figura hasta el centro de la vista.
     * Normaliza el vector de dirección para obtener una dirección unitaria.
     *
     * @return La dirección de la flecha normalizada.
     */
QPointF OpenGLWidget::getArrowDirection() const {

    QPointF figureCenter = getFigureCenter();
    QPointF viewCenter = getViewCenter();

    QPointF direction = figureCenter - viewCenter;
    qreal length = qSqrt(direction.x() * direction.x() + direction.y() * direction.y());

    if (length == 0.0) {
        return QPointF(0.0, 0.0);
    }

    return QPointF(direction.x() / length, direction.y() / length);
}

/**
     * @brief Establece el tamaño del plato de impresión.
     *
     * Actualiza las dimensiones del plato de impresión y redibuja la vista.
     *
     * @param plateX Ancho del plato.
     * @param plateY Alto del plato.
     */
void OpenGLWidget::setPlateSize(int plateX, int plateY) {

    this->plateX = plateX;
    this->plateY = plateY;
    update(); // Redibuja el widget con el nuevo tamaño del plato
}

/**
     * @brief Inicializa OpenGL.
     *
     * Configura el color de fondo de la ventana OpenGL.
     */
void OpenGLWidget::initializeGL() {

    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Color de fondo negro
}

/**
     * @brief Maneja el redimensionamiento del widget.
     *
     * Actualiza la vista ortográfica basada en el nuevo tamaño del widget.
     * Ajusta el desplazamiento de la vista y los límites de movimiento.
     *
     * @param w Nuevo ancho del widget.
     * @param h Nuevo alto del widget.
     */
void OpenGLWidget::resizeGL(int w, int h) {
    // Establecer el área de vista para el tamaño del widget
    glViewport(0, 0, w, h);

    // Configurar la matriz de proyección
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Calcular la relación de aspecto del widget
    float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
    // Configurar la proyección ortográfica basada en la relación de aspecto
    if (aspectRatio >= 1.0f) {
        glOrtho(-100.0f * aspectRatio, 100.0f * aspectRatio, -100.0f, 100.0f, -1.0f, 1.0f);
    } else {
        glOrtho(-100.0f, 100.0f, -100.0f / aspectRatio, 100.0f / aspectRatio, -1.0f, 1.0f);
    }

    // Configurar la matriz del modelo de vista
    glMatrixMode(GL_MODELVIEW);

    // Ajustar el desplazamiento de la vista basado en el tamaño de la placa
    float xOffset = -1.0f * plateX;
    float yOffset = -1.0f * plateY;
    offsetX = xOffset;
    offsetY = yOffset;

    // Actualizar los límites de movimiento en función del nuevo tamaño
    updateMovementLimits();
}

/**
     * @brief Actualiza los límites de movimiento basados en el factor de zoom actual.
     *
     * Calcula los límites de movimiento permitidos en función del factor de zoom.
     */
void OpenGLWidget::updateMovementLimits() {
    movementLimits.left = -150.0f * zoomFactor;
    movementLimits.right = 100.0f * zoomFactor;
    movementLimits.bottom = -150.0f * zoomFactor;
    movementLimits.top = 100.0f * zoomFactor;
}

/**
     * @brief Dibuja el contenido de la ventana OpenGL.
     *
     * Limpia el búfer de color y profundidad. Aplica desplazamiento y escala.
     * Dibuja la cuadrícula, los ejes, el borde del plato y las líneas de datos.
     * Muestra el nivel de zoom y la dirección de la flecha.
     */
void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(offsetX, offsetY, 0.0f);
    glScalef(zoomFactor, zoomFactor, zoomFactor);

    // Dibuja las líneas de la cuadrícula
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

    // Dibuja el borde del plato
    glColor3f(1.0f, 1.0f, 1.0f); // Color blanco
    glBegin(GL_LINE_LOOP);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(plateX, 0.0f, 0.0f);
    glVertex3f(plateX, plateY, 0.0f);
    glVertex3f(0.0f, plateY, 0.0f);
    glEnd();

    // Dibuja las líneas de datos
    for (const LineSegment &segment : lineSegments) {
        QColor color = segment.color;
        glColor3f(color.redF(), color.greenF(), color.blueF()); // Convertir QColor a flotantes

        // Ajusta el grosor de la línea
        glLineWidth(segment.thickness);

        glBegin(GL_LINES);
        glVertex3f(segment.start.x(), segment.start.y(), 0.0f);
        glVertex3f(segment.end.x(), segment.end.y(), 0.0f);
        glEnd();
    }

    // Restaura el grosor de la línea al valor predeterminado
    glLineWidth(1.0f);

    QPainter painter(this);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 12));

    // Muestra el nivel de zoom
    QString zoomText = QString("Zoom: %1").arg(zoomFactor, 0, 'f', 2); // 2 decimales
    painter.drawText(width() - 105, 25, zoomText);

    // Dibuja la flecha de dirección
    drawDirectionArrow(painter);
}

/**
     * @brief Dibuja una flecha que indica la dirección desde el centro de la figura al centro de la vista.
     *
     * Calcula la dirección de la flecha basada en la posición relativa entre el centro de la figura y la vista.
     * Dibuja la flecha en la esquina superior derecha del widget.
     *
     * @param painter El objeto QPainter utilizado para dibujar la flecha.
     */
void OpenGLWidget::drawDirectionArrow(QPainter &painter) const {
    // Obtener la dirección de la flecha
    QPointF arrowDirection = getArrowDirection();
    float arrowLength = 25.0f;

    // Configurar el color de la flecha
    painter.setPen(Qt::green);
    painter.setBrush(Qt::green);

    // Calcular el ángulo de rotación para alinear la flecha con la dirección
    float angle = std::atan2(arrowDirection.y(), arrowDirection.x());

    // Definir el punto central de la flecha en el widget
    QPointF center(width() - 140, 30);

    // Guardar el estado del pintor antes de la transformación
    painter.save();

    // Mover el punto de origen al centro de la flecha y rotar para alinear la dirección
    painter.translate(center);
    painter.rotate(-angle * 180.0 / M_PI);

    // Dibujar la línea de la flecha
    QLineF line(-arrowLength, 0, 0, 0);
    painter.drawLine(line);

    // Dibujar la cabeza de la flecha
    QPolygonF arrowHead;
    arrowHead << QPointF(-10, -5)
              << QPointF(0, 0)
              << QPointF(-10, 5);

    painter.drawPolygon(arrowHead);

    // Restaurar el estado del pintor a su estado original
    painter.restore();
}

/**
     * @brief Analiza un archivo GCODE y actualiza la lista de segmentos de línea.
     *
     * Lee el archivo GCODE especificado en `filePath`, procesando los comandos `G1`, `G0` y `T`.
     * Los comandos `G1` y `G0` actualizan las coordenadas de los vértices y generan segmentos de línea,
     * mientras que los comandos `T` cambian el color del material. Los segmentos de línea se escalan en grosor
     * en función del valor de extrusión `E`. Los comentarios y líneas vacías se ignoran.
     *
     * @param filePath Ruta del archivo GCODE a analizar.
     */
void OpenGLWidget::parseGCode(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return; // No se puede abrir el archivo, salir de la función
    }

    QTextStream in(&file);
    float x = 0, y = 0, e = 0;
    bool hasPosition = false;

    vertices.clear(); // Limpiar la lista de vértices
    lineSegments.clear(); // Limpiar la lista de segmentos de línea

    const float thicknessScale = 0.5f; // Factor de escala para el grosor

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith(';')) {
            continue; // Ignorar comentarios y líneas vacías
        }

        if (line.startsWith("G1")) {
            QStringList tokens = line.split(' ');
            float newX = x, newY = y, newE = e;
            bool foundX = false, foundY = false, foundE = false;

            for (const QString &token : tokens) {
                if (token.startsWith('X')) {
                    newX = token.mid(1).toFloat();
                    foundX = true;
                } else if (token.startsWith('Y')) {
                    newY = token.mid(1).toFloat();
                    foundY = true;
                } else if (token.startsWith('E')) {
                    newE = token.mid(1).toFloat();
                    foundE = true;
                }
            }

            // Procesar la línea solo si tiene extrusión (E) y posición
            if (foundE && (foundX || foundY)) {
                if (hasPosition) {
                    LineSegment segment;
                    segment.start = QPointF(x, y);
                    segment.end = QPointF(newX, newY);
                    segment.color = materialColors.value(currentMaterialCode, Qt::green); // Color por defecto
                    segment.thickness = (newE - e) * thicknessScale; // Aplicar el factor de escala
                    if (segment.thickness < 1.0f) segment.thickness = 1.0f; // Grosor mínimo
                    lineSegments.push_back(segment); // Añadir el segmento de línea a la lista
                }

                x = newX; // Actualizar posición actual
                y = newY;
                e = newE;
                hasPosition = true; // Indicar que se tiene una posición válida
            }
        } else if (line.startsWith("G0")) {
            QStringList tokens = line.split(' ');
            for (const QString &token : tokens) {
                if (token.startsWith('X')) {
                    x = token.mid(1).toFloat();
                } else if (token.startsWith('Y')) {
                    y = token.mid(1).toFloat();
                }
            }
            hasPosition = true; // Indicar que se tiene una posición válida
        } else if (line.startsWith("T")) {
            // Cambio de material
            QString materialCode = line.section(' ', 0, 0);
            if (!materialColors.contains(materialCode)) {
                QColor newColor = QColor(rand() % 256, rand() % 256, rand() % 256); // Generar un nuevo color aleatorio
                materialColors[materialCode] = newColor;
            }
            currentMaterialCode = materialCode; // Actualizar el código del material actual
        }
    }
    file.close(); // Cerrar el archivo
}

/**
     * @brief Verifica si el archivo GCODE ha sido modificado desde la última verificación.
     *
     * Comprueba la hora de la última modificación del archivo y la compara con la última hora registrada.
     * Si el archivo ha sido modificado, actualiza el registro de la última modificación y retorna `true`.
     * Si no, retorna `false`.
     *
     * @return `true` si el archivo ha sido modificado, `false` en caso contrario.
     */
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

/**
     * @brief Comprueba si el archivo GCODE ha sido modificado y actualiza el contenido si es necesario.
     *
     * Llama a `fileModified` para verificar si el archivo ha cambiado. Si ha cambiado, reanaliza el archivo
     * GCODE y actualiza la vista llamando a `parseGCode` y `update()`.
     */
void OpenGLWidget::checkFile() {

    if (fileModified()) {
        parseGCode(filePath);
        update(); // Redibuja el widget para reflejar los cambios
    }
}

/**
     * @brief Maneja el evento de la rueda del ratón para el zoom de la vista.
     *
     * Ajusta el factor de zoom en función del desplazamiento de la rueda del ratón.
     * Calcula el nuevo desplazamiento para que el punto en el que se encuentra el ratón se mantenga constante.
     * Restringe el factor de zoom a los límites definidos y actualiza los límites de movimiento.
     *
     * @param event El evento de rueda del ratón que contiene la información del desplazamiento.
     */
void OpenGLWidget::wheelEvent(QWheelEvent *event) {
    // Calcular la posición del ratón en el sistema de coordenadas del contenido
    float mouseX = (event->position().x() - width() / 2) / zoomFactor + offsetX;
    float mouseY = (height() / 2 - event->position().y()) / zoomFactor + offsetY;

    // Ajustar el factor de zoom basado en la dirección de desplazamiento de la rueda
    if (event->angleDelta().y() > 0) {
        zoomFactor *= 1.1f; // Acercar
    } else {
        zoomFactor /= 1.1f; // Alejar
    }

    // Limitar el factor de zoom dentro de los valores mínimo y máximo
    if (zoomFactor < minZoomFactor) {
        zoomFactor = minZoomFactor;
    } else if (zoomFactor > maxZoomFactor) {
        zoomFactor = maxZoomFactor;
    }

    // Actualizar los límites de movimiento según el nuevo factor de zoom
    updateMovementLimits();

    // Calcular el nuevo desplazamiento para que el punto del ratón se mantenga en la misma posición
    offsetX = mouseX - (event->position().x() - width() / 2) / zoomFactor;
    offsetY = mouseY - (height() / 2 - event->position().y()) / zoomFactor;

    // Restringir el desplazamiento a los límites de movimiento
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

    // Solicitar una actualización para reflejar los cambios en la vista
    update();
}
/**
     * @brief Maneja el evento de presión del ratón para iniciar el arrastre de la vista.
     *
     * Registra la posición actual del ratón y cambia el cursor a una mano cerrada cuando se presiona el botón izquierdo del ratón.
     *
     * @param event El evento de presión del ratón que contiene información sobre el botón presionado.
     */
void OpenGLWidget::mousePressEvent(QMouseEvent *event) {

    if (event->button() == Qt::LeftButton) {
        lastMousePosition = event->pos();
        setCursor(Qt::ClosedHandCursor); // Cambia el cursor para indicar arrastre
    }
}

/**
     * @brief Maneja el evento de movimiento del ratón para desplazar la vista.
     *
     * Ajusta el desplazamiento de la vista en función del movimiento del ratón cuando se mantiene presionado el botón izquierdo.
     * Restringe el desplazamiento dentro de los límites definidos y actualiza la posición del ratón.
     *
     * @param event El evento de movimiento del ratón que contiene la nueva posición del ratón.
     */
void OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
    // Verificar si el botón izquierdo del ratón está presionado
    if (event->buttons() & Qt::LeftButton) {
        // Calcular el cambio en la posición del ratón desde la última posición registrada
        QPoint delta = event->pos() - lastMousePosition;
        // Ajustar el desplazamiento en función del movimiento del ratón
        offsetX += delta.x() / 5.0f;
        offsetY -= delta.y() / 5.0f;

        // Limitar el desplazamiento a los bordes definidos
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

        // Actualizar la posición del ratón para el próximo evento de movimiento
        lastMousePosition = event->pos();
        // Solicitar una actualización para redibujar el widget con el nuevo desplazamiento
        update();
    }
}
/**
     * @brief Maneja el evento de liberación del ratón para finalizar el arrastre de la vista.
     *
     * Cambia el cursor de vuelta a la flecha cuando se libera el botón izquierdo del ratón.
     *
     * @param event El evento de liberación del ratón que contiene información sobre el botón liberado.
     */
void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event) {

    if (event->button() == Qt::LeftButton) {
        setCursor(Qt::ArrowCursor); // Cambia el cursor a la flecha normal
    }
}


// OPCIONAL
 // Actualiza la interfaz de usuario aquí
void OpenGLWidget::addMaterial(const QString &code, const QColor &color) {
    materialColors[code] = color;
}

void OpenGLWidget::removeMaterial(const QString &code) {
    materialColors.remove(code);

}

void OpenGLWidget::setMaterialColor(const QString &code, const QColor &color) {
    if (materialColors.contains(code)) {
        materialColors[code] = color;
    }
}

void OpenGLWidget::setCurrentMaterial(const QString &code) {
    if (materialColors.contains(code)) {
        currentMaterialCode = code;
    }
}
