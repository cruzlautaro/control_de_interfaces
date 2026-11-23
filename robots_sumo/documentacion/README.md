A CONTINUACION VA A ESTAR LO INVESTIGADO DEL PUENTE H: 

¿Qué es? Un puente H es un circuito electrónico formado por 4 interruptores (transistores) ordenados en forma de "H", con el motor en el centro. Sirve para controlar el sentido de giro y la velocidad de un motor DC.

¿Por qué hace falta? El microcontrolador (en nuestro caso la Raspberry Pi Pico W) entrega muy poca corriente, y un motor necesita mucha más. Además, un motor solo gira en un sentido si le aplicás tensión siempre igual. El puente H resuelve ambas cosas: amplifica la corriente y permite invertir la polaridad.

¿Cómo funciona?

Se cierran dos interruptores opuestos (S1 y S4): la corriente circula en un sentido y el motor gira adelante.
Se cierran los otros dos (S2 y S3): la corriente circula al revés y el motor gira atrás.
Nunca se cierran los dos de un mismo lado a la vez (S1+S3 o S2+S4), porque provocaría un cortocircuito.
IN1	IN2	Resultado
0	0	Motor libre
1	0	Adelante
0	1	Atrás
1	1	Freno
Control de velocidad: se usa una señal PWM (modulación por ancho de pulso) en el pin de habilitación (ENA/ENB). Prende y apaga la corriente muy rápido; cuanto más tiempo está encendida por ciclo, más rápido gira el motor.

Control de velocidad: se usa una señal PWM (modulación por ancho de pulso) en el pin de habilitación (ENA/ENB). Prende y apaga la corriente muy rápido; cuanto más tiempo está encendida por ciclo, más rápido gira el motor.

