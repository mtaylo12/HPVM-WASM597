#include <zmq.hpp>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
    // Create a ZeroMQ context
    zmq::context_t context(1);

    // Create a ZeroMQ socket for the server
    zmq::socket_t serverSocket(context, zmq::socket_type::rep);

    // Bind the socket to a specific address
    serverSocket.bind("tcp://*:5555");
    std::cout << "Server listening on tcp://*:5555" << std::endl;

    while (true) {
        // Wait for a request from a client
        zmq::message_t request;
        serverSocket.recv(request, zmq::recv_flags::none);

        // Process the request
        std::string requestData(static_cast<char*>(request.data()), request.size());
        std::cout << "Received request: " << requestData << std::endl;
	if (requestData == "Requesting file from server") {
		std::string send_f = "Sending file from server";
        	zmq::message_t m1(send_f.size());

        	memcpy(m1.data(), send_f.data(), send_f.size());
		serverSocket.send(m1, zmq::send_flags::none);

		std::cout << "Message sent: Sending file from server" << std::endl;
		
		zmq::message_t success_message;
        	serverSocket.recv(success_message, zmq::recv_flags::none);

		std::string messageStr(static_cast<char*>(success_message.data()), success_message.size());
		std::cout << "Response received:" << success_message << std::endl;
		
        	// Read the zip file into memory
        	std::ifstream file("test.txt", std::ios::binary | std::ios::ate);
        	std::streamsize fileSize = file.tellg();
        	file.seekg(0, std::ios::beg);
        	std::vector<char> fileData(fileSize);
        	if (file.read(fileData.data(), fileSize)) {
        	    	// Send the zip file data as a reply to the client
        	    	zmq::message_t reply(fileData.size());
        	    	memcpy(reply.data(), fileData.data(), fileData.size());
        	    	serverSocket.send(reply, zmq::send_flags::none);
        	} else {
            		std::cerr << "Failed to read the file" << std::endl;
        	}
        } else {
        	std::string reply = "Requesting file from client";
        	zmq::message_t message(reply.size());
        	memcpy(message.data(), reply.data(), reply.size());
        	serverSocket.send(message, zmq::send_flags::none);
        	
        	zmq::message_t receive;
        	serverSocket.recv(receive, zmq::recv_flags::none);
        	
        	std::ofstream file("received_from_client.zip", std::ios::binary);
        	file.write(static_cast<char*>(receive.data()), receive.size());
        	if (receive.size() > 0) { std::cout << "Received at server and saved" << std::endl; }
        }
    }

    return 0;
}

