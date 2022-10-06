# Proxy-server-Client
A proxy Server - client model developed using C language


Running the entire proxy-server client model begins from the server.
The server creates a socket to be able to communicate with the client. It prepares to receive connections from  a port that is specified by the user.
The server waits for a client to log in. Once logged in, he will create a copy of himself in the memory through the fork, which will read the client's message and serve it.

The client receives from the command line the name or address of the proxy server along with the port it listens to. It checks if there is a server and  connect with it.
It gets three commands from user (GET or GETNEW url)
Or (EXIT), which sends them to the proxy server. If the command given is EXIT terminates. Otherwise it expects to receive a response from the proxy server with the html.

The proxy server reads the message sent by the client and checks which command has received. If the client gave the GET command then he searches to find if there is near the executable (in the same dir) the requested html. If the server finds, it reads the file and returns it using socket back to the client.If there is not suitable html then calls GETNEW (which I'll analyze below). If the client gave the GETNEW command then it creates a http request message.
It also creates a tcp connection to the web server by sending a http request.
The webserver responds by sending an html. The html proxy server stores it in the same dir as the executable and sends it back to the client and kills the child serving the client.
The client in turn stores the html to be received in the same dir as the executable of the client with a name that will be requested by the user.
