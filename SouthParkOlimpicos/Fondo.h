#ifndef FONDO_H
#define FONDO_H

#include <QObject> // Incluye la clase base de Qt para dar soporte a señales y ranuras en el futuro
#include <QGraphicsPixmapItem> // Incluye el contenedor gráfico de Qt especializado en renderizar imágenes
#include <QPixmap> // Incluye la clase utilizada para cargar y almacenar mapas de píxeles en memoria

class Fondo : public QObject { // Define la clase Fondo heredando de las propiedades de QObject
    Q_OBJECT // Macro obligatoria de Qt para habilitar características avanzadas del framework

public:
    // Constructor que recibe la ruta de la imagen y establece el objeto padre para la gestión de memoria
    explicit Fondo(const QString& rutaImagen, QObject *parent = nullptr);

    // Destructor de la clase para asegurar una salida limpia de la memoria
    ~Fondo();

    // Método para obtener el puntero del item gráfico y así poder añadirlo a la escena desde GestionJuego
    QGraphicsPixmapItem* obtenerItem() const;

    // Método que recibe el ancho y alto del visor actual para estirar o ajustar la imagen de fondo
    void redimensionar(int anchoVentana, int altoVentana);

private:
    // Atributos privados de la clase
    QGraphicsPixmapItem* fondoItem; // Atributo solicitado: Puntero al elemento gráfico que se dibujará en el juego
    QPixmap pixmapOriginal; // Almacena la imagen original cargada para evitar que pierda calidad al reescalarse repetidamente
}; // Cierre de la definición de la clase Fondo

#endif // FONDO_H
