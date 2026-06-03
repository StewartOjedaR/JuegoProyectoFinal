#ifndef GESTIONJUEGO_H
#define GESTIONJUEGO_H

#include <QGraphicsView> // Cambiamos QObject por QGraphicsView para que esta clase sea la ventana misma
#include <QGraphicsScene> // Contenedor lógico donde se posicionarán todos los elementos gráficos del juego
#include <QTimer> // Temporizador de Qt que se utilizará como el reloj principal del juego
#include <QKeyEvent> // Incluye la clase encargada de empaquetar los eventos de pulsación del teclado
#include <QResizeEvent> // Incluye la clase encargada de empaquetar los eventos de redimensionamiento de la ventana

// Declaraciones adelantadas (Forward Declarations) para evitar la inclusión cíclica de archivos
class Fondo;
class Personaje;

class GestionJuego : public QGraphicsView { // Modificado: Ahora hereda directamente de QGraphicsView
    Q_OBJECT // Macro obligatoria de Qt para permitir el uso de eventos, señales y ranuras

public:
    // Constructor de la clase encargado de preparar la ventana y el estado inicial del juego
    explicit GestionJuego(QWidget *parent = nullptr);

    // Destructor de la clase encargado de liberar de forma segura toda la memoria dinámica utilizada
    ~GestionJuego();

    // Método solicitado: El Game Loop que se ejecuta constantemente para actualizar y redibujar items
    void loopJuego();

protected:
    // MÉTODOS NATIVOS DE QT SOBREESCRITOS PARA HACER TODO DESDE AQUÍ:

    // Sobreescribe los eventos del teclado para capturar las pulsaciones directamente en esta clase
    void keyPressEvent(QKeyEvent* evento) override;
    void keyReleaseEvent(QKeyEvent* evento) override;
    // Sobreescribe el evento de redimensionamiento para coordinar los tamaños automáticamente
    void resizeEvent(QResizeEvent* evento) override;

private:
    // --- COMPONENTES DEL ENTORNO DE QT ---
    QGraphicsScene* escena; // Puntero al lienzo o espacio bidimensional donde se colocan los objetos

    // --- ATRIBUTOS ---
    Fondo* fondo; // Puntero al objeto que manejará el fondo del juego
    Personaje* personaje; // Puntero al objeto que controlará al jugador
    QTimer* bucleJuego; // Temporizador único que funcionará como el motor de tiempo

    // --- CONFIGURACIÓN DE RENDIMIENTO ---
    const int FOTOGRAMAS_POR_SEGUNDO = 60; // Define el objetivo de rendimiento de 60 cuadros por segundo
    const int MILISEGUNDOS_POR_FRAME = 16; // Tiempo de espera aproximado para simular los 60 FPS (1000ms / 60)
    // DICCIONARIO DE TECLAS: Guarda la tecla (ej: Qt::Key_A) y si está presionada o no (true/false)
    QMap<int, bool> teclasPresionadas;
}; // Cierre de la definición de la clase GestionJuego

#endif // GESTIONJUEGO_H
