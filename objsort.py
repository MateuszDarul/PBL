from math import sqrt

file_to_open = input("Plik wejsciowy: ")
file_to_save = input("Plik wynikowy: ")

camera_position = (-10.0, 5.5, 0.0)

vertices = []
faces = {}

def get_midpoint(v1, v2, v3):
    x = (vertices[v1][0] + vertices[v2][0] + vertices[v3][0]) / 3.0
    y = (vertices[v1][1] + vertices[v2][1] + vertices[v3][1]) / 3.0
    z = (vertices[v1][2] + vertices[v2][2] + vertices[v3][2]) / 3.0
    return (x, y, z)

def get_distance(p1, p2):
    x = p1[0] - p2[0]
    y = p1[1] - p2[1]
    z = p1[2] - p2[2]
    return sqrt(x*x + y*y + z*z)


with open(file_to_open, 'r') as file_in:
    file_out = open(file_to_save, 'w')
    while line := file_in.readline():
        
        tokens = line.split()
        if tokens[0] == 'v':
            vertices.append((float(tokens[1]), float(tokens[2]), float(tokens[3])))

        if tokens[0] != 'f':
            file_out.write(line)
        else:
            v1 = int(tokens[1].split(sep='/')[0]) - 1
            v2 = int(tokens[2].split(sep='/')[0]) - 1
            v3 = int(tokens[3].split(sep='/')[0]) - 1

            midpoint = get_midpoint(v1, v2, v3)
            faces[get_distance(camera_position, midpoint)] = line
    
    sorted_faces = {key: val for key, val in sorted(faces.items(), key = lambda ele: ele[0])}
    for dist, string in sorted_faces.items():
        file_out.write(string)





