FLAGS=-std=c++17 -lpthread -ldl
INCLUDE_ARGS=-I./thirdparty/json -I./thirdparty/sqlite -I./thirdparty/sqlite_modern_cpp/hdr
SHARED_FILES=./shared/network.cpp
SERVER_FILES=./server/client_map.cpp ./server/database.cpp ./server/main.cpp ./server/server.cpp
CLIENT_FILES=./client/client.cpp ./client/main.cpp ./client/network.cpp ./client/user_interface.cpp ./client/data_structure/cond_message_queue.cpp ./client/ui/contact_window.cpp ./client/ui/utils.cpp ./client/data_structure/message_queue.cpp ./client/ui/login_window.cpp ./client/ui/chat_window.cpp ./client/ui/register_window.cpp

all : server client

server : $(SHARED_FILES) $(SERVER_FILES) sqlite3.o
	g++ -o ./bin/server $(FLAGS) $(INCLUDE_ARGS) $^ 

client : $(SHARED_FILES) $(CLIENT_FILES)
	g++ -o ./bin/client $(FLAGS) $(INCLUDE_ARGS) $^ `pkg-config gtkmm-3.0 --cflags --libs`

sqlite3.o : ./thirdparty/sqlite/sqlite3.c
	gcc -c $^
