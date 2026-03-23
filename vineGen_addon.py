bl_info = {
    "name": "vineGen",
    "blender": (5, 0, 0),
    "category": "Object",
}

import bpy
import sys
import os
import traceback
import time
import math
import random
import mathutils

#current problems
# - zig zaging instead of curving at end
# - generate two vines and then flower.001 doesnt get deleted in scene colection
# - totalbranches[0][0] still causing issues 

modulePath = r'C:\Users\miale\source\repos\COMP_438_Project\vineGen'
if modulePath not in sys.path:
    sys.path.append(modulePath)


    #these fumb elafs and flower have broken everything
flowerPath = r'C:\Users\miale\source\repos\COMP_438_Project\models\flower.blend'
if flowerPath not in sys.path:
    sys.path.append(flowerPath)
leafPath = r'C:\Users\miale\source\repos\COMP_438_Project\models\Leaf.blend'
if leafPath not in sys.path:
    sys.path.append(leafPath)




# this name isthe name of the pybind in main
try:
    import vineGen_cpp as main_cpp
except:
    print("I have failed to open the file, im very sorry")
    traceback.print_exc()
    main_cpp = None


class VineGenProperties(bpy.types.PropertyGroup):
    iterations: bpy.props.IntProperty(name="Iterations", default=5, min=1, max=12)
    axiom: bpy.props.StringProperty(name="Axiom: starting value of string", default="F")
    rules: bpy.props.StringProperty(name="Rules: Use -> for rule and , separate rules, ex: F->FF,X->F", default="F->F[+F]F[-F]F")
    thickness : bpy.props.FloatProperty(name="Starting Thickness: ", default=1, min=0.2, max=10)
    angle : bpy.props.FloatProperty(name="Angle", default=25.0, min=0.0, max=360.0)
    flowerProp : bpy.props.BoolProperty(name="Add Flowers", default=True)
    leafProp : bpy.props.BoolProperty(name="Add Leafs", default=True)


