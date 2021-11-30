# Thermal Control (TC)  Application
## First Time Setup
Starting with the fsw VM provided in by the professor<br/>

Open Terminal and type the following commands:<br/>
```
cd ~/cfs
rm apps -r -f
git clone https://github.com/cjohn5/apps.git apps
```

Follow the readme and deployment guide ~/cfs/docs/cFS Deployment Guide.pdf:<br/>
- Add app to the makefile
	- edit `cfs/build/cpu1/Makefile` to add the line (after the other similar lines)
		`THE_APPS += tc`
	- edit `cfs/build/cpu1/Makefile` to add the line (after the other similar lines)
			`THE_TBLS += tc`
	- edit `cfs/build/cpu1/exe/cfe_es_startup.scr` 
			`CFE_APP, /cf/apps/tc.so,     TC_AppMain,    TC,     100, 8192,  0x0, 0;`

## Update Code to current repo state:
```
cd ~/cfs/apps
git pull
```
Note: you may need to run `git checkout .` to discard changes

## Build Process:
```
cp ~/cfs/apps/tc/fsw/platform_inc/tc_msgids.h ~/cfs/build/cpu1/inc/
cp ~/cfs/apps/tc/fsw/tables/tc_tbldefs.h ~/cfs/build/cpu1/inc/
cp ~/cfs/apps/whe/fsw/src/whe_msg.h ~/cfs/build/cpu1/inc/
cp ~/cfs/apps/whe/fsw/src/whe.h ~/cfs/build/cpu1/inc/
cp ~/cfs/apps/to_lab/fsw/platform_inc/to_lab_sub_table.h ~/cfs/build/cpu1/inc/
cd ~/cfs
. ./setvars.sh
cd ~/cfs/build/cpu1
make config
make clean
make
```

## Run the Application:
```
cd ~/cfs/build/cpu1/exe
sudo ./core-linux.bin --reset=PO
```
## Launch COSMOS:
```
cd cfs_demo
ruby Launcher
```
## Run the Aliveness Script
```
cd ~/cfs_demo/config
rm targets -r -f
git clone https://github.com/rnstoddard/COSMOS.git targets
```
(TODO)
