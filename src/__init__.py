
import bpy
#compiled c++
from . import vineGen_cpp

def register():
    # Register your classes here (Operators, Panels)
    pass

def unregister():
    # Clean up here
    pass

if __name__ == "__main__":
    register()

bl_info = {
    "name": "vineGen",
    "blender": (4, 0, 0),
    "category": "Object",
}

class VineGenOperator(bpy.types.Operator):
    #vine gen operator
    bl_idname = "object.vine_gen_operator"
    bl_label = "Generate Vine"

    def execute(self, context):
        #Call the C++ function to generate the vine
        vineGen_cpp.generate_vine()
        return {'FINISHED'}