# EDA #level1: Orbital simulation

## Integrantes del grupo y contribución al trabajo de cada integrante

*Moschini Rita: inicializar las instancias de ortibital_body_t y uso de memoria dinamica.
*Schiaffino Ariana: seccion de la vista para asteroides y planetas. Eleccion de dubujar esferas o puntos dependiendo distancia.
Toda la parte de modelo fue realizada en conjunto. Optamos por trabajar en grupo.


## Verificación del timestep

[completar]

## Verificación del tipo de datos float

[completar]

## Complejidad computacional con asteroides

Al agregar los 500 asteroides los FSP bajaron notablemente y la simulacion se volvio muy lenta. Una de las causas mas influyentes sostenemos que fue el 
haber intentado calcular la fuerza de atraccion de cada asteroide con todos los planetas ya que supuso realizar por lo menos 4500 cuentas. Intentamos seguir la misma linea de pensamiento que cuando calculamos las de los planetas entre si(para evitar hacer dos veces las mismas cuentas, pensamos en la Ley de Accion y Reaccion y optimizamos notablemente el programa)pero no fue posible.
Se adjunta foto de como se pensaron los indices del los for aninados para el calculo de las aceleraciones de los planetas.
![matriztrianguladasup](https://github.com/user-attachments/assets/ceed4feb-b150-4316-8c41-b610bd5d4480)

Luego, por otro lado, identificamos un cuello de botella que involucra lo grafico. Resulta que probamos la seccion de vista usando las funciones DrawSphere y DrawPoint3D por separado cada una y en simultaneo, y llegamos a la conclusion de que cuando haciamos uso de la funcion DrawSphere significaba mas costo computacional. Esto se debe a que DrawPoint3D debe hacer una tarea muy sencilla como lo es ubicar un solo pixel en el espacio (o varios) mientras que DrawSphere
conlleva sombreados, calculo de curvas, iluminacion y mas vertices que un punto, es decir, requiere mas tiempo de procesamiento y significa una mayot complejidad
computacional. 

## Mejora de la complejidad computacional
Para resolver la cantidad de cuentas que significaba el calculo de las aceleraciones de los 500 asteroides con los 9 planetas del Sistema Solar, decidimos calcular solo la fuerza de atraccion mas significativa, la de cada asteroide con el Sol. 

Por otro lado, para resolver lo grafico, decidimos hacer uso de DrawSphere solo cuando fuese estrictamente necesario: para los planetas consideramos que siempre pero para los asteroides depende de la distancia de la camara al respectivo asteroide. Fue por esto que usamos un condicional que, depende esa distancia, qué es lo que representa en el espacio. Conclusion: consideramos que resolvimos un problema de complejidad computacional.


## Bonus points
Se aumento por 1000 la masa de Jupiter y, como primera observacion,notamos que los 8 planetas dejaron de orbitar al rededor del Sol. Algunos se alejaron y el Sol comenzo a cambiar su posicion bruscamente (cosa que antes claramente no ocurria) y pareceria que describia una especie de trayectoria al rededor de Jupiter pero este a la vez tambien cambiaba bruscamente de posicion. Planetas como Mercurio seguian orbitando al rededor del Sol, otros se alejaban y regresaban.

Luego se reemplazo al Sol por un agujero negro multiplicando la masa de este por cinco. Fuente: https://www.nationalgeographic.es/espacio/agujeros-negros.
Notamos que tanto los planetas como asteroides orbitaban con mayor rapidez y ,aquellos planetas que estaban mas alejados, se acercaban mas al Sol. Pero , a modo de conclusion, los planetas no dejaron de orbitan al rededor del Sol pero su fuerza de atraccion hacia el aumento notablemente.

Probamos reemplazar el Sistema Solar por el Alfa Centauri. Los asteroides formaban una especie de anillo que se expandia e interaccionaban mucho mas con la estrella Alfa Centauri B. Tambien nos dimos cuenta de que nuestro codigo es flexible ante cualquier sistema estelar , basta cambiar el campo numberofbodies y reemplazar lo que actualmente dice sistema solar por el sistema estelar deseado.

El easter egg es la linea 53 del archivo orbitalSim.cpp, al establecer phi=0, la unica variable aleatoria para cada asteroide es la r. Sin embargo, las coordenadas y,z son cero y entonces todos los asteroides , al comenzar la simulacion, estan todos a lo largo del eje x (tienen distintas r asignadas de manera aleatoria).
Si se corre el programa asi, al comienzo entonces veriamos:
![phi=0](https://github.com/user-attachments/assets/397a520e-5282-4b0c-9ca9-0b00f56735cc)
