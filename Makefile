CC = g++
CFLAGS = -Wall -O2
TARGET = ouput
OBJS = test.cpp
LIBS = -lc

.c.o: 
	$(CC) $(CFLAGS) $(INCLUDE) -c $<  

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET) *.o *~


