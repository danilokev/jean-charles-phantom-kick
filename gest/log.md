# Log del trabajo realizado por dia
**El log es obligatorio**

### 2025-02-18 Alfonso @(1h)
Llamada de **grupo** para revisar las guías de la plantilla y crear el shell script run.sh para agilizar el proceso de ejecución

### 2025-02-19 Marcos y Mario @(2.5h)
Una vez que Mario ha hecho el prototipo de entrada y colisión. Marcos ha podido hacer el prototipo de partículas, que contiene controles de entrada, añadiendo diagonales, el sprite correcto, limites en el "mapa" y la detección de colisión del jugador que se mueve por la pantalla con una esfera que hay en medio.

### 2025-02-19 Kevin @(1h)
Se ha creado el prototipo Audio con su estructura principal para manejar los archivos de sonido y asociarlo a un nombre. Además de añadir la función para reproducir un sonido a partir de su nombre.

## Semana 02-20 a 02-26

### 2025-02-24 Kevin @(3.0h)
Se ha completado la implementación base de la clase AudioManager para gestionar sonidos y música en el juego. Se ha comprobado AudioManager en el bucle principal del juego y se han realizado pruebas exitosas de compilación y funcionamiento utilizando recursos de prueba. Además, se ha configurado correctamente el archivo CMakeLists.txt para incluir los archivos necesarios.

### 2025-02-24 Alfonso @(>1.0h)
Github se ha caido. He estado mirando cual es la mejor forma de implementar las entidades. Al final llegué a la conclusión de desechar completamente lo que hice en la Ten Sheet v1.0 y apunté mis avances en `doc/arquitectura_entidades.md`

### 2025-02-25 Mario @(1.5h)
Se ha implementado y comprobado correctamente el prototipo de menú, el cual muestra las opciones "Menú 1","Menú 2" y "Salir", teniendo distintos resultados según la opción que se seleccione. En adición a eso, la opción que está marcada se muestra en rojo. En resumidas cuentas, he tenido que duplicar la carpeta de "plantilla_fv", añadir el prototipo en el archivo .workspace, hacer el archivo .h en la carpeta "include", el .cpp en "ej_modulos", y se sustituye el main.cpp que ya estaba.

### 2025-02-26 Marcos @(2.5h)
Se ha corregido los sprites para evitar bugs visuales, añadido un icono al juego y implementado una demo del sistema de particulas incluyendo las explosiones y efecto "estela" basico.

### 2025-02-26 Alfonso @(2.0h)
Traté de probar una arquitectura basada en el patrón composite, pero no fue demasiado bien, hay bastantes cosas que aún no entiendo, por lo que voy a separar los prototipos que quiero que funcionen y los prototipos para probar distintos modelos y soluciones, centrandome más por ahora en los primeros.

### 2025-02-27 Mario @(2.0h)
He cambiado el "Menú 1" del Prototipo 5 para que la pantalla esté dividida de forma que en la parte derecha estén los textos, por ejemplo, la vida, los powerups, etc. y en la parte izquierda esté la "zona" del juego, donde según mis planes de esta semana todavía queda juntar el personaje del prototipo de partículas (Marcos), para que este pueda moverse por la zona delimitada ya mencionanda.

### 2025-02-28 Mario @(1.5h)
Se ha implementado lo que quedaba del "Menú 1" del prototipo 5, he conseguido juntar el sprite del personaje y su movimiento del prototipo de partículas a este. Había que ajustar los límites por los que podía moverse y su velocidad. Queda por pulir la velocidad de movimiento del sprite, que varía a veces, y el escalado de la pantalla, no sé dónde está exactamente la solución, pero se estiran mucho los elementos.

### 2025-03-01 Marcos @(3.0h)
Se ha implementado varios efectos nuevos de particulas en ptt_particles, se ha implementado un
sistema de puntos y un reloj que se actualizan en el menu. Ademas se han comenzado las pruebas de los fondos paralax

### 2025-03-01 Alfonso @(2.5h)
Intenté hacer balas y un spawner, me he dado cuenta de que hay varias cosas en común, voy a intentar modulizarlo y hacer un main funcional. Por ahora lo que hay tiene muy poca funcionalidad.

### 2025-03-01 Marcos @(1.5h)
Se ha terminado la implementacion de fondos parallax

