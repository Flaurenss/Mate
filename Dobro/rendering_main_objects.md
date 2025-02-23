## VAO, VBO, EBO

### Vertex Buffer Object (VBO)
Object that stores large number of vertices in memory.
### Element Buffer Object (EBO)
Object that sotores `index data` on how the vertices must be drawn.
### Vertex Array Object (VAO)
This contains:
- Pointers to elements from `VBO`. That way we dont have to recreate VBO every time.
- Pointers to `EBO`.
- `VBO` is bind when `glVertexAttribPointer` is called.
- `EBO` can not be unbinded unil `VAO` it is.