# EDA #level1: Orbital simulation

## Integrantes del grupo y contribución al trabajo de cada integrante

*Moschini Rita: inicializar las instancias de ortibital_body_t y uso de memoria dinamica.
*Schiaffino Ariana: seccion de la vista para asteroides y planetas. Eleccion de dubujar esferas o
puntos dependiendo distancia.
Toda la parte de modelo fue realizada en conjunto. Optamos por trabajar en grupo.


## Verificación del timestep

Experimentamos con distintos valores de paso temporal, e incluso aumentando el número de asteroides
el comportamiento de la simulación se mantenía consistente con el paso de un número considerable de
años (100 o más). Probamos con un timeMultiplier = 1000 * SECONDS_PER_DAY para 100 fps, aumentando
así los días por segundo y la velocidad de simulación para un mismo valor de fps, es decir 
aumentamos la cantidad de cálculos por segundo. Aun así, la simulación funcionaba consistentemente. 
Repetimos la experiencia con timeMultiplier = 1500 * SECONDS_PER_DAY y con
timeMultiplier = 2000 * SECONDS_PER_DAY  y el modo de funcionamiento se repitió. Luego probamos 
aumentandolos fps a 150 para este ultimo valor de timeMultiplier, y continuó funcionando 
consistentemente.
En conclusión, consideramos que la simulación fue optimizada exitosamente, ya que soporta altos 
valores de velocidad de simulación. Igualmente, para visualizar la orbitaciones de los planetas
cómodamente, consideramos apropiado un timeMultiplier de 100 * SECONDS_PER_DAY a 100 fps.



## Verificación del tipo de datos float

Dados como están hechos los datos de tipo float, su rango es
•	-3.4028235 E+38 a -1.1754944 E-38
•	0.0E+0
•	+1.1754944 E-38 a +3.4028235 E+38

MASAS
La mayor masa del sistema solar es la del Sol, con un valor de 1.9885 E30, y la menor masa es la de
Mercurio, de 0.3302 E24. En el caso de Alfa Centauri, las masas son 2.167 E30 y 1.789 E30. Todos 
estos valores pertenecen al rango de los valores positivos de punto flotante, por lo cual este tipo
de dato sirve perfectamente para almacenar las masas de ambos sistemas.

POSICIONES
La coordenada de posición inicial de menor módulo es la y inicial de la Tierra, con un valor de 
1.907499306293577E+07 y la de mayor módulo es la x de Neptuno, que vale 4.431790029686977E+12F .
En el caso de Alfa Centauri, ambos cuerpos inician con posiciones de exponente 11. Todos estos 
valores pueden ser almacenados con precisión por variables de tipo float.

VELOCIDADES
La coordenada de velocidad inicial de menor módulo es la del Sol, con un valor de 
5.809369653802155E-00, y la de mayor módulo es la coordenada z de Mercurio con un valor de
5.056613955108243E+04. En el caso de Alfa Centauri, las coordenadas de velocidades iniciales de
menor y mayor módulo son 7.120E+03F y 8.430E03F respectivamente. Dado que Mercurio es el planeta
más cercano al Sol, siempre va a ser el cuerpo (sin contar los asteroides) cuya velocidad tenga
mayor módulo, y como tiene tanto margen (es de orden 4, cuando el orden el máximo y mínimo de punto
flotante es 32), consideramos que las velocidades pueden ser representadas mediante este tipo de
dato cómodamente.

ACELERACIONES
Para analizar la precisión de las aceleraciones evaluamos manualmente la aceleración que genera la
atracción entre el Sol y Mercurio en la primera iteración (pues el Sol es el cuerpo de mayor masa
del sistema solar, y Mercurio el planeta más cercano al mismo) y la aceleración que genera la 
atracción entre el Sol y Neptuno (siendo Neptuno el más lejano al mismo). Tanto el valor de las 
aceleraciones como los de las variables intermedias que utilizamos para las cuentas pertenecían al 
rango de punto flotante y dejaban margen (es decir, el riesgo de llegar a overflow o underflow era
muy bajo). Adicionalmente hicimos que el programa muestre en la terminal los valores de las 
aceleraciones de los cuerpos mencionados, y observamos resultados similares a los calculados manualmente.