### 2025-03-03 Kevin @(3.5h)
He implementado la clase `Combat`. Se ha añadido las variables necesarias para el estado del jugador y las bolas, así como métodos para asignar la textura de las balas, manejar la lógica de colisiones y la invulnerabilidad del jugador, gestión del disparo y su colisión con un objeto.

### 2025-03-03 Alfonso @(1h)
He declarado una estructura de ficheros definida para la implementación del patrón Component, empezaré a pasar lo viejo de las balas (trashbin/) a componentes, manejadores y sistemas

### 2025-03-06 Alfonso @(2h)
Después de intntar implementar el patrón composite, creo que va a ser mejor prescindir de el. Los objetos del juego pueden heredar de una clase Entity para sus métodos y atributos comunes y para hacer distintos tipos de balas y enemigos se puede emplear el patrón Strategy y Factory. Los prototipos de esta última semana intentarán empezar a emplear los patrones. 

### 2025-03-11 Kevin @(<1h)
He corregido errores menores en el AudioManager para finalizar la estructura base de esta clase y facilitar mejoras futuras e integraciones de patrones.

### 2025-03-01 Marcos @(1.5h)
Se ha mejorado el prototipo de fondos y se ha implementado el sistema de diálogos con clases separadas.

### 2025-03-13 Mario @(3h)
Completado 1 de las 2 tareas de esta semana. Se ha implementado el Prototipo 9: Estados, que es una mejora del Prototipo 4: Menú. Ahora permite dividir el juego en estados como el de gameplay o el de menú, con el objetivo de no saturar el main con todo el código, incorporando los correspondientes patrones y clases.

### 2025-03-13 Mario @(2h)
Completado 2 de las 2 tareas de esta semana. He implementado otra clase Singleton a Prototipo 9: Estados, que permite manejar mejor lo que es el tema del renderizado o dibujo. En pocas palabras, se sustituiría los window.draw() a renderManager->draw(), para que todo lo maneje una única instancia de la clase renderManager. Quizá haya que revisar el movimiento del personaje.

### 2025-03-17 Kevin @(2.5h)
He implementado la clase `SpriteFacade` que funciona como una abstracción sobre los sprites de SFML. Esta clase encapsula toda la funcionalidad de sf::Sprite. Incluye métodos para cargar texturas, modificar la posición, escala, rotación, color y origen del sprite, así como funcionalidades para dibujar el sprite en una ventana.

### 2025-03-27 Mario @(3h)
Se ha cambiado la rama del repositorio a 'developer'. Para empezar con esta rama, he partido del Prototipo 9: Estados (cambiando los detalles que hacían falta para adaptarlo a la estructura de la rama 'developer') para que funcionara ahora con el bucle tipo 4 dada en esta asignatura. Se basa en interpolaciones y una frecuencia de actualización fija. También he ajustado los valores de movimiento del jugador como: 
#define kVel 3.15f -> estaba en 0.15
puntos[i].sprite.move(direction * 0.5f) -> estando en 0.02

### 2025-03-28 Kevin @(1h)
He añadido un script de configuración (`setup.sh`) para descargar y extraer los recursos necesarios del proyecto desde Dropbox.

### 2025-03-29 Kevin @(2h)
He implementado la clase `MusicPlayer` para gestionar la reproducción de música en el proyecto. Esta clase permite cargar pistas de música desde archivos, asignarles un identificador único, y gestionar su reproducción, pausa, detención, volumen y bucle. Además, soporta múltiples pistas simultáneamente mediante un mapa de identificadores. También he añadido métodos para verificar el estado de reproducción y ajustar configuraciones globales como el volumen y el bucle.

### 2025-03-30 Kevin @(2h)
Implementé la clase SoundPlayer para gestionar efectos de sonido del juego. El sistema permite reproducir múltiples sonidos simultáneamente, gestiona volumen en tres niveles (individual, por categoría y maestro), y categoriza los efectos mediante SoundType.

### 2025-03-31 Mario @(3.5h)
He hecho algunos arreglos de las tareas anteriores que tuve, del archivo 4Mario.txt (he borrado las que he hecho), queda por limpiar la clase PlayState y pasar su funcionalidad a otra, para que PlayState solo tenga un array de niveles vacío.

### 2025-04-03 Alfonso@(2h)
Nuevas tareas, además he creado las clases vacías para empezar a implementarlas

