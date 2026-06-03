#include "Personaje.h"
#include <QTransform>
Personaje::Personaje(const QString& rutaSpriteSheet, int anchoHoja, int altoHoja,
                     int anchoFrame, int altoFrame, QTimer* timerGlobal, QObject *parent)
    : QObject(parent), personajeItem(nullptr), relojJuego(timerGlobal),
    anchoCuadro(anchoFrame), altoCuadro(altoFrame), spritesPorFila(0), totalFilas(0),
    filaActual(0), columnaActual(0), estaMoviendose(false), velocidadMovimiento(5),
    contadorVelocidad(0), umbralVelocidadDinamico(6), factorEscala(1.0),
    porcentajeX(0.125), // 100 píxeles iniciales / 800 de ancho base = 0.125 (12.5% de la pantalla)
    porcentajeY(0.166){  // 100 píxeles iniciales / 600 de alto base = 0.166 (16.6% de la pantalla)
    hojaSprites.load(rutaSpriteSheet);
    personajeItem = new QGraphicsPixmapItem();

    if (anchoFrame > 0 && altoFrame > 0) {
        spritesPorFila = anchoHoja / anchoFrame;
        totalFilas = altoHoja / altoFrame;

        // VELOCIDAD DE ANIMACIÓN PROPORCIONAL:
        // A mayor cantidad de sprites por fila, el umbral se reduce para que cambie más rápido y se vea fluido.
        umbralVelocidadDinamico = 60 / spritesPorFila-5;
        if (umbralVelocidadDinamico < 2) umbralVelocidadDinamico = 2;
    }

    aplicarRecorteSprite(0, 0); // Posición inicial de reposo

    if (relojJuego) {
        connect(relojJuego, &QTimer::timeout, this, &Personaje::actualizarBucleAnimacion);
    }
}

Personaje::~Personaje() {}

QGraphicsPixmapItem* Personaje::obtenerItem() const {
    return personajeItem;
}

// El director (GestionJuego) le dice hacia dónde ir (-1, 0, 1) y en qué fila de animación está esa dirección
void Personaje::caminar(int direccionFila, int deltaX, int deltaY, int limiteAncho, int limiteAlto) {
    if (!personajeItem || limiteAncho <= 0 || limiteAlto <= 0) return;

    estaMoviendose = true;

    if (filaActual != direccionFila && direccionFila < totalFilas) {
        filaActual = direccionFila;
        columnaActual = 0;
    }

    int nuevoX = personajeItem->x() + (deltaX * velocidadMovimiento);
    int nuevoY = personajeItem->y() + (deltaY * velocidadMovimiento);

    int anchoRealActual = static_cast<int>(anchoCuadro * factorEscala);
    int altoRealActual = static_cast<int>(altoCuadro * factorEscala);

    if (nuevoX >= 0 && (nuevoX + anchoRealActual) <= limiteAncho) {
        personajeItem->setX(nuevoX);
    }
    if (nuevoY >= 0 && (nuevoY + altoRealActual) <= limiteAlto) {
        personajeItem->setY(nuevoY);
    }

    // --- ENFOQUE DE SEGURIDAD CORREGIDO ---
    // Guarda de forma abstracta la posición exacta dividiendo la coordenada actual entre el tamaño del visor
    porcentajeX = personajeItem->x() / static_cast<double>(limiteAncho);
    porcentajeY = personajeItem->y() / static_cast<double>(limiteAlto);
}

void Personaje::detener() {
    estaMoviendose = false;
    columnaActual = 0; // Regresa al frame de descanso (columna 0) de la última dirección
    contadorVelocidad = 0;
    aplicarRecorteSprite(columnaActual, filaActual);
}

// Bucle de animación a 60 FPS (Maneja el timing visual)
void Personaje::actualizarBucleAnimacion() {
    if (!estaMoviendose) return;

    contadorVelocidad++;
    if (contadorVelocidad < umbralVelocidadDinamico) return;
    contadorVelocidad = 0;

    // Avanza horizontalmente en la fila actual
    columnaActual++;
    if (columnaActual >= spritesPorFila) {
        columnaActual = 0; // Hace bucle en la misma fila de movimiento
    }

    aplicarRecorteSprite(columnaActual, filaActual);
}

// // MÉTODO CORREGIDO: Extrae los píxeles y aplica el espejo directamente en la imagen para evitar saltos de posición
// void Personaje::aplicarRecorteSprite(int columna, int fila) {
//     if (!hojaSprites.isNull()) {
//         // 1. Calcula las coordenadas exactas de recorte en la hoja de sprites
//         int posicionX = columna * anchoCuadro;
//         int posicionY = fila * altoCuadro;
//         // 2. Extrae el fragmento cuadrado del sprite de la hoja original
//         QRect areaRecorte(posicionX, posicionY, anchoCuadro, altoCuadro);
//         QPixmap frameRecortado = hojaSprites.copy(areaRecorte);

