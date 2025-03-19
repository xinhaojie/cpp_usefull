############################### 编译系统定义 ########################################
COMPILE_DATE = $(shell date +"%Y%m%d")
COMPILE_TIME = $(shell date +"%Y-%m-%d %H:%M:%S")

HOST_ARCH = $(shell uname -m)
ifeq ($(V), 1)
Q =
else
Q = @
endif

DEBUG ?= 0
CROSS_COMPILER :=

CP = cp
MV = mv
MD = mkdir -p
RM = rm -rf
TAR = tar
LN = ln
CD = cd
CHMOD = chmod

HOSTCC = cc
HOSTLD = gcc
HOSTGCC = gcc

CPP = $(CROSS_COMPILE)g++
CC = $(CROSS_COMPILE)cc
LD = $(CROSS_COMPILE)g++
GCC = $(CROSS_COMPILE)gcc
STRIP = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
############################### 编译路径定义 ########################################
ROOT_DIR := $(shell pwd)
OUT_DIR = $(ROOT_DIR)/out

############################### 编译路径定义 ########################################
BASE64_DIR = $(ROOT_DIR)/base64
ENCRYPT_DIR = $(ROOT_DIR)/encrypt
INI_DIR = $(ROOT_DIR)/ini
JSON_DIR = $(ROOT_DIR)/json
LOG_DIR = $(ROOT_DIR)/log
NET_DIR = $(ROOT_DIR)/net
SQLITE3_DIR = $(ROOT_DIR)/sqlite3
SYSTEM_TOOLS_DIR = $(ROOT_DIR)/system
FLAG_DIR = $(ROOT_DIR)/flag
TIME_DIR = $(ROOT_DIR)/time
RANDOM_DIR = $(ROOT_DIR)/rand
TASK_DIR = $(ROOT_DIR)/task
PATH_DIR = $(ROOT_DIR)/path
FS_DIR = $(ROOT_DIR)/fs
STRING_DIR = $(ROOT_DIR)/string
UNITTEST_DIR = $(ROOT_DIR)/unit_test
############################### 编译头文件定义 #######################################
INCLUDE_ALL = -I$(BASE64_DIR) -I$(ENCRYPT_DIR) -I$(INI_DIR) -I$(JSON_DIR) -I$(JSON_DIR)/nlohmann
INCLUDE_ALL += -I$(LOG_DIR) -I$(NET_DIR) -I$(SQLITE3_DIR) -I$(SYSTEM_TOOLS_DIR) -I$(FLAG_DIR) -I$(TIME_DIR)
INCLUDE_ALL += -I$(RANDOM_DIR) -I$(TASK_DIR) -I$(PATH_DIR) -I$(FS_DIR)

############################### 编译参数定义 ########################################
CPPFLAGE_DEFINE = -std=c++17 -DDEBUG -g -Wall -Werror -Wno-unused-function
CFLAGE_DEFINE = -std=c11 -DDEBUG -g -Wall -Werror -Wno-unused-function

############################### 编译对象定义 ########################################
BASE64_OBJ = $(BASE64_DIR)/tiny_base64.o
BASE64_TEST_OBJ = $(BASE64_DIR)/test.o
ENCRYPT_OBJ = $(ENCRYPT_DIR)/encrypt.o
ENCRYPT_TEST_OBJ = $(ENCRYPT_DIR)/test.o
INI_OBJ = $(INI_DIR)/tiny_ini.o
INI_TEST_OBJ = $(INI_DIR)/test.o
JSON_OBJ = $(JSON_DIR)/json.o
JSON_TEST_OBJ = $(JSON_DIR)/test.o
LOG_OBJ = $(LOG_DIR)/tiny_logger.o
LOG_TEST_OBJ = $(LOG_DIR)/test.o
NET_OBJ = $(NET_DIR)/tiny_net.o
NET_TEST_OBJ = $(NET_DIR)/test.o
SQLITE3_OBJ = $(SQLITE3_DIR)/sqlite3.o
SQLITE3_TEST_OBJ = $(SQLITE3_DIR)/test.o
SYSTEM_OBJ = $(SYSTEM_TOOLS_DIR)/tiny_system.o
SYSTEM_TEST_OBJ = $(SYSTEM_TOOLS_DIR)/test.o
FLAG_OBJ = $(FLAG_DIR)/tiny_flag.o
FLAG_TEST_OBJ = $(FLAG_DIR)/test.o
TIME_OBJ = $(TIME_DIR)/tiny_time.o
TIME_TEST_OBJ = $(TIME_DIR)/test.o
RAND_OBJ = $(RANDOM_DIR)/tiny_rand.o
RAND_TEST_OBJ = $(RANDOM_DIR)/test.o
TASK_OBJ = $(TASK_DIR)/tiny_task.o
TASK_TEST_OBJ = $(TASK_DIR)/test.o
PATH_OBJ = $(PATH_DIR)/tiny_path.o
PATH_TEST_OBJ = $(PATH_DIR)/test.o
FS_OBJ = $(FS_DIR)/tiny_fs.o
FS_TEST_OBJ = $(FS_DIR)/test.o
STRING_OBJ = $(STRING_DIR)/tiny_string.o
STRING_TEST_OBJ = $(STRING_DIR)/test.o
UNITTEST_OBJ = $(UNITTEST_DIR)/tiny_unit.o
UNITTEST_TEST_OBJ = $(UNITTEST_DIR)/test.o

