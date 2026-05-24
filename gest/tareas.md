# Tareas
## Estados de tareas
`[X]`  -  Tarea terminada

`[.]`  -  Tarea en progreso

`[ ]`  -  Tarea por empezar

`[->]` -  Tarea no terminada y movida a otra fecha

`[-]`  -  Tarea cancelada


## Tareas que se nos vayan ocurriendo

## Tareas Actuales

### *Semana 02-20 a 02-26*
* `[X]` - Entender y aprender a usar la planilla | 60' | Grupo

* `[-]` - Interfaz de entidades | 120' | Alfonso
* `[-]` - Generalización de la clase Bullet | 180' | Alfonso

* `[X]` - (Particulas) Efecto de explosión en una entidad  | 180' | Marcos
* `[->]` - (Particulas) Efecto de spawnear balas | 120' | Marcos 

* `[X]` - Implementar sonido y música | 300' | Kevin

* `[X]` - Prototipo de menú | 180' | Mario
* `[X]` - Cambio de pantalla al seleccionar una opción | 120' | Mario

### *Semana 02-27 a 03-05*
* `[X]` - Prototipo de muchas balas (Que funcione) | 180' | Alfonso
* `[X]` - Implementar el reloj en el bucle e intentar interpolar frames | 120' | Alfonso

* `[X]` - Hacer varios tipos de particulas más optimizadas | 300' | Marcos

* `[X]` - El personaje muere al tocar una bala | 180' | Kévin
* `[X]` - El personaje dispara al pulsar Z | 120' | Kévin

* `[X]` - Dividir la pantalla en menú y juego | 180' | Mario
* `[X]` - Si se reescala la ventana del juego, se debe poder seguir sabiendo si estás dentro o fuera de la pantalla | 120' | Mario

### *Semana 03-06 a 03-17 (Final del hito)*
* `[X]` - *Estados de juego* : Mario
    - El juego usa el patrón 'State' para cambiar entre estados. Estos representan los menús, el juego, el menú de pausa... Las clases que manejan los métodos render y update siguen el patrón 'Singleton'. Necesitamos declarar todas las clases anteriores y que estas guarden su estado hasta el final de la ejecución.

* `[X]` - *Clase Motor2D* : Mario
    - Otra clase 'Singleton' que contiene una referencia a la pantalla usada para render. Su función es poder ser llamada por cualquier clase para imprimirse a si misma sin depender de otras clases

* `[X]` - *Fachada de Sprite* : Kévin
    - Se requiere  una clase que funcione de la misma manera que sf::Sprite para no usar directamente la librería SFML. De momento no tendrá más funcionalidad

* `[X]` - *Clases Entity, Bullet, MovementStrategy, Pattern* : Kévin
    - Estas clases se comportan de la siguiente manera, Entity es la clase base de la que heredan los objetos del juego, con sus atributos comunes (por ahora postion y Sprite) y métodos comunes (render y update). Bullet extiende Entity y añade nuevos atributos y métodos, como la colisión y que estas desaparezcan al salir de la pantalla. Por ahora solo le añadiremos MovementStrategy, una clase a la que se llama en el update para calcular su próxima posición. Pattern es una clase que contiene las balas y métodos para crearlas, por ahora crea las balas el mismo pero más adelante implementaremos una clase 'Factory' cuando crezca la complejidad

* `[X]` - *Mejorar el prototipo de fondos y hacer el sistema de dialogos*: Marcos
    - El prototipo de fondos necesita poder cambiar entre distintos fondos durante la ejecución, también estaría bien hacerlo una clase y no parte del main. Además, necesitamos un prototipo de dialogo, donde se pueden imprimir dos Sprites a izquierda y derecha, un cuadro de texto con transparencia y texto. El texto se imprime por letras, pulsar el botón Z mientras se imprime salta esta animación e imprime todo el cuadro de texto. Al pulsar Z otra vez o con el texto ya impreso, pasa al siguiente dialogo. Es necesario hacerlo en clases distintas, no solo en el main.

* `[X]` - *Sprites de los personajes*: Marcos
    - Hay que ir haciendo los sprites.

