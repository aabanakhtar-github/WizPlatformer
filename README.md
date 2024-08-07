# Multiplayer Platformer Experiment

Basic Multiplayer platformer experiment game that has an auhoritative server, supports 10 clients. Uses the ENet, SDL2, and SDL_image libraries.

## Features
- ENet based UDP Networking 
- Salt based security (Packets, basic IP-Spoofing prevention)
- SDL2 based rendering
- Custom ECS framework (IDs, Packed Component Pools, System Managers, Scene Views,  etc) based on Austin Morlan Implementation

## Installation (Requires CMake 3.26, C++20 Compiler)

Linux, Windows, Mac not tested
```
    mkdir build
    cd build
    cmake -S .. 
    cmake --build .
```
Move the ```Assets``` directory to the location of the Client/Server binary(usually its ```build/Game/```)
On Windows, you also need to move the ENet, SDL, and SDL_image DLL files to the Client/Server binary directory(You can find these in the library build directories).

## Usage
### Server: 
``` 
    ./Server <port>
```
### Client:
``` 
    ./Client <server ip> <port>
```
Use 127.0.0.1 as the Server IP on the client if you want to run this locally
