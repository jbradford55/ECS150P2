# Target library
lib := libuthread.a

all: $(lib)

## to control outputing terminal commands vs nice clean output
ifneq ($(V),1)
Q=@
endif

## obj files required by libuthread
objs := queue.o uthread.o context.o sem.o preempt.o


deps := $(patsubst %.o, %.d, $(objs))
-include $(deps)


#compiler is GCC
CC := gcc

##Compiler flags
CFlags := -Wall -Wextra -MMD

## main deliverable
libuthread.a: $(objs)
	@echo "CC $@"
	$(Q)ar rcs $@ $^

## generate any object files using the corresponding .c files
%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFlags)  -O  -c $@ $<

## remove libuthread.a, & all objects and dependencides
clean: 
	@echo "Clean"
	$(Q)rm -f $(lib) $(objs) $(deps)


