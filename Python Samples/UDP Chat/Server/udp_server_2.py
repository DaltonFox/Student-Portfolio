# Dalton Fox - Final UDP Dots Server

import socket
import pygame
import random
import msgpack
   
def get_net_message():
    try:    
        message,address=serverSocket.recvfrom(1024)
    except:
        return None,None
    message=message.decode('utf-8')    
    return message,address

def send_net_message_client(message,client_addr):
    serverSocket.sendto(message,client_addr)
    
def process_net_message(message,address):
    if message[0] == '<' and message[-1] == '>':
        message = message[1:-1]
        if ":" in message:
            command,data=message.split(":")
        else:
            command = message
            data = None

        if command=="JOIN":
            print("Sending dots to: ", data, address)
            ip_address, port = address
            send_net_message_client(game_state_str, (ip_address,port))
    else:
        print("invalid message.")


#Initialization
pygame.init()

serverSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
serverSocket.setblocking(0)
server_addr = ("", 12000)
serverSocket.bind(server_addr)

game_running = True

# Init Dots
dot_dict_list=[]
for i in xrange(100):
    dot_dict = {}
    dot_dict["x"] = random.randint(0, 640)
    dot_dict["y"] = random.randint(0, 480)
    dirt = random.randint(0, 3)
    dot_dict["direction"] = dirt
    r = random.randint(0, 255)
    g = random.randint(0, 255)
    b = random.randint(0, 255)
    dot_dict["color"] = (r, g, b)
    dot_dict_list.append(dot_dict)


game_state_dict = {}
game_state_dict["dot_dict_list"] = dot_dict_list
game_state_str = msgpack.dumps(game_state_dict)

print("Server is running.")

last = pygame.time.get_ticks()
while game_running:
    t = pygame.time.get_ticks()
    dt = t - last
    last = t

    #getting events / sending events
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            game_running=False
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                game_running = False
         
    while True:
        message, address = get_net_message()
        if message == None:
            break
        process_net_message(message, address)
    
print("Server is shutting down.")
pygame.display.quit()

