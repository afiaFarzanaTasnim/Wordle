#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>
#include <cctype>
#include <unordered_set>

using namespace std;
using namespace sf;

const int windowWidth = 800;
const int windowHeight = 600;
const int cellSize = 50;
const int gridRows = 6;
const int gridCols = 5;

const Color wordleGreen(58, 163, 74);
const Color wordleYellow(181, 159, 59);
const Color wordleGray(120, 124, 126);
const Color blackColor(0, 0, 0);
const Color whiteColor(255, 255, 255);
const Color ButtonGreen(83, 176, 97);
const Color backgroundColor(172, 199, 161);
const Color gameOverScreen(41, 74, 41);

enum class Page {
    MainMenu,
    Instructions,
    NameInput,
    Game,
    GameOver,
    Scoreboard
};

// Draw the Main Menu screen
void drawMainMenu(RenderWindow &window, RectangleShape &howToPlayButton, Text &howToPlayText,
                  RectangleShape &playButton, Text &playText, RectangleShape &scoreboardButton, Text &scoreboardText,
                  Sprite &backgroundSprite, Text &titleText) {
    window.clear(backgroundColor);
    window.draw(backgroundSprite);
    window.draw(titleText);

    window.draw(howToPlayButton);
    window.draw(howToPlayText);

    window.draw(playButton);
    window.draw(playText);

    window.draw(scoreboardButton);
    window.draw(scoreboardText);

    window.display();
}

// Draw the Instructions screen
void drawInstructions(RenderWindow &window, Font &font, RectangleShape &continueButton, Text &continueText, Sprite &backgroundSprite) {

    Text title;
    title.setFont(font);
    title.setString("INSTRUCTIONS");
    title.setCharacterSize(80);
    title.setFillColor(whiteColor);
    title.setPosition(100, 50);


    Text instructions;
    instructions.setFont(font);
    instructions.setString(
        "1. You have six attempts to guess a five-letter word.\n"
        "2. Each guess must be a valid five-letter word.\n"
        "3. After each guess, the color of the tiles will change\n"
        "   to show how close your guess was to the word.\n"
        "   - Green: Correct letter in the correct spot.\n"
        "   - Yellow: Correct letter in the wrong spot.\n"
        "   - Gray: Incorrect letter.\n\n"
        "                  GOOD LUCK AND HAVE FUN!"
    );
    instructions.setCharacterSize(25);
    instructions.setFillColor(whiteColor);
    instructions.setPosition(85, 150);


    window.clear(backgroundColor);
    window.draw(backgroundSprite);
    window.draw(title);
    window.draw(instructions);


    window.draw(continueButton);
    window.draw(continueText);

    window.display();
}

// Draw the game grid
void drawGrid(RenderWindow &window, const vector<vector<char>> &grid, const vector<vector<Color>> &colors, Font &font) {
    float startX = (windowWidth - gridCols * cellSize) / 2;
    float startY = (windowHeight - gridRows * cellSize) / 2;

    for (int i = 0; i < gridRows; ++i) {
        for (int j = 0; j < gridCols; ++j) {
            RectangleShape cell(Vector2f(cellSize, cellSize));
            cell.setFillColor(colors[i][j]);
            cell.setOutlineThickness(4);
            cell.setOutlineColor(blackColor);
            cell.setPosition(startX + j * cellSize, startY + i * cellSize);
            window.draw(cell);

            if (grid[i][j] != ' ') {
                Text letter;
                letter.setFont(font);
                letter.setString(grid[i][j]);
                letter.setCharacterSize(24);
                letter.setFillColor(whiteColor);
                letter.setPosition(startX + j * cellSize + cellSize / 3, startY + i * cellSize + cellSize / 6);
                window.draw(letter);
            }
        }
    }
}

// Generate a random word
string generateRandomWord(const vector<string> &words) {
    srand(static_cast<unsigned>(time(0)));
    return words[rand() % words.size()];
}

// Check the player's guess and assign colors to the grid cells
void checkGuess(const string &guess, const string &word, vector<Color> &colorRow) {
    vector<bool> letterUsed(word.size(), false);

    for (size_t i = 0; i < guess.size(); ++i) {
        if (guess[i] == word[i]) {
            colorRow[i] = wordleGreen;
            letterUsed[i] = true;
        }
    }

    for (size_t i = 0; i < guess.size(); ++i) {
        if (colorRow[i] != wordleGreen) {
            bool found = false;
            for (size_t j = 0; j < word.size(); ++j) {
                if (guess[i] == word[j] && !letterUsed[j]) {
                    colorRow[i] = wordleYellow;
                    letterUsed[j] = true;
                    found = true;
                    break;
                }
            }
            if (!found) {
                colorRow[i] = wordleGray;
            }
        }
    }
}