//         // 3. CONTROL DEL ESPEJO DIRECTO EN EL PIXMAP:
//         // Si la bandera 'esEspejo' está activa, invertimos la imagen usando una transformación nativa de la imagen
//         if (esEspejo) {
//             QTransform matrizEspejo;
//             matrizEspejo.scale(-1, 1); // Invierte el eje X de los píxeles

//             // Reemplaza el frame por su versión espejada (TrueRef se asegura de transformar los píxeles reales)
//             frameRecortado = frameRecortado.transformed(matrizEspejo);
//         }

//         // 4. Asigna la imagen final al ítem gráfico (sus coordenadas X e Y en la escena no variarán ni un solo píxel)
//         personajeItem->setPixmap(frameRecortado);
//     }
// }

// NUEVO MÉTODO: Invierte horizontalmente el elemento gráfico usando matrices de transformación de Qt
// MÉTODO SIMPLIFICADO: Activa el estado de espejo y fuerza el redibujado inmediato del cuadro
void Personaje::imagenEspejo(bool activarEspejo) {
    // Si el estado solicitado es diferente al actual, hacemos el cambio
    if (esEspejo != activarEspejo) {
        esEspejo = activarEspejo; // Cambia la bandera (true/false)

        // Vuelve a aplicar el recorte con los mismos índices actuales para actualizar la imagen con/sin espejo
        aplicarRecorteSprite(columnaActual, filaActual);
    }
}
// NUEVO MÉTODO: Convierte el porcentaje entero/flotante en un factor decimal y refresca el sprite
void Personaje::reescalar(double porcentaje) {
    // 1. Convertimos el porcentaje en un factor multiplicador (ej: 50% -> 0.5 | 100% -> 1.0)
    if (porcentaje > 0) {
        factorEscala = porcentaje / 100.0;

        // 2. Forzamos un re-recorte instantáneo del frame actual para que el cambio de tamaño se vea de inmediato
        aplicarRecorteSprite(columnaActual, filaActual);
    }
}

// MÉTODO ACTUALIZADO: Modificamos el método de pintado para aplicar la escala por software
void Personaje::aplicarRecorteSprite(int columna, int fila) {
    if (!hojaSprites.isNull()) {
        // 1. Calcula las coordenadas exactas de recorte en la hoja original
        int posicionX = columna * anchoCuadro;
        int posicionY = fila * altoCuadro;

        // 2. Extrae el fragmento del sprite con sus medidas originales e intactas
        QRect areaRecorte(posicionX, posicionY, anchoCuadro, altoCuadro);
        QPixmap frameFinal = hojaSprites.copy(areaRecorte);

        // 3. CONTROL DEL ESPEJO HORIZONTAL (Se mantiene como antes)
        if (esEspejo) {
            QTransform matrizEspejo;
            matrizEspejo.scale(-1, 1);
            frameFinal = frameFinal.transformed(matrizEspejo, Qt::SmoothTransformation);
        }

        // 4. APLICACIÓN DE LA ESCALA POR PORCENTAJE (Nuevo bloque matemático):
        // Si el factor es diferente de 1.0, calculamos el nuevo ancho y alto físico del sprite
        if (factorEscala != 1.0) {
            int nuevoAncho = static_cast<int>(anchoCuadro * factorEscala);
            int nuevoAlto = static_cast<int>(altoCuadro * factorEscala);

            // Redimensionamos la imagen final. Usamos Qt::SmoothTransformation para que no se vea pixelado
            frameFinal = frameFinal.scaled(nuevoAncho, nuevoAlto);
        }

        // 5. Asigna el mapa de píxeles final ya escalado y/o espejado al ítem de la escena
        personajeItem->setPixmap(frameFinal);
    }
}
// NUEVO MÉTODO: Calcula la proporción de la ventana y reescala el sprite en consecuencia
// MÉTODO ACTUALIZADO: Adapta el tamaño y la posición del personaje de forma responsiva a la ventana
void Personaje::adaptarAVentana(int anchoVentana, int altoVentana) {
    // 1. Validación de seguridad para evitar divisiones fatales entre cero
    if (anchoVentana <= 0 || altoVentana <= 0 || !personajeItem) return;

    // 2. Calculamos cuánto ha cambiado la ventana respecto al tamaño base
    double factorAncho = anchoVentana / 800;
    double factorAlto = altoVentana / 600;

    // 3. REESCALADO DE SPRITE: Usamos la escala uniforme más pequeña para no deformar la imagen
    double factorVentana = (factorAncho < factorAlto) ? factorAncho : factorAlto;
    reescalar(factorVentana * 100.0); // Esto altera los píxeles de forma limpia

    // 4. REPOSICIONAMIENTO MATEMÁTICO DIRECTO (Sin llamadas a scene()):
    // Multiplicamos el porcentaje guardado de ubicación por las nuevas dimensiones de la ventana
    int nuevoX = static_cast<int>(porcentajeX * anchoVentana);
    int nuevoY = static_cast<int>(porcentajeY * altoVentana);

    // 5. Aplicamos la posición de forma absoluta en el plano
    personajeItem->setPos(nuevoX, nuevoY);
}
