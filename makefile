SRC=*.c
TARGET=ppm2bmp

$(TARGET): $(SRC)
	gcc -o $(TARGET) $(SRC)
