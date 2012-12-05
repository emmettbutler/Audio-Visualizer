MAIN = Visualizer
SHAREDPATH = GLTools/src/
SHAREDINCPATH = GLTools/include/
INCDIRS = -I$(SHAREDINCPATH) -I$(SHAREDINCPATH)GL

LIBS = -lglut -lportaudio
CC = g++

all: $(MAIN).o graphics_helper.o externals
	$(CC) -g $(INCDIRS) -o $(MAIN) $(MAIN).o graphics_helper.o glew.o GLTools.o GLBatch.o GLTriangleBatch.o GLShaderManager.o $(LIBS)

$(MAIN).o : $(MAIN).cpp
	$(CC) $(INCDIRS) -c $(MAIN).cpp

graphics_helper.o : graphics_helper.cpp
	$(CC) $(INCDIRS) -c graphics_helper.cpp

audio_helper.o : audio_helper.cpp
	$(CC) $(INCDIRS) -c audio_helper.cpp

externals : GLShaderManager.o GLTriangleBatch.o GLBatch.o GLTools.o glew.o

GLShaderManager.o : $(SHAREDPATH)GLShaderManager.cpp
	$(CC) $(INCDIRS) -c $(SHAREDPATH)GLShaderManager.cpp
GLTriangleBatch.o : $(SHAREDPATH)GLTriangleBatch.cpp
	$(CC) $(INCDIRS) -c $(SHAREDPATH)GLTriangleBatch.cpp
GLBatch.o : $(SHAREDPATH)GLBatch.cpp
	$(CC) $(INCDIRS) -c $(SHAREDPATH)GLBatch.cpp
GLTools.o : $(SHAREDPATH)GLTools.cpp
	$(CC) $(INCDIRS) -c $(SHAREDPATH)GLTools.cpp
glew.o : $(SHAREDPATH)glew.c
	$(CC) $(INCDIRS) -c $(SHAREDPATH)glew.c


clean:
	rm -f *.o .*.un~ .*.swp $(MAIN)
