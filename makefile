TARGET = got.cpp
EXEC = got
CC = g++

build: $(TARGET)
	$(CC) $(TARGET) -o $(EXEC)

run:
	./$(EXEC)