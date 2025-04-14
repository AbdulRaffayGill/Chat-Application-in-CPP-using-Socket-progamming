# Chat-Application-in-CPP-using-Socket-progamming
A simple Chat Application in C++ using socket programming , it uses server client communication system to establish a connection and then allows them to communicate.
It is primarily for my oop project.
How does it works:
First , server side code stores all type of information in a struct (like IP family, how many devices can connect at a time and which type of IP adresses are allowed), then it binds the server socket to the struct. 
Next thing is another socket that establishes connection with the client and is responsible for actuall communication. It accepts any client request and upon connecting send the user name it wants on both devices to show while chatting.
If you want to run it, you need to first compile it externally, but first change the ip address to your IPv4 ip (if you want to make server to your machine) in the client class.Since Winsock will produce error if you compile it in gcc using VS-code or any other IDE.
Here is the command:
"g++ SC_Chat_app.cpp -o SC_Chat_app.exe -lws2_32"
After compiling ;
For communication , First open compiled file , click "I want to listen" on your machine and "I want to connect" on other machine. On client , it will be shown as "Raffay connected". you can change it your name, also in take_mess() method.
if any of you wants to end communication , they may send ";" and end communcation;
