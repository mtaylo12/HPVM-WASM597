all: server exec_client

server: fileserver.cpp
	em++ fileserver.cpp -o server -o server -L/home/mayat4/lib/ -Wl,-rpath=/home/mayat4/lib/ -lzmqpp -lzmq -I/home/mayat4/iaf/cppzmq/ -I/home/mayat4/iaf/libzmq/include/ -o server.html  --shell-file shell_minimal.html

exec_client: client/fileclient.cpp
	g++ client/fileclient.cpp -o exec_client -lzmqpp -lzmq -I/home/mayat4/iaf/cppzmq/ -std=c++11

clean:
	rm -rf exec_client server
