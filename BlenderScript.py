# WORLD EDITOR
import bpy
import os
import math

filename = "world.map"
modlesDir = "Exported\\"
workingDir = "C:\\Users\\MDarul\\Desktop\\swiat"

os.chdir(workingDir)

def ExportAssets(file_path="Exported\\"):
    for collection in bpy.data.collections:
        if collection.name == "Assets":
            for obj in collection.all_objects:
                bpy.context.view_layer.objects.active = obj
                obj.hide_set(False)
                obj.select_set(True)
                bpy.ops.export_scene.obj(filepath=file_path+obj.name+".obj", use_triangles=True, use_selection=True, axis_forward='Z')
                obj.select_set(False)
                obj.hide_set(True)
                
ExportAssets(modlesDir)

# Specialne nazwy obiektow
    # PointLight - dodaje siwatlo punktowe
    # SpotLight - dodaje swiatlo reflektorowe

# Atrybuty obiektow (dodawac w nazwie)
    # _UN - (Use Name) - zapisuje nazwe obiektu
    # _NF - (No Frustum) - wylacza frustum culling dla obiektu
    # _NS - (No Shade) - wylacza rzucanie cieni przez model
    # _NC - (No Collider) - wylacza kolizje dla obiektu
    # _Inst - (Instancing) - wczytuje jako modeling instancionowany
    
# Struktura zmiennej ObjMap (Musi byc posortowana nazwami od najdluzszej do najkrotszej)
#( [Nazwa obiektu z blendera, Nazwa modelu w pliku resources, [rozmiar kolizji xyz/r]] )
ObjMap = [
    ["Listwa.NR1","Exported/Listwa.NR1",[7.0,0.5,1.0]],
    ["Listwa.NR2","Exported/Listwa.NR2",[6.0,0.5,1.0]],
    ["Listwa.NR3","Exported/Listwa.NR3",[1.0,0.5,15.0]],
    ["Listwa.NR4","Exported/Listwa.NR4",[5.0,0.5,1.0]],
    ["Listwa.NR5","Exported/Listwa.NR5",[1.0,0.5,8.0]],
    ["Listwa.NR6","Exported/Listwa.NR6",[2.0,0.5,1.0]],
    ["Naroznik.NR1","Exported/Naroznik.NR1",[1.0,6.0,1.0]],
    ["Naroznik.NR2","Exported/Naroznik.NR2",[1.0,0.5,1.0]],
    ["Podloga.NR10","Exported/Podloga.NR10",[10.0,0.5,10.0]],
    ["Podloga.NR11","Exported/Podloga.NR11",[9.0,0.5,10.0]],
    ["Podloga.NR12","Exported/Podloga.NR12",[17.0,0.5,14.0]],
    ["Podloga.NR13","Exported/Podloga.NR13",[5.0,0.5,5.0]],
    ["Podloga.NR1","Exported/Podloga.NR1",[40.0,0.5,50.0]],
    ["Podloga.NR2","Exported/Podloga.NR2",[7.0,0.5,8.0]],
    ["Podloga.NR3","Exported/Podloga.NR3",[10.0,0.5,20.0]],
    ["Podloga.NR4","Exported/Podloga.NR4",[6.0,0.5,6.0]],
    ["Podloga.NR5","Exported/Podloga.NR5",[6.0,0.5,4.0]],
    ["Podloga.NR6","Exported/Podloga.NR6",[6.0,0.5,10.0]],
    ["Podloga.NR7","Exported/Podloga.NR7",[5.0,0.5,6.0]],
    ["Podloga.NR8","Exported/Podloga.NR8",[7.0,0.5,3.0]],
    ["Podloga.NR9","Exported/Podloga.NR9",[15.0,0.5,15.0]],
    ["Schodek.NR1","Exported/Schodek.NR1",[5.25,0.5,2.0]],
    ["Sufit.NR1","Exported/Sufit.NR1",[10.0,0.5,20.0]],
    ["Sufit.NR2","Exported/Sufit.NR2",[7.0,0.5,8.0]],
    ["Sufit.NR3","Exported/Sufit.NR3",[40.0,0.5,15.0]],
    ["Sufit.NR4","Exported/Sufit.NR4",[40.0,0.5,56.0]],
    ["Wall.NR1","Exported/Wall.NR1",[1.0,6.0,10.0]],
    ["Wall.NR2","Exported/Wall.NR2",[1.0,6.0,5.0]],
    ["Wall.NR3","Exported/Wall.NR3",[1.0,2.0,10.0]],
    ["Wall.NR4","Exported/Wall.NR4",[1.0,6.0,8.0]],
    ["Wall.NR5","Exported/Wall.NR5",[2.0,2.0,1.0]],
    ["Wall.NR6","Exported/Wall.NR6",[2.0,1.0,1.0]],
]
#Cube example ["xCube","xCube/xCube",[1.0,1.0,1.0]],
#Sphere example ["xSphere","xSphere/xSphere",[1.0]],
        
def GetModelName(string):
    result = "..."
    for i in range(len(ObjMap)):
        if string.startswith(ObjMap[i][0]):
            result = ObjMap[i][1]
            break;
    return result

