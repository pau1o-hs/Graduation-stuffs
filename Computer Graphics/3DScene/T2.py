import glfw
from OpenGL.GL import *
import OpenGL.GL.shaders
import numpy as np
import glm
import math
from PIL import Image

glfw.init()
glfw.window_hint(glfw.VISIBLE, glfw.FALSE);
altura = 1200
largura = 1600
window = glfw.create_window(largura, altura, "Malhas e Texturas", None, None)
glfw.make_context_current(window)

vertex_code = """
        attribute vec3 position;
        attribute vec2 texture_coord;
        varying vec2 out_texture;
                
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;        
        
        void main(){
            gl_Position = projection * view * model * vec4(position,1.0);
            out_texture = vec2(texture_coord);
        }
        """
        
fragment_code = """
        uniform vec4 color;
        varying vec2 out_texture;
        uniform sampler2D samplerTexture;
        
        void main(){
            vec4 texture = texture2D(samplerTexture, out_texture);
            gl_FragColor = texture;
        }
        """

# Request a program and shader slots from GPU
program  = glCreateProgram()
vertex   = glCreateShader(GL_VERTEX_SHADER)
fragment = glCreateShader(GL_FRAGMENT_SHADER)

# Set shaders source
glShaderSource(vertex, vertex_code)
glShaderSource(fragment, fragment_code)

# Compile shaders
glCompileShader(vertex)
if not glGetShaderiv(vertex, GL_COMPILE_STATUS):
    error = glGetShaderInfoLog(vertex).decode()
    print(error)
    raise RuntimeError("Erro de compilacao do Vertex Shader")

glCompileShader(fragment)
if not glGetShaderiv(fragment, GL_COMPILE_STATUS):
    error = glGetShaderInfoLog(fragment).decode()
    print(error)
    raise RuntimeError("Erro de compilacao do Fragment Shader")

# Attach shader objects to the program
glAttachShader(program, vertex)
glAttachShader(program, fragment)

# Build program
glLinkProgram(program)
if not glGetProgramiv(program, GL_LINK_STATUS):
    print(glGetProgramInfoLog(program))
    raise RuntimeError('Linking error')
    
# Make program the default program
glUseProgram(program)

def load_model_from_file(filename):
    """Loads a Wavefront OBJ file. """
    objects = {}
    vertices = []
    texture_coords = []
    faces = []

    material = None

    # abre o arquivo obj para leitura
    for line in open(filename, "r"): ## para cada linha do arquivo .obj
        if line.startswith('#'): continue ## ignora comentarios
        values = line.split() # quebra a linha por espaço
        if not values: continue


        ### recuperando vertices
        if values[0] == 'v':
            vertices.append(values[1:4])


        ### recuperando coordenadas de textura
        elif values[0] == 'vt':
            texture_coords.append(values[1:3])

        ### recuperando faces 
        elif values[0] in ('usemtl', 'usemat'):
            material = values[1]
        elif values[0] == 'f':
            face = []
            face_texture = []
            for v in values[1:]:
                w = v.split('/')
                face.append(int(w[0]))
                if len(w) >= 2 and len(w[1]) > 0:
                    face_texture.append(int(w[1]))
                else:
                    face_texture.append(0)

            faces.append((face, face_texture, material))

    model = {}
    model['vertices'] = vertices
    model['texture'] = texture_coords
    model['faces'] = faces

    return model

glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE)
glEnable( GL_BLEND )
glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA )
glEnable(GL_LINE_SMOOTH)
glEnable(GL_TEXTURE_2D)
qtd_texturas = 15
textures = glGenTextures(qtd_texturas)

def load_texture_from_file(texture_id, img_textura):
    glBindTexture(GL_TEXTURE_2D, texture_id)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    img = Image.open(img_textura)
    print(img_textura,img.mode)
    img_width = img.size[0]
    img_height = img.size[1]
    #image_data = img.tobytes("raw", "RGB", 0, -1)
    image_data = img.convert("RGBA").tobytes("raw", "RGBA",0,-1)

    #image_data = np.array(list(img.getdata()), np.uint8)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data)
    