### 2025-04-04 Alfonso@(2h)
Más tareas, más clases vacías, he cambiado el método principal de Sprite para incluirle una estructura escalable con switch case. Voy a seguir con las entidades montandolas con sus componentes.

### 2025-04-07 Kevin @(1h30)
Implementé la clase BoundingCircle para manejar colisiones circulares entre entidades. La clase contiene un centro (sf::Vector2f) y un radio (float), además de un método que detecta colisiones con otro BoundingCircle mediante distancia entre centros.

### 2025-04-07 Mario @(2h)
Implementé la clase DirectionalMovement, introduce un movimiento basado en la dirección, velocidad, y afectado por la aceleración, cambiando su posición y aumentando su velocidad hasta alcanzar una velocidad máxima determinada. Para probarlo, se ha implementado la clase temporal TestEntity que es un punto visual. Se ha modificado el main para que al iniciar el juego se lance una bola con ángulo 45º (noreste) y una aceleración, imprimiendo su posición, velocidad y ángulo en todo momento.

### 2025-04-07 Marcos @(3h)
Implementé la nueva clase sprite, para poder manejarlos

### 2025-04-07 Marcos @(1h)
Probe unos posibles cambios que acabe descartando y implemente los posibles casos.

### 2025-04-09 Alfonso @(1.5h)
Implementación de player incompleta, usando ya la fachada de sprite, el bounding circle que se utilizará para calcular la colisión. Implementado también el InputMovement que se encarga de recoger el input de las teclas y modificar la posición del jugador.

### 2025-04-10 Alfonso @(1.5h)
Implementación del enemigo sin bullet emitter hasta que tenga las balas. Más tareas para el equipo

### 2025-04-10 Mario @(2.5h)
Cambios en el menú principal, se ha cambiado la fuente de letra, y se han añadido dos pantallas adicionales, una al darle a jugar para elegir un disparo (que de momento no hace nada) y otra de puntuaciones que analiza un txt para puntuaciones que tendrán otros jugadores

### 2025-04-10 Mario @(1.5h)
He hecho progreso con la tarea de limpiar GamePlayState y perfeccionarlo, no se ha terminado todavía, pero he implementado la clase Bullet aunque todavía no se use.

### 2025-04-10 Kevin @(1h30)
Implementé un sistema de cooldown en la clase Player para solucionar el problema de disparo múltiple con una sola pulsación de tecla. Añadí la funcionalidad de rotar la textura de la bala 90 grados creando un método setRotation en la clase Sprite y utilizándolo en PlayerShoot. Ajusté la frecuencia y la velocidad de disparo modificando shootCooldown y bulletSpeed para un estilo más similar a Touhou.

### 2025-04-11 Kevin @(1h15)
Implementé "ChaosMovement" para jefes que se mueven suavemente entre posiciones aleatorias dentro de un área definida. Calcula vectores de dirección normalizados y añade pausas entre movimientos.

### 2025-04-11 Marcos @(8h)
Implementé "ChaosMovement" para jefes que se mueven suavemente entre posiciones aleatorias dentro de un área definida. Calcula vectores de dirección normalizados y añade pausas entre movimientos.

### 2025-04-11 Alfonso @(2h)
Limpieza de los elementos del prototipo de GamePlayState, implementada la lógica para tener balas y para que los enemigos puedan emitirlas con BulletEmitter

### 2025-04-12 Mario @(2.5h)
Implementados los márgenes del GamePlayState y la interfaz de la puntuación y las vidas del jugador. Inicialmente se ha intentado que interactuase con todo del juego, pero por el momento es visual.

### 2025-04-12 Kevin @(3h)
Actualicé la clase Sprite para optimizar la gestión de texturas y frames, usando variables estáticas para evitar recargas innecesarias, ajustando el origen, la escala y la posición de los sprites (incluyendo ROUND_BULLET, PLAYERSHOT y THRENODY) para que se muestren correctamente en pantalla.

### 2025-04-12 Kevin @(15min)
Modifiqué la clase Player para que el jugador se mueva más lento al presionar Shift.

### 2025-04-12 Marcos @(2h)
Consegui hacer y entender TileMap fuera del codigo, en unas pruebas externas. 

### 2025-04-13 Kevin @(1h50)
Cambié el método shoot() para devolver un vector con dos balas. Añadí un offset horizontal calculado dinámicamente usando el ancho del sprite. Por último, he creado dos métodos `increaseLives()` y `decreaseLives()` junto con un getter para el manejo de vidas.

