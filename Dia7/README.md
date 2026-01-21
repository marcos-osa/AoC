# Día 7 - Advent of Code 2025 (Tachyon Ray Laboratory)

## 1. Justificación de la Elección del Problema

Se seleccionó el **Día 7** del Advent of Code 2025 como problema de aplicación de **Árboles Binarios** por las siguientes razones:

### Adecuación Natural a Árboles
- **Estructura Inherente de Árbol**: El problema de los rayos de taquiones que se dividen en un divisor (`^`) crea naturalmente una estructura de árbol binario: cada divisor es un nodo que genera exactamente 2 ramas (izquierda y derecha).
- **Divide y Vencerás**: La naturaleza recursiva del problema (un rayo se divide en dos rayos más pequeños) es la descripción textual de la estructura recursiva de un árbol binario.

### Dificultad Técnica Apropiada
- Requiere implementar la construcción recursiva de un árbol.
- Necesita manejar estructuras dinámicas (punteros).
- Incluye la combinación de árboles con una estructura hash para evitar ciclos.

### Valor Formativo
- Demuestra que los árboles son abstracciones poderosas para resolver problemas con estructura recursiva.
- Enseña cómo combinar múltiples técnicas (árboles + hash) para resolver problemas más complejos.

---

## 2. Descripción Técnica Detallada

### 2.1 Enunciado del Problema

Se proporciona un laboratorio bidimensional representado como una rejilla de caracteres:
- `S`: Punto de entrada (fila 0, en alguna columna)
- `^`: Divisor de rayos (al golpear aquí, un rayo se divide en dos: diagonal-izquierda y diagonal-derecha)
- Espacios vacíos: El rayo viaja sin obstáculos
- Bordes del mapa: Límites del laboratorio

**Objetivo**: Un rayo de taquiones entra en `S` hacia abajo. Cada vez que golpea un divisor `^`, se divide en dos rayos (uno hacia la diagonal-izquierda y otro hacia la diagonal-derecha). Contar cuántos **divisores únicos** son activados en total.

### 2.2 Modelado del Problema como Árbol Binario

#### Estructura del Árbol
| Elemento | Rol en el Problema |
|----------|-------------------|
| **Raíz** | El punto de inicio `S` (aunque técnicamente la raíz es el primer divisor encontrado) |
| **Nodos** | Cada divisor `^` encontrado es un nodo |
| **Ramas** | Los rayos de luz que viajan entre divisores; exactamente 2 por divisor |
| **Hojas** | Rayos que alcanzan el borde del mapa sin encontrar otro divisor |

#### Ejemplo Visual
```
       S (entrada)
       ↓
      [^]  ← Nodo raíz
      / \
     ↙   ↘
   [^]   [^]  ← Nodos secundarios
   / \   / \
  ↙ ↘ ↙ ↘
 [leaf] [leaf] [leaf]
```

### 2.3 Estructura de Datos Principal

```cpp
struct NodoSplitter {
    int r, c;                    // Posición del divisor en la rejilla
    NodoSplitter *izq, *der;    // Punteros a ramas izquierda y derecha
    
    NodoSplitter(int fila, int col) 
        : r(fila), c(col), izq(nullptr), der(nullptr) {}
};

class LaboratorioTree {
    vector<string> grid;                  // La rejilla del laboratorio
    set<pair<int, int>> visitados;        // Divisores ya activados
    int filas, columnas;
};
```

### 2.4 Algoritmo: Construcción del Árbol Recursivamente

#### Pseudocódigo
```
función construirTrayectoria(fila_inicio, columna):
    1. Validar límites: columna dentro de [0, columnas), fila_inicio < filas
    2. Bajar verticalmente desde fila_inicio en la columna:
       Para cada fila r en [fila_inicio, filas):
           Si grid[r][columna] == '^':
               3. Verificar si ya he visitado este divisor
                  Si YES → return nullptr (evitar ciclos)
               4. Marcar como visitado: visitados.insert({r, columna})
               5. Crear nuevo nodo: nuevoNodo = new NodoSplitter(r, columna)
               6. Recursión izquierda: nuevoNodo->izq = construirTrayectoria(r+1, columna-1)
               7. Recursión derecha: nuevoNodo->der = construirTrayectoria(r+1, columna+1)
               8. return nuevoNodo
    9. Si no se encuentra '^', return nullptr

función main():
    1. Leer rejilla desde archivo
    2. Encontrar columna donde está 'S' en fila 0
    3. Llamar construirTrayectoria(1, col_S)
    4. Salida: visitados.size() (número de divisores activados)
```

