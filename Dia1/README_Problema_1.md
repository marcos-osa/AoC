# Día 1 - Advent of Code 2025 (Safe Lock)

## 1. Justificación de la Elección del Problema

Se seleccionó el **Día 1** del Advent of Code 2025 como problema de aplicación de **Recursión y Programación Dinámica** por las siguientes razones:

### Adecuación a las Técnicas
- **Estructura Recursiva Natural**: El problema se descompone de forma iterativa: cada movimiento depende del anterior, lo que invita naturalmente al uso de recursión.
- **Aplicabilidad de Memoización**: El espacio de estados es acotado (índice de movimiento × posición del dial), perfecto para demostrar cómo DP reduce cálculos redundantes.
- **Comparación de Paradigmas**: Permite comparar tres enfoques: iterativo, recursivo puro, y recursivo con memoización.

### Valor Formativo
- Para un estudiante de **Ingeniería Industrial y Robótica**, dominar recursión y DP es esencial en optimización de procesos y sistemas automatizados.
- Enseña la importancia de elegir la abstracción correcta: cada técnica tiene ventajas y compromisos.

---

## 2. Descripción Técnica Detallada

### 2.1 Enunciado del Problema
Se proporciona una caja fuerte con un dial numerado de 0 a 99, inicialmente en posición 50. Se reciben instrucciones de movimiento:
- `Lx`: rotar a la izquierda (restar x)
- `Rx`: rotar a la derecha (sumar x)

**Objetivo**: Contar cuántas veces el dial apunta a la posición 0 después de cada movimiento.

### 2.2 Tres Soluciones Implementadas

#### Solución 1: Iterativa (Sin Recursividad)
```cpp
// Pseudocódigo
for each movimiento:
    pos = (pos ± pasos) % 100
    if pos == 0: contador++
```
- **Complejidad Temporal**: O(n) donde n = número de movimientos
- **Complejidad Espacial**: O(1) - solo variables auxiliares
- **Ventaja**: Más simple y eficiente

#### Solución 2: Recursión Pura
```cpp
int solveRec(const vector<Move>& moves, int index, int pos) {
    if (index >= moves.size()) return 0;
    int newPos = (pos ± steps) % 100;
    return (newPos == 0 ? 1 : 0) + solveRec(moves, index + 1, newPos);
}
```
- **Complejidad Temporal**: O(n)
- **Complejidad Espacial**: O(n) - pila de recursión
- **Ventaja**: Demuestra descomposición recursiva natural
- **Desventaja**: Sin optimización, cada estado se recalcula

#### Solución 3: Programación Dinámica con Memoización
```cpp
int resolverDP(const vector<Move>& moves, int index, int pos, 
               map<pair<int,int>, int>& memo) {
    if (index >= moves.size()) return 0;
    
    auto key = make_pair(index, pos);
    if (memo.find(key) != memo.end()) return memo[key];
    
    int newPos = (pos ± steps) % 100;
    int result = (newPos == 0 ? 1 : 0) + resolverDP(moves, index + 1, newPos, memo);
    memo[key] = result;
    return result;
}
```
- **Complejidad Temporal**: O(n × 100) en el peor caso
- **Complejidad Espacial**: O(n × 100) para la tabla de memoización
- **Ventaja**: Evita recálculos; óptimo si hay muchos estados compartidos
- **Desventaja**: Overhead de memoria

### 2.3 Concepto Clave: Aritmética Modular Circular
El dial es circular: después del 99 viene el 0. Por eso:
```cpp
newPos = (pos + steps) % 100;  // Maneja automáticamente el envolvimiento
```

### 2.4 Análisis Comparativo

| Solución | Tiempo | Espacio | Recálculos | Caso de Uso |
|----------|--------|---------|-----------|-----------|
| Iterativa | O(n) | O(1) | N/A | Datos secuenciales simples |
| Recursiva Pura | O(n) | O(n) | Muchos | Demostrativo, datos pequeños |
| DP Memoizada | O(n×100) | O(n) | Ninguno | Si hay estados repetidos |

---

## 3. Alternativas Consideradas y Descartadas

### Alternativa 1: Representación Explícita del Dial
**Idea**: Usar un array de 100 elementos para modelar el dial.
**Por qué se descartó**: Innecesario. El operador módulo ya maneja la circularidad de forma elegante y sin overhead de memoria.

### Alternativa 2: Guardar Todas las Posiciones Recorridas
**Idea**: Almacenar cada posición visitada para análisis posterior.
**Por qué se descartó**: No requerido por el enunciado. Aumentaría memoria sin beneficio algorítmico.

### Alternativa 3: Lista Circular Explícita
**Idea**: Implementar una estructura de lista circular personalizada.
**Por qué se descartó**: Complejidad innecesaria. El módulo aritmético es más limpio y portable.

---

## 4. Análisis de Complejidad General

| Aspecto | Análisis |
|---------|----------|
| **Lectura de fichero** | O(n) - una pasada secuencial |
| **Procesamiento de movimientos** | O(n) en todas las soluciones |
| **Memoria mínima requerida** | O(1) para solución iterativa |
| **Bottleneck** | Lectura del fichero input.txt |

---

## 5. Valoración Personal

### Aprendizajes Principales
1. **Aritmética Modular en Dominios Circulares**: El operador `% 100` es elegante y suficiente para cualquier problema de rotación cíclica. Algo fundamental en sistemas de robótica.

2. **Comparación de Paradigmas**: Trabajar en paralelo con tres soluciones permitió ver cómo:
   - La iteración es siempre más rápida para este caso
   - La recursión es conceptualmente clara pero costosa en memoria
   - La DP es un equilibrio valioso cuando hay muchos estados repetidos

3. **Gestión de Ficheros**: Reafirmé la importancia de:
   - Validar apertura de fichero (`is_open()`)
   - Procesar línea a línea con `getline()`
   - Cerrar correctamente (`close()`)

4. **Minimalismo en Diseño**: Para este problema solo necesité:
   - Una variable de posición actual
   - Un contador de ceros alcanzados
   - No hay estructuras complejas que justificar

### Reflexión sobre la Carrera
En **Ingeniería Industrial y Robótica**, el control de procesos y la trazabilidad son críticos. Esta progresión de Día 1 —desde lo iterativo simple hasta la optimización dinámica— refleja cómo evolucionamos hacia sistemas más sofisticados.

### Próximos Pasos
Este Día 1 sentó bases sólidas:
- Validé que el entorno de compilación funciona
- Practiqué lectura de ficheros (recurrente en todos los días)
- Repasé tres paradigmas algorítmicos antes de abordar problemas más complejos

El "pequeño dial circular" del Día 1 es metáfora de cómo los problemas simples, bien resueltos, preparan el camino para soluciones cada vez más elegantes.
