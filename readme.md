Starting with the fsw VM provided in by the professor

Open Terminal and type the following commands:
cd ~/cfs
rm apps -r -f
git clone https://github.com/cjohn5/apps.git apps

Follow the readme and deployment guide ~/cfs/docs/cFS Deployment Guide.pdf:
	- Add app to the makefile
		edit cfs/build/cpu1/Makefile to with line (after the other similar lines)
			THE_APPS += tc
	- Add tables to the makefile
		edit cfs/build/cpu1/Makefile to with line (after the other similar lines)
			THE_TBLS += tc
	- Add app to startup script
		edit cfs/build/cpu1/exe/cfe_es_startup.scr
			CFE_APP, /cf/apps/tc.so,     TC_AppMain,    TC,     100, 8192,  0x0, 0;

Update Code:
cd ~/cfs/apps
git pull

Build Process:
cp ~/cfs/apps/tc/fsw/platform_inc/tc_msgids.h ~/cfs/build/cpu1/inc/
cp ~/cfs/apps/tc/fsw/tables/tc_tbldefs.h ~/cfs/build/cpu1/inc/
cp ~/cfs/apps/whe/fsw/src/whe_msg.h ~/cfs/build/cpu1/inc/
cd ~/cfs
. ./setvars.sh
cd ~/cfs/build/cpu1
make config
make clean
make

Run the Application:
cd ~/cfs/build/cpu1/exe
sudo ./core-linux.bin --reset=PO

Launch COSMOS:
cd cfs_demo
ruby Launcher
