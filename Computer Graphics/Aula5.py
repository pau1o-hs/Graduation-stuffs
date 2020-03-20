from PIL import Image
from IPython.display import display
import math

largura = 500
altura = 300

imagem = Image.new('RGB', (largura, altura))

# display(imagem)

def bresenham_line(v1, v2, imagem, cor='white'):
    
    # vertices recebidas no formato matricial
    x1 = v1[0][0]
    y1 = v1[1][0]
    
    x2 = v2[0][0]
    y2 = v2[1][0]
    
    cor_linha = (255,255,255) #white
    if cor == 'red':
        cor_linha = (255,0,0)

    matriz_pixels = imagem.load()
    
    # calculando diferenca em cada eixo
    dx = x2 - x1
    dy = y2 - y1
 
    # verificar se a linha é íngreme (steep)
    steep = False
    if abs(dy) > abs(dx): steep = True

 
    # se for íngreme, rotacionar a linha
    if steep:
        x1_temp = x1
        x1 = y1
        y1 = x1_temp
        
        x2_temp = x2
        x2 = y2
        y2 = x2_temp

 
    # Verificar se é necessario trocar as coordenadas
    # util para plotar da esquerda para direita
    swapped = False
    if x1 > x2:
        x1_temp = x1
        x1 = x2
        x2 = x1_temp
        
        y1_temp = y1
        y1 = y2
        y2 = y1_temp
        
        swapped = True
 
    # recalcular as diferencas no eixo
    dx = x2 - x1
    dy = y2 - y1

    # inicializando erro
    error = int(dx / 2.0)

    # inicilizando o incremento em y
    y_inc = -1
    if y1 < y2: y_inc = 1

    # inicializando y 
    y = y1
    
    # lista de coordenadas
    coordenadas = []
    
    # gerando coordenadas da linha
    for x in range(x1, x2 + 1): # incrementando x 
        coord = (x, y)
        if steep: # caso seja íngreme
            coord = (y, x)

        # adiciona a coordenada
        coordenadas.append(coord)
        
        # atualiza o erro
        error -= abs(dy)
        
        # incrementa y se erro for negativo
        if error < 0:
            y += y_inc
            error += dx
 
    # se as coordenadas foram trocadas, inverter a lista de coordenadas
    if swapped:
        coordenadas.reverse()
        
    for coord in coordenadas:
        x = coord[0]
        y = coord[1]
        if x < imagem.size[0] and y < imagem.size[1] and x >= 0 and y >= 0:
            matriz_pixels[x,y] = cor_linha

imagem = Image.new('RGB', (largura, altura))

v1 = [ [150], [50] ]
v2 = [ [250], [50] ]
v3 = [ [150], [150] ]

# de v1 para v2

bresenham_line(v1, v2, imagem)

# de v1 para v3
bresenham_line(v1, v3, imagem)

# de v3 para v2
bresenham_line(v3, v2, imagem)

def multiplica_matrizes(M1, M2):
    
    # recuperando dimensoes de M1
    m1_linhas = len(M1)
    m1_colunas = len(M1[0])

    # recuperando dimensoes de M2
    m2_linhas = len(M2)
    m2_colunas = len(M2[0])    
    
    if m1_colunas != m2_linhas:
        print(m1_linhas,m1_colunas,m2_linhas,m2_colunas)
        print('Nao posso multiplicar. Dimensoes incorretas.')
        return -1

    # criando espaco para a M3
    M3 = [[0 for row in range(m2_colunas)] for col in range(m1_linhas)]
    
    for i in range(m1_linhas):
        for j in range(m2_colunas):
            for k in range(m1_colunas):
                M3[i][j] += M1[i][k] * M2[k][j]

    for i in range(m1_linhas):
        for j in range(m2_colunas):
            M3[i][j] = int(M3[i][j])
                
    return M3

def translacao(vertice, t_x, t_y):
    
    # define a matriz de translacao
    matriz_translacao = [
                            [1, 0, t_x],
                            [0, 1, t_y],
                            [0, 0, 1  ]
                        ]
    
    # inicializa novo vertice
    vertice_t = multiplica_matrizes(matriz_translacao, vertice)
    
    # retorna novo vertice
    return vertice_t

def escala(vertice, e_x, e_y ):
    
    # define a matriz de translacao
    matriz_escala = [
                            [e_x, 0  , 0  ],
                            [0  , e_y, 0  ],
                            [0  , 0  , 1  ]
                        ]
    
    # inicializa novo vertice
    vertice_e = multiplica_matrizes(matriz_escala, vertice)
    
    # retorna novo vertice
    return vertice_e

def rotacao(vertice, angulo):

    rad = math.radians(angulo) 
    c = math.cos(rad)
    s = math.sin(rad)
    
    # define a matriz de rotacao 
    matriz_rotacao = [
                        [c  , -s , 0],
                        [s  , c  , 0],
                        [0  , 0  , 1]
                     ]

    # inicializa novo vertice
    vertice_r = multiplica_matrizes(matriz_rotacao, vertice)
    
    return vertice_r

def Transform(vertex, mx, my, sx, sy, angle, pivot):

	rad = math.radians(angle) 
	c = math.cos(rad)
	s = math.sin(rad)
	
	b = [	[vertex[0][0] - pivot[0][0]], 
 		[vertex[1][0] - pivot[1][0]]]

	v = [
         	[int (c * sx * (b[0][0] + mx) - s * sy * (b[1][0] + my))], 
          	[int (s * sx * (b[0][0] + mx) + c * sy * (b[1][0] + my))], 
           	[1]
            ]

	return v

# vertices do triângulo (lembre-se que mantemos h=1), no formato matricial
v1=[[150],[50],[1]]
v2=[[250],[50],[1]]
v3=[[150],[150],[1]]

bresenham_line(v1, v2, imagem)
bresenham_line(v1, v3, imagem)
bresenham_line(v2, v3, imagem)

obj = [v1, v2, v3]
print(len(obj))

for v in range(0, len(obj)):
	obj[v] = Transform(obj[v], 0.0, 0.0, 1.0, 1.0, 0.0, obj[0])

bresenham_line(obj[0], obj[1], imagem, cor='red')
bresenham_line(obj[0], obj[2], imagem, cor='red')
bresenham_line(obj[1], obj[2], imagem, cor='red')

imagem.show()