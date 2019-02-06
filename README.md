# [Battleship game](https://en.wikipedia.org/wiki/Battleship_(game))
This is a local multiplayer game, where there are 2 players (clients) and a server.
It is implemented in C, on a Linux environment.

### Developed in December 2017 - January 2018 by Paul TORTEL & Vincent COURSAC

### Notes:
To compile the project, you must compile with "-pthread".

Tutorial to play:
1/ Launch the project twice
2/ In one of the console, type "1" to host a game
	2.1/ Enter a port that you will use to listen for connections
3/ For the other console, type "2" to join a game
	3.1/ Enter the IP of your server (if you want to play in local, type "localhost")
	3.2/ Enter the corresponding port that you just entered previously when you started the server
4/ You are ready to play! Place your ships and good luck!