class VineGenOperator(bpy.types.Operator):
    bl_idname = "object.vine_gen_operator"
    bl_label = "Generate Vine"
    bl_options = {'REGISTER', 'UNDO'}    

    # the meat and potatoes of the run
    def execute(self, context):
       
        scene_props = context.scene.vine_gen_props

        collection = "Vine_" +int(time.time()).__str__() 
        if collection not in bpy.data.collections:  
            vineCol = bpy.data.collections.new(collection)
            bpy.context.scene.collection.children.link(vineCol)
        else:
            vineCol = bpy.data.collections[collection]
            for vines in vineCol.objects:
                bpy.data.objects.remove(vines, do_unlink=True)
        
        #leaf and flower stuff
        #every .blend file has mutliple fiels in it, one is object which we need
        leaf = bpy.data.objects.get("Leaf") or bpy.data.objects.get("leaf")
        flower = bpy.data.objects.get("flower") or bpy.data.objects.get("Flower")
        try: 
            if not flower:
                bpy.ops.wm.append(filepath=flowerPath + "\\Object\\flower", directory=flowerPath + "\\Object", filename="flower")
                flower = bpy.data.objects.get("flower") or bpy.data.objects.get("Flower")
            if not leaf:
                bpy.ops.wm.append(filepath=leafPath + "\\Object\\Leaf", directory=leafPath + "\\Object", filename="Leaf")
                leaf = bpy.data.objects.get("Leaf") or bpy.data.objects.get("leaf")
        except Exception as e:
            print ("failed loading leaf/flower")
        
        
        #this for loop is necessary because otherwise what will happen (and has)
        # is that you create one vine, then the next will not delete the masters because thier new name is not trackable
        for master in [leaf,flower]:
            if master:
                master.hide_set(True)
                master.hide_render = True
                master.hide_viewport = True

                if master.name not in  vineCol.objects.keys():
                    vineCol.objects.link(master)

                if master.name in bpy.context.collection.objects.keys():
                    bpy.context.collection.objects.unlink(master)
            else:
                print("failed to load leaf or flower, they will not be used in the generation")


        curve_data = bpy.data.curves.new(name="VineCurve", type='CURVE')
        curve_data.dimensions = '3D'
        curve_data.fill_mode = 'FULL'
        curve_data.bevel_depth = 0.05
        curve_data.twist_mode = 'Z_UP'

        #this is for if/wehn you turn a line into a mesh, makes it so instead of multiplying the vertes by 12 to get faces, it doe si tby 2 
        curve_data.resolution_u = 2
        curve_data.bevel_resolution = 2

        allRules = {}
        passedRules = scene_props.rules.split(",")
        for rule in passedRules:
            key, value = rule.split("->")
            allRules[key.strip()] = value.strip()
        ruleA = {"F": scene_props.rules}
        totalBranches = main_cpp.generate_vine(scene_props.iterations, scene_props.axiom, allRules, scene_props.angle)
        
        actaulBracnhes = bpy.data.objects.new("Vine", curve_data)
        vineCol.objects.link(actaulBracnhes)
      

        maxDepth = 1
        minDepth = 1

        for _, depth2 in totalBranches:
            if depth2 < minDepth:
                minDepth = depth2
            elif depth2 > maxDepth:
                maxDepth = depth2
        minDepth = max(1, minDepth)
        taperOut = 3
        rootTapersNums = []
        for i in range(1,21):
            rootTapersNums.append((i/20.0) )
        #material for vine
        materialVine = bpy.data.materials.new(name="VineMaterial")
        materialVine.use_nodes = True
        bsdf = materialVine.node_tree.nodes["Principled BSDF"]
        bsdf.inputs["Base Color"].default_value = (0.171, 0.095, 0.04, 1)  
        bsdf.inputs["Roughness"].default_value = 0.8
        curve_data.materials.append(materialVine)
        allGeneratedFlowers = []
        allGeneratedLeafs = []

        for branchNum, (currentBranch, depth) in enumerate(totalBranches):
            if len(currentBranch) < 2:
                continue
            #line = curve_data.splines.new('POLY')
            line = curve_data.splines.new('NURBS')
            line.use_endpoint_u = True
            line.points.add(len(currentBranch) - 1)
            globalDepth = depth +1
            randomTurnDirX = random.choice([-1,1])
            randomTurnDirY = random.choice([-1,1])

            for i, point in enumerate(currentBranch):
                x = point[0]
                y = point[1]
                z = point[2]
                line.points[i].co = (point[0], point[1], point[2], 1.0)
                #creates a natural thickness
                #better to do it here then rewrite a bucnh of c++ code
                #need to update this here to maek each new branch start at thickness of branch its tapering off from
                #need to now update it so taht there is no way it can be these impossibly skinny lines
                #taperRatio = min((globalDepth/minDepth), (globalDepth/maxDepth)) 
                taperRatio = min((1.0), (globalDepth / maxDepth))
                taper = abs(scene_props.thickness  * (1.0 - (taperRatio*.8)))
                pointsEnd = currentBranch.__len__() - i -1
                flowerLeafTaper = taper
                #if multiple bracnhes start at origin, need to both start tapered
                disFromOr = math.sqrt(currentBranch[0][0]**2 + currentBranch[0][1]**2 + currentBranch[0][2]**2)

                #first if ensures the bracnh starts off small and grows, swcond makes ure each new brean does the same
                if (branchNum == 0 and i < 20) or (disFromOr < .0001 and i < 20):
                    taper *= rootTapersNums[i]
                elif branchNum > 0 and i < 10:
                    taper *= (((i+1)/20.0) +.5)
                    
                if pointsEnd < 20:
                    if pointsEnd == 0:
                        taper = 0.1    
                    taper *= (pointsEnd / 20.0)
                #facing a major problem here where the end has insane zig zag
                #fixed
                if pointsEnd < 10:
                    pointEndStart = (((10-pointsEnd)/10)  ** 2) * .6
                    #what this should do is slowly turn it until pi/2
                    x += pointEndStart * randomTurnDirX
                    y += pointEndStart * randomTurnDirY
                    z -= (0.05 * pointEndStart)

               
                number = (random.random() * 0.1) - 0.05
                    
                line.points[i].co = (x,y,z,1.0)
                line.points[i].radius = max(0.1, taper)
                
                

                if leaf and i %15 == 0 and i != 0 and i < len(currentBranch)-1 and scene_props.leafProp:
                    pointLast = currentBranch[i-1]
                    pointNow = currentBranch[i]

                    direction = mathutils.Vector((pointNow[0] - pointLast[0], pointNow[1] - pointLast[1], pointNow[2] - pointLast[2]))
                    newLeaf = leaf.copy()
                    newLeaf.rotation_euler = (x,y,z)
                    if (len(allGeneratedLeafs) == 0):
                        newLeaf.data = leaf.data.copy()

                    newLeaf.location = (x,y,z)
                    newLeaf.scale = (flowerLeafTaper*3, flowerLeafTaper*3, flowerLeafTaper*3)
                    if direction.length > 0.001:
                        newLeaf.rotation_euler = direction.to_track_quat('Z', 'Y').to_euler()
                    newLeaf.hide_set(False)
                    newLeaf.hide_render = False 
                    newLeaf.hide_viewport = False
                    vineCol.objects.link(newLeaf)
                    allGeneratedLeafs.append(newLeaf)
                    
                if flower and i == len(currentBranch)-1 and len(currentBranch) > 2 and scene_props.flowerProp:
                    #need direction to make look normal
                    pointLast = currentBranch[i-1]
                    pointNow = currentBranch[i]

                    direction = mathutils.Vector((pointNow[0] - pointLast[0], pointNow[1] - pointLast[1], pointNow[2] - pointLast[2]))
                    newFlower = flower.copy()
                    newFlower.rotation_euler = (x,y,z)
                    if len(allGeneratedFlowers) == 0:
                        newFlower.data = flower.data.copy()
                    
                    newFlower.location = (x,y,z)
                    newFlower.scale = (flowerLeafTaper*2, flowerLeafTaper*2, flowerLeafTaper*2)
                    if direction.length > 0.001:
                        newFlower.rotation_euler = direction.to_track_quat('Z', 'Y').to_euler()
                    newFlower.hide_set(False)
                    newFlower.hide_render = False
                    newFlower.hide_viewport = False
                    vineCol.objects.link(newFlower)
                    allGeneratedFlowers.append(newFlower)

                    
                
                  

        #combinew all the flowers into one object       
        if len(allGeneratedFlowers) > 1 and scene_props.flowerProp:
            bpy.ops.object.select_all(action='DESELECT')
            for flower in allGeneratedFlowers:
                flower.select_set(True)
            bpy.context.view_layer.objects.active = allGeneratedFlowers[0]
            bpy.ops.object.join()

            allGeneratedFlowers[0].name = "All Flowers"
            allGeneratedFlowers[0].hide_set(False)
            allGeneratedFlowers[0].hide_render = False
            allGeneratedFlowers[0].hide_viewport = False
            bpy.context.view_layer.update()

        if len(allGeneratedLeafs) > 1 and scene_props.leafProp:
            bpy.ops.object.select_all(action='DESELECT')
            for leaf in allGeneratedLeafs:
                leaf.select_set(True)
            bpy.context.view_layer.objects.active = allGeneratedLeafs[0]
            bpy.ops.object.join()
            
            allGeneratedLeafs[0].name = "All Leafs"
            allGeneratedLeafs[0].hide_set(False)
            allGeneratedLeafs[0].hide_render = False
            allGeneratedLeafs[0].hide_viewport = False
            bpy.context.view_layer.update()
            bpy.ops.object.select_all(action='DESELECT')


        
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
        layout.prop(scene_props, "flowerProp")
        layout.prop(scene_props, "leafProp")

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