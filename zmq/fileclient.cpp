#include <zmq.hpp>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
    // Create a ZeroMQ context
    zmq::context_t context(1);

    // Create a ZeroMQ socket for the client
    zmq::socket_t clientSocket(context, zmq::socket_type::req);

    // Connect the socket to the server
    clientSocket.connect("tcp://172.17.0.1:5555");
    std::cout << "Connected to server" << std::endl;
    
    //for (int i = 0; i < argc; i++) {
    // if (argc > 1) {    
    //	    std::cout << "HERE" << std::endl;
    //	    if (argv[1] == "send") {std::cout<< "YES" << std::endl;}
    //}

    if (argc > 1) {
	// Send a request to the server
	std::string request = "Sending file from client";
	zmq::message_t message(request.size());

	memcpy(message.data(), request.data(), request.size());

	clientSocket.send(message, zmq::send_flags::none);
	std::cout << "Message sent: Sending file from client." << std::endl;
	
	zmq::message_t success_message;
	clientSocket.recv(success_message, zmq::recv_flags::none);

	// Read the zip file into memory
	std::ifstream file("/home/companion/zmq.zip", std::ios::binary | std::ios::ate);
	std::streamsize fileSize = file.tellg();
	file.seekg(0, std::ios::beg);
	std::vector<char> fileData(fileSize);
	if (file.read(fileData.data(), fileSize)) {
		// send the zip file as a request to the server
		zmq::message_t filerequest(fileData.size());
		memcpy(filerequest.data(), fileData.data(), fileData.size());
		clientSocket.send(filerequest, zmq::send_flags::none);
	} else {
		std::cerr << "Failed to read the file" << std::endl;
	}
	return 0;
    } else {
    	// Send a request to the server
    	std::string request = "Requesting file from server";
    	zmq::message_t message(request.size());

    	memcpy(message.data(), request.data(), request.size());
    	clientSocket.send(message, zmq::send_flags::none);

	std::cout << "Message sent: Requesting file from server" << std::endl;
	
    	// Receive the zip file data from the server
	zmq::message_t text_reply;
	clientSocket.recv(text_reply, zmq::recv_flags::none);
	std::cout << "Reply received:" << text_reply << std::endl;
	
	zmq::message_t success_message;
	clientSocket.send(success_message, zmq::send_flags::none);

	zmq::message_t reply;
    	clientSocket.recv(reply, zmq::recv_flags::none);

    	// Process the received zip file data
    	std::ofstream file("received_from_server.txt", std::ios::binary);
    	file.write(static_cast<char*>(reply.data()), reply.size());
    	std::cout << "Received at client and saved" << std::endl;
    }
    return 0;
}

