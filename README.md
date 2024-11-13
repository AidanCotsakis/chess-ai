# Chess AI Game

A chess game where players can challenge a custom Chess AI built in C, with a visual interface designed in Python using Pygame.

## **Features**

- **AI-Driven Gameplay**: Built with a C-based chess engine, the AI provides challenging gameplay.
- **Python-Powered UI**: Uses Pygame to offer a responsive and visually engaging chessboard for the player.
- **Player and AI Turn Management**: Tracks board states and updates the UI seamlessly between moves.
- **Multi-Step and Single-Step Move Options**: Toggle between move types with interactive feedback and customizable turns.

## **Requirements**

- **Python**: Version 3.7 or higher
- **Pygame**: For visual rendering, install via `pip install pygame`
- **GCC or compatible C compiler**: For compiling the AI engine

## **Setup Instructions**

1. Clone the repository and navigate to the project directory:

2. Compile the AI engine and run it (C code):
    ```bash
    gcc -o chessAI chess.c
    ```

3. Run the game interface:
    ```bash
    python playerMovement.py
    ```

## **How to Play**
- **Single-Step and Multi-Step Moves**: Select the desired mode with the "Multi-Step Move" button on the screen.
- **Move Pieces**: Click on pieces to select and place them on valid squares.
- **Game States**: Visual feedback indicates whose turn it is and highlights selected pieces.

## File Structure
- `chess.c`: Contains the AI logic and chess engine written in C.
- `playerMovement.py`: The main UI for the game, managing player interactions and visual updates.
- `Images/`: Contains piece images for rendering the board in Pygame.
- `data.txt`: Temporary file for board state exchange between AI and UI.