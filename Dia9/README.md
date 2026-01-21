# Día 9 - Advent of Code 2025 (Movie Theater)

## 1. Justificación de la Elección del Problema

Se seleccionó el **Día 9** del Advent of Code 2025 como problema de aplicación de **Tablas Hash y Optimización Espacial** por las siguientes razones:

### Adecuación Técnica
- **Estructura de Datos Natural**: El problema requiere acceder rápidamente a puntos agrupados por coordenada X. Una tabla hash es la estructura perfecta para esta operación.
- **Optimización Espacial**: El problema permite demostrar cómo una organización inteligente de datos puede reducir significativamente el espacio de búsqueda.

### Dificultad Técnica Apropiada
- Requiere implementar una tabla hash manualmente (en lugar de usar `std::unordered_map`).
- Necesita calcular máximos sobre pares de puntos de forma eficiente.
- Incluye lógica geométrica para calcular áreas de rectángulos.

### Valor Formativo
- Demuestra que no siempre debería confiar en estructuras de datos built-in; a veces implementarlas me ayuda a entender su funcionamiento.
- Muestra cómo las tablas hash son fundamentales para resolver problemas de búsqueda y agrupamiento.

---

## 2. Descripción Técnica Detallada

### 2.1 Enunciado del Problema

Se proporciona un conjunto de puntos (baldosas rojas) en un plano infinito 2D, cada uno representado por coordenadas (X, Y).

**Objetivo**: Encontrar el **rectángulo de mayor área** definido por dos puntos cualesquiera del conjunto. El área se calcula como:
- **Ancho**: |X₁ - X₂|
- **Altura**: Depende de las coordenadas Y de los puntos

**Complejidad**: Naive: O(n²) comparando cada par de puntos. Con tabla hash: O(n²) pero mucho más rápido en práctica.

### 2.2 Modelado del Problema

#### Estructura de Datos: Tabla Hash

En lugar de usar `std::unordered_map`, implementamos una tabla hash manual:

```cpp
const int HASH_SIZE = 10007;  // Tamaño primo para dispersión

struct Node {
    long long x;          // Coordenada X (clave)
    long long y_min;      // Y mínima encontrada para esta X
    long long y_max;      // Y máxima encontrada para esta X
};

struct ManualHashTable {
    std::vector<Node> buckets[HASH_SIZE];  // Array de buckets (encadenamiento)
    
    int hash_function(long long x) {
        return std::abs(x) % HASH_SIZE;
    }
};
```

**¿Por qué una tabla hash en lugar de otros datos estructurados?**

| Estructura | Tiempo de Inserción | Tiempo de Búsqueda | Espacio | Ventajas |
|-----------|-------------------|------------------|--------|----------|
| **Tabla Hash** | O(1) promedio | O(1) promedio | O(n) | Búsqueda rápida, ideal para agrupamiento |
| **Vector sin ordenar** | O(1) al final | O(n) | O(n) | Simple, pero búsqueda lenta |
| **Árbol BST** | O(log n) | O(log n) | O(n) | Ordenado, pero más lento que hash |
| **Matriz de adyacencia** | N/A | O(1) | O(n²) | Consume demasiada memoria |

### 2.3 Algoritmo: Construcción del Índice

#### Paso 1: Agrupar Puntos por Coordenada X
```
Para cada punto (x, y) en la entrada:
    1. Calcular hash_index = hash_function(x)
    2. Buscar en buckets[hash_index] si ya existe una entrada para x
       - Si existe: actualizar y_min y y_max
       - Si no existe: crear nueva entrada con y_min = y_max = y
    3. Actualizar variables globales global_min_x y global_max_x
```

**Pseudocódigo**:
```cpp
void insert(long long x, long long y) {
    int index = hash_function(x);
    
    // Búsqueda en el bucket (encadenamiento)
    for (auto &node : buckets[index]) {
        if (node.x == x) {
            // Ya existe, actualizar rango Y
            if (y < node.y_min) node.y_min = y;
            if (y > node.y_max) node.y_max = y;
            return;
        }
    }
    
    // No existe, crear entrada nueva
    buckets[index].push_back({x, y, y});
}
```

**Complejidad**: 
- Inserción única: O(1) promedio (con buen factor de carga)
- Total para n puntos: O(n) promedio

### 2.4 Algoritmo: Calcular Máxima Área

#### Paso 2: Extraer Todos los Nodos
```cpp
std::vector<Node*> all_nodes;
for (int i = 0; i < HASH_SIZE; ++i) {
    for (size_t j = 0; j < ht.buckets[i].size(); ++j) {
        all_nodes.push_back(&ht.buckets[i][j]);
    }
}
```