#### Implementación Clave
```cpp
NodoSplitter* construirTrayectoria(int fila_inicio, int col) {
    if (col < 0 || col >= columnas || fila_inicio >= filas) return nullptr;

    // Bajar en columna hasta encontrar un divisor
    for (int r = fila_inicio; r < filas; ++r) {
        if (grid[r][col] == '^') {
            // Evitar ciclos infinitos
            if (visitados.count({r, col})) return nullptr;
            
            visitados.insert({r, col});
            NodoSplitter* nuevoNodo = new NodoSplitter(r, col);

            // Divide: crear dos rayos a partir del siguiente nivel
            nuevoNodo->izq = construirTrayectoria(r + 1, col - 1);
            nuevoNodo->der = construirTrayectoria(r + 1, col + 1);

            return nuevoNodo;
        }
    }
    return nullptr;
}
```

### 2.5 Rol del Set `visitados` (Estructura Hash)

**Problema Sin el Set**: Sin una estructura para controlar qué divisores ya he activado, podría entrar en ciclos infinitos si el diseño del laboratorio permitiera que dos rayos convergieran y golpearan el mismo divisor desde direcciones diferentes.

**Solución**: Usar `set<pair<int, int>> visitados` para:
1. **Evitar Ciclos**: Antes de crear un nuevo nodo, verifico si el divisor ya fue visitado.
2. **Conteo Eficiente**: Al final, `visitados.size()` me da el número total de divisores únicos activados.
3. **Complejidad de Búsqueda**: `set` proporciona búsqueda en O(log n), lo que es eficiente.

**Comparativa**:
- Usar `vector` y búsqueda lineal: O(n) por búsqueda → ineficiente si tenemos muchos divisores.
- Usar `set`: O(log n) por búsqueda → mucho mejor.
- Usar `unordered_set`: O(1) promedio, pero requerirá hash personalizado para `pair<int, int>`.

### 2.6 Análisis de Complejidad

| Operación | Complejidad | Justificación |
|-----------|-------------|---------------|
| **Lectura del mapa** | O(filas × columnas) | Lectura de cada celda |
| **Construcción del árbol** | O(filas × columnas) | En el peor caso, visitamos cada celda una vez |
| **Búsqueda en visitados (por divisor)** | O(log d) | d = número de divisores |
| **Total** | **O(filas × columnas)** | Lineal respecto al tamaño del mapa |

**Espacio**: O(d), donde d es el número de divisores encontrados.

### 2.7 Flujo de Ejecución

```
main()
  ↓
Leer rejilla desde "input.txt"
  ↓
Encontrar columna de 'S' en fila 0
  ↓
construirTrayectoria(1, col_S)  → Construcción recursiva del árbol
  - Baja verticalmente
  - Encuentra primer '^'
  - Se divide en dos rayos (izquierda y derecha)
  - Ambos rayos siguen de forma recursiva
  - Termina cuando alcanzan bordes sin encontrar divisores
  ↓
Contar visitados.size()  → Número total de divisores únicos
  ↓
Salida: Número de divisores activados
```

---

## 3. Alternativas Consideradas y Descartadas

### 3.1 BFS (Búsqueda en Amplitud) con Cola
**Descripción**: Usar una cola (queue) para explorar rayos de forma iterativa (bottom-up).

```cpp
queue<pair<int, int>> q;
q.push({1, col_S});
while (!q.empty()) {
    auto [fila, col] = q.front(); q.pop();
    // Procesar rayo...
}
```