.PHONY: all start

usage:
	@echo "************************************************************************"
	@echo "'make all' to Build libtiny_tools, use 'make test' cmd build all test image"
	@echo "************************************************************************"
	@echo "* make all:      Build libtiny_tools"
	@echo "* make usage:    Get makefile info"
	@echo "* make clean:    Clean all"
	@echo "* make base64:   Build base64"
	@echo "* make encrypt:  Build encrypt"
	@echo "* make ini:      Build ini"
	@echo "* make json:     Build json"
	@echo "* make log:      Build log"
	@echo "* make net:      Build net"
	@echo "* make sqlite3:  Build sqlite3"
	@echo "* make system_tools:  Build system_tools"
	@echo "* make test:     Build all test"
	@echo "************************************************************************"
all:$(BASE64_OBJ) $(ENCRYPT_OBJ) $(INI_OBJ) $(JSON_OBJ) $(LOG_OBJ) $(NET_OBJ) $(SQLITE3_OBJ) $(SYSTEM_OBJ)
	$(Q) echo "    LD "$(notdir $@)
	$(Q) $(cPP) -shared -fPIC -o libtiny_tools.so $(BASE64_OBJ) $(ENCRYPT_OBJ) $(INI_OBJ) $(JSON_OBJ) $(LOG_OBJ) $(NET_OBJ) $(SQLITE3_OBJ) $(SYSTEM_OBJ)
base64:start $(BASE64_OBJ) $(BASE64_TEST_OBJ)
	$(Q) echo "    LD "$(notdir $@)
	$(Q) $(CPP) -o $(OUT_DIR)/base64_test $(BASE64_TEST_OBJ) $(BASE64_OBJ)

log:start $(LOG_OBJ) $(LOG_TEST_OBJ)
	$(Q) echo "    LD "$(notdir $@)
	$(Q) $(CPP) -o $(OUT_DIR)/log_test $(LOG_TEST_OBJ) $(LOG_OBJ) -lz

ini:start $(INI_OBJ) $(INI_TEST_OBJ)
	$(Q) echo "    LD "$(notdir $@)
	$(Q) $(CPP) -o $(OUT_DIR)/ini_test $(INI_TEST_OBJ) $(INI_OBJ)

net:start $(NET_OBJ) $(NET_TEST_OBJ)
	$(Q) echo "    LD "$(notdir $@)
	$(Q) $(CPP) -o $(OUT_DIR)/net_test $(NET_TEST_OBJ) $(NET_OBJ) -lpthread

flag:start $(FLAG_OBJ) $(FLAG_TEST_OBJ)
	$(Q) echo "    LD "$(notdir $@)
	$(Q) $(CPP) -o $(OUT_DIR)/flag_test $(FLAG_TEST_OBJ) $(FLAG_OBJ) -lpthread

time:start $(TIME_OBJ) $(TIME_TEST_OBJ)
	$(Q) echo "    LD "$(notdir $@)
	$(Q) $(CPP) -o $(OUT_DIR)/time_test $(TIME_TEST_OBJ) $(TIME_OBJ) -lpthread

