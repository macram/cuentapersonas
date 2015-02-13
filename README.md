# Cuentapersonas
Aplicación en Qt/C++ para contar personas a partir de un vídeo (¡o del feed de una cámara IP!). 

# Motivación
La aplicación fue realizada como la tarea final de la asignatura Imagen Digital, impartida en el curso 2014/2015 del Grado de Ingeniería del Software en la Universidad de Extremadura por el profesor [José Moreno](https://robolab.unex.es/index.php?option=com_content&view=article&id=124&catid=23&Itemid=45).

Parte del código es código libre elaborado por el laboratorio de robótica de la UEx; el código de mi autoría es básicamente gran parte del que se encuentra en el fichero `mainwindow.cpp`.

# Fallos conocidos

La aplicación, en este momento, no es capaz de identificar más de una persona en la imagen. Es decir, si hay dos personas en la imagen no las "distinguirá". Tampoco se comporta bien si cambia la luz de manera significativa en una única ejecución de la aplicación.

Además, comienza a contar en 1 (en un valor *hardcodeado*) porque el vídeo que utilicé como prueba comenzaba con una persona dentro del aula. El vídeo que yo he utilizado es un vídeo de una cámara colocada encima de la puerta de una de las aulas de la Escuela Politécnica. 