vertices_list = []    
textures_coord_list = []

modelo = load_model_from_file('terrain/terrain.obj')

### inserindo vertices do modelo no vetor de vertices
print('Processando modelo terrain.obj. Vertice inicial:',len(vertices_list))
for face in modelo['faces']:
    for vertice_id in face[0]:
        vertices_list.append( modelo['vertices'][vertice_id-1] )
    for texture_id in face[1]:
        textures_coord_list.append( modelo['texture'][texture_id-1] )
print('Processando modelo terrain.obj. Vertice final:',len(vertices_list))

### inserindo coordenadas de textura do modelo no vetor de texturas


### carregando textura equivalente e definindo um id (buffer): use um id por textura!
load_texture_from_file(0,'terrain/terrain.png')

modelo = load_model_from_file('skybox/skybox.obj')

### inserindo vertices do modelo no vetor de vertices
print('Processando modelo skybox.obj. Vertice inicial:',len(vertices_list))
faces_visited = []
for face in modelo['faces']:
	if face[2] not in faces_visited:
		print(face[2],' vertice inicial =',len(vertices_list))
		faces_visited.append(face[2])
	for vertice_id in face[0]:
		vertices_list.append( modelo['vertices'][vertice_id-1] )
	for texture_id in face[1]:
		textures_coord_list.append( modelo['texture'][texture_id-1] )
print('Processando modelo skybox.obj. Vertice final:',len(vertices_list))

### inserindo coordenadas de textura do modelo no vetor de texturas

### carregando textura equivalente e definindo um id (buffer): use um id por textura!
load_texture_from_file(1,'skybox/skybox.png')

modelo = load_model_from_file('house/house.obj')

### inserindo vertices do modelo no vetor de vertices
print('Processando modelo house.obj. Vertice inicial:',len(vertices_list))
faces_visited = []
for face in modelo['faces']:
	if face[2] not in faces_visited:
		print(face[2],' vertice inicial =',len(vertices_list))
		faces_visited.append(face[2])
	for vertice_id in face[0]:
		vertices_list.append( modelo['vertices'][vertice_id-1] )
	for texture_id in face[1]:
		textures_coord_list.append( modelo['texture'][texture_id-1] )
print('Processando modelo house.obj. Vertice final:',len(vertices_list))

### inserindo coordenadas de textura do modelo no vetor de texturas

### carregando textura equivalente e definindo um id (buffer): use um id por textura!
load_texture_from_file(2,'house/house.png')
load_texture_from_file(3,'house/grunt.png')
load_texture_from_file(4,'house/branch.png')
load_texture_from_file(5,'house/vane.tga')
load_texture_from_file(6,'house/grass.tga')

modelo = load_model_from_file('outside_objs/objs.obj')

### inserindo vertices do modelo no vetor de vertices
print('Processando modelo outside_objs.obj. Vertice inicial:',len(vertices_list))
faces_visited = []
for face in modelo['faces']:
	if face[2] not in faces_visited:
		print(face[2],' vertice inicial =',len(vertices_list))
		faces_visited.append(face[2])
	for vertice_id in face[0]:
		vertices_list.append( modelo['vertices'][vertice_id-1] )
	for texture_id in face[1]:
		textures_coord_list.append( modelo['texture'][texture_id-1] )
print('Processando modelo outside_objs.obj. Vertice final:',len(vertices_list))

### inserindo coordenadas de textura do modelo no vetor de texturas

### carregando textura equivalente e definindo um id (buffer): use um id por textura!
load_texture_from_file(7,'outside_objs/cart1.png')
load_texture_from_file(8,'outside_objs/cart2.png')
load_texture_from_file(9,'outside_objs/carrot.tga')

modelo = load_model_from_file('bunny/bunny.obj')

