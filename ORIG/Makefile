CC=gcc
CFLAGS=-Wall -O3
SOURCES=LinearRegressionCPUOnly.c
EXECUTABLE=LinearRegressionCPUOnly

defaults: run

$(EXECUTABLE):  
	$(CC) $(SOURCES) $(CFALGS) -o $(EXECUTABLE)

clean:
	rm $(EXECUTABLE)

run: $(EXECUTABLE) 
	./$(EXECUTABLE)