Este paso recopila todos los nodos únicos de la tabla hash en un vector para acceso rápido.

#### Paso 3: Comparar Pares de Columnas (X)

```
Para cada par de nodos (i, j) donde i < j:
    1. Calcular ancho: width = |x_i - x_j|
    2. Calcular altura:
       - h1 = |y_max_i - y_min_j|
       - h2 = |y_min_i - y_max_j|
       - altura = max(h1, h2)
    3. Calcular área: area = width × altura
    4. Si area > max_area, actualizar max_area
```

**Pseudocódigo**:
```cpp
long long max_area = 0;
for (size_t i = 0; i < all_nodes.size(); ++i) {
    for (size_t j = i + 1; j < all_nodes.size(); ++j) {
        long long width = std::abs(all_nodes[i]->x - all_nodes[j]->x);
        
        // Dos formas posibles de calcular altura
        long long h1 = std::abs(all_nodes[i]->y_max - all_nodes[j]->y_min);
        long long h2 = std::abs(all_nodes[i]->y_min - all_nodes[j]->y_max);
        
        long long area = width * std::max(h1, h2);
        if (area > max_area) max_area = area;
    }
}
```

**Complejidad**:
- Doble iteración sobre columnas: O(d²), donde d es el número de coordenadas X únicas
- En el peor caso (todos los puntos en X diferentes): O(n²)
- En el mejor caso (muchos puntos comparten X): O(d²) << O(n²)

### 2.5 Análisis Completo de Complejidad

| Fase | Complejidad | Justificación |
|------|-------------|---------------|
| **Lectura de archivo** | O(n) | Leer cada punto una vez |
| **Construcción de tabla hash** | O(n) | Insertar cada punto una vez |
| **Extracción de nodos** | O(d) | d = número de X únicos, d ≤ n |
| **Cálculo de máxima área** | O(d²) | Comparar cada par de columnas |
| **Total** | **O(n + d²)** | Normalmente mucho mejor que O(n²) si d << n |

**Espacio**: O(n) para almacenar todos los puntos.

### 2.6 Ejemplo de Ejecución

**Entrada**:
```
10, 5
10, 15
20, 5
20, 20
```

**Fase 1: Construcción de Tabla Hash**
```
X=10: y_min=5, y_max=15
X=20: y_min=5, y_max=20
```

**Fase 2: Cálculo de Áreas**
- Comparar X=10 con X=20:
  - Ancho = |10 - 20| = 10
  - h1 = |15 - 5| = 10
  - h2 = |5 - 20| = 15
  - altura = max(10, 15) = 15
  - **área = 10 × 15 = 150**

**Salida**: 150

### 2.7 Ventajas de la Tabla Hash en Este Problema

1. **Agrupamiento Automático**: Puntos con la misma X se agrupan automáticamente.
2. **Rápida Localización**: Encontrar todos los puntos de una X toma O(1) en promedio.
3. **Reducción de Comparaciones**: Si muchos puntos comparten X, O(d²) << O(n²).
4. **Escalabilidad**: Para conjuntos enormes de puntos, la mejora es significativa.

---

## 3. Alternativas Consideradas y Descartadas

### 3.1 Enfoque Bruto: Comparar Todos los Pares de Puntos
**Descripción**: Iterar sobre todos los pares de puntos sin agrupamiento previo.

```cpp
long long max_area = 0;
for (size_t i = 0; i < puntos.size(); ++i) {
    for (size_t j = i + 1; j < puntos.size(); ++j) {
        long long area = calcularArea(puntos[i], puntos[j]);
        if (area > max_area) max_area = area;
    }
}
```

**Motivos del Descarte**:
- **Complejidad O(n²)** sin optimización.
- Comparamos muchos pares innecesarios (por ejemplo, dos puntos con la misma X siempre formarán un rectángulo de área 0).
- No escalable para conjuntos grandes de datos.

### 3.2 Usar `std::map` en lugar de Tabla Hash Manual
**Descripción**: Usar `std::map<long long, pair<long long, long long>>` para almacenar X → (y_min, y_max).

```cpp
std::map<long long, std::pair<long long, long long>> indexPorX;
```

**Motivos del Descarte**:
- Implementar manualmente nos ayuda a entender el funcionamiento de tablas hash.
- `std::map` usa árboles BST, que son O(log n) para búsqueda (más lento que hash O(1)).
- El objetivo formativo es comprender cómo funcionan las tablas hash.

