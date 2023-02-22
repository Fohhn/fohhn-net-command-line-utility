
CC = gcc

TARGET = fnet

SOURCE = main.c options.c cmd.c serial.c util.c decode.c udp.c

make:
	$(CC) -o $(TARGET) $(SOURCE)

debug:
	$(CC) -g -o $(TARGET) $(SOURCE)

clean: 	
	rm -v $(TARGET)