* `[X]` - *Clases Enemy y Level*: Alfonso
    - Enemy hereda de Entity, se mueve de manera similar a Bullet y también puede generarlas. Voy a tener que pensar antes como hacerlo. Level contiene varios patrones que se ejecutan de manera consecutiva según un reloj o una vez que uno de ellos termina o todos los enemigos del anterior son derrotados. Trataré de ayudar a los demás integrantes del equipo con el código mientras pienso como seguir escalando el juego

### *Semana 03-27 a 04-02*

* `[X]` - Bucle 4 y patrón State: Mario
    - Siguiendo el diagrama de clases, empieza a programar un bucle 4 con los estados. Todas estas clases van en la librería/carpeta core salvo el main.

* `[X]` - Motor2D y menús: Mario
    - Esta tarea sería mejor hacerla después de la anterior, pero hazlo como quieras.

* `[X]` - Dropbox y resources: Kevin
    - Crear un almacenamiento de recursos en dropbox y un script para descargarlos siguiendo las guías.

* `[X]` - SoundPlayer y MusicPlayer: Kevin
    - Dos clases, una para la música y otra para los efectos de sonido. Por definir su implementación.

* `[X]` - Recursos de sonido: Kevin
    - Consigue los sonidos, asígnalos a una acción y guárdalos en el dropbox. Se pueden usar los sonidos originales de touhou

* `[X]` - Recursos gráficos: Marcos

### *Semana 04-03 a 04-10*

* `[X]` - BoundingCircle.h y BoundingCircle.cpp: Kévin
    - Implementar BoundingCircle, la clase que se encarga de calcular la colisión entre entidades. Contiene un punto de centro y un radio, además de un método que devuelve un booleano al pasarle otro BoundingCircle en función de si colisiónan. Para poder probarlo, implementar un método que dibuje el círculo en pantalla.

* `[X]` - Sprite.h y Sprite.cpp: Marcos
    - Implementar Sprite, la clase fachada que simplifica y oculta la clase Sprite de sfml. Debemos poder usarla como un sprite, y debe tener un constructor al que se le pasa una string y crea una instancia de la clase con el recurso que le pedimos. Ej: Sprite("round_bullet"); Sprite("player");. Si el sprite tiene animación, debe guardarse todas sus frames, el sprite actual (primero por default) y métodos para cambiar de frame.

* `[X]` - MovementStrategy.h: Alfonso
    - Interfaz general para las estrategias de movimiento. Todas las estrategias tienen un método común updatePosition(sf::Vector2f *pos)

* `[X]` - DirectionalMovement.h y DirectionalMovement.cpp: Mario
    - Movimiento direccional, tiene una dirección, una velocidad y una aceleración. Además, tiene los atributos con valores default velMax = 99.0, velMin = 1.0. 

* `[X]` - Player.h y Player.cpp: Alfonso
    - Sustituir en el bucle de juego el sprite del personaje por el objeto de la clase Player con lo necesario para moverlo y mostrarlo por pantalla.

### *Semana 04-11 a 04-14*
  
* `[X]` - Corregir el sprite del jugador: Marcos
    - Centrar y animar el sprite asociado al jugador, que ahora mismo está mal implementado.

* `[X]` - Enemy.h y Enemy.cpp: Alfonso
    - Crear la clase básica de Enemy, sin incluir aún la lógica para disparar balas (BulletEmitter)

* `[X]` - Bullet.h y Bullet.cpp: Mario
    - Implementación básica de la clase bala que puede colisionar con el personaje, además de que dichas balas deben marcarse como muertas al salir de la pantalla, es decir, solo tendrá un booleano que se marcará como true si lo está. Tendrá un BoundingCircle que actualizará su posición a la vez que bullet y un MovementStrategy, se basará en TestEntity su desarrollo.
 
* `[X]` - Menús: Mario
    - Cambiar menú principal para que tenga otra fuente de texto, cambiar a "Jugar" y "Opciones", junto con otra opción para "Puntuaciones" (ver formato de Touhou como ejemplo) y que siga un archivo txt para mostrarlo. Añadir una pantalla intermedia entre el gameplay y el menú principal, pero que de momento no hará nada.

* `[X]` - PlayerShoot.h y PlayerShoot.cpp: Kevin
    - Implementación de los disparos del personaje, que suben hacia arriba rápidamente y se eliminan al cabo de 2 o 3 segundos.

* `[X]` - ChaosMovement: Kevin 
    - Movimiento usado mayoritariamente por jefes, escoge una posición aleatoria dentro de un marco y se desplaza hacia ella. El desplazamiento debe ser suave

