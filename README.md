# Pacman

A pacman game loaded with weapons and fully customizable with lots of themes :)

# Installation

1. Clone the repository 
2. Run the script `chmod +x install_script.sh && sudo ./install_script.sh`
3. Build the game using `make`

**Note:** The install script has been written for a ubuntu system and will not work on windows system. Try running the script in WSL instead.

# Usage

Run `make run` in the same directory and a game window will popup. The game is presented with several options to choose from 1 player or 2 player or change the settings.

To run in single player mode, choose **1 player**. For 2 player mode choose **2 player** mode, this will spawn a server on the window that was launched initially. To connect to the server using a client run `make run args=<IP>` where `<IP>` is the IP address of the machine on the network. A similar window will spawn and choose 2 player in the newly spawned window to automatically connect to the server. The 2 player game will now start on both sides and the players can enjoy the game. :)

# Default Controls

1. Use arrow keys to navigate through the maze.
2. Every player has two sets of weapons which can be switched by pressing `1` or `2`.
3. The weapons can be fired by pressing `F` and reloaded by pressing `R`.
4. The second player controls the enemies and can switch between enemies by pressing `Tab`.
