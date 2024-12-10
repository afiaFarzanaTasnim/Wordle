# **_Wordle_**

Wordle, a widely known game popularized by the New York Times, is a fun word-guessing game. In this game, players have a limited number of chances to guess a hidden word. Each time they guess a letter, they get feedback on whether it's:
- **Green**: Correct letter in the correct position.
- **Yellow**: Correct letter but in the wrong position.
- **Gray**: Incorrect letter.

The goal is to figure out the word by making smart guesses and using clues from previous tries. Doing well in Wordle is all about using brainpower to solve the puzzle with words.

---

## **How It Works**

<font size="4">
The gameplay mechanics of Wordle are as follows:
</font>

- Players start the game and are presented with a blank grid representing the number of attempts they have.
- When a player guesses a word (word length is 5):
  - **Green** tiles indicate the letter is correct and in the correct position.
  - **Yellow** tiles indicate the letter is correct but in the wrong position.
  - **Gray** tiles indicate the letter is not in the word.
- The game ends when:
  - The player successfully guesses the word within the given attempts.
  - The player exhausts all attempts without guessing the word.
- Scoring is implemented in the game as follows:
  - If the word is guessed at first try then the players' score is 12.
  - With each increasing attempt the score decreases by 2 and the game ends at the 6th attempt when the score is 0.

---

## **Features**

- A **color-coded feedback system** to guide players toward the solution.
- A limited number of attempts for added challenge.
- Supports different word lengths for varied difficulty.

---

## **How to Play**

1. Clone this repository to your system.
2. Open the project in CodeBlocks and ensure you have SFML already setup (or any compatible C++ IDE).
3. Compile and run the `wordle2.0.cbp` file.
4. Guess words until you solve the puzzle or run out of attempts.

---

