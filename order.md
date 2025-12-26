# Project Specification: Modular C++ Gomoku (VS2022 Multi-file Project)

## 1. Project Overview
**Goal:** Develop a maintainable, Object-Oriented Gomoku game in C++14.
**Structure:** **Multi-file Project** (.h and .cpp separation).
**Target Environment:** Visual Studio 2022 (Windows Console).
**Core Design Pattern:** Separation of Concerns (MVC-like).
    * **Model:** `Board`, `Piece`
    * **View:** `Display`
    * **Logic:** `Referee` (Crucial for Rules)
    * **Controller:** `Game` (Scheduler)

## 2. Technical Constraints (Strict)
* **Memory Management:** Classic OOP. Use `new` and `delete`. **No Smart Pointers**.
* **Encoding:** Windows Console UTF-8 support (`system("chcp 65001")`).
* **Input:** Standard Keyboard only.
* **Main Function:** `main.cpp` must be minimal, only instantiating the `Game` scheduler and calling `run()`.

## 3. Class Design & Responsibilities (Detailed)

### 3.1 `Piece` Class (The Atom)
* **File:** `Piece.h`, `Piece.cpp`
* **Role:** Represents a single stone.
* **Members:** Color (Black/White), Coordinates (x, y).

### 3.2 `Board` Class (The Data Container)
* **File:** `Board.h`, `Board.cpp`
* **Role:** Manages the grid data state.
* **Members:** A 2D container (e.g., `Piece* grid[15][15]` or `int grid[15][15]`).
* **Methods:** `placePiece`, `getPieceType`, `isEmpty`, `clear`, `isFull`.
* **Note:** It should **NOT** handle printing/displaying (that's for `Display` class).

### 3.3 `Display` Class (The View)
* **File:** `Display.h`, `Display.cpp`
* **Role:** Handles all console output/rendering.
* **Methods:**
    * `drawBoard(const Board& board, const Piece* lastMove)`: 
        * Renders the grid using ASCII/Unicode (e.g., ┼, ●, ○).
        * **REQUIREMENT:** **Visually highlight the last move** (e.g., using brackets `[●]` or a different color) so the user can see where the opponent played.
    * `showMenu()`: Prints the start menu (1. PvP, 2. PvE).
    * `showResult(int winner)`: **REQUIREMENT:** Explicitly displays "Black Wins", "White Wins", or "Draw".
    * `showTimeout(int color)`: Display "Timeout! [Color] loses".
    * **Constraint:** Must handle Windows console color/clearing (`system("cls")`).

### 3.4 `Referee` Class (The Logic Engine)
* **File:** `Referee.h`, `Referee.cpp`
* **Role:** Enforces rules.
* **Methods:**
    * `checkWin(board, color)`: Standard 5-in-a-row.
    * `checkForbidden(board, piece)`: **REQUIREMENT: Black Player ONLY**.
        * **Rules:**
            1. **3-3:** Two simultaneous "Live Threes".
            2. **4-4:** Two simultaneous "Fours".
            3. **Long Chain (Overline):** 6 or more stones.
        * **Priority Logic:** **Win > Forbidden**. If a move creates 5 AND a Forbidden shape, it is a WIN. If it creates Forbidden WITHOUT 5, it is a LOSS.

### 3.5 `Player` Hierarchy (The Actors)
* **File:** `Player.h`, `Player.cpp`
* **Structure:** `Player` (Abstract Base) -> `HumanPlayer`, `AIPlayer`.
* **Role:**
    * `HumanPlayer`: Handles `cin` input and validation.
    * `AIPlayer`: **REQUIREMENT:** Basic AI for "Man vs Machine" mode. Calculates move based on scoring (Attack/Defense).
    * **Constraint:** Use virtual functions (`getMove`) and virtual destructors.

### 3.6 `Game` Class (The Scheduler/Controller)
* **File:** `Game.h`, `Game.cpp`
* **Role:** The central hub ("总调度").
* **Members:** Pointers to `Board`, `Display`, `Referee`, `Player* p1`, `Player* p2`.
* **Methods:**
    * `init()`: **REQUIREMENT:** Handles mode selection.
        * If (1) PvP: `p2 = new HumanPlayer(WHITE)`
        * If (2) PvE: `p2 = new AIPlayer(WHITE)`
    * `run()`: The main loop.
    * `switchTurn()`: Toggles current player.
    * `checkTime()`: **REQUIREMENT:** Checks if the current move took > 15 seconds. If yes, trigger Game Over.
    * **Destructor:** Manually `delete` all member pointers to prevent leaks.

## 4. Instructions for Generation
1.  **File Structure:** Output the code clearly separated by file blocks (e.g., `// --- Board.h ---`).
2.  **Implementation Order:**
    * Low-level: `Piece`, `Board`
    * View: `Display` (Ensure Last Move Highlight is implemented)
    * Logic: `Referee` (Ensure Forbidden Logic is implemented)
    * Actors: `Player`
    * Control: `Game` (Ensure Timer & Menu are implemented)
    * Entry: `main.cpp`
3.  **VS2022 Compatibility:** ensure `#define _CRT_SECURE_NO_WARNINGS` is in `main.cpp` and `HumanPlayer.cpp`.

**Generate the complete source code structure now, ensuring all requirements (Menu, Forbidden Hands, Last Move Highlight, Timer) are met.**