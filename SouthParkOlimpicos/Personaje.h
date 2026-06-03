#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QObject> // Clase base para señales y ranuras
#include <QGraphicsPixmapItem> // Elemento gráfico para la escena
#include <QPixmap> // Para cargar la hoja de sprites
#include <QTimer> // Para conectarse al reloj global de 60 FPS

class Personaje : public QObject {
    Q_OBJECT

public:
    // El constructor recibe dimensiones de la hoja, del frame y el temporizador global
    explicit Personaje(const QString& rutaSpriteSheet, int anchoHoja, int altoHoja,
                       int anchoFrame, int altoFrame, QTimer* timerGlobal, QObject *parent = nullptr);
    ~Personaje();

    QGraphicsPixmapItem* obtenerItem() const;

    // MÉTODOS DE CONTROL: Reciben la dirección y los límites de la ventana para moverse
    void caminar(int direccionFila, int deltaX, int deltaY, int limiteAncho, int limiteAlto);
    void detener();
    // NUEVO MÉTODO SOLICITADO: Aplica o quita el efecto espejo horizontal al personaje
    void imagenEspejo(bool activarEspejo);
    // NUEVO MÉTODO: Recibe el porcentaje (ej: 50 para cincuenta por ciento, 150 para ciento cincuenta)
    void reescalar(double porcentaje);
    // NUEVO MÉTODO: Adapta el tamaño del personaje según la escala de la ventana actual
    void adaptarAVentana(int anchoVentana, int altoVentana);

private:
    void actualizarBucleAnimacion(); // Conectado al Timer a 60 FPS
    void aplicarRecorteSprite(int columna, int fila);
    bool esEspejo=false; // Guarda el estado actual del reflejo del personaje (true = invertido, false = normal)
    QGraphicsPixmapItem* personajeItem;
    QPixmap hojaSprites;
    QTimer* relojJuego;
    double factorEscala; // Atributo para guardar el multiplicador de tamaño actual (ej: 1.0 = 100%)
    // Dimensiones y geometría
    int anchoCuadro;
    int altoCuadro;
    int spritesPorFila;
    int totalFilas;

    // Control de animación y posición
    int filaActual;
    int columnaActual;
    bool estaMoviendose;
    int velocidadMovimiento;

    // Control de velocidad proporcional de animación
    int contadorVelocidad;
    int umbralVelocidadDinamico;
    // --- NUEVOS ATRIBUTOS DE UBICACIÓN LOGICA ---
    double porcentajeX; // Guarda la posición horizontal en formato de porcentaje (0.0 a 1.0)
    double porcentajeY; // Guarda la posición vertical en formato de porcentaje (0.0 a 1.0)
};

#endif // PERSONAJE_H
