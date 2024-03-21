#include "mainwindow.h"
#include "section1.h"
#include "section2.h"
#include "section3.h"
#include "section4.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Crear un widget central para la ventana principal
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setFixedSize(300,300);
    setCentralWidget(centralWidget);

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

    //generateButton = new QPushButton("Generate GCode", this);
    //connect(generateButton, &QPushButton::clicked, this, &MainWindow::generateGCode);

    // Crear y configurar el botón para pasar a la siguiente sección
    QPushButton *nextButton = new QPushButton("Next Section", this);
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::nextSection);

    // Crear y configurar el botón para retroceder a la sección anterior
    QPushButton *prevButton = new QPushButton("Previous Section", this);
    connect(prevButton, &QPushButton::clicked, this, &MainWindow::previousSection);

   // Crear QStackedWidget para manejar las secciones
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setFixedSize(300,300);
    stackedWidget->addWidget(section1Widget);
    stackedWidget->addWidget(section2Widget);
    stackedWidget->addWidget(section3Widget);
    stackedWidget->addWidget(section4Widget);



    // Añadir QStackedWidget y los botones al formulario
    formLayout->addWidget(stackedWidget);
    formLayout->addRow(nextButton);
    formLayout->addRow(prevButton);




}



void MainWindow::previousSection() {
    // Mostrar la sección anterior
    int prevIndex = stackedWidget->currentIndex() - 1;
    if (prevIndex >= 0) {
        stackedWidget->setCurrentIndex(prevIndex);
    }
}


void MainWindow::nextSection() {
    // Mostrar la siguiente sección
    int nextIndex = stackedWidget->currentIndex() + 1;
    if (nextIndex < stackedWidget->count()) {
        stackedWidget->setCurrentIndex(nextIndex);
    }
}

void MainWindow::generateGCode() {
    // Implementar la lógica para generar el GCode basado en la entrada del usuario
    // En este caso, simplemente mostrar un mensaje informativo

}
