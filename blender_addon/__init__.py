"""
Vine Generator – Blender Add-on
================================
Entry point for the Blender plugin.  The UI is written in Python; the
heavy lifting (L-system expansion, spline/mesh construction) is delegated
to the C++ core via a Python extension module.
"""

bl_info = {
    "name": "Vine Generator",
    "author": "",
    "version": (0, 1, 0),
    "blender": (4, 0, 0),
    "location": "View3D > Sidebar > Vine",
    "description": "Procedural vine growth using an L-system and spline mesh",
    "category": "Add Mesh",
}


def register():
    # TODO: register operators, panels, and properties
    pass


def unregister():
    # TODO: unregister operators, panels, and properties
    pass
