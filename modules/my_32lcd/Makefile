CC=arm-linux-gcc
CFLAGS=-Wall

#应用程序名
AN:=my_lcd

#应用程序生成的目标目录
DEST=/root/rootfs/home

#objects := $(patsubst %.c,%.o,$(wildcard *.c))
SRC  :=$(wildcard *.c ./sub/*.c)
OBJS := $(patsubst %.c,%.o,$(SRC))
DIR :=$(notdir $(SRC))

all : $(OBJS)
	@mkdir -p ${DEST}/$(AN)
	${CC} -o $(AN) $(OBJS)
	cp HZK32 $(AN) ${DEST}/$(AN)/
	@@rm -rf  .tmp_versions *.o *.mod.o *.mod.c  *.bak *.symvers *.markers *.unsigned *.order *~ .*.*.cmd .*.*.*.cmd

PHONY:clean
clean:
	rm -f *.o $(AN) *.bak *.symvers *.markers *.unsigned *.order *~ .*.*.cmd .*.*.*.cmd