En conclusión, consideramos que la precisión del punto flotante es suficiente para actualizar las
posiciones de los cuerpos representados en esta simulación.
![sol-mercurio](https://github.com/user-attachments/assets/a36dd832-aeeb-4c87-bb31-1ff1fd15cba3)
![cuentas-sol-neptuno](https://github.com/user-attachments/assets/124a0ab9-ff6c-474f-96a0-9fadbc17c485)
![aceleraciones](https://github.com/user-attachments/assets/a0c9a90c-74c3-4415-aaed-ded2bd35dd49)


## Complejidad computacional con asteroides
Al agregar los 500 asteroides los FPS bajaron notablemente y la simulación se volvió muy lenta. Una
de las causas más influyentes sostenemos que fue el haber intentado calcular la fuerza de atracción 
entre cada asteroide y cada planeta, ya que supuso realizar por lo menos 4500 operaciones. Para 
resolverlo, decidimos calcular solo la influencia del cuerpo de mayor masa del sistema sobre cada 
asteroide; en el caso del sistema solar el mismo es el Sol.

Por otro lado, identificamos un cuello de botella que involucra lo gráfico. Resulta que  probamos 
la sección de vista usando las funciones DrawSphere y DrawPoint3D por separado cada una y en 
simultáneo, y llegamos a la conclusión de que cuando hacíamos uso de la función DrawSphere 
significaba mayor costo computacional, por lo tanto el programa se ralentizaba y se reducían los FPS.
Esto se debe a que DrawPoint3D debe hacer una tarea muy sencilla como lo es ubicar uno o varios
píxeles, mientras que DrawSphere conlleva sombreados, cálculo de curvas, iluminación y más vértices 
que un punto, es decir, requiere más procesamiento y significa una mayor complejidad computacional.
Para resolver esto decidimos limitar el uso DrawSphere. En el caso de los planetas y el Sol, al ser
solo 9 instancias dibujamos siempre tanto la esfera como el punto, mientras que para los asteroides
el programa elije de qué forma dibujar cada uno en base a la distancia de la cámara al mismo.



## Mejora de la complejidad computacional
Inicialmente hicimos dos for anidados y para cada planeta calculamos la atracción de todos los demás
sobre el mismo. Luego nos dimos cuenta de que estábamos haciendo dos veces las mismas cuentas, es 
decir estábamos calculando la influencia del planeta A sobre el planeta B y viceversa, así que 
teniendo en consideración la Ley de Acción y Reacción (que toda acción genera una reacción igual 
y opuesta) logramos optimizar notablemente el programa. Se adjunta foto de cómo se pensaron los 
índices de los for anidados para este último caso:



## Bonus points
Se multiplicó por 1000 la masa de Júpiter, de manera que su masa terminó siendo 1.89818722E30
y la del Sol 1.988500E230, es decir que tienen el mismo orden y una diferencia relativamente baja.
Es por esto que no nos sorprendió observar que el Sol y Júpiter "orbitaban uno alrededor del otro". 
En lo que respecta a los otros cuerpos, Mercurio siguió orbitando alrededor del Sol, y a partir 
aproximadamente de los 100 años Venus comenzó a orbitar alrededor de Júpiter. Los demás planetas
fueron eyectándose del sistema uno a uno.
![jupiterMasaPor1000](https://github.com/user-attachments/assets/57979b52-1ba4-447e-aa73-b948bd7ae1c2)

Se reemplazo al Sol por un agujero negro multiplicando su masa por distintos valores. Al
multiplicarlo por 3, 12 y 25, algunos planetas cambiaron sus órbitas y otros salieron eyectados. Al 
multiplicarlo por 50, todos salían eyectados rápidamente. En todos los casos, los asteroides
orbitaban más rápido.
Elegimos emular un agujero negro de esa forma basándonos en las siguientes fuentes:
https://ciencia.nasa.gov/universo/que-son-los-agujeros-negros/
https://www.nationalgeographic.es/espacio/agujeros-negros
https://ciencia.nasa.gov/universo/diez-preguntas-que-podrias-tener-sobre-los-agujeros-negros/
![masaSolarPor25](https://github.com/user-attachments/assets/77407763-8629-48a8-9d20-46475a4fe87b)

Se probó reemplazar el Sistema Solar por Alfa Centauri, cambiando el valor de numberOfBodies a 2
y las asignaciones de valores iniciales de bodiesArray a las efemérides de Alfa Centauri. Dado que 
hacer estos cambios fue sencillo, inicialmente se consideró que el código escrito era bastante
flexible, pero rápidamente se pudo observar en la simulación que no habíamos tomado algo en
consideración: al programar la simulación adecuándola al sistema solar, las aceleraciones de los 
asteroides eran calculadas considerando solo el primer elemento del arreglo de cuerpos, pues en 
Alfa Centauri orbitaban alrededor de uno solo de los cuerpos y no de ambos. Esto tenía sentido 
asumiendo que había 9 cuerpos en el sistema y que el de mayor masa era el primero del arreglo,
pero habiendo dos cuerpos de masas similares se consideró que sería más adecuado calcular las 
aceleraciones de los asteroides respecto a ambos. Para solucionarlo, se podría complejizar el 
análisis de las aceleraciones de los asteroides, agregando que si la cantidad de cuerpos supera un 
valor umbral, entonces solo se tendrían en cuenta uno o alguno de los cuerpos de mayor masa; sino, 
se tendrían en cuenta todos los cuerpos.
![alphaCentaury](https://github.com/user-attachments/assets/0bc5bc47-6a98-4f67-8237-5493dc27b20b)


El easter egg está en la lénea 54 del archivo orbitalSim.cpp, la cual establece que phi = 0. Esto
hace que al calcular la posición de los asteroides, las coordenada x que es r * cosf(phi) quede en 
r, y la coordenada z que es r * sinf(phi) quede en 0, por lo que todos los asteroides comienzan
posicionados a lo largo del eje x. Si se corre el programa así, al comienzo veríamos algo similar a
lo siguiente:
![phi=0](https://github.com/user-attachments/assets/97a79782-65b4-4209-906b-139f39035162)
