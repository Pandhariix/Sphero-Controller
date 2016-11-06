CC  	 = g++ -std=c++11 -O2 -Wall
SRC 	 = src/main.cpp src/controller.cpp
EXE 	 = SpheroController
LIBS_DIR = libs/
SDL_INC	 = SDL2-2.0.5/include
HEADERS  = src/controller.hpp
SDL_LIB	 = SDL2-2.0.5/build
LINK 	 = -lmingw32 -lSDL2main -lSDL2

all:
	echo 'Compiling...'

ifeq ($(OS),Windows_NT)
	$(CC) $(SRC) -o $(EXE) -I $(LIBS_DIR)$(SDL_INC) -I$(HEADERS) -L $(LIBS_DIR)$(SDL_LIB) $(LINK)

else
	echo 'not implemented for $(OS) for now'

endif

clean:

ifeq ($(OS),Windows_NT)
	del $(EXE).exe

else

endif