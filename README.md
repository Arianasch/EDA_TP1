# EDA #level1: Orbital simulation

## Integrantes del grupo y contribución al trabajo de cada integrante

*Moschini Rita: inicializar las instancias de ortibital_body_t y uso de memoria dinamica.
*Schiaffino Ariana: seccion de la vista para asteroides y planetas. Eleccion de dubujar esferas o
puntos dependiendo distancia.
Toda la parte de modelo fue realizada en conjunto. Optamos por trabajar en grupo.


## Verificación del timestep

Experimentamos con distintos valores de paso temporal, e incluso aumentando el número de asteroides
el comportamiento de la simulación se mantenía consistente con el paso de un número considerable de
años.

## Verificación del tipo de datos float

Dados como están hechos los datos de tipo float, su rango es
•	-3.4028235 E+38 a -1.1754944 E-38
•	0.0E+0
•	+1.1754944 E-38 a +3.4028235 E+38

MASAS
La mayor masa del sistema solar es la del sol, con un valor de 1.9885 E30, y la menor masa es la de
Mercurio, de 0.3302 E24. En el caso de Alfa Centauri, las masas son 2.167 E30 y 1.789 E30. Todos 
estos valores pertenecen al rango de los valores positivos de punto flotante, por lo cual este tipo
de dato sirve perfectamente para almacenar las masas de ambos sistemas.

POSICIONES
La coordenada de posición inicial de menor módulo es la y inicial de la Tierra, con un valor de 
1.907499306293577E+07 y la de mayor módulo es la x de Neptuno, que vale 4.431790029686977E+12F .
En el caso de Alfa Centauri, ambos cuerpos inician con posiciones de exponente 11. Todos estos 
valores pueden ser almacenados con precisión por variables de tipo float.

VELOCIDADES
La coordenada de velocidad inicial de menor módulo es la del sol, con un valor de 
5.809369653802155E-00, y la de mayor módulo es la coordenada z de Mercurio con un valor de
5.056613955108243E+04. En el caso de Alfa Centauri, las coordenadas de velocidades iniciales de
menor y mayor módulo son 7.120E+03F y 8.430E03F respectivamente. Dado que Mercurio es el planeta
más cercano al Sol, siempre va a ser el cuerpo (sin contar los asteroides) cuya velocidad tenga
mayor módulo, y como tiene tanto margen (es de orden 4, cuando el orden el máximo y mínimo de punto
flotante es 32), consideramos que las velocidades pueden ser representadas mediante este tipo de
dato cómodamente.

ACELERACIONES
Para analizar la precisión de las aceleraciones evaluamos en papel la aceleración de que genera la
atracción entre el sol y Mercurio en la primera iteración, dado que el sol es el cuerpo más másico
del sistema solar, y Mercurio el planeta más cercano al mismo. Tanto el valor de la aceleración
como los de las variables intermedias que utilizamos para las cuentas pertenecían al rango de punto
flotante y dejaban margen (es decir, el riesgo de llegar a overflow o underflow era muy bajo).

En conclusión, consideramos que la precisión del punto flotante es suficiente para actualizar las
posiciones de los cuerpos representados en esta simulación.
			


## Complejidad computacional con asteroides
Al agregar los 500 asteroides los FSP bajaron notablemente y la simulación se volvió muy lenta. Una
de las causas más influyentes sostenemos que fue el haber intentado calcular la fuerza de atracción 
entre cada asteroide y cada planeta, ya que supuso realizar por lo menos 4500 operaciones. Para 
resolverlo, decidimos calcular solo la influencia del cuerpo de mayor masa del sistema sobre cada 
asteroide; en el caso del sistema solar el mismo es el sol. 

También consideramos seguir la misma línea de pensamiento que cuando calculamos las atracciones de 
los planetas entre sí, es decir considerar la Ley de Acción y Reacción para optimizar el programa como
se explicará más adelante, pero al final descartamos la idea porque no la consideramos óptima.

Por otro lado, identificamos un cuello de botella que involucra lo gráfico. Resulta que  probamos 
la sección de vista usando las funciones DrawSphere y DrawPoint3D por separado cada una y en 
simultáneo, y llegamos a la conclusión de que cuando hacíamos uso de la función DrawSphere 
significaba más costo computacional. Esto se debe a que DrawPoint3D debe hacer una tarea muy 
sencilla como lo es ubicar uno o varios píxeles, mientras que DrawSphere conlleva sombreados,
cálculo de curvas, iluminación y más vértices que un punto, es decir, requiere más procesamiento y
significa una mayor complejidad computacional. Para resolver esto decidimos limitar el uso 
DrawSphere. En el caso de los planetas y el sol, al ser solo 9 instancias dibujamos siempre tanto la
esfera como el punto, mientras que para los asteroides el programa elije de qué forma dibujar
cada uno en base a la distancia de la cámara al mismo.

## Mejora de la complejidad computacional
Inicialmente hicimos dos for anidados y para cada planeta calculamos la atracción de todos los demás
sobre el mismo. Luego nos dimos cuenta de que estábamos haciendo dos veces las mismas cuentas, es 
decir estábamos calculando la influencia del planeta A sobre el planeta B y viceversa, así que 
teniendo en consideración la Ley de Acción y Reacción (que toda acción genera una reacción igual 
y opuesta) logramos optimizar notablemente el programa. Se adjunta foto de cómo se pensaron los 
índices de los for anidados para este último caso: 
![matriztrianguladasup](https://github.com/user-attachments/assets/ceed4feb-b150-4316-8c41-b610bd5d4480)

## Bonus points
Se aumento por 1000 la masa de Jupiter y, como primera observación, notamos que los 8 planetas
dejaron de orbitar al rededor del Sol. Algunos se alejaron y el Sol comenzo a cambiar su posicion 
bruscamente (cosa que antes claramente no ocurría) y parecería que describía una especie de 
trayectoria al rededor de Jupiter, pero este a la vez también cambiaba bruscamente de posicion. 
Planetas como Mercurio seguian orbitando al rededor del Sol, otros se alejaban y regresaban.

Luego se reemplazo al Sol por un agujero negro multiplicando la masa de este por cinco. 
Fuente: https://www.nationalgeographic.es/espacio/agujeros-negros.
Notamos que tanto los planetas como asteroides orbitaban con mayor rapidez, y aquellos planetas 
que estaban más alejados se acercaban más al sol. Podemos observar que la fuerza de atracción de
los cuerpos hacia el sol aumento notablemente.

Probamos reemplazar el Sistema Solar por el Alfa Centauri. Los asteroides formaban una especie de
anillo que se expandia e interaccionaban mucho mas con la estrella Alfa Centauri B. Tambien nos 
dimos cuenta de que nuestro codigo es flexible ante cualquier sistema estelar, basta cambiar el 
campo numberOfBodies y reemplazar lo que actualmente dice sistema solar por el sistema estelar deseado.

El easter egg es la linea 53 del archivo orbitalSim.cpp. Al establecer phi=0, la unica variable 
aleatoria para cada asteroide es la r. Sin embargo, las coordenadas y,z son cero y entonces todos 
los asteroides, al comenzar la simulación, estan todos a lo largo del eje x (tienen distintas r 
asignadas de manera aleatoria). Si se corre el programa así, al comienzo veríamos:
![phi=0](https://github.com/user-attachments/assets/397a520e-5282-4b0c-9ca9-0b00f56735cc)
