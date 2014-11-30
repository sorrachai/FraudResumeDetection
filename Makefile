CC=c++
CFLAGS=-I.
DEPS = resume.h
OBJ = main.o  

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

FraudDetector: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o FraudDetector

