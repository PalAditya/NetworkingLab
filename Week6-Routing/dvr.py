import sys
import thread
import time
import pickle
import socket

IP = '127.0.0.1'

script, name, portno, file = sys.argv
print (portno, file)
portno = int(portno)

table = {}

while True:
    fd = open(file)
    n = int(fd.readline())

    table = {}
    ports = []
    neighbors = []
    timer = {}

    table[name]=(0,name)

    while(n>0):
        line = fd.readline()
        info = line.split()
        neighbors.append(info[0])
        ports.append(int(info[2]))
        table[info[0]] = (float(info[1]),name)
        n = n - 1

    # print table
    # print neighbors

    sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    sock.bind((IP,portno))

    for neighbor in neighbors:
        timer[neighbor]=time.time()
    refresh = time.time()

    while time.time()<refresh+100:
        print table
        sock.settimeout(1)

        if(time.time()-refresh>5):
            try:
                clientSock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
            except:
                print ('Socket not formed')

            for port in ports:
                # print 'Sent to '+str(port)
                message = pickle.dumps(table)
                clientSock.sendto(name+':'+message, (IP,port))           

        for neighbor in neighbors:
            if time.time()-timer[neighbor] > 5:
                table[neighbor]=(float('inf'),name)

        try:
            message,client = sock.recvfrom(2048)
            client = message.split(':')[0];
            route = pickle.loads(message.split(':')[1])
        except:
            # print 'No new message'          
            continue

        print 'Recived from '+str(client)
        print route

        if table[client][0] == float('inf'):
            table[client] = (route[name][0],name)

        for key in route:
            if key not in table.keys():
                table[key] = (table[client][0]+route[key][0],client)
            elif table[key][1] == client:
                table[key] = (table[client][0]+route[key][0],client)
            elif table[key][0]>table[client]+route[key]:
                    table[key]=(table[client][0]+route[key][0],client)

        timer[client] = time.time()           


        print ('\n')
        time.sleep(1)

    refresh = time.time()