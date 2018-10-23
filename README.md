# SketchLearn

## OpenVSwitch

1. Install SketchLearn extensions for OpenVSwitch 2.7.3

Move all files in openvswtich/ to your OpenVSwitch directory

* openvswitch/datapath includes kernel-based OpenVSwitch Datapath extension
* openvswitch/lib includes OpenVSwitch DPDK extension

2. Compile user-space data plane programs

Run make, two files will be generated:

* *dp\_ovs* extracts packets from kernel-based Datapath and updates SketchLearn multi-level sketch
* *dp\_ovs* extracts packets from OVS-DPDK and updates SketchLearn multi-level sketch

## Simulation

Run make to generate dp\_simulate, it reads packets from traces and updates SketchLearn multi-level sketch

## P4

all in p4/ directory, it can be compiled and executed in bmv2 target

## Controller

Run make to generate the controller program, it reads sketch results specified in config.ini and performs inference and queries

