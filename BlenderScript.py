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

f = open(filename, "w")


for ob in bpy.data.objects:
    if(ob.hide_get()):
        continue
    f.write(("NEW\n"))
    f.write(("Name:\n"))
    f.write((ob.name + "\n"))
    f.write(("Path:\n"))
    f.write((GetModelName(ob.name) + "\n"))
    f.write(("Transformations:\n"))
    f.write((str(round(-ob.location.x, 3)) + "\n"))
    f.write((str(round(ob.location.z, 3)) + "\n"))
    f.write((str(round(ob.location.y, 3)) + "\n"))
    f.write((str(round(ob.rotation_euler.x * (180 / math.pi), 0)) + "\n"))
    f.write((str(round(ob.rotation_euler.z * (180 / math.pi), 0)) + "\n"))
    f.write((str(round(ob.rotation_euler.y * (180 / math.pi), 0)) + "\n"))
    f.write(("END\n"))
    

f.close()
print("File location: ", os.getcwd(), filename)

