#include "Fondo.h" // Incluye la cabecera de la clase Fondo para usar sus metodos
#include "GestionJuego.h" // Incluye la cabecera con la estructura de GestionJuego
#include "Personaje.h"
// Constructor: Configura la ventana base, la escena, el fondo y el temporizador unificado
GestionJuego::GestionJuego(QWidget *parent)
    : QGraphicsView(parent), // Inicializa la clase base de la ventana grafica de Qt
    escena(nullptr), // Inicializa el puntero de la escena en nulo por seguridad
    fondo(nullptr), // Inicializa el puntero del fondo en nulo por seguridad
    personaje(nullptr), // Inicializa el puntero del personaje en nulo por seguridad
    bucleJuego(nullptr) { // Inicializa el puntero del temporizador en nulo por seguridad

    // 5. Instancia nuestra clase Fondo pasándole la ruta de la imagen
    fondo = new Fondo(":/imagenes/fondo.jpg", this);
    int anchoVentana =    800; // Define la variable local para el ancho base de la resolucion
    int altoVentana =     fondo->obtenerItem()->pixmap().height(); // Define la variable local para el alto base de la resolución, en este caso definida por la altura de la imagen

    // 1. Instancia la escena logica con las dimensiones base del juego
    escena = new QGraphicsScene(0, 0, anchoVentana, altoVentana, this);

    // 2. Asocia formalmente esta ventana (QGraphicsView) a la escena que acabamos de crear
    this->setScene(escena);

    // 3. Desactiva las barras de desplazamiento para que el juego se vea limpio al redimensionar
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 4. Define el tamaño fisico inicial de esta ventana de juego
  //  this->setFixedSize(anchoVentana, altoVentana);


    // 6. Obtiene el item gráfico del fondo y lo añade formalmente al lienzo de la escena
    escena->addItem(fondo->obtenerItem());

    // 7. Ajusta el tamaño de la imagen del fondo al tamaño actual de la ventana
   // fondo->redimensionar(anchoVentana, altoVentana);

    // 8. Instancia el temporizador principal del juego
    bucleJuego = new QTimer(this);

    // 9. Conecta el "tic" del temporizador directamente con nuestro método loopJuego
    connect(bucleJuego, &QTimer::timeout, this, &GestionJuego::loopJuego);

    // 10. Enciende el bucle a 60 FPS (Dispara el evento usando la constante de milisegundos)
    bucleJuego->start(MILISEGUNDOS_POR_FRAME);
    // Instanciamos el personaje pasandole: Ruta, AnchoFrame, AltoFrame, Padre
    personaje = new Personaje(":/imagenes/fondo.png", 140*4, 180,140,180,bucleJuego,this);
 //   personaje->reescalar(50);
    personaje->obtenerItem()->setScale(0.50);
    // Agregamos el item grafico del personaje a nuestra escena (quedara encima del fondo)
    escena->addItem(personaje->obtenerItem());

    // Lo ubicamos en una coordenada inicial comoda en la pantalla de 800x600
    personaje->obtenerItem()->setPos(100, 100);
}

// Destructor: Garantiza la liberacion estricta de la memoria al cerrar la ventana
GestionJuego::~GestionJuego() {
    delete fondo; // Libera la memoria reservada para el objeto fondo si fue creado
    delete bucleJuego; // Destruye el temporizador principal de forma segura
    delete escena; // Libera el lienzo logico de la pantalla
    // Nota: 'vista' ya no se borra aqui porque esta misma clase es la vista (se destruye sola al cerrar)
}

// Método protegido: Se dispara automaticamente cuando el usuario estira o cambia el tamaño de la ventana
// void GestionJuego::resizeEvent(QResizeEvent* evento) {
//     // 1. Llama al metodo original de Qt para que realice el procesamiento interno por defecto de la ventana
//     QGraphicsView::resizeEvent(evento);
//     // 2. Validacion de seguridad para asegurarse de que la escena y el objeto fondo existan
//     if (escena && fondo) {

//         // 3. Obtiene el ancho y alto del área visible de la ventana (viewport)
//         int nuevoAncho = this->viewport()->width();
//         int nuevoAlto = this->viewport()->height();

//         // 4. Reajusta los límites logicos de la escena al nuevo tamaño detectado
//         escena->setSceneRect(0, 0, nuevoAncho, nuevoAlto);

