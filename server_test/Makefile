CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -lSDL2 -lSDL2_net -Wunused-parameter
SERVER = server
CLIENT = client
COMMON_DATA = serialisation.cpp game_items.cpp
SERVER_SRC = server.cpp $(COMMON_DATA)
CLIENT_SRC = client.cpp $(COMMON_DATA)
all: $(SERVER) $(CLIENT)
$(SERVER): $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) -o $(SERVER) $(SERVER_SRC)
$(CLIENT): $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) -o $(CLIENT) $(CLIENT_SRC)
clean:
	rm -f $(SERVER) $(CLIENT)
run-server: $(SERVER)
	./$(SERVER)
run-client: $(CLIENT)
	./$(CLIENT)
