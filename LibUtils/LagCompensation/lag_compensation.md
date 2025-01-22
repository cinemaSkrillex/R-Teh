# Lag compensation library

## Description

This library is used to compensate for the lag between the client and the server. It is used to predict the position of the player on the server side based on the position of the player on the client side.

Two methods are available:
- **Interpolation**: This method is used to predict the position of the player on the server side based on the position of the player on the client side. It is used to smooth the movement of the player.
- **Extrapolation**: This method is used to predict the position of the player on the server side based on the position of the player on the client side. It is used to predict the position of the player when the player is moving at a high speed.

## This library is compiled in the CmakeLists.txt file of the "LibUtils" library.