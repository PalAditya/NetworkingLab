f = open("test.tr", "r")
lines = f.readlines()
drop=0

start=[]
end=[]
ty=[]
siz=0

cbr_packet_size=1000
ack_packet_size=40
tcp_packet_size=1040

for i in range(0,5000):
	start.append(100)
	end.append(0)
	ty.append('cbr')

for line in lines:
	x = line.split(' ');
	x[11] = x[11].split('\n')[0]

	if x[0] == 'd':
		drop=drop+1

	if float(x[1])<start[int(x[11])]:
		start[int(x[11])]=float(x[1])
	if float(x[1])>end[int(x[11])]:
		end[int(x[11])]=float(x[1])

	ty[int(x[11])]=x[4]

	if int(x[11])>siz:
		siz=int(x[11])

f.close()

cbr_delay=0
tcp_delay=0
ack_delay=0
cbr_count=0
tcp_count=0
ack_count=0

for i in range(0,siz):
	if(ty[i]=='cbr'):
		cbr_delay=cbr_delay+end[i]-start[i];
		cbr_count=cbr_count+1;

	elif(ty[i]=='ack'):
		ack_delay=ack_delay+end[i]-start[i];
		ack_count=ack_count+1;

	else:
		tcp_delay=tcp_delay+end[i]-start[i];
		tcp_count=tcp_count+1;

print ("\nAvg. Delay(ms): ")
print ((cbr_delay+ack_delay+tcp_delay)/(cbr_count+ack_count+tcp_count));
print ("\nAvg. Throughput(CBR)(bps): ")
print (cbr_count*cbr_packet_size/(cbr_delay*1000));
print ("\nAvg. Throughput(ACK)(bps): ")
print (ack_count*ack_packet_size/(ack_delay*1000));
print ("\nAvg. Throughput(TCP)(bps): ")
print (tcp_count*tcp_packet_size/(tcp_delay*1000));
print ("\nPacket Delivery Ratio: ")
print ((cbr_count+ack_count+tcp_count-drop)*100/(cbr_count+ack_count+tcp_count));
