TARGET:=main
SRCS:=pwm_busy.c
OBJS:=$(SRCS:.c=.o)
INCLUDE_DIRS:=..
CFLAGS:=$(foreach incdir, $(INCLUDE_DIRS), -I$(incdir)) -g -O
LZED:=-lzed

# default rule to build target
all: $(TARGET)

include ../common/zed.mk
.PHONY: clean


$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LZED) -lm -O2

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(OBJS) $(TARGET)
