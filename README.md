# ChessMagnetRobot
### An arduino script to make a robot play chess </br> 

Blogpost: https://bskdany.com/posts/chess-robot/
</br>


This was my first project in C++ and I did it to help a friend of mine with his high school graduaton project in robotics</br>
There are two rails that move a magnet underneath the chessboard</br>

The script works like that:</br>
1) a calibration sequence is started to find the limits of the magnet movement</br>
2) the user gives the arduino a start and a finish square, that input is translated to coordinates</br>
3) the magnet moves to the start square and takes the chess piece</br>
4) a pathfinding algorithm chooses a way to move the piece from the start square to the end square</br>
5) tha magnet releases the chess piece and gets to the starting position</br>

This is the result (yeah we drew the chessboard unevenly)</br>






https://user-images.githubusercontent.com/54674556/177008927-394a58bb-d777-4074-8bcf-88b826973314.mp4




<br />
This was made in three days after christmas while I was sick from covid, not gonna lie it turned out pretty good
