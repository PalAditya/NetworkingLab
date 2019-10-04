## NS2 Simulator

***Note: Sample Images in `Images/` directory***

### Instructions
To generate `.tr` and `.nam` files again (fresh)
```
ns test.tcl
```
To use existing `.nam` file (for simulation)
```
ns 
nam test.nam
```
To use the existing `.tr` file (for faster analysis)
```
python analysis.py
```
To use the existing `.tr` file (for analysis using awk)
```
chmod +x analysis.sh
./analysis.sh
```

### Working
1. The Topology is according to the problem statement given in lab
2. Upon running the tcl code, we get a `.tr` trace file for further analysis with `awk` and `.nam` file for running GUI
3. Run the `analysis.sh` with the `test.tr` and give some packets to run the calculations on.
4. There are calculations for node-to-node throughput, delay, packet count, protocol-wise throughput, delay, count and also avg calculations. The disadvantage of these clear calculations is processing time. Trace file has around 4411 packets and each packet has to go through the delay calculations in a serial way.
5. The analysis script has been written with the ideology to provide even individual packet calculations.