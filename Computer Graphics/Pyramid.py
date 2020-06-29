import glfw
from OpenGL.GL import *
import OpenGL.GL.shaders
import numpy as np
import random
import math

msg = "Hello World!"
print(msg)

# INITIALIZING WINDOW
glfw.init()
glfw.window_hint(glfw.VISIBLE, glfw.FALSE)
window = glfw.create_window(720, 600, "CG", None, None)
glfw.make_context_current(window)

# GLSL (OpenGL Shading Langage) Vertex/Fragment Shader
vertex_code = """
        attribute vec3 position;
	uniform mat4 mat_transformation;
        void main(){
            	gl_Position = mat_transformation * vec4(position,1.0);
        }
        """
fragment_code = """
	uniform vec4 color;
        void main(){
            	gl_FragColor = color;
        }
        """

# Request a program and shader slots from GPU
program = glCreateProgram()
vertex = glCreateShader(GL_VERTEX_SHADER)
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

vp = 16

# # preparando espaço para 500 vértices usando 2 coordenadas (x,y)
vertices = np.zeros(vp, [("position", np.float32, 3)])

# preenchendo as coordenadas de cada vértice
vertices['position'] = [
	# Face 1 do Cubo (vértices do quadrado)
	# Bottom
	(-0.2, -0.2, +0.2),
	(-0.2, -0.2, -0.2),         
	(+0.2, -0.2, +0.2),
	(+0.2, -0.2, -0.2),
 
	# Front
	(-0.2, -0.2, +0.2),
	(+0.2, -0.2, +0.2),
	(-0.0, +0.2, +0.0),

	# Back
	(-0.2, -0.2, -0.2),
	(+0.2, -0.2, -0.2),
	(-0.0, +0.2, -0.0),            

	# Right
	(+0.2, -0.2, +0.2),
	(+0.2, -0.2, -0.2),
	(-0.0, +0.2, -0.0),         

	# Left
	(-0.2, -0.2, +0.2),
	(-0.2, -0.2, -0.2),         
	(-0.0, +0.2, +0.0)]

# Request a buffer slot from GPU
buffer = glGenBuffers(1)
# Make this buffer the default one
glBindBuffer(GL_ARRAY_BUFFER, buffer)

# Upload data
glBufferData(GL_ARRAY_BUFFER, vertices.nbytes, vertices, GL_DYNAMIC_DRAW)
glBindBuffer(GL_ARRAY_BUFFER, buffer)

# Bind the position attribute
# --------------------------------------
stride = vertices.strides[0]
offset = ctypes.c_void_p(0)

loc = glGetAttribLocation(program, "position")
glEnableVertexAttribArray(loc)

glVertexAttribPointer(loc, 3, GL_FLOAT, False, stride, offset)

loc_color = glGetUniformLocation(program, "color")

R = 0.15
G = 0.15
B = 0.15

maxVelocity = 0.001
maxTurnVelocity = 0.3

moveAcc = maxVelocity / 1000
turnAcc = maxTurnVelocity / 100

turnVelocity = 0
velocity = 0

s_x = 1.0
s_y = 1.0

angle = 0.0
c = 1.0
s = 0.0

radz = radx = 0.0

class Button:
	status = 0
        
	def __init__(self, keycode):
		self.keycode = keycode

class Axis:
        status = 0
        
        def __init__(self, button0, button1):
        	self.positiveButton = button0
        	self.negativeButton = button1

class Vector:
        x = 0.0
        y = 0.0
        z = 0.0

v = Vector()
buttons = {	"W" 		: Button(87),
        	"S" 		: Button(83),
		"A"		: Button(65),
		"D" 		: Button(68),
		"Up"		: Button(265),
		"Down"		: Button(264),
		"Right"		: Button(262),
		"Left"		: Button(263)}

axis = {
		"Vertical" 	: Axis(buttons["W"], buttons["S"]),
		"Horizontal" 	: Axis(buttons["A"], buttons["D"]),
		"Forward" 	: Axis(buttons["Up"], buttons["Down"]),
		"Turn" 		: Axis(buttons["Right"], buttons["Left"])}

