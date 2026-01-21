# Día 4 - Advent of Code 2025 (Paper Roll Distribution)

## 1. Justificación de la Elección del Problema

Se seleccionó el **Día 4** del Advent of Code 2025 como problema de aplicación de **Grafos** por las siguientes razones técnicas y formativas:

### Adecuación al Enfoque de Grafos
- **Estructura Natural de Grafos**: El problema presenta una estructura inherentemente de grafos. Cada rollo de papel (`@`) puede interpretarse como un **vértice**, y las relaciones de adyacencia entre rollos actúan como **aristas**.
- **Concepto Central**: El problema se fundamenta en el **grado de un vértice** (número de vecinos adyacentes), que es un concepto fundamental de la teoría de grafos.

### Nivel de Dificultad Apropiado
- No es un problema trivial: requiere entender conceptos como grafos implícitos, validación de límites, y cálculo eficiente de grados.
- Suficientemente complejo para justificar la técnica, pero asequible para su implementación clara.

### Valor Formativo para la Carrera
- Para un estudiante de **Ingeniería Industrial y Robótica**, comprender cómo modelar problemas reales como grafos es esencial.
- Enseña la importancia de elegir la abstracción correcta: un grafo no siempre requiere almacenamiento explícito.

---

## 2. Descripción Técnica Detallada

### 2.1 Enunciado del Problema
Se proporciona una cuadrícula bidimensional con:
- Rollos de papel representados por `@`
- Espacios vacíos (espacios o caracteres especiales)

**Objetivo**: Determinar cuántos rollos son "accesibles", donde un rollo es accesible si tiene **menos de 4 rollos adyacentes** (considerando las 8 direcciones: arriba, abajo, izquierda, derecha y 4 diagonales).

### 2.2 Modelado del Problema como Grafo

#### Componentes del Grafo
| Elemento | Definición |
|----------|-----------|
| **Vértices** | Cada celda de la cuadrícula que contiene `@` |
| **Aristas** | Implícitas. Dos vértices están conectados si sus celdas son adyacentes (hasta 8 vecinos) |
| **Tipo de Grafo** | No dirigido, no ponderado |
| **Representación** | Implícita (no almacenamos aristas explícitamente) |

#### Propiedad Crítica
El **grado** de cada vértice (número de vecinos) determina directamente si un rollo es accesible:
- Accesible: grado < 4
- No accesible: grado ≥ 4

### 2.3 Estructura de Datos Principal

```cpp
class GrafoDeRollosConGrados {
private:
    vector<PosicionEnMapa> verticesRollo;   // Almacena (fila, columna) de cada rollo
    vector<int> gradoPorRollo;              // gradoPorRollo[i] = grado del vértice i
};
```

**Decisión de Diseño**: ¿Por qué no almacenamos una lista de adyacencia completa?
- **Razón de Eficiencia**: Solo necesitamos el grado de cada vértice para resolver el problema.
- **Ahorro de Memoria**: O(n) vs. O(n + m), donde n es el número de vértices y m el número de aristas.
- **Simplicidad**: Reduce la complejidad del código sin sacrificar funcionalidad.

### 2.4 Algoritmo: Cálculo de Grados

#### Pseudocódigo
```
1. Leer mapa desde archivo en vector<vector<char>>
2. Recorrer cuadrícula completa:
   - Guardar posición de cada '@' en verticesRollo
3. Inicializar gradoPorRollo con todos los valores en 0
4. Para cada vértice i en verticesRollo:
   a. Obtener su posición (fila, columna)
   b. Revisar las 8 posiciones adyacentes usando DESP_FILA[k] y DESP_COLUMNA[k]
   c. Para cada posición adyacente:
      - Verificar que está dentro del mapa (estaDentroDelMapa)
      - Si contiene '@', incrementar contador de vecinos
   d. Guardar el contador como gradoPorRollo[i]
5. Contar vértices con grado < 4
```

#### Implementación Clave: Arrays de Desplazamiento
```cpp
const int DESP_FILA[8]    = {-1, -1, -1,  0, 0,  1, 1, 1};
const int DESP_COLUMNA[8] = {-1,  0,  1, -1, 1, -1, 0, 1};
```
Estos arrays permiten iterar eficientemente sobre las 8 direcciones sin escribir múltiples condicionales.

### 2.5 Análisis de Complejidad

| Operación | Complejidad | Justificación |
|-----------|-------------|---------------|
| **Lectura del mapa** | O(f × c) | f = filas, c = columnas |
| **Construcción de verticesRollo** | O(f × c) | Recorre cada celda una vez |
| **Cálculo de grados** | O(n × 8) = O(n) | n = número de vértices, 8 direcciones fijas |
| **Conteo de accesibles** | O(n) | Itera sobre cada vértice una vez |
| **Total** | **O(f × c)** | Lineal respecto al tamaño del mapa |

#### Espacio
- **Almacenamiento del mapa**: O(f × c)
- **Vértices y grados**: O(n), donde n ≤ f × c
- **Total**: O(f × c)

### 2.6 Flujo de Ejecución

