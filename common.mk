SHELL := /bin/bash
ifeq ($(DEBUG),true)
CC=g++ --std=c++2a -g
VERSION=debug
else
CC=g++ --std=c++2a
VERSION=release
endif

SRCS=$(wildcard *.cpp)

OBJS=$(SRCS:.cpp=.o)

DEPS=$(SRCS:.cpp=.d)

BIN:=$(addprefix $(BUILD_ROOT)/,$(BIN))

LINK_OBJ_DIR=$(BUILD_ROOT)/app/link_obj
DEP_DIR=$(BUILD_ROOT)/app/dep

$(shell mkdir -p $(LINK_OBJ_DIR))
$(shell mkdir -p $(DEP_DIR))

OBJS:=$(addprefix $(LINK_OBJ_DIR)/,$(OBJS))
DEPS:=$(addprefix $(DEP_DIR)/,$(DEPS))

# 待研究
LINK_OBJ=$(wildcard $(LINK_OBJ_DIR)/*.o)
LINK_OBJ+=$(OBJS)
# 疑问：如果上次编译之后

all:$(DEPS) $(OBJS) $(BIN)
ifneq ("$(wildcard $(DEPS))", "")
include $(DEPS)
endif

$(BIN):$(LINK_OBJ)
	@echo "------------------------build $(VERSION) mode--------------------------------!!!"
	$(CC) -o $@ $^ -lpthread

# 对这个%的匹配还是不懂
$(LINK_OBJ_DIR)/%.o:%.cpp
	$(CC) -I$(INCLUDE_PATH) -o $@ -c $(filter %.cpp,$^)

$(DEP_DIR)/%.d:%.cpp
	echo $(OBJS)
	echo -n $(LINK_OBJ_DIR)/ > $@
# 这里需要注意，因为gcc -MM生成的语句头中对于目标只有目标名称，不带地址，所以要自己加上地址

	gcc -I$(INCLUDE_PATH) -MM $^ >> $@
# 疑问：-I可以不要吗？ 这里$^换成$<是一样的把？







