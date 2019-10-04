# Assignment 6

***Note: Sample Images in `Images/` directory***

## Requirements
python2

## Selective Repeat ARQ

### Run
```
python sr.py <num_packets> <window_size>
```
### Working
1. Both sender and reciever are in the same file, running independantly in different threads.
2. Senders generates random data, and caculates crc for given num_packets
3. Sender attempts to send a group of packets of window_size
4. The packets are received by always listening (with some timeout) receiver
5. Reciever checks crc of packet, at random times, it is assumed to have an error, in which case, it doesnot send back ack
6. If the packet is not corrupt, an ack, is sent back with a random delay falling between 0-1 sec.
7. The recvfrom() func on the sender side waiting for ack's timeouts at 0.5 sec, so only some ack are received.
8. The ack's can be received out of order and only those unsuccesful packets will be sent again with some more packets to fill the window
9. This protocol is more efficient that go-back-n since it does not send full window upon failure of only some packets in the window.


## Distance Vector Routing

### Run
```
python dvr.py <id> <port> <config_file> ex: A 5000 config/configA.txt
```
While running the code, wait for 30 seconds for everything to converge and set to equillibruim.  


### Working 
1. The code is completely dynamic, it checks for links failures, new links, new weights all in real time.
2. Once in 30 sec, the code checks the config file to check for updated cost values.
3. Run any number of nodes according to the config file (should be strictly correct) and upon receiving messages, the distance will be updated.
4. The graph weights from individual nodes are send in json format to neighbor nodes and they send to their neighbor nodes and this goes on... until all nodes have the complete graph weights at that time.
5. After that, bellman ford algorithm is applied on each node on the graph and the distances are printed.