**Motivos del Descarte**:
- **Menos natural para el problema**: Aunque funcionaría, la naturaleza recursiva de "un rayo se divide en dos" es más natural de expresar con recursión.
- **Requiere estado adicional**: Una cola es más compleja de manejar que simplemente usar la pila de recursión.
- **Menos clara conceptualmente**: La recursión espeja directamente la estructura del problema.

### 3.2 Simulación Iterativa sin Árbol
**Descripción**: Para cada rayo, guardar manualmente su trayectoria en una estructura sin construir un árbol.

```cpp
struct Rayo {
    int fila, columna;
    // estado adicional...
};
vector<Rayo> rayos_activos;
// Simulación manual...
```

**Motivos del Descarte**:
- **Más complejo de rastrear**: Sin la estructura de árbol, es fácil perder track de qué rayos están activos.
- **Difícil de depurar**: Sin una visualización clara de la estructura, los errores son más difíciles de encontrar.
- **Menos eficiente en lógica**: Duplicaría código para manejo de rayos.

### 3.3 Grafo Explícito en lugar de Árbol
**Descripción**: Modelar divisores como nodos y usar una representación general de grafo.

```cpp
struct Nodo {
    int r, c;
    vector<Nodo*> vecinos;  // Lista general de vecinos
};
```

**Motivos del Descarte**:
- **Sobrecomplicación**: Aunque un árbol ES un grafo, un árbol binario es mucho más específico y eficiente.
- **No refleja la estructura**: El problema tiene exactamente 2 ramas por divisor, no un número variable de vecinos.
- **Innecesariamente general**: Añadiría complejidad sin beneficio.

---

## 4. Compilación y Ejecución

### Compilación
Los archivos `input.txt` y `Dia_7.cpp` deben estar en el mismo directorio:

```bash
g++ -std=c++17 Dia_7.cpp -o dia7
```

### Ejecución
```bash
./dia7
```

**Salida esperada**: Un número entero representando el total de divisores (`^`) únicos activados.

---

## 5. Valoración Personal

Este problema fue extremadamente valioso para consolidar conceptos de **estructuras recursivas** y **árboles binarios**:

### 5.1 Aprendizajes Clave

1. **Recursión como Herramienta Natural**
   - Vi cómo la recursión es la forma más natural de construir estructuras arbóreas.
   - Comprendí el patrón "divide y vencerás": un problema se divide en dos subproblemas más pequeños.
   - Reforcé la importancia del **caso base** (cuando no hay más divisores o he salido del mapa).

2. **Combinación de Técnicas**
   - Combiné un árbol binario (para modelar la estructura) con un `set` (para evitar ciclos).
   - Aprendí que a menudo necesito múltiples estructuras de datos juntas para resolver un problema.
   - La solución final es más robusta porque usa ambas técnicas apropiadamente.

3. **Gestión de Memoria Dinámica**
   - Practiqué con `new` para crear nodos dinámicamente.
   - Aunque no libería la memoria (para un programa pequeño es aceptable), reforcé la importancia de entender punteros.

4. **Manejo de Ciclos en Grafos**
   - Descubrí un problema potencial: ¿qué pasa si dos rayos convergen y golpean el mismo divisor?
   - Implementé la solución elegante: usar `visitados` para marcar qué divisores ya fueron procesados.

5. **Pensamiento Crítico sobre Alternativas**
   - Consideré BFS como alternativa, pero reconocí que la recursión era más natural.
   - Esto reforzó que no siempre "la técnica más general es la mejor"; a veces la más específica es más clara.

### 5.2 Reflexión Final
Este problema demostró la potencia de las estructuras recursivas para modelar problemas complejos. Aunque podría implementarse de muchas formas, la elección de usar un árbol binario hace que:
- El código sea **directo y legible**.
- La lógica sea **fácil de seguir**.
- Las **extensiones futuras sean simples** (por ejemplo, si quisiera contar caminos en lugar de nodos).

Para un futuro ingeniero de robótica, entender cómo modelar y construir árboles recursivamente es crítico para algoritmos de planificación, búsqueda en espacios de estados, y árboles de decisión.
