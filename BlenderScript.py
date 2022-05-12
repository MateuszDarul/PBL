# WORLD EDITOR
import bpy
import os
import math

filename = "Tests.map"

# Atrybuty obiektow (dodawac w nazwie)
    # _UN - (Use Name) - zapisuje nazwe obiektu
    # _NF - (No Frustum) - wylacza frustum culling dla obiektu
    # _NC - (No Collider) - wylacza kolizje dla obiektu
    # _Inst - (Instancing) - wczytuje jako modeling instancionowany
    
# Struktura zmiennej ObjMap (Musi byc posortowana nazwami od najdluzszej do najkrotszej)
#( [Nazwa obiektu z blendera, Nazwa modelu w pliku resources, [rozmiar kolizji xyz/r]] )
ObjMap = [
    ["PodlogaMini","floorMin/floorMin",[10.0,1.0,1.0]],
    ["Podloga","floor/floor",[30.0,1.0,25.0]],
    ["Sciana_1","wall/wall",[1.0,5.0,5.0]],
    ["xCube","Crate/Crate",[2.0,2.0,2.0]],
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
    f.write(("END\n"))
    i += j

f.close()
print("File created: ", os.getcwd(), "\\", filename, sep="")

