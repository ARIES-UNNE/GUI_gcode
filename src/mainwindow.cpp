#include "mainwindow.h"
#include "section1.h"
#include "section2.h"
#include "section3.h"
#include "section4.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Crear un widget central para la ventana principal
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Configurar la ventana principal como no redimensionable
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedSize(300, 300);  // Establecer el tamaño fijo

    // Ocultar la esquina de redimensionamiento
    QSizeGrip *sizeGrip = findChild<QSizeGrip*>();
    if (sizeGrip)
        sizeGrip->setVisible(false);

    // Crear un diseño de formulario para organizar los widgets
    QFormLayout *formLayout = new QFormLayout(centralWidget);

    // Sección 1: Tamaño de la placa
    Section1 *section1Widget= new Section1(this);

    // Sección 2: Tamaño de la placa
    Section2 *section2Widget= new Section2(this);

    // Sección 3: Tamaño de la placa
    Section3 *section3Widget= new Section3(this);

    // Sección 4: Tamaño de la placa
    Section4 *section4Widget= new Section4(this);

    // Crear QStackedWidget para manejar las secciones
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(section1Widget);
    stackedWidget->addWidget(section2Widget);
    stackedWidget->addWidget(section3Widget);
    stackedWidget->addWidget(section4Widget);

    // Añadir QStackedWidget al formulario
    formLayout->addWidget(stackedWidget);

    // Crear y configurar el layout horizontal para los botones
    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    // Crear y configurar el botón para pasar a la siguiente sección
    QPushButton *nextButton = new QPushButton("Next", this);
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::nextSection);
    buttonsLayout->addWidget(nextButton);

    QPushButton *prevButton = new QPushButton("Previous", this);
    connect(prevButton, &QPushButton::clicked, this, &MainWindow::previousSection);
    buttonsLayout->addWidget(prevButton);

    // Crear y configurar el botón para cancelar
    QPushButton *cancelButton = new QPushButton("Cancel", this);
    connect(cancelButton, &QPushButton::clicked, this, &MainWindow::cancelConfirmation);
    buttonsLayout->addWidget(cancelButton);

    // Establecer el tamaño fijo de los botones
    prevButton->setFixedSize(80, 30);
    nextButton->setFixedSize(80, 30);
    cancelButton->setFixedSize(80, 30);
    buttonsLayout->setAlignment(cancelButton, Qt::AlignRight);


    // Añadir el layout de botones al formulario
    formLayout->addRow(buttonsLayout);
}


void MainWindow::adjustSectionSize(int sectionIndex) {
    // Ajustar el tamaño de la ventana según la sección actual
    switch (sectionIndex) {
    case 0:
        setFixedSize(300, 300);
        break;
    case 1:
        setFixedSize(300, 300);
        break;
    case 2:
        setFixedSize(300, 300);
        break;
    case 3:
        setFixedSize(430, 500);
        break;
    default:
        break;
    }
}

void MainWindow::cancelConfirmation() {
    // Mostrar una ventana de confirmación
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Cancel Confirmation", "Are you sure you want to cancel?",
                                  QMessageBox::Yes|QMessageBox::No);

    // Si el usuario elige "Yes", cerrar la aplicación
    if (reply == QMessageBox::Yes) {
        close();
    }
}

void MainWindow::previousSection() {
    // Mostrar la sección anterior
    int prevIndex = stackedWidget->currentIndex() - 1;
    if (prevIndex >= 0) {
        adjustSectionSize(prevIndex);
        stackedWidget->setCurrentIndex(prevIndex);
    }
}

void MainWindow::nextSection() {
    // Mostrar la siguiente sección
    int nextIndex = stackedWidget->currentIndex() + 1;
    if (nextIndex < stackedWidget->count()) {
        adjustSectionSize(nextIndex);
        stackedWidget->setCurrentIndex(nextIndex);
    }
}


void MainWindow::generateGCode() {
    // Implementar la lógica para generar el GCode basado en la entrada del usuario
    // En este caso, simplemente mostrar un mensaje informativo

}