rand:start $(RAND_OBJ) $(RAND_TEST_OBJ)
	$(Q) echo "    LD "$(notdir $@)
	$(Q) $(CPP) -o $(OUT_DIR)/rand_test $(RAND_TEST_OBJ) $(RAND_OBJ) -lpthread

task:start $(TASK_OBJ) $(TASK_TEST_OBJ)
	$(Q) echo "    LD "$(notdir $@)
	$(Q) $(CPP) -o $(OUT_DIR)/task_test $(TASK_TEST_OBJ) $(TASK_OBJ) -lpthread

path:start $(PATH_OBJ) $(PATH_TEST_OBJ)
	$(Q) echo "    LD "$(notdir $@)
	$(Q) $(CPP) -o $(OUT_DIR)/path_test $(PATH_TEST_OBJ) $(PATH_OBJ) -lpthread

fs:start $(FS_OBJ) $(FS_TEST_OBJ)
	$(Q) echo "    LD "$(notdir $@)
	$(Q) $(CPP) -o $(OUT_DIR)/fs_test $(FS_TEST_OBJ) $(FS_OBJ) -lpthread

system:start $(SYSTEM_OBJ) $(SYSTEM_TEST_OBJ)
	$(Q) echo "    LD "$(notdir $@)
	$(Q) $(CPP) -o $(OUT_DIR)/system_test $(SYSTEM_TEST_OBJ) $(SYSTEM_OBJ)

string:start $(STRING_OBJ) $(STRING_TEST_OBJ)
	$(Q) echo "    LD "$(notdir $@)
	$(Q) $(CPP) -o $(OUT_DIR)/string_test $(STRING_TEST_OBJ) $(STRING_OBJ)

unit_test:start $(UNITTEST_OBJ) $(UNITTEST_TEST_OBJ)
	$(Q) echo "    LD "$(notdir $@)
	$(Q) $(CPP) -o $(OUT_DIR)/unit_test $(UNITTEST_TEST_OBJ) $(UNITTEST_OBJ)
start:
	$(Q) echo "    Start Build Project"
	$(Q) $(RM) -rf $(OUT_DIR)
	$(Q) $(MD) $(OUT_DIR)
#######################################static ruls###############################################
$(OBJ_C):%.o: %.c
	@echo "    CC "$(notdir $@)
	$(Q) $(GCC) -c $< -o $@ $(CFLAGE_DEFINE) $(INCLUDE_ALL)

$(BASE64_OBJ) $(BASE64_TEST_OBJ):%.o: %.cpp
	@echo "    CPP "$(notdir $@)
	$(Q) $(CPP) -c $< -o $@ $(CPPFLAGE_DEFINE) $(INCLUDE_ALL)

$(ENCRYPT_OBJ) $(ENCRYPT_TEST_OBJ):%.o: %.cpp
	@echo "    CPP "$(notdir $@)
	$(Q) $(CPP) -c $< -o $@ $(CPPFLAGE_DEFINE) $(INCLUDE_ALL)

$(INI_OBJ) $(INI_TEST_OBJ):%.o: %.cpp
	@echo "    CPP "$(notdir $@)
	$(Q) $(CPP) -c $< -o $@ $(UPDATE_CFALG) $(INCLUDE_ALL)

$(JSON_OBJ) $(JSON_TEST_OBJ):%.o: %.cpp
	@echo "    CPP "$(notdir $@)
	$(Q) $(CPP) -c $< -o $@ $(CPPFLAGE_DEFINE) $(INCLUDE_ALL)

$(LOG_OBJ) $(LOG_TEST_OBJ):%.o: %.cpp
	@echo "    CPP "$(notdir $@)
	$(Q) $(CPP) -c $< -o $@ $(CPPFLAGE_DEFINE) $(INCLUDE_ALL)

$(NET_OBJ) $(NET_TEST_OBJ):%.o: %.cpp
	@echo "    CPP "$(notdir $@)
	$(Q) $(CPP) -c $< -o $@ $(CPPFLAGE_DEFINE) $(INCLUDE_ALL)

$(SQLITE3_OBJ) $(SQLITE3_TEST_OBJ):%.o: %.c
	@echo "    CC "$(notdir $@)
	$(Q) $(GCC) -c $< -o $@ $(CFLAGE_DEFINE) $(INCLUDE_ALL)

