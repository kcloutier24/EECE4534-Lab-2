ESL_ZEDBOARD_IP?=qemu
DEBUG_PORT?=1234
BIN_DEST?=/home/root
GDB?=arm-esl-linux-gnueabi-gdb
TARGET_ARGS?=

.PHONY: upload run kill debug debug-remote

upload: $(TARGET)
	@scp -q $^ root@$(ESL_ZEDBOARD_IP):$(BIN_DEST)

run: upload
	@ssh -q root@$(ESL_ZEDBOARD_IP) $(BIN_DEST)/$(TARGET) $(TARGET_ARGS)

kill:
	@ssh -q root@$(ESL_ZEDBOARD_IP) pkill -9 -f $(TARGET) || true

debug-remote: upload
	@ssh -q -f root@$(ESL_ZEDBOARD_IP) "bash -c 'nohup gdbserver --once :$(DEBUG_PORT) $(BIN_DEST)/$(TARGET) $(TARGET_ARGS) &> /dev/null < /dev/null &'"

debug: debug-remote
	$(GDB) -iex "target remote $(ESL_ZEDBOARD_IP):$(DEBUG_PORT)" $(TARGET)
