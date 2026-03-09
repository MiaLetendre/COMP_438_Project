bl_info = {
    "name": "vineGen",
    "blender": (5, 0, 0),
    "category": "Object",
}

import bpy
import sys
import os
import traceback


module_path = r'C:\Users\miale\source\repos\COMP_438_Project\vineGen'
if module_path not in sys.path:
    sys.path.append(module_path)
    

# this name isthe name of the pybind in main
try:
    import vineGen_cpp as main_cpp
except:
    print("I have failed to open the file, im very sorry")
    traceback.print_exc()
    main_cpp = None


class VineGenProperties(bpy.types.PropertyGroup):
    iterations: bpy.props.IntProperty(name="Iterations", default=5, min=1, max=12)
    axiom: bpy.props.StringProperty(name="Axiom", default="F")
    rules: bpy.props.StringProperty(name="Rules", default="F[+F]F[-F]F")
    thickness : bpy.props.FloatProperty(name="Thickness", default=0.04, min=0.01, max=1)
    angle : bpy.props.FloatProperty(name="Angle", default=25.0, min=0.0, max=360.0)

class VineGenOperator(bpy.types.Operator):
    bl_idname = "object.vine_gen_operator"
    bl_label = "Generate Vine"
    bl_options = {'REGISTER', 'UNDO'}    


    def execute(self, context):

        scene_props = context.scene.vine_gen_props

        curve_data = bpy.data.curves.new(name="VineCurve", type='CURVE')
        curve_data.dimensions = '3D'
        curve_data.fill_mode = 'FULL'
        curve_data.bevel_depth = 0.02

        allRules = {}
        passedRules = scene_props.rules.split(",")
        for rule in passedRules:
            key, value = rule.split("->")
            allRules[key.strip()] = value.strip()
        ruleA = {"F": scene_props.rules}
        totalBranches = main_cpp.generate_vine(scene_props.iterations, scene_props.axiom, allRules, scene_props.angle)
        actaulBracnhes = bpy.data.objects.new("Vine", curve_data)
        bpy.context.collection.objects.link(actaulBracnhes)

        for currentBranch in totalBranches:
            line = curve_data.splines.new('POLY')
            line.points.add(len(currentBranch) - 1)
            for i, point in enumerate(currentBranch):
                line.points[i].co = (point[0], point[1], point[2], 1.0)
                #creates a natural thickness
                #better to do it here then rewrite a bucnh of c++ code
                taper = scene_props.thickness * (1 - (i / len(currentBranch)))
                line.points[i].radius = max(0.01, taper)


        
        return {'FINISHED'}

class panelForInputs(bpy.types.Panel):
    bl_idname = "vine_gen_panel"
    bl_label = "Vine Generation"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = 'VineGen'

    def draw(self, context):
        layout = self.layout
        scene_props = bpy.context.scene.vine_gen_props

        layout.prop(scene_props, "iterations")
        layout.prop(scene_props, "axiom")
        layout.prop(scene_props, "rules")
        layout.prop(scene_props, "thickness")
        layout.prop(scene_props, "angle")

        layout.separator()
        layout.operator("object.vine_gen_operator", text ="Generate Vine", icon ='PLUGIN')


def menu_func(self, context):
    self.layout.operator(VineGenOperator.bl_idname)

classes = [VineGenProperties, VineGenOperator, panelForInputs]
def register():
    for cls in classes:
        bpy.utils.register_class(cls)
    bpy.types.Scene.vine_gen_props = bpy.props.PointerProperty(type=VineGenProperties)

def unregister():
    for cls in reversed(classes):
        bpy.utils.unregister_class(cls)
    del bpy.types.Scene.vine_gen_props


if __name__ == "__main__":
    register()