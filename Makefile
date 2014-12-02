CPPFLAGS=-std=c++11 -g -O2 -Wall

CC=c++
DEPS = resume.h ResumeDetection.h utilities.h edge.h pair_section.h
OBJ = main.o  

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CPPFLAGS)

FraudDetector: $(OBJ)
	$(CC) -o $@ $^ $(CPPFLAGS)

clean:
	rm -f *.o FraudDetector

