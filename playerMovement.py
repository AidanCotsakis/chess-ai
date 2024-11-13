import pygame
import time
import os
import random
from customPygame import *

pygame.init()

AIgame = False

windowSize = [1920,1080]

# init pygame and make pygame window
pygame.init()
clock = pygame.time.Clock()
os.environ['SDL_VIDEO_CENTERED'] = '0'
win = pygame.display.set_mode(windowSize, pygame.NOFRAME)
pygame.display.set_caption("Chess (C)")

fps = 30

gridSize = 135
turnNum = 1

pieceNames = ["p", "n", "b", "r", "q", "k"]

whiteImages = []
blackImages = []

text = textbox("Black To Move", "fonts\\Cabin-VariableFont_wdth,wght.ttf", 100, (255,255,255), (windowSize[1] + (windowSize[0]-windowSize[1])/2, 100), boundingBox = None, boundingPlacement = "midMid", fontPlacement = "midMid", verticalMargin = 0, horizontalMargin = 0, baseColour = None, boundingRadius = None, editable = False, selectedColour = None)
sendButton = textbox("Multi-Step Move", "fonts\\Cabin-VariableFont_wdth,wght.ttf", 50, (255,255,255), (windowSize[1] + (windowSize[0]-windowSize[1])/2, windowSize[1] - 100), boundingBox = None, boundingPlacement = "midMid", fontPlacement = "midMid", verticalMargin = 10, horizontalMargin = 50, baseColour = (10,100,10), boundingRadius = 100, editable = False, selectedColour = None)

multiStepMove = False

if AIgame:
	text.text = "AI vs AI"
	text.renderSurface()

for name in pieceNames:
	whiteImages.append(pygame.image.load(f"Images/w{name}.png"))
	blackImages.append(pygame.image.load(f"Images/b{name}.png"))

board = [
[[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0]],
[[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0]],
[[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0]],
[[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0]],
[[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0]],
[[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0]],
[[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0]],
[[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0]]]

with open("data.txt", "w") as f:
	f.write("c?2,4,0.2,2,0.2,3,0.2,5,0.2,6,0.2,3,0.2,2,0.2,4,0/2,1,0.2,1,0.2,1,0.2,1,0.2,1,0.2,1,0.2,1,0.2,1,0/0,0,0.0,0,0.0,0,0.0,0,0.0,0,0.0,0,0.0,0,0.0,0,0/0,0,0.0,0,0.0,0,0.0,0,0.0,0,0.0,0,0.0,0,0.0,0,0/0,0,0.0,0,0.0,0,0.0,0,0.0,0,0.0,0,0.0,0,0.0,0,0/0,0,0.0,0,0.0,0,0.0,0,0.0,0,0.0,0,0.0,0,0.0,0,0/1,1,0.1,1,0.1,1,0.1,1,0.1,1,0.1,1,0.1,1,0.1,1,0/1,4,0.1,2,0.1,3,0.1,5,0.1,6,0.1,3,0.1,2,0.1,4,0")

def readBoard(board):
	with open("data.txt", "r") as f:
		content = f.read()

	success = False
	if content[0] == "c":
		success = True

		content = content[2:]

		board = content.split("/")
		for i, line in enumerate(board):
			board[i] = line.split(".")

		for i, line in enumerate(board):
			for j, tile in enumerate(line):
				board[i][j] = [int(i) for i in tile.split(",")]

	return board, success

def writeBoard(board):
	string = "p?"

	for i in range(8):
		for j in range(8):
			for k in range(3):
				string += str(board[i][j][k])
				if k != 2:
					string += ","
			if j != 7:
				string += "."
		if i != 7:
			string += "/"

	with open("data.txt", "w") as f:
		f.write(string)

def draw():
	win.fill((10,10,10))

	for i in range(8):
		for j in range(8):
			if (i + j) % 2 == 0:
				pygame.draw.rect(win, (233,237,204), (j*gridSize, i*gridSize, gridSize, gridSize))
			else:
				pygame.draw.rect(win, (119,153,84), (j*gridSize, i*gridSize, gridSize, gridSize))

			if [j, i] == initialPos:
				pygame.draw.rect(win, (230,200,150), (j*gridSize, i*gridSize, gridSize, gridSize))

			elif board[i][j][0] == 1:
				win.blit(whiteImages[board[i][j][1] - 1], (j*gridSize, i*gridSize))

			elif board[i][j][0] == 2:
				win.blit(blackImages[board[i][j][1] - 1], (j*gridSize, i*gridSize))

	if piece != [0,0]:
		mousePos = pygame.mouse.get_pos()

		if piece[0] == 1:
			win.blit(whiteImages[piece[1] - 1], (mousePos[0] - gridSize/2, mousePos[1] - gridSize/2))
		elif piece[0] == 2:
			win.blit(blackImages[piece[1] - 1], (mousePos[0] - gridSize/2, mousePos[1] - gridSize/2))

	text.draw(win)
	if not waitingForBlack and not AIgame:
		sendButton.draw(win)

	pygame.display.update()

loop = True
initialPos = []
piece = [0,0]
waitingForBlack = True
lastChecked = 0

while loop:

	clock.tick(fps)

	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			loop = False

		if event.type == pygame.MOUSEBUTTONDOWN and not waitingForBlack and not AIgame:
			if event.button == 1:
				mousePos = pygame.mouse.get_pos()
				if mousePos[0] < windowSize[1] and mousePos[1] < windowSize[1]:
					initialPos = [int(mousePos[0]/gridSize), int(mousePos[1]/gridSize)]
					piece = [board[initialPos[1]][initialPos[0]][0],board[initialPos[1]][initialPos[0]][1]]

				if not waitingForBlack and not AIgame and pointBoxCollision(mousePos, sendButton.box):
					if not multiStepMove:
						multiStepMove = True
						sendButton.text = "Commit Move"
						sendButton.renderSurface()

					else:
						sendButton.text = "Multi-Step Move"
						sendButton.renderSurface()
						text.text = "Black To Move"
						text.renderSurface()
						turnNum += 2
						waitingForBlack = True
						writeBoard(board)
						multiStepMove = False

		if event.type == pygame.MOUSEBUTTONUP and not waitingForBlack and not AIgame:
			if event.button == 1:
				mousePos = pygame.mouse.get_pos()
				newPos = [int(mousePos[0]/gridSize), int(mousePos[1]/gridSize)]

				if mousePos[0] < windowSize[1] and mousePos[1] < windowSize[1] and newPos != initialPos and piece != [0,0] and board[newPos[1]][newPos[0]][0] != piece[0]:
					board[newPos[1]][newPos[0]] = board[initialPos[1]][initialPos[0]]
					board[initialPos[1]][initialPos[0]] = [0,0,0]
					board[newPos[1]][newPos[0]][2] = turnNum

					# queen
					if piece == [1,1] and newPos[1] == 0:
						board[newPos[1]][newPos[0]][1] = 5
					elif piece == [2,1] and newPos[1] == 7:
						board[newPos[1]][newPos[0]][1] = 5

					if not multiStepMove:
						text.text = "Black To Move"
						text.renderSurface()
						turnNum += 2
						waitingForBlack = True
						writeBoard(board)

				initialPos = []
				piece = [0,0]

	if (waitingForBlack or AIgame) and time.time() - lastChecked > 1:
		lastChecked = time.time()
		board, success = readBoard(board)
		if success:
			waitingForBlack = False
			if not AIgame:
				text.text = "White To Move"
				text.renderSurface()

	draw()