def GetCollider(string, rotation):
    result = ""
    for i in range(len(ObjMap)):
        if string.startswith(ObjMap[i][0]):
            if len(ObjMap[i][2]) == 1:
                result += str(1) + "\n"
                size = 0.0
                size = ObjMap[i][2][0]
                result += str(size[0])
                break
            elif len(ObjMap[i][2]) == 3:
                result += str(0) + "\n"
                size = [0.0,0.0,0.0]
                size[0] = ObjMap[i][2][0]
                size[1] = ObjMap[i][2][1]
                size[2] = ObjMap[i][2][2]
                if rotation[1] in [-270, -90, 90, 270]:
                    a = size[0]
                    size[0] = size[2]
                    size[2] = a
                result += str(size[0]) + "\n"
                result += str(size[1]) + "\n"
                result += str(size[2])
                break
    return result

f = open(filename, "w")

all_elements = len(bpy.data.objects)
i = 0
while i < all_elements:
    if(bpy.data.objects[i].hide_get()):
        i += 1
        continue
    if "PointLight" in bpy.data.objects[i].name or "SpotLight" in bpy.data.objects[i].name:
        f.write(("NEW\n"))
        f.write(("Name:\n"))
        f.write((bpy.data.objects[i].name + "\n"))
        f.write(("Light:\n"))
        if "PointLight" in bpy.data.objects[i].name:
            f.write(("Point\n"))
        elif "SpotLight" in bpy.data.objects[i].name:
            f.write(("Spot\n"))
        f.write(("Position:\n"))
        f.write((str(round(-bpy.data.objects[i].location.x, 3)) + "\n"))
        f.write((str(round(bpy.data.objects[i].location.z, 3)) + "\n"))
        f.write((str(round(bpy.data.objects[i].location.y, 3)) + "\n"))
        f.write(("Color:\n"))
        f.write((str(round(bpy.data.objects[i].color[0], 3)) + "\n"))
        f.write((str(round(bpy.data.objects[i].color[1], 3)) + "\n"))
        f.write((str(round(bpy.data.objects[i].color[2], 3)) + "\n"))
        f.write(("SpecularColor:\n"))
        f.write((str(round(bpy.data.objects[i].color[0], 3)) + "\n"))
        f.write((str(round(bpy.data.objects[i].color[1], 3)) + "\n"))
        f.write((str(round(bpy.data.objects[i].color[2], 3)) + "\n"))
        f.write(("Range:\n"))
        f.write((str(10) + "\n"))
        if "SpotLight" in bpy.data.objects[i].name:
            f.write(("CutOff:\n"))
            f.write(("12.5" + "\n"))
            f.write(("45" + "\n"))
            f.write(("Rotation:\n"))
            rotation = [0,0,0]
            rotation[0] = math.fmod(round(bpy.data.objects[i].rotation_euler.x * (180 / math.pi), 0), 360)
            rotation[1] = math.fmod(round(bpy.data.objects[i].rotation_euler.z * (180 / math.pi), 0), 360)
            rotation[2] = math.fmod(round(bpy.data.objects[i].rotation_euler.y * (180 / math.pi), 0), 360)
            f.write((str(-rotation[0]) + "\n"))
            f.write((str(rotation[1]) + "\n"))
            f.write((str(rotation[2]) + "\n"))
        f.write(("END\n"))
        i += 1
        continue
    f.write(("NEW\n"))
    if "_UN" in bpy.data.objects[i].name:
        f.write(("Name:\n"))
        f.write((bpy.data.objects[i].name + "\n"))
    if "_Inst" not in bpy.data.objects[i].name:
        f.write(("Model:\n"))
    else:
        f.write(("Inst:\n"))
    f.write((GetModelName(bpy.data.objects[i].name) + "\n"))
    if "_Inst" not in bpy.data.objects[i].name:
        if "_NF" in bpy.data.objects[i].name:
            f.write(("0" + "\n"))
        else:
            f.write(("1" + "\n"))
    j = 1
    if i+j < all_elements and "_Inst" in bpy.data.objects[i].name:
        while bpy.data.objects[i].name[:-4] == bpy.data.objects[i+j].name[:-4]:
            if i+j == all_elements-1:
                break
            j += 1
    if j > 1:
        f.write((str(j) + "\n"))
    for k in range(j):
        f.write(("Transformations:\n"))
        f.write((str(round(-bpy.data.objects[i+k].location.x, 3)) + "\n"))
        f.write((str(round(bpy.data.objects[i+k].location.z, 3)) + "\n"))
        f.write((str(round(bpy.data.objects[i+k].location.y, 3)) + "\n"))
        rotation = [0,0,0]
        rotation[0] = math.fmod(round(bpy.data.objects[i+k].rotation_euler.x * (180 / math.pi), 0), 360)
        rotation[1] = math.fmod(round(bpy.data.objects[i+k].rotation_euler.z * (180 / math.pi), 0), 360)
        rotation[2] = math.fmod(round(bpy.data.objects[i+k].rotation_euler.y * (180 / math.pi), 0), 360)
        f.write((str(rotation[0]) + "\n"))
        f.write((str(rotation[1]) + "\n"))
        f.write((str(rotation[2]) + "\n"))
    if "_Inst" not in bpy.data.objects[i].name and "_NC" not in bpy.data.objects[i].name:
        f.write(("Collider:\n"))
        f.write((GetCollider(bpy.data.objects[i].name, rotation) + "\n"))
    f.write(("Shade:\n"))
    if "_NS" in bpy.data.objects[i].name:
        f.write(("0" + "\n"))
    else:
        if "_Inst" in bpy.data.objects[i].name:
            f.write(("0" + "\n"))
        else:
            f.write(("1" + "\n"))
    f.write(("END\n"))
    i += j

f.close()
print("File created: ", os.getcwd(), "\\", filename, sep="")


