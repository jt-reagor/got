TARGET = got.cpp
EXEC = got
CC = g++
TESTDIR = test

build: $(TARGET)
	$(CC) $(TARGET) -o $(EXEC)

run: 
	./$(EXEC)

clean:
	rm $(EXEC)
	rm -rf $(TESTDIR)

init: $(EXEC)
	./$(EXEC) init $(TESTDIR)