// Display a message on the screen
void displayMessage(RenderWindow &window, Font &font, const string &message, float yPos) {
    Text text(message, font, 24);
    text.setFillColor(whiteColor);
    text.setPosition((windowWidth - text.getLocalBounds().width) / 2, yPos);
    window.draw(text);
}

// Save the player's score to a file
void saveScore(const string &playerName, int score) {
    ofstream outFile("scores.txt", ios::app);
    if (outFile.is_open()) {
        outFile << playerName << " " << score << endl;
    }
    outFile.close();
}

// Draw the Game Over screen
void drawGameOver(RenderWindow &window, Font &font, const string &message1, const string &message2) {
    window.clear(gameOverScreen);

    Text messageText1(message1, font, 30);
    messageText1.setFillColor(whiteColor);
    messageText1.setPosition((windowWidth - messageText1.getLocalBounds().width) / 2, windowHeight / 3);
    window.draw(messageText1);

    Text messageText2(message2, font, 24);
    messageText2.setFillColor(whiteColor);
    messageText2.setPosition((windowWidth - messageText2.getLocalBounds().width) / 2, windowHeight / 3 + 40);
    window.draw(messageText2);

    window.display();
}

// Load scores from the file
vector<pair<string, int>> loadScores() {
    vector<pair<string, int>> scores;
    ifstream inFile("scores.txt");
    string name;
    int score;

    while (inFile >> name >> score) {
        scores.push_back(make_pair(name, score));
    }

    inFile.close();
    return scores;
}

// Draw the Scoreboard screen
void drawScoreboard(RenderWindow &window, Font &font, RectangleShape &backButton, Text &backButtonText) {
    vector<pair<string, int>> scores = loadScores();

    window.clear(backgroundColor);

    Text title("SCOREBOARD", font, 60);
    title.setFillColor(whiteColor);
    title.setPosition((windowWidth - title.getLocalBounds().width) / 2, 30);
    window.draw(title);

    float startY = 100;
    for (const auto &scoreEntry : scores) {
        Text scoreText(scoreEntry.first + ": " + to_string(scoreEntry.second), font, 24);
        scoreText.setFillColor(whiteColor);
        scoreText.setPosition(100, startY);
        window.draw(scoreText);
        startY += 30;
    }

    window.draw(backButton);
    window.draw(backButtonText);

    window.display();
}

// Play the game
void playGame(RenderWindow &window, Font &font, const vector<string> &words, const unordered_set<string> &wordSet, const string &playerName) {
    string targetWord = generateRandomWord(words);
    vector<vector<char>> grid(gridRows, vector<char>(gridCols, ' '));
    vector<vector<Color>> colors(gridRows, vector<Color>(gridCols, backgroundColor));
    string currentGuess;
    int currentRow = 0;

   // cout << "The correct word is: " << targetWord << endl;  //Uncomment this to reveal the answer

    while (window.isOpen() && currentRow < gridRows) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::TextEntered) {
                if (isalpha(event.text.unicode) && currentGuess.size() < gridCols) {
                    currentGuess += toupper(static_cast<char>(event.text.unicode));
                    grid[currentRow][currentGuess.size() - 1] = currentGuess.back();
                } else if (event.text.unicode == 8 && !currentGuess.empty()) { // Handle backspace
                    currentGuess.pop_back();
                    grid[currentRow][currentGuess.size()] = ' ';
                }
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                if (currentGuess.size() == gridCols) {
                    if (wordSet.find(currentGuess) != wordSet.end()) {
                        checkGuess(currentGuess, targetWord, colors[currentRow]);
                        if (currentGuess == targetWord) {
                            window.clear(backgroundColor);
                            drawGrid(window, grid, colors, font);
                            window.display();
                            Clock clock;
                            while (clock.getElapsedTime().asSeconds() < 2) {
                                Event event;
                                while (window.pollEvent(event)) {
                                    if (event.type == Event::Closed) {
                                        window.close();
                                        return;
                                    }
                                }
                            }

                            int score = 12 - 2 * currentRow;
                            saveScore(playerName, score);

                            drawGameOver(window, font, "\t  CONGRATULATIONS!!!\n You guessed the correct word.",
                                         "\nYour score is " + to_string(score) + ".");
                            clock.restart();
                            while (clock.getElapsedTime().asSeconds() < 3) {
                                Event event;
                                while (window.pollEvent(event)) {
                                    if (event.type == Event::Closed) {
                                        window.close();
                                        return;
                                    }
                                }
                            }
                            return;
                        }
                        currentGuess.clear();
                        currentRow++;
                    } else {
                        window.clear(backgroundColor);
                        drawGrid(window, grid, colors, font);
                        displayMessage(window, font, "Invalid word. Try again.", windowHeight - 100);
                        window.display();
                        Clock clock;
                        while (clock.getElapsedTime().asSeconds() < 2) {
                            Event event;
                            while (window.pollEvent(event)) {
                                if (event.type == Event::Closed) {
                                    window.close();
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }

        window.clear(backgroundColor);
        drawGrid(window, grid, colors, font);
        window.display();
    }

    if (currentRow == gridRows) {
        int score = 0;
        saveScore(playerName, score);

        drawGameOver(window, font, "GAME OVER", "The correct word was: " + targetWord + ".");
        Clock clock;
        while (clock.getElapsedTime().asSeconds() < 5) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                    return;
                }
            }
        }
    }
}

