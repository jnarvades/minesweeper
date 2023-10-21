#ifndef CREATEBOARD_H
#define CREATEBOARD_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <random>
#include <chrono>
#include <memory>

class ButtonTexture {
  private:
    int adjacentMines;
    float xPos;
    float yPos;
    bool isMine;
    bool isFlagged;
    sf::Texture texture;
    std::vector<ButtonTexture*> adjacentButtons;

  public:
    ButtonTexture(){}
    // constructor that sets the texture and button properties
    ButtonTexture(int adjacentMines, float xPos, float yPos, bool isMine, bool isFlagged, std::vector<ButtonTexture*> adjacentButtons) 
        : adjacentMines(adjacentMines), xPos(xPos), yPos(yPos), isMine(isMine), isFlagged(isFlagged), adjacentButtons(adjacentButtons) {}

    // getter
    std::vector<ButtonTexture*> getAdjacentButtons() const {return adjacentButtons;}
    int getAdjacentMines() const {return adjacentMines;}
    float getXPos() const {return xPos;}
    float getYPos() const {return yPos;}
    bool getIsMine() const {return isMine;}
    bool getIsFlagged() const {return isFlagged;}

    // override the setters to update both texture and button properties

    void setAdjacentMines(int a){adjacentMines = a;}
    void setXPos(float x){xPos = x;}
    void setYPos(float y){yPos = y;}
    void setIsMine(bool mine){isMine = mine;}
    void setIsFlagged(bool flag){isFlagged = flag;}
    void setAdjacentButtons(std::vector<ButtonTexture*> adjacentButtons) {
      this->adjacentButtons = adjacentButtons;
    }

    ~ButtonTexture(){}

};

class Board : public ButtonTexture {
  private:
    int boardWidth;
    int boardHeight;
    int mineNumber;
    //two dimensional array of button pointers "buttons[row][col]"
    std::vector<std::vector<ButtonTexture*>> buttons;
    std::vector<std::vector<sf::Sprite*>> tileHiddenSprites;

  public:
    Board(){}
    //getters
    int getBoardWidth() const {return boardWidth;}
    int getBoardHeight() const {return boardHeight;}
    int getMineNumber() const {return mineNumber;}
    std::vector<std::vector<ButtonTexture*>> getButtons() const {return buttons;}
    std::vector<std::vector<sf::Sprite*>> getTileHiddenSprites() const {return tileHiddenSprites;}

    //setters
    void setBoardWidth(int w){boardWidth = w;}
    void setBoardHeight(int h){boardHeight = h;}
    void setMineBumber(int m){mineNumber = m;}
    void setButtons(std::vector<std::vector<ButtonTexture*>>& b){buttons = b;}
    void setTileHiddenSprites(std::vector<std::vector<sf::Sprite*>>& t){tileHiddenSprites = t;}

    //reveal empty tiles
    void removeTiles(std::vector<std::vector<sf::Sprite*>>& tileHiddenSprites, std::vector<std::vector<ButtonTexture*>>& buttons, int xFloor, int yFloor, int& tileWinCounter);

    //reveal mines
    void revealMines(std::vector<std::vector<sf::Sprite*>>& tileHiddenSprites, std::vector<std::vector<ButtonTexture*>>& buttons, int xFloor, int yFloor);


    //create board
    void createBoard
    //in
    (int width, int height, int mineNumber, sf::Texture& tileHiddenTexture, sf::Texture& mineTexture, bool testingBoard,
    //out
    std::vector<std::vector<ButtonTexture*>>& allButtons, 
    std::vector<sf::Vector2f>& mineXYCoordinates, std::vector<ButtonTexture*>& adjacentButtons, std::vector<std::vector<bool>> testBoard);

    void destroyBoard() {
    // Deallocate memory used by the buttons vector

      for (auto& row : buttons) {
          for (auto& button : row) {
            delete button;
          }
      }
      buttons.clear();
      
      // Deallocate memory used by the tileHiddenSprites
      for (auto& row : tileHiddenSprites) {
          for (auto& sprite : row) {
            if(sprite){delete sprite;}
          }
      }
      tileHiddenSprites.clear();
    }


};

#endif