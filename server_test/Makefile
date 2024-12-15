CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wunused-parameter
LDFLAGS = -lSDL2 -lSDL2_net
SERVER = server
CLIENT = client
COMMON_DATA = serialisation.cpp game_items.cpp
SERVER_SRC = server_mix1.cpp $(COMMON_DATA)
CLIENT_SRC = client_mix1.cpp $(COMMON_DATA)
TIMESTAMP = $(shell date +%Y%m%d_%H%M%S)

all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_SRC)
	rm -f $(SERVER)
	$(CXX) $(CXXFLAGS) -o $(SERVER) $(SERVER_SRC) $(LDFLAGS) 2>&1 | tee logs/server_build_$(TIMESTAMP).log

$(CLIENT): $(CLIENT_SRC)
	rm -f $(CLIENT)
	$(CXX) $(CXXFLAGS) -o $(CLIENT) $(CLIENT_SRC) $(LDFLAGS) 2>&1 | tee logs/client_build_$(TIMESTAMP).log

clean:
	rm -f $(SERVER) $(CLIENT)

clean-logs:
	rm -f logs/*.log

run-server: $(SERVER)
	./$(SERVER)

run-client: $(CLIENT)
	./$(CLIENT)