* `[X]` - Tilemap: Marcos
    - Hacer los mapas con tilemap, por ahora no vamos a hacer ninguna colisión con el mapa, pero nos servirá para hacer un bucle infinito de arriba a abajo


* `[X]` - Limpiar GamePlayState para el juego: Alfonso
    - Se debe eliminar la lógica provisional de puntos y añadir los arrays para los enemigos y las balas
    
* `[X]` - HUD bien hechas: Mario
    - Mejorar la HUD con información más relevante, como la puntuaación y el número de vidas.

* `[X]` - Márgenes de la pantalla: Mario
    - Los márgenes para la UI deben ser 16px 224px 16px 32px en orden horario.

* `[X]` - BulletEmitter.cpp y BulletEmitter.h: Alfonso
    - Crear las clases que se utilizan para spawnear las balas en la pantalla, asociadas a los enemigos y a los jefes que aún están por hacer.

* `[X]` - Implementar pattern y level para por fin tener un enemigo en pantalla: Alfonso
    - Eso mismo que acabo de decir.

* `[X]` - Sprites para enemigos, balas y algún jefe: Kevin
    - Conseguir y cargar los sprites correctos para las siguientes entidades: BULLET, PLAYERSHOT, ENEMY. Borra todo lo que no tiene un sprite o no está implementado aún.
    
* `[X]` - Detalles de player: Kevin
    - Centrar los frames de player, hacer que al pulsar Shift se mueva más lento, que dispare dos columnas de balas en vez de una y que haya un método para bajarle o subirle vidas.
 
* `[X]` - Calculo de colisiones y lógica de vidas: Mario
    - Ahora que tenemos balas y enemigos, debemos calcular la colisión usando el bounding circle. Para cada iteración del update, todas las balas comprueban si colisionan con el jugador y todos los player shot comprueban si colisionan con algún enemigo. Si hay colisión, se debe implementar que se pierda vida, tanto del enemigo como del jugador

*  `[X]` - Efectos especiales de morir para el jugador, morir para el enemigo y spawnear una bala. Y sonidos especiales: Kevin
    - Necesitamos efectos especiales para esas tres acciones, normalmente esto lo haría Marcos pero va a estar ocupado con el tilemap. Añade también efectos de sonido a esas acciones

### Busqueda de aassets - Para empezar una tarea, escribir [.] y tu nombre

* `[X]` - Crear todos los tilemaps usando tiled: Alfonso y Marcos
* `[X]` - Sprites para los enemigos: Alfonso
* `[X]` - Balas, las mismas de touhou, en spritesheet: Alfonso
* `[X]` - Sprites de los jefes: Marcos
* `[X]` - Portada y fondo del menú de selección de arma, pausa y gameover: Marcos

### Implementación de assets

* `[X]` - Corregir la clase TileMap siguiendo el código del profesor en las diapositivas: Marcos

* `[X]` - Necesitamos agrupar todos los efectos con una interfaz, con los métodos y atributos comunes que se consideren necesario: Marcos

* `[X]` - Corregir DialogueState para que se muestre encima del juego: Marcos

* `[X]` - La fachada Sprite debe cargar todas las balas, crear un nuevo enum con colores y un nuevo constructor al que se le pasa un tipo de sprite y un COLOR para sacar una bala del color específico (El color servirá para desplazar la selección del sprite dentro de la textura). Te digo exactamente los sprites que quiero: Kevin
    - De la primera columna de la imagen, saca la primera fila (lasers), la segunda (esferas), la cuarta (óvalos o granos de arroz), la sexta (kunais), la séptima (disparos), la octava (balas), la décima (carámbanos), la undécima (estrellas), la duodécima (gotas) y la trigésima (monedas). Todas son 16x16. De esta columna también quiero los óvalos grandes, los cuchillos y las balas diminutas (solo la primera fila). Saca ttambién los lasers de abajo.
    - De la segunda columna, solo quiero los mini lasers, la primera fila de ellos, sería la fila 19
    - De la tercera columna quiero los sprites de la primera columna (esferas grandes). Además los sprites animados de los orbes, las notas musicales y las esferas con aura (fila 12 a 15). Las notas de silencio también.
    - De la cuarta columna, saca la primera subcolumna (esferas enormes con borde) y la segunda columna (esferas enormes sin borde).

