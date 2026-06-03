#include "Fondo.h" // Incluye la cabecera con la declaración de la clase Fondo

// Constructor: Carga la imagen en memoria y prepara el contenedor gráfico de Qt
Fondo::Fondo(const QString& rutaImagen, QObject *parent) : QObject(parent) {
    pixmapOriginal.load(rutaImagen); // Carga el archivo de imagen desde la ruta especificada en los recursos o disco
    fondoItem = new QGraphicsPixmapItem(pixmapOriginal); // Instancia el item gráfico asignándole la imagen cargada

}

// Destructor: Se ejecuta al destruir el objeto Fondo
Fondo::~Fondo() {
    // Nota: Si el 'fondoItem' se añade a una QGraphicsScene, la escena se adueña de él
    // y lo borrará automáticamente de la memoria al cerrarse el juego.
}

// Método de acceso para exponer de forma segura el puntero del item gráfico
QGraphicsPixmapItem* Fondo::obtenerItem() const {
    return fondoItem; // Retorna la dirección de memoria del contenedor de la imagen
}

// Método para redimensionar el fondo adaptándose al tamaño que le mande el visor
void Fondo::redimensionar(int anchoVentana, int altoVentana) {
    // Validacion de seguridad para asegurar que la imagen es valida y las dimensiones son mayores a cero
    if (!pixmapOriginal.isNull() && anchoVentana > 0 && altoVentana > 0) {

        QSize nuevoTamano(anchoVentana, altoVentana); // Crea un objeto de tamaño de Qt con las medidas recibidas

        // Escala la imagen original al nuevo tamaño segun la ventana
        QPixmap pixmapEscalado = pixmapOriginal.scaled(nuevoTamano,Qt::KeepAspectRatioByExpanding );

        fondoItem->setPixmap(pixmapEscalado); // Reemplaza la imagen del item grafico por la nueva imagen escalada
    }
} // Cierre del método redimensionar
