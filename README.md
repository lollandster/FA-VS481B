# FA-VS481B
Remote controll box for VS481B
All code is arduino compatible. The project's using Arduino Nano (ATMega328P)

This project aims to add a remote control box to controll the VS481B switch. The switch uses RS232 for control, but for this project a longer distance is needed and two seperate controls has to be connected. 
I decided to use a master that communicates with the VS481B and is then communicating with slaves (the remotes) using RS-485. This way I can use as many slaves as I want.