//         // 5. Llama al metodo de tu clase Fondo para reescalar la imagen de manera responsiva
//         fondo->redimensionar(nuevoAncho, nuevoAlto);
//         if (personaje) {
//     //    personaje->reescalar(25);
//             personaje->adaptarAVentana(nuevoAncho, nuevoAlto);
//         }
//     } // Cierre de la validación de seguridad
// }
void GestionJuego::resizeEvent(QResizeEvent* evento) {
    // 1. Ejecuta el comportamiento interno por defecto de Qt
    QGraphicsView::resizeEvent(evento);

    // 2. Hace que Qt estire y escale absolutamente TODO lo que esté dentro de la escena
    // (Fondo, Personajes, Enemigos) manteniendo la proporción perfecta (4:3).
    // Si el usuario estira de más la ventana, Qt añadirá barras negras automáticas a los lados.
    this->fitInView(0, 0,800,     fondo->obtenerItem()->pixmap().height());
}
// Captura cuando se presiona una tecla y la enciende en el mapa
void GestionJuego::keyPressEvent(QKeyEvent* evento) {
    if (evento && !evento->isAutoRepeat()) {
        teclasPresionadas[evento->key()] = true; // Marca la tecla como "presionada"
    }
}

// Captura cuando se suelta una tecla y la apaga en el mapa
void GestionJuego::keyReleaseEvent(QKeyEvent* evento) {
    if (evento && !evento->isAutoRepeat()) {
        teclasPresionadas[evento->key()] = false; // Marca la tecla como "liberada"

        // Si ya no queda ninguna tecla de movimiento presionada, le avisamos al personaje que se detenga
        if (!teclasPresionadas[Qt::Key_A] && !teclasPresionadas[Qt::Key_Left] &&
            !teclasPresionadas[Qt::Key_D] && !teclasPresionadas[Qt::Key_Right] &&
            !teclasPresionadas[Qt::Key_W] && !teclasPresionadas[Qt::Key_Up] &&
            !teclasPresionadas[Qt::Key_S] && !teclasPresionadas[Qt::Key_Down]) {

            if (personaje) {
                personaje->detener(); // Frena la animación y lo pone en reposo
            }
        }
    }
}

// EL GAME LOOP (60 FPS): Aqui es donde se procesa el movimiento real de forma instantanea
void GestionJuego::loopJuego() {
    if (!personaje) return;

    int anchoV = this->viewport()->width();
    int altoV = this->viewport()->height();
    bool huboMovimiento = false;

    // EVALUACIÓN EN TIEMPO REAL (Sin retrasos del sistema operativo)
    if (teclasPresionadas[Qt::Key_A] || teclasPresionadas[Qt::Key_Left]) {

        // 1. Activamos el modo espejo para que el sprite mire instantáneamente en la otra dirección
        personaje->imagenEspejo(true);

        // 2. Lo movemos de forma normal hacia la izquierda (X negativo) manteniendo su animación habitual
        personaje->caminar(0, -1, 0, anchoV, altoV);
        // --- REFRESCO GRÁFICO ---
        if (escena) {
            escena->update(); // Redibuja la escena a 60 FPS
        }
    }
    // --- Al presionar la tecla 'D' para ir a la derecha ---
    if (teclasPresionadas[Qt::Key_D] || teclasPresionadas[Qt::Key_Right]) {

        // Apagamos el modo espejo para que regrese a su forma y renderizado natural
        personaje->imagenEspejo(false);

        // Se mueve de forma normal hacia la derecha
        personaje->caminar(0, 1, 0, anchoV, altoV);
        // --- REFRESCO GRÁFICO ---
        if (escena) {
            escena->update(); // Redibuja la escena a 60 FPS
        }
    }
    if (teclasPresionadas[Qt::Key_W] || teclasPresionadas[Qt::Key_Up]) {
        personaje->caminar(0, 0, -1, anchoV, altoV); // Fila 3: Arriba
        huboMovimiento = true;
        // --- REFRESCO GRÁFICO ---
        if (escena) {
            escena->update(); // Redibuja la escena a 60 FPS
        }
    }
    if (teclasPresionadas[Qt::Key_S] || teclasPresionadas[Qt::Key_Down]) {
        personaje->caminar(0, 0, 1, anchoV, altoV);  // Fila 0: Abajo
        huboMovimiento = true;
        // --- REFRESCO GRÁFICO ---
        if (escena) {
            escena->update(); // Redibuja la escena a 60 FPS
        }
    }

    // // --- REFRESCO GRÁFICO ---
    if (escena) {
        escena->update(); // Redibuja la escena a 60 FPS
    }
}
