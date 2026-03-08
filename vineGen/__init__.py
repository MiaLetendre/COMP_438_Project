
import bpy
#compiled c++

try:
    from . import vineGen_cpp
except ImportError:
    print("Could not import vineGen_cpp. Make sure it is compiled and in the same directory.")
    vineGen_cpp = None


class VineGenOperator(bpy.types.Operator):
    #vine gen operator
    bl_idname = "object.vine_gen_operator"
    bl_label = "Generate Vine"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        if vineGen_cpp is None:
            self.report({'ERROR'}, "vineGen_cpp module not found. Cannot generate vine.")
            return {'CANCELLED'}
        else:
            self.report({'INFO'}, "Generating vine...") 
        return {'FINISHED'}

    def execute(self, context):
        #Call the C++ function to generate the vine
        vineGen_cpp.generate_vine()
        return {'FINISHED'}

classes = (VineGenOperator,)

def register():
    # Register your classes here (Operators, Panels)
    for cls in classes:
        bpy.utils.register_class(cls)

def unregister():
    # Clean up here
    for cls in classes:
        bpy.utils.unregister_class(cls)

if __name__ == "__main__":
    register()



bl_info = {
    "name": "vineGen",
    "author": "Mia Letendre",
    "version": (1, 0, 0),
    "blender": (4, 0, 0),
    "category": "Object",
    "location": "View3D > Object > Generate Vine",
    "description": "Generates a procedural vine using C++",
}

