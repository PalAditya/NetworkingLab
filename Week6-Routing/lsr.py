import sys
import thread
import time
import pickle
import socket

IP = '127.0.0.1'

def client():
    time.sleep(1)
    while True:
        try:
            clientSock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        except:
            print ('Socket not formed')

        for port in ports:
            # print 'Sent to '+str(port)
            message = pickle.dumps({name:graph[name]})
            # message = pickle.dumps(graph)
            clientSock.sendto(message, (IP,port))

        time.sleep(1)

def bellmanford():
    dis = {}
    for node in graph:
        dis[node]=float('inf')
    dis[name]=0

    for _ in range(len(graph)-1):
        for u in graph:
            for v in graph[u]:
                if(dis[v] >= dis[u]+graph[u][v]):
                    dis[v] = dis[u]+graph[u][v]

    for i in dis:
        print ('Distance to %s is %f' % (i, dis[i]))


script, name, portno, file = sys.argv
print (name,portno)
portno = int(portno)
graph = {}
ports = []
neighbors = []

thread.start_new_thread(client, ())

while True:
    fd = open(file)
    n = int(fd.readline())

    graph[name]={}
    ports = []
    neighbors = []
    timer = {}

    while(n>0):
        line = fd.readline()
        info = line.split()
        neighbors.append(info[0])
        ports.append(int(info[2]))
        if info[0] not in graph.keys():
            graph[info[0]]={}
        graph[name][info[0]] = float(info[1])
        graph[info[0]][name] = float(info[1])
        n = n - 1

    print graph
    print neighbors

    # time.sleep(2)

    sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    sock.bind((IP,portno))

    for neighbor in neighbors:
        timer[neighbor]=time.time()
    refresh = time.time()

    print timer

    while time.time()<refresh+30:
        sock.settimeout(1)
        # print('\033c')
        bellmanford()
        # print graph
        # print timer
        # for neighbor in neighbors:
            # print time.time()-timer[neighbor]

        for neighbor in neighbors:
            if time.time()-timer[neighbor] > 5:
                graph[name][neighbor]=float('inf')
                graph[neighbor][name]=float('inf')

        print ('\n')

        try:
            message,client = sock.recvfrom(2048)
            message = pickle.loads(message)
        except:
            # print 'No new message'
            continue

        # print 'Recived from'+str(client)
        # print message

        for key in message:
            if key not in graph.keys():
                graph[key]={}
            for i in message[key]:
                if i not in graph.keys():
                    graph[i]={}
                graph[i][key]=message[key][i]
                graph[key][i]=message[key][i]
            timer[key] = time.time()

        # print ('\n')s
        time.sleep(1)

    refresh = time.time()