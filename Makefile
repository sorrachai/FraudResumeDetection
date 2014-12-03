CPPFLAGS=-std=c++11 -g -O0 -Wall

CC=c++
DEPS = resume_detection.h resume.h utilities.h edge.h pair_section.h
OBJ = main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CPPFLAGS)

FraudDetector: $(OBJ)
	$(CC) -o $@ $^ $(CPPFLAGS)

clean:
	rm -f *.o FraudDetector