// Draw the name input screen
void drawNameInputScreen(RenderWindow &window, Font &font, string &playerName, RectangleShape &nameBox, Text &nameText, Text &startText, Text &playerLabelText) {
    window.clear(backgroundColor);

    window.draw(playerLabelText);


    window.draw(nameBox);


    nameText.setString(playerName);
    window.draw(nameText);


    window.draw(startText);

    window.display();
}

// Handle name input for typing
void handleNameInput(Event &event, string &playerName) {
    if (event.type == Event::TextEntered) {
        if (isalpha(event.text.unicode) && playerName.size() < 15) {
            playerName += toupper(static_cast<char>(event.text.unicode));
        } else if (event.text.unicode == 8 && !playerName.empty()) {
            playerName.pop_back();
        }
    }
}

// Play the game after getting player's name
void playGameWithNameInput(RenderWindow &window, Font &font, const vector<string> &words, const unordered_set<string> &wordSet) {

    RectangleShape nameBox(Vector2f(300, 50));
    nameBox.setPosition((windowWidth - 300) / 2, windowHeight / 2 - 50);
    nameBox.setFillColor(ButtonGreen);
    nameBox.setOutlineThickness(3);
    nameBox.setOutlineColor(blackColor);

    string playerName = "";

    Text nameText("", font, 24);
    nameText.setFillColor(blackColor);
    nameText.setPosition(nameBox.getPosition().x + 10, nameBox.getPosition().y + 10);

    Text startText("START", font, 30);
    startText.setFillColor(whiteColor);
    startText.setPosition((windowWidth - 100) / 2, windowHeight / 2 + 50);

    Text playerLabelText("Enter Your Name", font, 40);
    playerLabelText.setFillColor(whiteColor);
    playerLabelText.setPosition((windowWidth - playerLabelText.getLocalBounds().width) / 2, 150);


    bool nameEntered = false;

    while (window.isOpen() && !nameEntered) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::MouseButtonPressed) {
                // Check if the "Start" button is clicked
                if (startText.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && !playerName.empty()) {
                    nameEntered = true;
                }
            }
            // Handle name input (typing)
            handleNameInput(event, playerName);
        }

        // Draw the name input screen
        drawNameInputScreen(window, font, playerName, nameBox, nameText, startText, playerLabelText);
    }


    playGame(window, font, words, wordSet, playerName);
}

