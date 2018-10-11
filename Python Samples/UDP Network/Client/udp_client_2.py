# Dalton Fox - Final UDP Dots Client

import socket
import pygame
import msgpack
import random
import os, sys

class Dot(object):
    def __init__(self):
        self.x = 0
        self.y = 0
        self.direction = 0
        self.color = (255, 255, 255)
        self.size = random.randint(2, 6)
        self.speed = 0.25

    def load_dict(self,temp_dict):
        self.x = temp_dict["x"]
        self.y = temp_dict["y"]
        self.direction = temp_dict["direction"]
        self.color = temp_dict["color"]

    def update(self):
        if self.direction == 0:
            self.x -= self.speed
        elif self.direction == 1:
            self.y -= self.speed
        elif self.direction == 2:
            self.x += self.speed
        elif self.direction == 3:
            self.y += self.speed
        if self.y > 480:
            self.y = 0
        elif self.y < 0:
            self.y = 480
        if self.x > 640:
            self.x = 0
        elif self.x < 0:
            self.x = 640

    def render(self):
        pygame.draw.circle(ds, self.color, (int(self.x), int(self.y)), int(self.size), 0)
        

def send_net_message(message):
     clientSocket.sendto(message.encode('utf-8'), server_addr)
       
    
def get_net_message():
    try:    
        message,address=clientSocket.recvfrom(4024)
    except:
        return None,None
    
    return message,address


def process_net_message(message,address):
    global game_state_dict
    global dot_list
    global loaded

   
    game_state_dict = msgpack.loads(message, raw = False)
    dot_dict_list = game_state_dict["dot_dict_list"]

    if not loaded:
        dot_list=[]
        for dot_dict in dot_dict_list:
            newdot=Dot()
            newdot.load_dict(dot_dict)
            dot_list.append(newdot)
        loaded = True
 
pygame.init()

if len(sys.argv) > 1:
    y = (pygame.display.Info().current_h / 2) - (480 / 2)
    if sys.argv[1] == '0':
        os.environ['SDL_VIDEO_WINDOW_POS'] = "%d,%d" % ((pygame.display.Info().current_w / 2) - (640 + 100), y)
    else:
        os.environ['SDL_VIDEO_WINDOW_POS'] = "%d,%d" % ((pygame.display.Info().current_w / 2) + 100, y)
        
ds=pygame.display.set_mode((640, 480))
pygame.display.set_caption("client")

clientSocket=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
clientSocket.setblocking(0) 
server_addr=("127.0.0.1", 12000)
send_net_message("<JOIN:" + "null" + ">")

game_state_dict = {}
dot_list = []
loaded = False


game_running = True


last = pygame.time.get_ticks()
while game_running:
    t = pygame.time.get_ticks()
    dt = t - last
    last = t
    
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            game_running=False
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                game_running=False
    
    while True:
        message,address = get_net_message()
        if message == None:
            break
        process_net_message(message,address)

    pygame.draw.rect(ds,(0,0,0),(0,0,640,480),0)
        
    for dot in dot_list:
        dot.update()
        dot.render()

    pygame.display.update()

send_net_message("<QUIT>")

pygame.display.quit()

