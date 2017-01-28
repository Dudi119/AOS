PROJ_DIR = $(HOME)/Projects/AOS
# Compiler/Linker
###################################################
CC  = clang
LINKER = ld
# Compiler options
CCOPTS  = -fno-stack-protector -m32 -march=i386 --target=i386-elf -g -std=c++11
LINKER_SCRIPT = $(PROJ_DIR)/Linker.ld
LINKEROPTS = -nostdlib -melf_i386 --script=$(LINKER_SCRIPT)
# External Libraries
###################################################
LIBC_OBJ = $(PROJ_DIR)/Runtime/Newlib/libc.a
LIBM_OBJ = $(PROJ_DIR)/Runtime/Newlib/libm.a
LIBGCC = $(PROJ_DIR)/Runtime/Libgcc/libgcc.a
LIBCXX = $(PROJ_DIR)/Runtime/Libcxx/libc++.a $(PROJ_DIR)/Runtime/Libcxx/libc++abi.a

# Makefile recipes
###################################################
OS_OBJECTS = BootStrap.o KernelMain.o Syscalls.o StackProtector.o
INC_NEWLIB= $(PROJ_DIR)/Runtime/Newlib/include
INC_LIBCXX = $(PROJ_DIR)/Runtime/Libcxx/include
INC_MULTIBOOT = $(PROJ_DIR)/Src/MultiBoot

INCLUDES = -I$(INC_NEWLIB) -I$(INC_LIBCXX) -I$(INC_MULTIBOOT)
LIBS = $(LIBCXX) $(LIBC_OBJ) $(LIBGCC) 

AOS: $(OS_OBJECTS) 
	@echo "\n>> Building the OS binary"
	$(LINKER) $(LINKEROPTS) -o $@ $(OS_OBJECTS) $(LIBS)

%.o: %.cpp
	$(CC) $(INCLUDES) -c $(CCOPTS) $< -o $@ 
%.o: %.asm
	@echo "\n"
	nasm -f elf $< -o $@