// Main function
int main() {
    RenderWindow window(VideoMode(windowWidth, windowHeight), "WORDLE");
    window.setFramerateLimit(60);

    Font font;
    if (!font.loadFromFile("Merriweather-Regular.ttf")) {
        cerr << "Error loading font\n";
        return -1;
    }
    Font merriweather;
    if (!merriweather.loadFromFile("Mb.ttf")) {
        cerr << "Error loading font\n";
        return -1;
    }
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.png")) {
        cerr << "Error loading background image\n";
        return -1;
    }
    Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(
        static_cast<float>(windowWidth) / backgroundTexture.getSize().x,
        static_cast<float>(windowHeight) / backgroundTexture.getSize().y
    );

    // Main Menu UI setup
    RectangleShape howToPlayButton(Vector2f(200, 75));
    howToPlayButton.setFillColor(ButtonGreen);
    howToPlayButton.setOutlineThickness(3);
    howToPlayButton.setOutlineColor(whiteColor);
    howToPlayButton.setPosition((windowWidth / 2) - 210, windowHeight / 2);

    Text howToPlayText("How to Play", font, 24);
    howToPlayText.setFillColor(whiteColor);
    howToPlayText.setPosition(
        howToPlayButton.getPosition().x + (howToPlayButton.getSize().x - howToPlayText.getLocalBounds().width) / 2,
        howToPlayButton.getPosition().y + (howToPlayButton.getSize().y - howToPlayText.getLocalBounds().height) / 2
    );

    RectangleShape playButton(Vector2f(200, 75));
    playButton.setFillColor(ButtonGreen);
    playButton.setOutlineThickness(3);
    playButton.setOutlineColor(whiteColor);
    playButton.setPosition((windowWidth / 2) + 10, windowHeight / 2);

    Text playText("Play", font, 24);
    playText.setFillColor(whiteColor);
    playText.setPosition(
        playButton.getPosition().x + (playButton.getSize().x - playText.getLocalBounds().width) / 2,
        playButton.getPosition().y + (playButton.getSize().y - playText.getLocalBounds().height) / 2
    );

    RectangleShape scoreboardButton(Vector2f(300, 50));
    scoreboardButton.setPosition(250, 450);
    scoreboardButton.setFillColor(ButtonGreen);
    scoreboardButton.setOutlineThickness(3);
    scoreboardButton.setOutlineColor(whiteColor);


    Text scoreboardText("Scoreboard", font, 24);
    scoreboardText.setFillColor(whiteColor);
    scoreboardText.setPosition(scoreboardButton.getPosition().x + 80, scoreboardButton.getPosition().y + 10);

    Text titleText("WORDLE", merriweather, 90);
    titleText.setFillColor(whiteColor);
    titleText.setPosition(
        (windowWidth - titleText.getLocalBounds().width) / 2,
        windowHeight / 8
    );

    // Back button for scoreboard
    RectangleShape backButton(Vector2f(300, 50));
    backButton.setPosition(250, 500);
    backButton.setFillColor(ButtonGreen);

    Text backButtonText("Back to Menu", font, 24);
    backButtonText.setFillColor(whiteColor);
    backButtonText.setPosition(backButton.getPosition().x + 70, backButton.getPosition().y + 10);

    // Continue button for instructions
    RectangleShape continueButton(Vector2f(300, 50));
    continueButton.setPosition(250, 500);
    continueButton.setFillColor(ButtonGreen);

    Text continueText("Continue", font, 24);
    continueText.setFillColor(whiteColor);
    continueText.setPosition(continueButton.getPosition().x + 90, continueButton.getPosition().y + 10);

    // Load the word list
    vector<string> words;
    unordered_set<string> wordSet;
    ifstream wordFile("wordList.txt");
    string word;
    while (wordFile >> word) {
        transform(word.begin(), word.end(), word.begin(), ::toupper);
        words.push_back(word);
        wordSet.insert(word);
    }
    wordFile.close();

    if (words.empty()) {
        cerr << "Word list is empty!\n";
        return -1;
    }

    Page currentPage = Page::MainMenu;
    string playerName;

    while (window.isOpen()) {
        Event event;
        Vector2i mousePos = Mouse::getPosition(window);

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (currentPage == Page::MainMenu) {
                if (event.type == Event::MouseButtonPressed) {
                    if (howToPlayButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        currentPage = Page::Instructions;
                    } else if (playButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        currentPage = Page::NameInput;
                    } else if (scoreboardButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        currentPage = Page::Scoreboard;
                    }
                }
            } else if (currentPage == Page::Instructions) {
                if (event.type == Event::MouseButtonPressed) {
                    if (continueButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        currentPage = Page::MainMenu;
                    }
                }
            } else if (currentPage == Page::Scoreboard) {
                if (event.type == Event::MouseButtonPressed) {
                    if (backButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        currentPage = Page::MainMenu;
                    }
                }
            }

            if (currentPage == Page::NameInput) {
                playGameWithNameInput(window, font, words, wordSet);
                currentPage = Page::MainMenu;  // Return to Main Menu after the game ends
            }
        }

        // Draw the appropriate screen
        if (currentPage == Page::MainMenu) {
            drawMainMenu(window, howToPlayButton, howToPlayText, playButton, playText, scoreboardButton, scoreboardText, backgroundSprite, titleText);
        } else if (currentPage == Page::Instructions) {
            drawInstructions(window, font, continueButton, continueText, backgroundSprite);
        } else if (currentPage == Page::Scoreboard) {
            drawScoreboard(window, font, backButton, backButtonText);
        }
    }

    return 0;
}