### inserindo vertices do modelo no vetor de vertices
print('Processando modelo bunny.obj. Vertice inicial:',len(vertices_list))
faces_visited = []
for face in modelo['faces']:
	if face[2] not in faces_visited:
		print(face[2],' vertice inicial =',len(vertices_list))
		faces_visited.append(face[2])
	for vertice_id in face[0]:
		vertices_list.append( modelo['vertices'][vertice_id-1] )
	for texture_id in face[1]:
		textures_coord_list.append( modelo['texture'][texture_id-1] )
print('Processando modelo bunny.obj. Vertice final:',len(vertices_list))

### inserindo coordenadas de textura do modelo no vetor de texturas

### carregando textura equivalente e definindo um id (buffer): use um id por textura!
load_texture_from_file(10,'bunny/bunny.png')

modelo = load_model_from_file('inside_objs/objs.obj')

### inserindo vertices do modelo no vetor de vertices
print('Processando modelo inside_objs.obj. Vertice inicial:',len(vertices_list))
faces_visited = []
for face in modelo['faces']:
	if face[2] not in faces_visited:
		print(face[2],' vertice inicial =',len(vertices_list))
		faces_visited.append(face[2])
	for vertice_id in face[0]:
		vertices_list.append( modelo['vertices'][vertice_id-1] )
	for texture_id in face[1]:
		textures_coord_list.append( modelo['texture'][texture_id-1] )
print('Processando modelo inside_objs.obj. Vertice final:',len(vertices_list))

### inserindo coordenadas de textura do modelo no vetor de texturas

### carregando textura equivalente e definindo um id (buffer): use um id por textura!
load_texture_from_file(11,'inside_objs/floor.jpg')
load_texture_from_file(12,'inside_objs/table.tga')
load_texture_from_file(13,'inside_objs/stove.png')
load_texture_from_file(14,'inside_objs/chair.jpg')

modelo = load_model_from_file('house/vane.obj')

### inserindo vertices do modelo no vetor de vertices
print('Processando modelo vane.obj. Vertice inicial:',len(vertices_list))
faces_visited = []
for face in modelo['faces']:
	if face[2] not in faces_visited:
		print(face[2],' vertice inicial =',len(vertices_list))
		faces_visited.append(face[2])
	for vertice_id in face[0]:
		vertices_list.append( modelo['vertices'][vertice_id-1] )
	for texture_id in face[1]:
		textures_coord_list.append( modelo['texture'][texture_id-1] )
print('Processando modelo vane.obj. Vertice final:',len(vertices_list))

### inserindo coordenadas de textura do modelo no vetor de texturas


# Request a buffer slot from GPU
buffer = glGenBuffers(2)

vertices = np.zeros(len(vertices_list), [("position", np.float32, 3)])
vertices['position'] = vertices_list


# Upload data
glBindBuffer(GL_ARRAY_BUFFER, buffer[0])
glBufferData(GL_ARRAY_BUFFER, vertices.nbytes, vertices, GL_STATIC_DRAW)
stride = vertices.strides[0]
offset = ctypes.c_void_p(0)
loc_vertices = glGetAttribLocation(program, "position")
glEnableVertexAttribArray(loc_vertices)
glVertexAttribPointer(loc_vertices, 3, GL_FLOAT, False, stride, offset)

textures = np.zeros(len(textures_coord_list), [("position", np.float32, 2)]) # duas coordenadas
textures['position'] = textures_coord_list

# Upload data
glBindBuffer(GL_ARRAY_BUFFER, buffer[1])
glBufferData(GL_ARRAY_BUFFER, textures.nbytes, textures, GL_STATIC_DRAW)
stride = textures.strides[0]
offset = ctypes.c_void_p(0)
loc_texture_coord = glGetAttribLocation(program, "texture_coord")
glEnableVertexAttribArray(loc_texture_coord)
glVertexAttribPointer(loc_texture_coord, 2, GL_FLOAT, False, stride, offset)

