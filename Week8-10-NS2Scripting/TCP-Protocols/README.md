## Assignment 9 - TCP Congestion Control

***Note: Sample Images in `Images/` directory***

### Instructions
To generate `.tr` and `.nam` files again (fresh)
```
ns tcp.tcl
```
To use existing `.nam` file (for simulation)
```
ns 
nam test.nam
```
To use the existing `.tr` file (for caluculating avg tcp delay)
```
gcc parameters.c
./a.out
```

### Working
1. tcp.tcl has all the implementation of ns2 simulation for all kinds of congestion control mechanisms in tcp.
2. The `tr` files are already present, for newReno, reno, tahoe, sack, vegas. These are the congestion control protocols implemented in tcl file also.
3. The graph can be generated and plotted manually by using parameters.c or directly plotted using xgraph and the given `.xg` file.