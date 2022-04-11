import bpy

def GetModelName(string):
    result = "..."
    if string.startswith("Box"):
        result = "AAA/AAA"
    elif string.startswith("Cube"):
        result = "NWM/NWM"
    return result

f = open("world.map", "w")


for ob in D.objects:
    f.write(("NEW\n"))
    f.write(("Name:\n" + ob.name + "\n"))
    f.write(("Path:\n"))
    f.write((GetModelName(ob.name) + "\n"))
    f.write(("Position:\n"))
    f.write((str(ob.location.x) + "\n"))
    f.write((str(ob.location.z) + "\n"))
    f.write((str(ob.location.y) + "\n"))
    f.write(("Rotation:\n"))
    f.write((str(ob.rotation_euler.x) + "\n"))
    f.write((str(ob.rotation_euler.z) + "\n"))
    f.write((str(ob.rotation_euler.y) + "\n"))
    f.write(("END\n"))
    

f.close()