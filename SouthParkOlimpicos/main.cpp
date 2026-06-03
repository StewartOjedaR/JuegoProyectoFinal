#include <QApplication>
#include "GestionJuego.h"

// Nota: Es obligatorio que tenga (int argc, char *argv[])
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    GestionJuego juego;
    juego.show();
    return a.exec();
}
