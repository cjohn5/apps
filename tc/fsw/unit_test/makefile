##############################################################################
## File:
##   $Id: makefile 1.1 2016/06/14 16:47:23EDT czogby Exp  $
##
## Purpose:
##   Makefile for building the UT-Assert tc unit tests
##
##   $Log: makefile  $
##   Revision 1.1 2016/06/14 16:47:23EDT czogby 
##   Initial revision
##   Member added to project /FSW-TOOLs-REPOSITORY/ut-assert/tc/fsw/unit_test/project.pj
##
##############################################################################
## GNU Makefile for building UT unit tests

#
# Supported MAKEFILE targets:
#   clean - deletes object files, executables, output files, and gcov files
#   all   - makes tc_testrunner.exe
#   run   - runs tc_testrunner.exe
#   gcov  - prints a GCOV coverage report (make all, make run, make gcov)
#
# GCOV is disabled by default.  If you are using the source level debugger you will want to 
# disable GCOV.  To enable GCOV you can override the ENABLE_GCOV variable on the command line 
# by setting it to TRUE.  For example "make ENABLE_GCOV=TRUE".
#

#
# VPATH specifies the search paths for source files outside of the current directory.  Note that
# all object files will be created in the current directory even if the source file is not in the 
# current directory.
#
VPATH := .
VPATH += /home/fsw/cfs/tools/ut_assert/src
VPATH += /home/fsw/cfs/tools/ut_assert/inc
VPATH += /home/fsw/cfs/apps/cfs_lib/fsw/src
VPATH += ../src
VPATH += ../tables
VPATH += ../for_build

CFE_PATH=/home/fsw/cfs/cfe/fsw/cfe-core
OSAL_PATH=/home/fsw/cfs/osal
PSP_PATH=/home/fsw/cfs/psp
APPS_PATH=/home/fsw/cfs/apps
TC_PATH=$(APPS_PATH)/tc
WISE_PATH=$(APPS_PATH)/wise

#
# INCLUDES specifies the search paths for include files outside of the current directory.  
# Note that the -I is required. 
#
INCLUDES := -I.
INCLUDES += -I/home/fsw/cfs/tools/ut_assert/inc
INCLUDES += -I/home/fsw/cfs/build/cpu1/inc
INCLUDES += -I/home/fsw/cfs/apps/cfs_lib/fsw/public_inc
INCLUDES += -I$(CFE_PATH)/os/inc
INCLUDES += -I$(CFE_PATH)/src/inc
INCLUDES += -I$(CFE_PATH)/src/time
INCLUDES += -I$(CFE_PATH)/src/sb
INCLUDES += -I$(CFE_PATH)/src/es
INCLUDES += -I$(CFE_PATH)/src/evs
INCLUDES += -I$(CFE_PATH)/src/fs
INCLUDES += -I$(CFE_PATH)/src/tbl
INCLUDES += -I$(CFE_PATH)/../mission_inc
INCLUDES += -I$(CFE_PATH)/../platform_inc/cpu1
INCLUDES += -I$(OSAL_PATH)/src/os/inc
INCLUDES += -I$(OSAL_PATH)/build/inc
INCLUDES += -I$(PSP_PATH)/fsw/inc
INCLUDES += -I$(PSP_PATH)/fsw/pc-linux/inc
INCLUDES += -I$(PSP_PATH)/fsw/pc-linux/inc
INCLUDES += -I$(TC_PATH)/fsw/src
INCLUDES += -I$(TC_PATH)/fsw/mission_inc
INCLUDES += -I$(TC_PATH)/fsw/platform_inc
INCLUDES += -I$(TC_PATH)/fsw/tables
INCLUDES += -I$(WISE_PATH)/fsw/src
INCLUDES += -I$(WISE_PATH)/fsw/mission_inc
INCLUDES += -I$(WISE_PATH)/fsw/platform_inc

#
# FLIGHT_OBJS specifies flight software object files.
#
FLIGHT_OBJS := tc_app.o

#
# UT_OBJS specifies unit test object files.
#
UT_OBJS := ut_osapi_stubs.o
UT_OBJS += ut_osfileapi_stubs.o
UT_OBJS += ut_cfe_sb_stubs.o
UT_OBJS += ut_cfe_es_stubs.o
UT_OBJS += ut_cfe_evs_stubs.o
UT_OBJS += ut_cfe_time_stubs.o
UT_OBJS += ut_cfe_fs_stubs.o
UT_OBJS += ut_cfe_psp_memutils_stubs.o
UT_OBJS += ut_cfe_tbl_stubs.o
UT_OBJS += ut_cfe_sb_hooks.o
UT_OBJS += ut_cfe_es_hooks.o
UT_OBJS += ut_cfe_evs_hooks.o
UT_OBJS += ut_cfe_tbl_hooks.o
UT_OBJS += ut_cfe_time_hooks.o
UT_OBJS += utassert.o
UT_OBJS += utlist.o
UT_OBJS += uttest.o
UT_OBJS += uttools.o
UT_OBJS += tc_app_test.o
UT_OBJS += tc_testrunner.o
UT_OBJS += tc_test_utils.o
UT_OBJS += tc_iload_utils.o
UT_OBJS += tc_iloads.o
UT_OBJS += cfs_utils.o

#
# UT_TEST_RUNNER specifies the name of the test runner.
#
UT_TEST_RUNNER = tc_testrunner.exe

###############################################################################

COMPILER=gcc
LINKER=gcc

#
# Compiler and Linker Options
#
GCOV_COPT = -fprofile-arcs -ftest-coverage -pg -p
GCOV_LOPT = -pg -p -fprofile-arcs -ftest-coverage -lgcov -lm

#WARNINGS = -Wall -W -ansi -Werror -Wstrict-prototypes -Wundef
WARNINGS = -Wall -Wstrict-prototypes -Wundef
DEBUGGER = -g

COPT = $(WARNINGS) $(DEBUGGER) $(GCOV_COPT) -DSOFTWARE_LITTLE_BIT_ORDER -D_EL -D_ix86_ -DUT_VERBOSE

LOPT = $(GCOV_LOPT)

###############################################################################
## Rule to make the specified TARGET
##
%.exe: %.o
	$(LINKER) $(LOPT) $^ -o $*.exe

###############################################################################
##  "C" COMPILER RULE
##
%.o: %.c
	$(COMPILER) -c $(COPT) $(INCLUDES) $<

##############################################################################
##

all:$(UT_TEST_RUNNER)

$(UT_TEST_RUNNER): $(UT_OBJS) $(FLIGHT_OBJS)

clean ::
	rm -f *.o *.exe *.gcda *.gcno *.gcov gmon.out

run ::
	./$(UT_TEST_RUNNER)

gcov ::
	@echo
	@gcov $(FLIGHT_OBJS:.o=.gcda) | sed 'N;s/\n/ /' | \
		sed -n '/File/p' | sed '/ads/d' | sed -e '/\.h/d'  | \
		sed 's/ Lines executed:/ /; s/File/gcov:/; s/of// '
	@rm -f *.gcda *.gcno
	@echo

# end of file
