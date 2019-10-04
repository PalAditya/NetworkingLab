## Assignment 8 - DVR NS2 Simulation

***Note: Sample Images in `Images/` directory***

### Instructions
To generate `.tr` and `.nam` files again (fresh)
```
ns dvProg.tcl
```
To use existing `.nam` file (for simulation)
```
ns 
nam thro.nam
```
To use the existing `.tr` file (for faster analysis)
```
python analysis.py
```

### Working
1. dvr protocol has been implemented by using rtProtoDV, a defined protocol in ns2.
2. The simulation has been done on a custom topology consisting of 11 nodes.