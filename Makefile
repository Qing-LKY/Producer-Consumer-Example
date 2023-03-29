CFLAGS := -g -O2
LDFLAGS := -lpthread

TARGET := main
OBJ := args.o main.o

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJ):%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.phony: clean

clean:
	$(RM) $(OBJ) $(TARGET)