def desenha_terreno():
    
    
    # aplica a matriz model
    
    # rotacao
    angle = 0.0;
    r_x = 0.0; r_y = 0.0; r_z = 0.1;
    
    # translacao
    t_x = 0.0; t_y = 0.0; t_z = 0.0;
    
    # escala
    s_x = 1.0; s_y = 1.0; s_z = 1.0;
    
    mat_model = model(angle, r_x, r_y, r_z, t_x, t_y, t_z, s_x, s_y, s_z)
    loc_model = glGetUniformLocation(program, "model")
    glUniformMatrix4fv(loc_model, 1, GL_TRUE, mat_model)
       
    #define id da textura do modelo
    glBindTexture(GL_TEXTURE_2D, 0)
    
    
    # desenha o modelo
    glDrawArrays(GL_TRIANGLES, 0, 6) ## renderizando
    
def desenha_ceu():
    
    # aplica a matriz model
    
    # rotacao
    angle = 0.0;
    r_x = 0.0; r_y = 0.0; r_z = 0.1;
    
    # translacao
    t_x = 0.0; t_y = 0.0; t_z = 0.0;
    
    # escala
    s_x = 1.0; s_y = 1.0; s_z = 1.0;
    
    mat_model = model(angle, r_x, r_y, r_z, t_x, t_y, t_z, s_x, s_y, s_z)
    loc_model = glGetUniformLocation(program, "model")
    glUniformMatrix4fv(loc_model, 1, GL_TRUE, mat_model)
       
    #define id da textura do modelo
    glBindTexture(GL_TEXTURE_2D, 1)
    
    # desenha o modelo
    glDrawArrays(GL_TRIANGLES, 6, 42-6) ## renderizando
      
def desenha_casa():
    
    # aplica a matriz model
    
    # rotacao
    angle = 0.0;
    r_x = 0.0; r_y = 0.0; r_z = 0.1;
    
    # translacao
    t_x = 0.0; t_y = 0.0; t_z = 0.0;
    
    # escala
    s_x = 1.0; s_y = 1.0; s_z = 1.0;
    
    mat_model = model(angle, r_x, r_y, r_z, t_x, t_y, t_z, s_x, s_y, s_z)
    loc_model = glGetUniformLocation(program, "model")
    glUniformMatrix4fv(loc_model, 1, GL_TRUE, mat_model)
       
    #define id da textura do modelo
    glBindTexture(GL_TEXTURE_2D, 2)
    glDrawArrays(GL_TRIANGLES, 42, 15180-42) ## renderizando

	#define id da textura do modelo
    glBindTexture(GL_TEXTURE_2D, 3)
    glDrawArrays(GL_TRIANGLES, 15180, 16395-15180) ## renderizando

	#define id da textura do modelo
    glBindTexture(GL_TEXTURE_2D, 4)
    glDrawArrays(GL_TRIANGLES, 16395, 17079-16395) ## renderizando

	# #define id da textura do modelo
    # glBindTexture(GL_TEXTURE_2D, 5)
    # glDrawArrays(GL_TRIANGLES, 17079, 17091-17079) ## renderizando

	#define id da textura do modelo
    glBindTexture(GL_TEXTURE_2D, 6)
    glDrawArrays(GL_TRIANGLES, 17091, 17979-17091) ## renderizando

def desenha_exterior():
    
    # aplica a matriz model
    
    # rotacao
    angle = 0.0;
    r_x = 0.0; r_y = 0.0; r_z = 0.1;
    
    # translacao
    t_x = 0.0; t_y = 0.0; t_z = 0.0;
    
    # escala
    s_x = 1.0; s_y = 1.0; s_z = 1.0;
    
    mat_model = model(angle, r_x, r_y, r_z, t_x, t_y, t_z, s_x, s_y, s_z)
    loc_model = glGetUniformLocation(program, "model")
    glUniformMatrix4fv(loc_model, 1, GL_TRUE, mat_model)
       
    #define id da textura do modelo
    glBindTexture(GL_TEXTURE_2D, 7)
    glDrawArrays(GL_TRIANGLES, 17979, 24369-17979) ## renderizando

	#define id da textura do modelo
    glBindTexture(GL_TEXTURE_2D, 8)
    glDrawArrays(GL_TRIANGLES, 24369, 33699-24369) ## renderizando

	#define id da textura do modelo
    glBindTexture(GL_TEXTURE_2D, 9)
    glDrawArrays(GL_TRIANGLES, 33699, 38883-33699) ## renderizando

