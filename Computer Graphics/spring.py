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
        attribute vec2 position;
	uniform mat4 mat_transformation;
        void main(){
            	gl_Position = mat_transformation * vec4(position,0.0,1.0);
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

springRounds = 5
num_vertices = 64
pi = 3.14
counter = 0
radius = 0.1
vertices = np.zeros(num_vertices, [("position", np.float32, 2)])

angle = 0.0
steps = 0.005

for counter in range(num_vertices):
        
	angle += 2 * pi * springRounds / num_vertices
 
	x = (radius * math.cos(angle))
	y = (radius * math.sin(angle))
	z = steps * counter
	vertices[counter] = [x, z]


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

glVertexAttribPointer(loc, 2, GL_FLOAT, False, stride, offset)

loc_color = glGetUniformLocation(program, "color")

R = 0.15
G = 0.15
B = 0.15

jumpSpring = 0
maxVelocity = 0.001
maxTurnVelocity = 0.3

moveAcc = maxVelocity / 1000
turnAcc = maxTurnVelocity / 100

turnVelocity = 0
velocity = 0
turnChoice = [-1, 1]
turnDir = random.choice(turnChoice)

s_x = 1.0
s_y = 1.0

angle = 0.0
c = 1.0
s = 0.0

# def Button_Pressed(key, )
# 	if (key == 264 and action) return 1
# 	if (key == 264 and !action) return 0



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
v.y = -num_vertices * steps / 2
buttons = {	"Up" 	: Button(265),
        	"Down" 	: Button(264),
		"Right" : Button(68),
		"Left"	: Button(65)}

axis = {
		"Vertical" 	: Axis(buttons["Up"], buttons["Down"]),
		"Horizontal" 	: Axis(buttons["Left"], buttons["Right"])}

def key_event(window, key, scancode, action, mods):
	global buttons, axis

	# print('[key event] key=', key)
	# print('[key event] scancode=', scancode)
	# print('[key event] action=', action)
	# print('[key event] mods=', mods)
	# print('-------')
 
	# print(key, action)
 
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

while not glfw.window_should_close(window):

	glfw.poll_events()

	glClear(GL_COLOR_BUFFER_BIT)
	glClearColor(1.0, 1.0, 1.0, 1.0)

	if jumpSpring == 0:
		if axis["Vertical"].status < 0:
			velocity = max(-maxVelocity, min(velocity + moveAcc * axis["Vertical"].status, maxVelocity))
		if axis["Vertical"].status >= 0 and s_y < 1:
			velocity = velocity + 5 * moveAcc * (1 if s_y < 1 else -1)
			if s_y + velocity >= 1: 
				jumpSpring = 1
				velocity = abs(velocity)
		s_y += velocity
  	
	if jumpSpring == False and (s_y == 0.2 or s_y == 1):
		velocity = 0.0
  
	if jumpSpring == 1:
		v.x -= s * velocity / 4
		v.y += c * velocity / 4
		
		angle += turnDir / 10
		rad = math.radians(angle)
		c = math.cos(rad)
		s = math.sin(rad)
  
		if abs(rad) >= math.pi: jumpSpring = 2
  
	s_y = max(0.2, min(s_y, 1))

	# Draw Triangle
	transform = np.array  ([
                    		c * s_x, -s * s_y, 0.0, v.x,
				s * s_x,  c * s_y, 0.0, v.y,
				0.0, 1.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0], np.float32)
  
	loc = glGetUniformLocation(program, "mat_transformation")
	#     glUniformMatrix4fv(loc, 1, GL_TRUE, mat_translation)
	glUniformMatrix4fv(loc, 1, GL_TRUE, transform)

	glDrawArrays(GL_LINE_STRIP, 0, len(vertices))
	glUniform4f(loc_color, R, G, B, 1.0)  # modificando a cor do objeto!

	glfw.swap_buffers(window)

glfw.terminate()
