Continuation of Middle Ages project. Added:
 - unit tests in Cmocka
 - AI
 - Bash script to comunicate two players (supports Human vs Human, Human vs AI or AI vs AI).
 
Usage of bash script: </br>
type 
```
./game.sh -n NUM -k NUM -s NUM -p1 (NUM, NUM) -p2 (NUM, NUM) -ai1 PATH_TO_PROGRAM -ai2 PATH_TO_PROGRAM
```
All of the flags are optional. If you will not include f.e. '-ai1' flag, Player 1 will be human. Flag meaning:
 - `-n` size of board (board is a square)
 - `-k` max num. of rounds
 - `-s` delay between command which would be send by AI
 - `-p1` Player 1 start coordinates
 - `-p2` Player 2 start coordinates
 - `-ai1` path to AI which will be Player 1
 - `-ai2` same as above, but for Player 2

`NUM` should be an integer.</br></br>
For more info look into Middle-Ages project.

Author: Arkadiusz Tomczak</br>
Version: 1.0</br>
Date: 30-06-2016</br>
Language:
 - Program language: English
 - Variable names: English
 - Comments: English