```
main()
  ↓
leerMapaDesdeArchivo("input.txt")  → vector<vector<char>>
  ↓
GrafoDeRollosConGrados(mapa)  → Construcción:
  - Identifica todos los '@'
  - Calcula grado de cada uno
  ↓
contarRollosAccesibles(mapa)  → Cuenta vértices con grado < 4
  ↓
Salida: Número total de rollos accesibles
```

---

## 3. Alternativas Consideradas y Descartadas

### 3.1 Recorrido Directo de la Matriz (Enfoque Ingenuo)
**Descripción**: Simplemente iterar sobre todas las celdas y para cada rollo contar manualmente sus vecinos.

```cpp
// Pseudocódigo del enfoque ingenuo
for (int f = 0; f < mapa.size(); f++) {
    for (int c = 0; c < mapa[f].size(); c++) {
        if (mapa[f][c] == '@') {
            int vecinos = contarVecinos(f, c, mapa);
            if (vecinos < 4) accesibles++;
        }
    }
}
```

**Motivos del Descarte**:
- Funcionalmente correcto, pero pierde la oportunidad de aplicar conceptos de teoría de grafos.
- No proporciona una mejora algorítmica (ambos son O(f × c)).
- Sacrifica claridad conceptual sin beneficio práctico.
- Menos escalable para extensiones futuras (por ejemplo, si necesitáramos hacer BFS o DFS).

### 3.2 Lista de Adyacencia Explícita
**Descripción**: Guardar explícitamente cada arista en una estructura `vector<vector<int>>` representando la lista de adyacencia.

```cpp
class Grafo {
    vector<vector<int>> listaAdyacencia;  // listaAdyacencia[i] = lista de vecinos de i
};
```

**Motivos del Descarte**:
- **Innecesario para este problema**: Solo necesitamos el grado, no la lista completa.
- **Desperdicio de memoria**: O(n + m) en lugar de O(n).
- **Complejidad innecesaria**: Añade código sin beneficio funcional.
- **Construcción costosa**: Requeriría dos pasadas: primero identificar vértices, luego construir aristas.

### 3.3 Matriz de Adyacencia
**Descripción**: Usar una matriz n×n donde n es el número de rollos, con valores 1/0 indicando adyacencia.

```cpp
vector<vector<int>> matrizAdyacencia(n, vector<int>(n, 0));
```

**Motivos del Descarte**:
- **Excesivamente costoso en memoria**: O(n²). Con miles de puntos, sería inmanejable.
- **Innecesario**: No necesitamos verificar adyacencia entre pares arbitrarios; usamos la cuadrícula.
- **Más lento de construir**: O(n²) vs. O(n) de nuestra solución.

---

## 4. Compilación y Ejecución

### Compilación
Los archivos `input.txt` y `dia4.cpp` deben estar en el mismo directorio:

```bash
g++ -std=c++17 dia4.cpp -o dia4
```

### Ejecución
```bash
./dia4
```

**Salida esperada**: Un número entero representando la cantidad de rollos accesibles.

---

## 5. Valoración Personal

Este problema fue especialmente valioso para consolidar conceptos de **teoría de grafos** aplicados a un contexto práctico:

### 5.1 Aprendizajes Clave

1. **Grafos Implícitos vs. Explícitos**
   - Aprendí que no siempre es necesario representar un grafo de forma explícita en memoria.
   - La cuadrícula ya contiene toda la información que necesito.
   - Esta es una lección fundamental en optimización de recursos y diseño eficiente.

2. **Concepto de Grado en Grafos**
   - Vi cómo el "grado de un vértice" (número de vecinos) es un concepto fundamental que aparece en muchos problemas.
   - Comprendí que a menudo solo necesito esta métrica local, no la estructura completa del grafo.

3. **Mantenibilidad y Claridad Conceptual**
   - Aunque el enfoque ingenuo (bucles anidados) habría funcionado, el enfoque de grafos proporciona:
     - Código más legible para alguien familiarizado con teoría de grafos.
     - Base sólida para extender la solución (por ejemplo, si añadiera búsquedas de caminos).
     - Mejor documentación del pensamiento detrás del código.

4. **Validación de Límites**
   - El método `estaDentroDelMapa()` es un patrón recurrente cuando trabajo con matrices.
   - Reforzó la importancia de evitar accesos fuera de rango, un error común en C++.

5. **Uso de Estructuras Dinámicas**
   - Practiqué con `vector<T>`, inicialización con `assign()`, y acceso mediante índices.
   - Gané familiaridad con operaciones comunes en C++ que son fundamentales.

### 5.2 Reflexión Final
Este problema demostró que la correcta elección de abstracciones (en este caso, representar el problema como un grafo) no siempre proporciona mejoras algorítmicas, pero sí:
- **Mejora la legibilidad** del código.
- **Facilita el mantenimiento** y las extensiones futuras.
- **Proporciona valor educativo** al entrenar mi mente para pensar en términos de abstracciones matemáticas.

Para un estudiante de Ingeniería Industrial y Robótica, esta habilidad de modelar problemas reales como grafos es crítica para resolver problemas de optimización, rutas, y redes en la industria.
