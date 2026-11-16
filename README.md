# Compiladores-Scheduler
Trabajo Simulador de Scheduler (Backend) en ILOC para el curso de Compiladores Ccomp 8-1

## Datos del trabajo
- **Estudiante**: Alan Alvarez Puma
- **Curso**: Ccomp8-1
- **Semestre**: 2026-II
- **Profesora**: Gina Muñoz Salas

## Funcionalidades Implementadas
1) *Parser*: Se implemento un Parser apropiado respetando la sintaxis propuesta en la tarea para poder separar la instrucción, operandos, flecha y operandos destino para la gama de instrucciones propuestas
2) *Grafo*: A partir de ello se implemento un grafo donde los nodos (instrucciones) guardando su información como sus sucesores y predecesores, además de las aristas con las etiquetas de *Verdadero*,*Anti*,*Salida* y casos de *Memoria*
3) *Prioridad*: Se hizo el calculo de la prioridad en base a la latencia y la prioridad maxima de los sucesores (varia dependiendo del scheduling)
4) *Scheduling*: Ademas de implementar el forward scheduling clasico tambien se añadio el renombramiento de registros, Backward Scheduling y RBF heuristico

## Compilación

### Requisitos
- Git para clonar el repositorio
- Un compilador de C++ compatible con C++17
- En Windows, se puede utilizar `g++` de MinGW-w64

### Descargar el proyecto

```powershell
git clone https://github.com/AlanAlvarezAP/Compiladores-Scheduler.git
cd Compiladores-Scheduler
```

### Compilar

Desde la carpeta raíz del repositorio, ejecutar:
```powershell
g++ -std=c++17 -O2 Compiladores_Scheduling/Compiladores_Scheduling.cpp Compiladores_Scheduling/GrafoPredSec.cpp -o Compiladores_Scheduling.exe
```

Esto genera el ejecutable `Compiladores_Scheduling.exe` en la carpeta raíz del repositorio

## Ejecución

El programa recibe la cantidad de unidades de ejecución, las flags opcionales y la ruta del archivo ILOC:

```text
<ejecutable> <unidadesDeEjecucion> [flags] <ruta.iloc>
```

- **unidadesDeEjecucion**: número entero mayor que cero que indica cuántas unidades de ejecución se utilizarán
- **flags**: opciones que permiten seleccionar el algoritmo de scheduling y activar el renombramiento de registros
- **ruta.iloc**: ruta absoluta o relativa al archivo de entrada. Debe colocarse como último argumento

### Flags disponibles

| Flag | Comportamiento |
|---|---|
| Sin flags | Ejecuta Forward Scheduling |
| `-r` | Activa el renombramiento de registros. Si no se indica otro algoritmo, utiliza Forward Scheduling |
| `-b` | Ejecuta Backward Scheduling |
| `-rbf` | Ejecuta la heurística RBF, que realiza cinco intentos forward y cinco backward con desempates aleatorios y conserva el resultado de menor longitud |

El renombramiento puede combinarse con Backward Scheduling (`-r -b`) o RBF (`-r -rbf`)