def desenha_coelho():
    
    # aplica a matriz model
    
    # rotacao
    angle = 0.0;
    r_x = 0.0; r_y = 0.0; r_z = 0.1;
    
    # translacao
    t_x = 0.0; t_y = 0.0; t_z = 0.0;
    
    # escala
    s_x = 1.0; s_y = 1.0; s_z = 1.0;
    
    mat_model = model(angle, r_x, r_y, r_z, t_x, t_y, t_z, s_x, s_y, s_z)
    loc_model = glGetUniformLocation(program, "model")
    glUniformMatrix4fv(loc_model, 1, GL_TRUE, mat_model)
       
    #define id da textura do modelo
    glBindTexture(GL_TEXTURE_2D, 10)
    glDrawArrays(GL_TRIANGLES, 38883, 107565-38883) ## renderizando

def desenha_interior():
    
    # aplica a matriz model
    
    # rotacao
    angle = 0.0;
    r_x = 0.0; r_y = 0.0; r_z = 0.1;
    
    # translacao
    t_x = 0.0; t_y = 0.0; t_z = 0.0;
    
    # escala
    s_x = 1.0; s_y = 1.0; s_z = 1.0;
    
    mat_model = model(angle, r_x, r_y, r_z, t_x, t_y, t_z, s_x, s_y, s_z)
    loc_model = glGetUniformLocation(program, "model")
    glUniformMatrix4fv(loc_model, 1, GL_TRUE, mat_model)
       
    #define id da textura do modelo
    glBindTexture(GL_TEXTURE_2D, 11)
    glDrawArrays(GL_TRIANGLES, 107565, 107571-107565) ## renderizando

	#define id da textura do modelo
    glBindTexture(GL_TEXTURE_2D, 12)
    glDrawArrays(GL_TRIANGLES, 107571, 108351-107571) ## renderizando

	#define id da textura do modelo
    glBindTexture(GL_TEXTURE_2D, 13)
    glDrawArrays(GL_TRIANGLES, 108351, 112935-108351) ## renderizando

	#define id da textura do modelo
    glBindTexture(GL_TEXTURE_2D, 14)
    glDrawArrays(GL_TRIANGLES, 112935, 401559-112935) ## renderizando

def desenha_catavento(angle):
    
    # aplica a matriz model
    
    # rotacao
    # angle = 0.0;
    r_x = 0.0; r_y = 1.0; r_z = 0.0;
    
    # translacao
    t_x = -1.0; t_y = 28; t_z = -30.0;
    
    # escala
    s_x = 1.0; s_y = 1.0; s_z = 1.0;

    mat_model = model(angle, r_x, r_y, r_z, t_x, t_y, t_z, s_x, s_y, s_z)


    loc_model = glGetUniformLocation(program, "model")
    glUniformMatrix4fv(loc_model, 1, GL_TRUE, mat_model)
       
    #define id da textura do modelo
    glBindTexture(GL_TEXTURE_2D, 5)
    glDrawArrays(GL_TRIANGLES, 401559, 401565-401559) ## renderizando

cameraPos   = glm.vec3(70.0,  25.0,  100.0);
cameraFront = glm.vec3(0.0,  0.0, -1.0);
cameraUp    = glm.vec3(0.0,  1.0,  0.0);


polygonal_mode = False

def key_event(window,key,scancode,action,mods):
    global cameraPos, cameraFront, cameraUp, polygonal_mode
    
    cameraSpeed = 0.8
    if key == 87 and (action==1 or action==2) and (cameraPos + (cameraSpeed * cameraFront)).y > 2 and (cameraPos + (cameraSpeed * cameraFront)).y < 245: # tecla W
        cameraPos += cameraSpeed * cameraFront
    
    if key == 83 and (action==1 or action==2): # tecla S
        cameraPos -= cameraSpeed * cameraFront
    
    if key == 65 and (action==1 or action==2): # tecla A
        cameraPos -= glm.normalize(glm.cross(cameraFront, cameraUp)) * cameraSpeed
        
    if key == 68 and (action==1 or action==2): # tecla D
        cameraPos += glm.normalize(glm.cross(cameraFront, cameraUp)) * cameraSpeed
        
    if key == 80 and action==1 and polygonal_mode==True:
        polygonal_mode=False
    else:
        if key == 80 and action==1 and polygonal_mode==False:
            polygonal_mode=True
        
        
        
