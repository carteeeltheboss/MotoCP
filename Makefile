CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wunused-parameter

########## macOS build ##########
# Normal mac build
LDFLAGS = -lSDL2 -lSDL2_net -lSDL2_ttf
SERVER = compResults/serverMACOS
CLIENT = compResults/clientMACOS
COMMON_DATA = src/serialisation.cpp src/game_items.cpp src/menu.cpp src/options.cpp
SERVER_SRC = src/server_mix3.cpp $(COMMON_DATA)
CLIENT_SRC = src/client_mix3.cpp $(COMMON_DATA)
TIMESTAMP = $(shell date +%Y%m%d_%H%M%S)

########## Windows cross-compile ##########
CXX_WIN = x86_64-w64-mingw32-g++
SERVER_WIN = compResults/serverWINOS.exe
CLIENT_WIN = compResults/clientWINOS.exe

# The folder where you extracted everything:
SDL2_WIN_PATH = SDL2-windows
# Where SDL.h is located:
SDL2_WIN_INCLUDE = $(SDL2_WIN_PATH)/SDL2/include
# Where SDL2.lib or libSDL2.a is located:
SDL2_WIN_LIB = $(SDL2_WIN_PATH)/SDL2/lib

# Where SDL_net.h is located:
SDL2_NET_WIN_INCLUDE = $(SDL2_WIN_PATH)/SDL2_net/include
SDL2_NET_WIN_LIB     = $(SDL2_WIN_PATH)/SDL2_net/lib

# Where SDL_ttf.h is located:
SDL2_TTF_WIN_INCLUDE = $(SDL2_WIN_PATH)/SDL2_ttf/include
SDL2_TTF_WIN_LIB     = $(SDL2_WIN_PATH)/SDL2_ttf/lib

# Linker flags for Windows
#WIN_LDFLAGS = -lSDL2 -lSDL2_net -lSDL2_ttf
WIN_LDFLAGS = -static-libgcc -static-libstdc++ -lwinpthread -lmingw32 \
              -lSDL2main -lSDL2 -lSDL2_net -lSDL2_ttf \
              -lsetupapi -lole32 -lwinmm -lws2_32 -lgdi32 -limm32
#WIN_LDFLAGS = -static -static-libgcc -static-libstdc++ -lwinpthread -lmingw32 -lSDL2main -lSDL2 -lSDL2_net -lSDL2_ttf
# Linker flags for Windows with static linking
#WIN_LDFLAGS = -static -lmingw32 -lSDL2main -lSDL2 -lSDL2_net -lSDL2_ttf
########## Build Rules ##########

all: $(SERVER) $(CLIENT) $(SERVER_WIN) $(CLIENT_WIN)
win: $(SERVER_WIN) $(CLIENT_WIN)
mac: $(SERVER) $(CLIENT)

# macOS build
$(SERVER): $(SERVER_SRC)
	@rm -f $(SERVER)
	@$(CXX) $(CXXFLAGS) -o $(SERVER) $(SERVER_SRC) $(LDFLAGS) \
    2>&1 | tee logs/server_build_$(TIMESTAMP).log

$(CLIENT): $(CLIENT_SRC)
	@rm -f $(CLIENT)
	@$(CXX) $(CXXFLAGS) -o $(CLIENT) $(CLIENT_SRC) $(LDFLAGS) \
    2>&1 | tee logs/client_build_$(TIMESTAMP).log

# Windows cross-compile
$(SERVER_WIN): $(SERVER_SRC)
	@rm -f $(SERVER_WIN)
	@$(CXX_WIN) $(CXXFLAGS) \
      -I$(SDL2_WIN_INCLUDE) -I$(SDL2_NET_WIN_INCLUDE) -I$(SDL2_TTF_WIN_INCLUDE) \
      -L$(SDL2_WIN_LIB) -L$(SDL2_NET_WIN_LIB) -L$(SDL2_TTF_WIN_LIB) \
      -o $(SERVER_WIN) $(SERVER_SRC) $(WIN_LDFLAGS) \
        2>&1 | tee logs/server_build_win_$(TIMESTAMP).log

$(CLIENT_WIN): $(CLIENT_SRC)
	@rm -f $(CLIENT_WIN)
	@$(CXX_WIN) $(CXXFLAGS) \
      -I$(SDL2_WIN_INCLUDE) -I$(SDL2_NET_WIN_INCLUDE) -I$(SDL2_TTF_WIN_INCLUDE) \
      -L$(SDL2_WIN_LIB) -L$(SDL2_NET_WIN_LIB) -L$(SDL2_TTF_WIN_LIB) \
      -o $(CLIENT_WIN) $(CLIENT_SRC) $(WIN_LDFLAGS) \
        2>&1 | tee logs/client_build_win_$(TIMESTAMP).log

clean:
	@rm -f $(SERVER) $(CLIENT) $(SERVER_WIN) $(CLIENT_WIN)

clean-logs:
	@rm -f logs/*.log