### 2025-04-13 Alfonso @(2.5h)
He creado tres patrones para demostrar el motor de entidades y una función para enseñar texto de debug para la cantidad de balas y enemigos en pantalla.

### 2025-04-13 Mario @(1.5h)
He implementado las colisiones que tienen las balas del juego, haciendo que las balas del jugador bajen la vida de los enemigos y viceversa, teniendo el jugador 3 vidas. También he implementado que cuando la vida del jugador baje a 0 el juego regrese automáticamente al menú principal, por lo que no es posible volver al juego una vez pierdas. Por último, hago que los enemigos desaparezcan al bajarles la vida a 0 y cuando el jugador recibe daño, obtiene unos frames de invencibilidad, atravesando balas y que su sprite parpadee con un intervalo.

### 2025-04-13 Alfonso @(0.5h)
He reformado la clase SoundPlayer para que sea singleton y que se pueda llamar desde cualquier sitio.

### 2025-04-12 Marcos @(1h)
Consegui hacer y entender TileMap fuera del codigo, en unas pruebas externas. 

### 2025-04-13 Kevin @(4h50)
He añadido varios efectos especiales cuando el jugador dispara, al morir un enemigo y cuando el jugador recibe daño. Así mismo, cada efecto tiene su propio sonido especial en específico. 

### 2025-04-14 Alfonso @(2.5h)
Tareas para vacaciones, y anteriormente solucioné todos los warnings residuales para la entrega

### 2025-04-19 Alfonso @(2h)
Mejoras de BulletEmitter, ahora soporta ráfagas y disparos caóticos. He incluido también una doumentación Doxygen de cada uno de los métodos para ayudar a crear patrones

### 2025-04-19 Mario @(2h)
Implementación del menú de pausa (PauseState) y game over (GameOverState). El menú de pausa detiene todo y muestra las opciones de continuar con el juego o salir al menú principal, si se le da a continuar con el juego otra vez tanto por el menú principal como por la pausa, el juego reanuadará en el estado donde, tanto si tenía vidas o no, le falta añadir transparencia al fondo. El menú de Game Over aparece al perder todas las vidas, te muestra la puntuación que has obtenido, y al pulsar Z (indicado en la pantalla), vuelve al menú principal. Como el fondo está pendiente por hacer, he puesto el fondo de ScoreState para solo tener que reemplazarlo después.

### 2025-04-27 Kevin @(2h)
He hecho algunos cambios en la clase de Spite. He añadido un nuevo constructor para pasarle un color, y de esta manera a partir de un tipo de color que le indiquemos este se desplazará en el spritesheet y lo cambiará. También he añadido nuevos tipos de balas, se leccionando cada frame del spritesheet que hemos empleado.

### 2025-04-29 Alfonso @(1h)
Durante una reunión del grupo, hemos renovado las tareas y discutido lo que nos queda por hacer

### 2025-05-04 Mario @(2h)
Tareas de PlayerShoot hechas, he incluido la clase DiagonalMovement (un MovementStrategy) para que podamos crear más tipos de disparos. La pantalla de WeaponSelectState ya es funcional, inicialmente está seleccionado el Disparo 1, y cada disparo es totalmente distinto: ángulo, número de balas, daño... He creado un array de powerups (3 de longitud).

### 2025-05-06 Alfonso @(6h)
Terminé de conseguir implementar los Niveles bien formados, los Bosses y las bombas. También añadí un nuevo MovementStrategy, TurningMovement y empecé a crear el primer nivel del juego.

### 2025-05-06 Marcos @(4h)
Unos dialgos mejor implementados.

### 2025-05-07 Kevin @(3.5h)
He sacado más Sprites de balas (spritesheet). Todo los cambios se encuentran en la clase `Sprite`. He hecho algunos cambios a la hora de cambiar el color de cada frame según la sección donde se encuentre, también se ajusta a un ancho variable. Por otro lado, he creado un nuevo método para cargar únicamente diferentes tipos de enemigos del correspondiente SpriteSheet.

### 2025-05-09 Kevin @(3h)
He modificado la clase `Sprite` para incluir animaciones de los enemigos. Ha sido necesario incluir una linea de código dentro de `Enemy` para actualizar cada frame según el delta time. También se ha includo las animaciones de algunas balas de la hoja de sprites de Bullets. Para que funcione las animaciones de las balas en necesario descomentar la línea que se encuentra en el método update de la clase `PlayerShoot`.