* `[X]` - Algunos sprites deberían tomar la misma direcció que la entitad a la que están asociada, haz un método para hacer un set de direction. Lo mismo con los BoundingCircle: Kevin

* `[X]` - Un BoundingCircle debe tener un alto y un ancho, y debe ser capaz de rotar junto a las balas: Kevin

* `[X]` - Asignar efectos de sonido a acciones y música de fondo: Kevin

* `[X]` - Menú de pausa y game over: Mario
    - Menú de pausa con capa gris transparente con las opciones de seguir o jugar. Dado que el fondo de game over lo hará Marcos añadir un fondo de ejemplo (repetido)

### Bosses

* `[X]` - La clase de Boss debe tener varios BulletSpawner, valores de vidas y movimientos, uno por cada fase. Implementar lo que es un boss: Alfonso
* `[X]` - Los niveles deben ser capaces de ejecutar patrones de bosses y secciones de dialogos con DialogueState y DialogueSystem: Alfonso

### PlayerShoots

* `[X]` - PlayerShoot debe emplear un MovementStrategy, sobretodo el direccional, para poder crear varios playershoot: Mario
* `[X]` - Cada playershoot tiene un valor de daño distinto, hay que modificar el código para que en los impactos con los enemigos se use el daño del playershoot que lo ha tocado: Mario
* `[X]` - El jugador debe tener un playershoot inicial escogido en la pantalla de selección de arma y un array estático de 3 posiciones con los power-up: Mario

### Nivel 1

* `[X]` - Terminar de hacer un BulletEmitter complejo y explicarlo: Alfonso
* `[X]` - Hacer el primer nivel en libre con las explicaciones de Alfonso: Kevin y Mario

* `[X]` - Bombas simples, un círculo expandiendose elimina todas las balas que toca: Alfonso


### Tareas para la demo

* `[X]` - Dificultades: hacer un menú intermedio de dificultad que cambia las vidas y bombas con las que empiezas: Easy(Vidas: 10, Bombas: 4 por vida), Normal(Vidas: 6, Bombas: 3 por vida), Hard(Vidas:4, Bombas: 2 por vida): Mario

* `[X]` - Al eliminar un boss, te suma una vida, al vencer una de sus fases, te suma una bomba: Mario

* `[X]` -  Las vidas y las bombas se representan en la HUD con iconos: Mario

* `[X]` - Puntos, una entidad que se suelta al eliminar a un enemigo, al cogerla player, te suma puntos:  Alfonso.

* `[X]` - Subir todos los portraits de los jefes, y los sprites de estos: Marcos

* `[X]` - Tilemap que se mueva: Alfonso

* `[X]` - Asignar un efecto de sonido a todas las acciones y poner música de fondo: Kevin

* `[X]` - Terminar de crear el nivel 5 para demostrar como funciona nuestro juego: Alfonso, Marcos, Mario y Kevin

### Tareas finales

* `[X]` - Nivel 1: El más sencillo, empieza con babyAttack y su boss es Tenebris (3 fases): Marcos

* `[X]` - Nivel 2: Contiene un subboss Hortensia (1 fase) y el boss Cloroth (4 fases): Mario

* `[X]` - Nivel 3: El tilemap que se usa es el de pasillo, su Boss es Vespera (4 fases): Kevin

* `[X]` - Nivel 4: su Boss es Godolfo (5 fases): Alfonso

* `[X]` - Implementar el sistema de dialogos de una vez por todas y escribir todos los dialogos de los jefes, pre-dialogo y post-dialogo requeridos: Marcos

* `[X]` - Terminar de incluir los sonidos y la música: Kevin

* `[X]` - Implementar el sistema de reinicio al final del juego: Mario

* `[X]` - Implementar el sistema de reinicio tras perder y no continuar o al salir por el menú de pausa y lógica de cambio de niveles tras terminar uno: Alfonso

* `[X]` - Actualizar el menú de gameover para dar la opción de continuar: Alfonso


* `[X]` - Actualizar el menú inicial para incluir el Modo de práctica, y un menú nuevo de selección de nivel para prácticas. En modo práctica, te dan 100 vidas y 3 bombas.

* `[X]` - Implementar ranking de puntuación, al pasarse el modo Historia, guardará la puntuación con la fecha en la pantalla del ranking.