$(SYSTEM_OBJ) $(SYSTEM_TEST_OBJ):%.o: %.cpp
	@echo "    CPP "$(notdir $@)
	$(Q) $(CPP) -c $< -o $@ $(CPPFLAGE_DEFINE) $(INCLUDE_ALL)

$(FLAG_OBJ) $(FLAG_TEST_OBJ):%.o: %.cpp
	@echo "    CPP "$(notdir $@)
	$(Q) $(CPP) -c $< -o $@ $(CPPFLAGE_DEFINE) $(INCLUDE_ALL)

$(TIME_OBJ) $(TIME_TEST_OBJ):%.o: %.cpp
	@echo "    CPP "$(notdir $@)
	$(Q) $(CPP) -c $< -o $@ $(CPPFLAGE_DEFINE) $(INCLUDE_ALL)

$(RAND_OBJ) $(RAND_TEST_OBJ):%.o: %.cpp
	@echo "    CPP "$(notdir $@)
	$(Q) $(CPP) -c $< -o $@ $(CPPFLAGE_DEFINE) $(INCLUDE_ALL)

$(TASK_OBJ) $(TASK_TEST_OBJ):%.o: %.cpp
	@echo "    CPP "$(notdir $@)
	$(Q) $(CPP) -c $< -o $@ $(CPPFLAGE_DEFINE) $(INCLUDE_ALL)

$(PATH_OBJ) $(PATH_TEST_OBJ):%.o: %.cpp
	@echo "    CPP "$(notdir $@)
	$(Q) $(CPP) -c $< -o $@ $(CPPFLAGE_DEFINE) $(INCLUDE_ALL)

$(FS_OBJ) $(FS_TEST_OBJ):%.o: %.cpp
	@echo "    CPP "$(notdir $@)
	$(Q) $(CPP) -c $< -o $@ $(CPPFLAGE_DEFINE) $(INCLUDE_ALL)

$(STRING_OBJ) $(STRING_TEST_OBJ):%.o: %.cpp
	@echo "    CPP "$(notdir $@)
	$(Q) $(CPP) -c $< -o $@ $(CPPFLAGE_DEFINE) $(INCLUDE_ALL)

$(UNITTEST_OBJ) $(UNITTEST_TEST_OBJ):%.o: %.cpp
	@echo "    CPP "$(notdir $@)
	$(Q) $(CPP) -c $< -o $@ $(CPPFLAGE_DEFINE) $(INCLUDE_ALL)

.PHONY:clean usage install
clean:
	@echo "Clean Project"
	$(Q) -rm -rf $(OUT_DIR)
	$(Q) -rm -rf $(BASE64_OBJ) $(BASE64_TEST_OBJ)
	$(Q) -rm -rf $(ENCRYPT_OBJ) $(ENCRYPT_TEST_OBJ)
	$(Q) -rm -rf $(INI_OBJ) $(INI_TEST_OBJ)
	$(Q) -rm -rf $(JSON_OBJ) $(JSON_TEST_OBJ)
	$(Q) -rm -rf $(LOG_OBJ) $(LOG_TEST_OBJ)
	$(Q) -rm -rf $(NET_OBJ) $(NET_TEST_OBJ)
	$(Q) -rm -rf $(SQLITE3_OBJ) $(SQLITE3_TEST_OBJ)
	$(Q) -rm -rf $(SYSTEM_OBJ) $(SYSTEM_TEST_OBJ)
	$(Q) -rm -rf $(FLAG_OBJ) $(FLAG_TEST_OBJ)
	$(Q) -rm -rf $(TIME_OBJ) $(TIME_TEST_OBJ)
	$(Q) -rm -rf $(RAND_OBJ) $(RAND_TEST_OBJ)
	$(Q) -rm -rf $(TASK_OBJ) $(TASK_TEST_OBJ)
	$(Q) -rm -rf $(PATH_OBJ) $(PATH_TEST_OBJ)
	$(Q) -rm -rf $(FS_OBJ) $(FS_TEST_OBJ)
	$(Q) -rm -rf $(STRING_OBJ) $(STRING_TEST_OBJ)
	$(Q) -rm -rf $(UNITTEST_OBJ) $(UNITTEST_TEST_OBJ)