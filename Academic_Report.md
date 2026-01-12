# MEMORIA DEL PROYECTO: THE VAULT CHALLENGE
**Asignatura:** Microcontroladores / Sistemas Embebidos
**Fecha:** Enero 2026

---

## 1. Portada
*(Espacio reservado para los datos del alumno. Rellene los siguientes campos)*
*   **Título del Proyecto:** The Vault Challenge: Sistema Híbrido de Entretenimiento Arcade
*   **Autor:** [Tu Nombre Completo]
*   **Titulación:** [Tu Grado/Ingeniería]
*   **Grupo:** [Tu Grupo de Prácticas]
*   **Convocatoria:** Enero 2026

---

## 2. Resumen (Abstract)
"The Vault Challenge" es un proyecto de ingeniería que integra sistemas embebidos y desarrollo web moderno para crear una experiencia de usuario gamificada única. El sistema consiste en una caja fuerte física controlada por un microcontrolador Arduino Uno, que interactúa en tiempo real con una interfaz gráfica "Arcade" desplegada en un PC. El objetivo principal es demostrar la viabilidad de arquitecturas híbridas donde el hardware gestiona tareas de tiempo real crítico (como la medición de reflejos con precisión de milisegundos) y el software de alto nivel gestiona la lógica de negocio y la presentación visual. El proyecto implementa protocolos de comunicación serie robustos, máquinas de estados finitos y una arquitectura de software modular, resultando en un producto final pulido, educativo y entretenido.

---

## 3. Introducción y Objetivos
En el contexto actual del Internet de las Cosas (IoT) y la Industria 4.0, la interacción entre el mundo físico y el digital es fundamental. Este proyecto nace con el objetivo de explorar y dominar esta interfaz.

**Objetivos Principales:**
1.  **Sinergia Hardware-Software:** Diseñar un sistema donde el Arduino no sea un mero periférico pasivo, sino un controlador activo que condiciona la ejecución de una aplicación web compleja.
2.  **Gamificación Educativa:** Convertir una evaluación técnica (preguntas sobre voltaje, pines, bits) en una mecánica de juego ("Security Checks") para hacer el aprendizaje más atractivo.
3.  **Robustez Técnica:** Implementar patrones de diseño profesionales (Máquinas de Estados, Debouncing por software, Comunicación asíncrona) para asegurar un funcionamiento libre de fallos.

---

## 4. Hardware y Diseño del Sistema
El núcleo del hardware es un **Arduino Uno (ATmega328P)**. El diseño se ha optimizado para maximizar la funcionalidad utilizando el mínimo número de pines, empleando técnicas de multiplexación analógica.

### 4.1 Esquema de Conexiones
*(Por favor, inserte aquí su captura de pantalla de SimulIDE o Wokwi)*

**[INSERTAR IMAGEN DEL ESQUEMA AQUÍ]**

*   **Interfaz Humana (HMI):** Se utiliza una pantalla **LCD 16x2** conectada por bus **I2C** (pines SDA/SCL) para reducir el cableado de 6 a 2 hilos.
*   **Actuador:** Un **Servomotor** conectado al pin digital **D6** (PWM) simula el mecanismo de cierre de la caja fuerte.
*   **Comunicación:** El puerto USB del Arduino actúa como puerto serie virtual (UART) a 9600 baudios.

### 4.2 Botonera Analógica (Resistor Ladder)
Uno de los desafíos de diseño fue la gestión de 5 botones (Arriba, Abajo, Izquierda, Derecha, Selección) con un número limitado de pines.
**Solución:** En lugar de utilizar 5 pines digitales, se implementó una **Escalera de Resistencias** conectada a un único pin analógico (**A0**).
*   **Funcionamiento:** Cada botón cierra el circuito a través de una resistencia de valor diferente, creando un divisor de tensión único.
*   **Lectura:** El ADC (Conversor Analógico-Digital) del Arduino lee un valor de voltaje (0-1023). El software (`InputManager`) decodifica este valor en rangos (ej. 0-50 = Derecha, 140-150 = Arriba) para identificar el botón pulsado. Esto permite controlar una interfaz completa con un solo pin de E/S.

