# Vine Generator – Initial Design Document

## Overview

A Blender add-on that generates procedural vine geometry using an
[L-system](https://en.wikipedia.org/wiki/L-system) for growth simulation and
spline interpolation to produce smooth tube-like meshes.

---

## Architecture

```
Python (Blender UI / add-on)
        │
        │  calls
        ▼
C++ Core (Python extension module)
   ├── LSystem          – string-rewriting grammar
   ├── Turtle           – interprets L-system string → vine segments
   ├── Geometry         – Vec3, ControlPoint, Bezier evaluation
   └── HalfEdgeMesh     – mesh data structure for output geometry
```

---

## Data Flow

1. **L-system expansion** – The `LSystem` class rewrites the axiom string
   over *n* iterations according to user-defined production rules.

2. **Turtle interpretation** – A turtle reads the resulting string symbol by
   symbol and emits a list of *vine segments*, each described by a start
   position, end position, and orientation.

3. **Spline interpolation** – Each vine segment is converted to a cubic
   Bezier spline using `ControlPoint` objects.  Blender's native NURBS/Bezier
   curve objects (or a custom tube mesh) are built from these points.

4. **Mesh output** – The final geometry is stored in a `HalfEdgeMesh` (for
   operations that need adjacency) and exported to a Blender `bpy.types.Mesh`.

---

## Key Classes

| File | Class / Struct | Responsibility |
|------|---------------|----------------|
| `core/lsystem.hpp` | `LSystem` | L-system string rewriting |
| `core/geometry.hpp` | `Vec3` | 3-D vector math |
| `core/geometry.hpp` | `ControlPoint` | Spline control point + radius |
| `core/half_edge.hpp` | `HalfEdgeMesh` | Half-edge mesh (DCEL) |
| `core/half_edge.hpp` | `Vertex`, `HalfEdge`, `Face` | Mesh primitives |
| `blender_addon/__init__.py` | – | Blender add-on entry point |

---

## References

- Prusinkiewicz & Lindenmayer, *The Algorithmic Beauty of Plants* (1990)
- de Berg et al., *Computational Geometry* – half-edge / DCEL structure
- [Blender Python API](https://docs.blender.org/api/current/)