### 2025-05-09 Alfonso @(3h)
Principalmente corrección de bugs y limitación de las bombas en función del nivel de power-up que tenga el player. Las balas y disparos del jugador ahora giran su sprite, el modo burst del bullet emiter ha sido refactorizado y funciona, a player se le ha añadido un nuevo sprite que representa su hitbox y una nueva colisión preparada para la implementación de items, se ha añadido un patrón nuevo al nivel y otros cambios varios.

### 2025-05-09 Kevin @(1h)
He modificado las animaciones de los enemigos para que visualmente se cargan los frames en el orden adecuado. Así mismo, he cambiado el tamaño de todos los enemigos, reduciendolos con una misma escala. Por último, he cargado los portraint de los jefes desde la carpeta de recursos y su sprite en el juego.

### 2025-05-10 Alfonso @(3h)
He añadido los tilemaps y los items al completo

### 2025-05-10 Marcos @(1.5h)
He mejorado los dialogos

### 2025-05-11 Mario @(1.5h)
El jugador obtiene una vida cuando derrota un boss, falta obtener una bomba al cambiar de fase

### 2025-05-11 Mario @(2h)
La obtención de vida y bombas funcionan correctamente. Las barras con iconos también. Las dificultades de Normal y Difícil funcionan correctamente, pero queda por corregir Fácil, se traba el juego al abrirlo.

### 2025-05-12 Alfonso @(2h)
Jefe de la fase 5 terminado

### 2025-05-13 Mario @(3h)
Implementados 6 patrones del nivel 5, que es el que hay ahora mismo: spiralWave, zigZagBarrage, crossFire, waveCrest, startBurst y pincerStrike. Van después de los patrones que ya hay y antes que el jefe.

### 2025-05-13 Kevin @(1h)
He añadido varios efectos de sonido

### 2025-05-14 Kevin @(2h)
He modificado la clase MusicPlayer para que funcione con un patrón singleton. Además, he añadido varias pistas de música que se ejecutan a lo largo del juego. Por último, he añadido varios sonidos de efectos al menú del juego. Aún quedas cosas que pulir, y comprobar todo el funcionamiento de los sonidos y pistas de música.

### 2025-05-16 Marcos @(4h)
He mejorado los dialogs para implementarlos a lo actual

### 2025-05-16 Marcos @(2..5h)
Arreglo de dialogos, se mostraban los diálogos de preboss antes de los de postboss e implementación del jefe entero Tenebris  

### 2025-05-17 Kevin @(2h)
He añadido algunos sonidos de efectos, y he optimizado la carga de los recursos dentro de la clase SoundPlayer en lugar de hacerlo en el main.cpp. Por otro lado, he añadido diferentes canciones (pistas) de música en cada nivel del jefe y una pista en específico al menú; falta hacer comprobaciones si el cambio de música en cada nivel lo hace de forma correcta. Por último, he añadido los últimos Sprites de los jefes.

### 2025-05-17 Alfonso @(6h)
Fase 4 con su respectivo jefe y subjefe añadido, ahora se continua al siguiente nivel después de superar una fase.

### 2025-05-18 Mario @(3h)
Implementado el reinicio de los niveles al pasarse el último nivel, junto con una pantalla al finalizar el último nivel.

### 2025-05-18 Mario @(5h)
Nivel 2 implementado, incluye 6 patrones de enemigos comunes, 1 minijefe y 1 jefe con ataques temáticos.

### 2025-05-19 Kevin @(2h)
He añadido el nivel 3, el cuál es el Boss Vespera con 4 fases distintas. La fase 2 se podría pulir aún mas, el estado actual es acorde.

### 2025-05-19 Alfonso @(1h)
Añadir la ten sheet actualizada por Marcos, el diagrama de clases, el manual de usuario y el trabajo realizado en reparto.md

### 2025-05-19 Mario @(4h),
Funcionalidad básica del modo práctica, como la selección nivel y disparo, pero con algunos fallos. Ranking de puntuación totalemente funcional, ordena el archivo scores.txt (el juego coge ese archivo en ScoreState para mostrarlo) y cada vez que el jugador se pasa el juego se actualiza el ranking , tiene un máximo de 10 y se borra el último y se inserta en la posición correspondiente.
