#目标文件
SRC    :=  $(shell find . -iname '*.c' -o -iname '*.cpp')

OBJ := $(patsubst %.c,%.o, $(patsubst %.cpp,%.o, $(SRC))) 
#编译选项


#程序名
PROGRAM = TestDemo_vpr_fujian


CC := gcc
CXX := g++
AR := ar
STRIP := strip
#C/C++代码的宏定义
CFLAG := -DLINUX  -D_GNU_SOURCE -Wall -O2 -fPIC

CXXFLAG := $(CFLAG)

#连接库路径
LIBPATH := ./lib
OUTPUT_PATH := ./bin

LDFLAG := -L$(LIBPATH)\
       -lpthread\
       -lrt\
       -ldl \
	   -lMGS_STDVLPR

INC := -I.
		
#编译规则
all:$(PROGRAM)

#debug version
debug:	CFLAG += -g3
debug:	$(PROGRAM)

#release version
release:$(PROGRAM)

#依赖关系
Q := @
W := -w
#编译规则
.c.o:
	@echo "Compile $@"
	$(Q)$(CC) $(CFLAG) -o $@ -c $< $(INC) $(W)

.cpp.o:
	@echo "Compile $@"
	$(Q)$(CXX) $(CXXFLAG) -o $@ -c $< $(INC) $(W)
	
	
$(PROGRAM):$(OBJ)
	$(Q)$(CXX) -o $(PROGRAM) $(OBJ) $(LDFLAG)
#	$(Q)rm *.o
	$(Q)mkdir -p $(OUTPUT_PATH)
	$(Q)cp $@ $(OUTPUT_PATH)/$@

clean:
	$(Q)rm -rf $(PROGRAM) $(OUTPUT_PATH)/$(PROGRAM) $(OBJ) 
