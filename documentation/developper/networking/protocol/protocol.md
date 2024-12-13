# Rtype Server Protocol

## Overview

This document describes the protocol used by the R-type server to communicate with clients. The protocol defines various events that can be called to manage the game state and synchronize between the server and clients.

## Usage

The server and clients use these events to communicate and synchronize the game state. The server processes incoming messages from clients, updates the game state, and broadcasts the necessary events to all clients to keep them in sync.

## A simple example

A simple example is provided inside [simple_client.cpp](./simple_client.cpp). It will need UDP Client as dependencies. You could also simply look at the main.cpp of the [rtype_game](../../../../rtype_game/main.cpp) project.

## Events

### New_client

**Description**: This event is triggered when a new client connects to the server. It notifies all other clients about the new client and synchronizes the new client with the current game state.

**Message Format**: Event:New_client Uuid:<UUID> Position:(x,y)

**Example**:
Event:New_client Uuid:123456789 Position:(50.0,250.0)

### Synchronize

**Description**: This event is used to synchronize the state between the server and the client. It sends the current game state to the new client, including the positions of all players.

**Message Format**: Event:Synchronize Uuid:<UUID> Clock: timestamp Position:(x,y) Players:[<UUID1>,(x1,y1)|<UUID2>,(x2,y2)]

**Example**: Event:Synchronize Uuid:123456789 Clock:1627891234 Position:(50.0,250.0) Players:[987654321,(100.0,200.0)|123456789,(50.0,250.0)]


### Player_position

**Description**: This event is used to update the player's position. It sends the updated position of a player to all clients.

**Message Format**: Event:Player_position Uuid:<UUID> Position:(x,y)

**Example**: Event:Player_position Uuid:7038767116 Step:0.051000 Position:(401.1,265.4)

## What about security against Hackers?

The client is only sending the direction of his movement, therefore the client has no control over the position of the player. The server is the only one that have authority over the position of the player.

## Additional Information

For more details on how the server and clients handle these events, refer to the respective implementation files in the codebase.

The server and clients are written in C++ using the Boost.Asio library for networking.