firstMouse = True
yaw = -90.0 
pitch = 0.0
lastX =  largura/2
lastY =  altura/2

def mouse_event(window, xpos, ypos):
    global firstMouse, cameraFront, yaw, pitch, lastX, lastY
    if firstMouse:
        lastX = xpos
        lastY = ypos
        firstMouse = False

    xoffset = xpos - lastX
    yoffset = lastY - ypos
    lastX = xpos
    lastY = ypos

    sensitivity = 0.3 
    xoffset *= sensitivity
    yoffset *= sensitivity

    yaw += xoffset;
    pitch += yoffset;

    
    if pitch >= 90.0: pitch = 90.0
    if pitch <= -90.0: pitch = -90.0

    front = glm.vec3()
    front.x = math.cos(glm.radians(yaw)) * math.cos(glm.radians(pitch))
    front.y = math.sin(glm.radians(pitch))
    front.z = math.sin(glm.radians(yaw)) * math.cos(glm.radians(pitch))
    cameraFront = glm.normalize(front)


    
glfw.set_key_callback(window,key_event)
glfw.set_cursor_pos_callback(window, mouse_event)

def model(angle, r_x, r_y, r_z, t_x, t_y, t_z, s_x, s_y, s_z):
    
    angle = math.radians(angle)
    
    matrix_transform = glm.mat4(1.0) # instanciando uma matriz identidade

    # aplicando translacao
    matrix_transform = glm.translate(matrix_transform, glm.vec3(0, 0, 0))  

    # aplicando rotacao
    matrix_transform = glm.rotate(matrix_transform, angle, glm.vec3(r_x, r_y, r_z))
        
    # aplicando translacao
    matrix_transform = glm.translate(matrix_transform, glm.vec3(t_x, t_y, t_z)) 
    
    # aplicando escala
    matrix_transform = glm.scale(matrix_transform, glm.vec3(s_x, s_y, s_z))
    
    matrix_transform = np.array(matrix_transform).T # pegando a transposta da matriz (glm trabalha com ela invertida)
    
    return matrix_transform

def view():
    global cameraPos, cameraFront, cameraUp
    mat_view = glm.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    mat_view = np.array(mat_view)
    return mat_view

def projection():
    global altura, largura
    # perspective parameters: fovy, aspect, near, far
    mat_projection = glm.perspective(glm.radians(60.0), largura/altura, 0.1, 1000.0)
    mat_projection = np.array(mat_projection)    
    return mat_projection


glfw.show_window(window)
glfw.set_cursor_pos(window, lastX, lastY)

glEnable(GL_DEPTH_TEST) ### importante para 3D

catavento_rot = 5;

while not glfw.window_should_close(window):

	glfw.poll_events() 


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

	glClearColor(1.0, 1.0, 1.0, 1.0)

	if polygonal_mode==True:
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE)
	if polygonal_mode==False:
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL)

	catavento_rot += 5;
	desenha_terreno()
	desenha_ceu()
	desenha_casa()
	desenha_exterior()
	desenha_coelho()
	desenha_interior()

	desenha_catavento(catavento_rot)

	mat_view = view()
	loc_view = glGetUniformLocation(program, "view")
	glUniformMatrix4fv(loc_view, 1, GL_FALSE, mat_view)

	mat_projection = projection()
	loc_projection = glGetUniformLocation(program, "projection")
	glUniformMatrix4fv(loc_projection, 1, GL_FALSE, mat_projection)    




	glfw.swap_buffers(window)

glfw.terminate()