import bpy
import os
import math

filename = "world.map"

def GetModelName(string):
    result = "..."
    if string.startswith("Podloga"):
        result = "floor/floor"
    elif string.startswith("Sciana_1"):
        result = "wall/wall"
    return result

def GetCollider(object, rotation):
    result = ""
    if object.name.startswith("Podloga"):
        result += "0\n"
        size = [40.0,1.0,40.0]
    elif object.name.startswith("Sciana_1"):
        result += "0\n"
        size = [1.0,5.0,5.0]
    if rotation[1] in [-270, -90, 90, 270]:
        a = size[0]
        size[0] = size[2]
        size[2] = a
    result += str(size[0]) + "\n"
    result += str(size[1]) + "\n"
    result += str(size[2])
    return result

f = open(filename, "w")

for ob in bpy.data.objects:
    if(ob.hide_get()):
        continue
    f.write(("NEW\n"))
    if "_UseName" in ob.name or "_UN" in ob.name:
        f.write(("Name:\n"))
        f.write((ob.name + "\n"))
    f.write(("Path:\n"))
    f.write((GetModelName(ob.name) + "\n"))
    f.write(("Transformations:\n"))
    f.write((str(round(-ob.location.x, 3)) + "\n"))
    f.write((str(round(ob.location.z, 3)) + "\n"))
    f.write((str(round(ob.location.y, 3)) + "\n"))
    rotation = [0,0,0]
    rotation[0] = math.fmod(round(ob.rotation_euler.x * (180 / math.pi), 0), 360)
    rotation[1] = math.fmod(round(ob.rotation_euler.z * (180 / math.pi), 0), 360)
    rotation[2] = math.fmod(round(ob.rotation_euler.y * (180 / math.pi), 0), 360)
    f.write((str(rotation[0]) + "\n"))
    f.write((str(rotation[1]) + "\n"))
    f.write((str(rotation[2]) + "\n"))
    if "_NoCol" not in ob.name or "_NC" not in ob.name:
        f.write(("Collider:\n"))
        f.write((GetCollider(ob, rotation) + "\n"))
    f.write(("END\n"))

f.close()
print("File created: ", os.getcwd(), "\\", filename, sep="")

