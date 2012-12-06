MAIN = Visualizer
SHAREDPATH = GLTools/src/
SHAREDINCPATH = GLTools/include/
INCDIRS = -I$(SHAREDINCPATH) -I$(SHAREDINCPATH)GL

GRAPHICSLIBS = -lglut
AUDIOLIBS = -lportaudio -lsndfile
CC = g++

all: $(MAIN).o graphics_helper.o audio_helper.o externals
	$(CC) -g $(INCDIRS) -o $(MAIN) $(MAIN).o graphics_helper.o audio_helper.o glew.o GLTools.o GLBatch.o GLTriangleBatch.o GLShaderManager.o math3d.o $(GRAPHICSLIBS) $(AUDIOLIBS)

$(MAIN).o : $(MAIN).cpp
	$(CC) $(INCDIRS) -c $(MAIN).cpp

graphics_helper.o : graphics_helper.cpp
	$(CC) $(INCDIRS) -c graphics_helper.cpp

audio_helper.o : audio_helper.cpp
	$(CC) $(INCDIRS) -c audio_helper.cpp

audio_only :
	$(CC) audio_helper.cpp -o audio_main $(AUDIOLIBS)

externals : GLShaderManager.o GLTriangleBatch.o GLBatch.o GLTools.o glew.o math3d.o

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
math3d.o : $(SHAREDPATH)math3d.cpp
	$(CC) $(INCDIRS) -c $(SHAREDPATH)math3d.cpp


clean:
	rm -f *.o .*.un~ .*.swp $(MAIN)
