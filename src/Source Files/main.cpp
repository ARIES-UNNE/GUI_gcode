#include <QApplication>
#include "mainwindow.h"

/**
 * @brief Punto de entrada principal para la aplicación.
 *
 * Este es el punto de entrada principal para la aplicación Qt. Inicializa la aplicación, crea la ventana principal
 * y entra en el bucle de eventos. Cuando el bucle de eventos termina, el programa finaliza.
 *
 * @param argc Número de argumentos de la línea de comandos.
 * @param argv Array de argumentos de la línea de comandos.
 *
 * @return Código de salida de la aplicación. Generalmente, 0 indica que la aplicación terminó correctamente.
 */
int main(int argc, char *argv[]) {
    QApplication a(argc, argv); // Inicializa la aplicación Qt
    MainWindow w; // Crea la ventana principal
    w.show(); // Muestra la ventana principal
    return a.exec(); // Entra en el bucle de eventos de la aplicación
}