---

## 5. Implementación Software

### 5.1 Firmware (C++ / PlatformIO)
El código del Arduino sigue un paradigma de **Programación Orientada a Objetos**, modularizando cada componente hardware en una clase (`LCDManager`, `UARTManager`, `Vault`).
La lógica central reside en el archivo `main.cpp`, que implementa una **Máquina de Estados Finitos (FSM)**.
*   **Estados:** `IDLE` (Esperando comando) ⮕ `RUN_GAME1` (Reflejos) ⮕ `RUN_GAME2` (Memoria) ⮕ `RUN_QUIZ` (Preguntas) ⮕ `WAIT_PIN` (Apertura).
*   **Ventaja:** Esta arquitectura impide condiciones de carrera y asegura que el sistema solo responda a los comandos permitidos en cada fase del juego.

### 5.2 Backend (Python Flask)
El servidor web actúa como puente entre el navegador y el Arduino.
*   **Multithreading:** Flask ejecuta un hilo en segundo plano (`background_thread`) que sondea continuamente el puerto serie. Esto es crítico: si la lectura fuera síncrona, la interfaz web se congelaría esperando datos del Arduino.
*   **SocketIO:** Utiliza WebSockets para "empujar" (push) los datos a la web en tiempo real, permitiendo que la interfaz reaccione instantáneamente (ej. iluminar una flecha) cuando se pulsa un botón físico.

### 5.3 Protocolo de Comunicación
Se diseñó un protocolo de texto ASCII personalizado para facilitar la depuración:
*   **PC ⮕ Arduino:** Comandos imperativos (ej. `START_GAME1`, `UNLOCK:568`).
*   **Arduino ⮕ PC:** Telemetría y Eventos (ej. `GAME1_OK:350` para reportar 350ms de tiempo de reacción, `GUESS:1:5` para reportar que el usuario seleccionó la respuesta 5).

---

## 6. Manual de Usuario
1.  **Conexión:** Conecte el Arduino Uno al puerto USB del ordenador.
2.  **Inicio:** Ejecute el script `run_app.py`. El navegador se abrirá automáticamente.
3.  **Configuración:** En la pantalla "Attract Mode", seleccione el puerto COM correcto y pulse "CONNECT SYSTEM".
4.  **Juego:**
    *   Pulse "INITIALIZE HACK".
    *   **Nivel 1:** Siga las instrucciones de voz/texto. Pulse el botón indicado lo más rápido posible.
    *   **Security Check:** Use Arriba/Abajo para seleccionar la respuesta correcta.
    *   **Nivel 2:** Repita la secuencia de direcciones mostrada.
    *   **Nivel 3:** Use los botones del Arduino para controlar el personaje en pantalla.
5.  **Final:** Introduzca el código obtenido. El servo se abrirá.

### Capturas de Funcionamiento

**[INSERTAR CAPTURA DE LA INTERFAZ WEB (JUEGO) AQUÍ]**
*(Pie de foto: El juego "Cyber Runner" controlado por las entradas del Arduino)*

**[INSERTAR FOTO DEL MONTAJE ARDUINO AQUÍ]**
*(Pie de foto: El montaje físico mostrando el LCD y la botonera)*

---

## 7. Conclusiones
El desarrollo de "The Vault Challenge" ha permitido consolidar conocimientos clave en el diseño de sistemas embebidos:
1.  **Gestión de Recursos:** El uso de I2C y entradas analógicas multiplexadas demuestra cómo optimizar el uso de pines en microcontroladores limitados.
2.  **Comunicación Asíncrona:** La mayor dificultad técnica (sincronizar la web con el hardware) se resolvió eficazmente mediante un diseño basado en eventos y WebSockets, eliminando la latencia perceptible.
3.  **Experiencia de Usuario (UI/UX):** Se ha demostrado que una interfaz "retro" y cuidada (CSS/JS) aumenta significativamente el valor percibido de un proyecto hardware sencillo.

Este proyecto sienta las bases para futuros desarrollos IoT más complejos, validando la arquitectura Arduino-Python-Web como una solución robusta y escalable.
