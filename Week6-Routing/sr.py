#!/usr/bin/python
# -*- coding: utf-8 -*-
import socket
import threading
import random
import time
import sys

SERVER = ('127.0.0.1', 8080)
CLIENT = ('127.0.0.1', 8081)
window_size = int(sys.argv[2])
n = int(sys.argv[1])
key='1001'

def xor(a, b):
    result = []
    for i in range(1, len(b)):
        if a[i] == b[i]:
            result.append('0')
        else:
            result.append('1')

    return ''.join(result)

def mod2div(divident, divisor):
    pick = len(divisor)
    tmp = divident[0:pick]

    while pick < len(divident):
        if tmp[0] == '1':
            tmp = xor(divisor, tmp) + divident[pick]
        else:
            tmp = xor('0' * pick, tmp) + divident[pick]
        pick += 1

    if tmp[0] == '1':
        tmp = xor(divisor, tmp)
    else:
        tmp = xor('0' * pick, tmp)

    checkword = tmp
    return checkword

def make_packet(seq):
    data = bin(random.randint(0,5000))
    appended_data = data + '0' * (len(key) - 1)
    remainder = mod2div(appended_data, key)

    return str(seq)+','+data+','+remainder

def send_ack(sock, message):
    delay = random.random()
    time.sleep(delay)
    # print 'Sending to client: ' + message.split(',')[0]
    sock.sendto(message.split(',')[0], CLIENT)
    exit()

def server():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(SERVER)

    sock.settimeout(2)
    while True:
        try:
            (message, addr) = sock.recvfrom(1024)
        except:
            # print ('Error in server')
            break
        # print 'Received on server: ' + message.split(',')[0]
        appended_data = message.split(',')[1] + '0' * (len(key) - 1)
        p = random.randint(0,9)
        if(mod2div(appended_data,'1001')==message.split(',')[2] and p<8):
            threading.Thread(target=send_ack, args=(sock, message)).start()
        else:
            print('Packet %s is Corrupt, not sending ACK' %message.split(',')[0])


def client():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(CLIENT)

    ack = []
    messages = []
    for i in range(n):
        ack.append(0)
        messages.append(make_packet(i))
    
    sock.settimeout(0.5)
    while not all(item == 1 for item in ack):

        # print("Still need to send")
        # sending window
        k=0
        i=0
        while k<window_size and i<n:
            if ack[i]!=1:
                # print 'Sending from client: ' + messages[i].split(',')[0]
                sock.sendto(messages[i], SERVER)
                k=k+1
            i=i+1

        # recieving ack
        t_end = time.time() + 0.5
        while t_end > time.time():
            try:
                (data, addr) = sock.recvfrom(1024)
                # print 'Received on client: ' + data
                ack[int(data)] = 1
            except:
                continue
        print ('Reached Timeout')
        print (ack)
        print ('\n')


if __name__ == '__main__':

    t1 = threading.Thread(target=server)
    t2 = threading.Thread(target=client)

    t1.start()
    t2.start()

    t1.join()
    t2.join()