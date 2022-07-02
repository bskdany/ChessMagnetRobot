# CheckMate
An arduino script to make a robot play chess </br>
This was my first project in C++ and I did it to help a friend of mine with his high school graduaton project in robotics</br>
There are two rails that move a magnet underneath the chessboard.</br>

The script works like that:</br>
1) a calibration sequence is started to find the limits of the magnet movement</br>
2) the user gives the arduino a start and a finish square, that input is translated to coordinates</br>
3) the magnet moves to the start square and takes the chess piece</br>
4) a pathfinding algorithm chooses a way to move the piece from the start square to the end square</br>
5) tha magnet releases the chess piece and gets to the starting position</br>

This is the result</br>




https://user-images.githubusercontent.com/54674556/177008442-b942a64e-5fa5-4193-9050-2a8397daf6f4.mp4

This was made in three days after christmas while I was sick from covid so don't judge the bad quality lol