### 3.3 Ordenamiento + Búsqueda Binaria
**Descripción**: Ordenar los puntos por X, luego usar búsqueda binaria para localizar rangos.

```cpp
std::sort(puntos.begin(), puntos.end());
// Luego: búsqueda binaria por X...
```

**Motivos del Descarte**:
- **Complejidad O(n log n)** para ordenar, versus O(n) para tabla hash.
- La búsqueda binaria es O(log n) versus O(1) de tabla hash.
- No aprovecha el agrupamiento natural de los datos.

### 3.4 Estructura Personalizada con Encadenamiento vs. Direccionamiento Abierto
**Descripción**: Usar direccionamiento abierto (sondeo lineal) en lugar de encadenamiento.

```cpp
// Encadenamiento (elegido):
std::vector<Node> buckets[HASH_SIZE];

// Direccionamiento abierto (descartado):
std::vector<Node> tabla(HASH_SIZE);
// Buscar siguiente posición libre...
```

**Motivos de mi elección (Encadenamiento)**:
- Más simple de implementar correctamente.
- Mejor rendimiento cuando hay colisiones (lo que es probable con muchos puntos).
- Código más claro y menos propenso a errores.

### 3.4 Estructura Personalizada con Encadenamiento vs. Direccionamiento Abierto
**Descripción**: Usar direccionamiento abierto (sondeo lineal) en lugar de encadenamiento.

```cpp
// Encadenamiento (elegido):
std::vector<Node> buckets[HASH_SIZE];

// Direccionamiento abierto (descartado):
std::vector<Node> tabla(HASH_SIZE);
// Buscar siguiente posición libre...
```

**Motivos de nuestra elección (Encadenamiento)**:
- Más simple de implementar correctamente.
- Mejor rendimiento cuando hay colisiones (lo que es probable con muchos puntos).
- Código más claro y menos propenso a errores.

---

## 4. Compilación y Ejecución

### Compilación
Los archivos `input1.txt` y `dia_9.cpp` deben estar en el mismo directorio:

```bash
g++ -std=c++17 dia_9.cpp -o dia9
```

**Nota**: El código busca `input1.txt`. Si tu archivo se llama diferente, modifica la línea:
```cpp
std::ifstream archivo("input1.txt");  // Cambiar aquí si es necesario
```

### Ejecución
```bash
./dia9
```

**Salida esperada**: Un número entero representando el área máxima encontrada.

---

## 5. Valoración Personal

Este problema fue extremadamente valioso para consolidar conceptos de **tablas hash y optimización de búsqueda**:

### 5.1 Aprendizajes Clave

1. **Tablas Hash: Funcionamiento Interno**
   - Implementar manualmente me permitió entender qué ocurre "bajo el capó".
   - Comprendí cómo el **factor de carga** y la **función hash** afectan el rendimiento.
   - Aprendí que O(1) promedio no siempre significa O(1) en el peor caso (con colisiones).

2. **Agrupamiento Espacial**
   - Vi cómo reorganizar datos (agrupar por X) puede reducir drásticamente el espacio de búsqueda.
   - Comprendí el concepto de **índices espaciales**, que es crítico en bases de datos y sistemas geométricos.

3. **Optimización sin Cambiar el Algoritmo**
   - El algoritmo sigue siendo O(d²) en comparaciones, pero d es mucho menor que n.
   - Esta mejora práctica es tan importante como la complejidad teórica.

4. **Aritmética de Coordenadas**
   - Practiqué cálculos geométricos (áreas de rectángulos).
   - Reforcé la importancia de manejar bien los signos y valores absolutos.

5. **Lecturas de Archivos Complejas**
   - El archivo de entrada requerí parsear valores separados por comas y otros caracteres.
   - Usé `sscanf` y reemplazo de caracteres para normalizar la entrada.

### 5.2 Conexión con la Carrera

Para un ingeniero de **Robótica e Industria**:
- Las tablas hash son fundamentales en **planificación de rutas** y **detección de colisiones**.
- El agrupamiento espacial es crítico en sistemas que deben procesar **sensores espacialmente distribuidos**.
- Los índices espaciales (como quad-trees, basados en hashing) son esenciales en robótica de tiempo real.

### 5.3 Reflexión Final

Este problema demostró que la correcta organización de datos puede ser tan importante como la elección del algoritmo. Una mejora de O(n²) a O(n + d²) puede significar la diferencia entre un programa que completa en segundos y uno que tarda horas.

Para un ingeniero industrial, esta habilidad de "pensar en datos" y optimizar la representación es crítica para resolver problemas reales donde los datos son masivos.