def key_event(window, key, scancode, action, mods):
	global buttons, axis
 
	for i in buttons:
		if key == buttons[i].keycode: 
          		buttons[i].status = action
	
	for i in axis:
        
		if action != 0:
			if key == axis[i].positiveButton.keycode: 
				axis[i].status = min(action, 1)

			if key == axis[i].negativeButton.keycode: 
				axis[i].status = -min(action, 1)
		else:
			if key == axis[i].positiveButton.keycode: 
				axis[i].status = -max(axis[i].negativeButton.status, 0)

			if key == axis[i].negativeButton.keycode: 
				axis[i].status = max(axis[i].positiveButton.status, 0)


glfw.set_key_callback(window, key_event)

glfw.show_window(window)

d = 0.0
glEnable(GL_DEPTH_TEST) ### importante para 3D

def multiplica_matriz(a,b):
    m_a = a.reshape(4,4)
    m_b = b.reshape(4,4)
    m_c = np.dot(m_a,m_b)
    c = m_c.reshape(1,16)
    return c

while not glfw.window_should_close(window):

	glfw.poll_events()

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
	glClearColor(1.0, 1.0, 1.0, 1.0)
  
	v.x += axis["Horizontal"].status * -0.001
	v.y += axis["Vertical"].status * 0.001
 
	r_y = axis["Turn"].status * -0.1
	r_x = axis["Forward"].status * 0.1

	radz += math.radians(r_y)
	radx += math.radians(r_x)
	
 	# rad -= 0.001
	cy = math.cos(radz)
	sy = math.sin(radz)

	cx = math.cos(radx)
	sx = math.sin(radx)
	
	# mat_rotation_z = np.array([     cz, -sz, 0.0, 0.0, 
	# 				sz,  cz, 0.0, 0.0, 
	# 				0.0,      0.0, 1.0, 0.0, 
	# 				0.0,      0.0, 0.0, 1.0], np.float32)
	
	mat_rotation_x = np.array([     1.0,   0.0,    0.0, 0.0, 
					0.0, cx, -sx, 0.0, 
					0.0, sx,  cx, 0.0, 
					0.0,   0.0,    0.0, 1.0], np.float32)
	
	mat_rotation_y = np.array([     cy,  0.0, sy, v.x, 
					0.0,    1.0,   0.0, v.y, 
					-sy, 0.0, cy, 0.0, 
					0.0,    0.0,   0.0, 1.0], np.float32)
  
	mat_transform = multiplica_matriz(mat_rotation_x,mat_rotation_y)
	# mat_transform = multiplica_matriz(mat_rotation_y,mat_transform)

	loc = glGetUniformLocation(program, "mat_transformation")
	glUniformMatrix4fv(loc, 1, GL_TRUE, mat_transform)

	#glPolygonMode(GL_FRONT_AND_BACK,GL_LINE)
	
	glUniform4f(loc_color, 1, 0, 0, 1.0) ### vermelho
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4)
	
	glUniform4f(loc_color, 0, 0, 1, 1.0) ### azul
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 3)
	
	glUniform4f(loc_color, 0, 1, 0, 1.0) ### verde
	glDrawArrays(GL_TRIANGLE_STRIP, 7, 3)
	
	glUniform4f(loc_color, 1, 1, 0, 1.0) ### amarela
	glDrawArrays(GL_TRIANGLE_STRIP, 10, 3)
	
	glUniform4f(loc_color, 0.5, 0.5, 0.5, 1.0) ### cinza
	glDrawArrays(GL_TRIANGLE_STRIP, 13, 3)
	
	# glUniform4f(loc_color, 0.5, 0, 0, 1.0) ### marrom
	# glDrawArrays(GL_TRIANGLE_STRIP, 20, 4)

	glfw.swap_buffers(window)

glfw.terminate()
