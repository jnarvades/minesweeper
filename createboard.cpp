#include "createboard.h"


//create a board of buttons
inline void Board::createBoard
  //in
  (int width, int height, int mineNumber, sf::Texture& tileHiddenTexture, sf::Texture& mineTexture, bool testingBoard,
  //out
  std::vector<std::vector<ButtonTexture*>>& allButtons, std::vector<sf::Vector2f>& mineXYCoordinates, std::vector<ButtonTexture*>& adjacentButtons, std::vector<std::vector<bool>> testBoard)
{
  setBoardWidth(width);
  setBoardHeight(height);

  allButtons.clear();

// Create ButtonTexture objects and add them to a 2d array
for (int row = 0; row < height; row++) {
    std::vector<ButtonTexture*> buttons;
    for (int col = 0; col < width; col++) { 
        //create the object
        auto button = new ButtonTexture(0, col * 32, row * 32, false, false, adjacentButtons);
        
        button->setAdjacentMines(0);
        buttons.push_back(button);
    }
    allButtons.push_back(buttons);
}
setButtons(allButtons);

// Loop through buttons again to set adjacent buttons
for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) { 
        auto button = allButtons.at(row).at(col);
        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                if (x != 0 || y != 0)
                {
                    try {
                        adjacentButtons.push_back(allButtons.at(row + x).at(col + y));
                    } catch (std::out_of_range){}
                }
            }
        }
        button->setAdjacentButtons(adjacentButtons);
        adjacentButtons.clear();
    }
}

if(testingBoard){
  int mineNumber = 0;
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      allButtons[row][col]->setIsMine(testBoard.at(row).at(col));
      if (testBoard.at(row).at(col)){
        ++mineNumber;
      }
    }
  }
  Board::setMineBumber(mineNumber);

} else {
  // Place mines randomly
  //   Generate possible coordinates
  for (int x = 0; x < width; x++){
    for (int y = 0; y < height; y++){
      mineXYCoordinates.push_back(sf::Vector2f(x, y));
    }
  }
  
  // Excessively random
  std::mt19937 random_mt(std::chrono::system_clock::now().time_since_epoch().count());

  //Get random coordinates from vector to make mines
  for (int i = 0; i < mineNumber; i++){
    
    std::uniform_int_distribution<int> dist(0, mineXYCoordinates.size() - 1);
    int index = dist(random_mt);
    
    int x = mineXYCoordinates.at(index).x;
    int y = mineXYCoordinates.at(index).y;
    auto currentMine = allButtons.at(y).at(x);
    currentMine->setIsMine(true);
    currentMine->setAdjacentMines(INT_FAST32_MIN);
    
    //remove coordinates
    auto it = std::find(mineXYCoordinates.begin(), mineXYCoordinates.end(), sf::Vector2f(x, y));
    mineXYCoordinates.erase(it);
    
  }

  mineXYCoordinates.clear();
  Board::setMineBumber(mineNumber);

}

  //Now that the mines are set calculate adjacent mines for each button
  for (const auto& buttons : allButtons) {
    for (const auto& button : buttons) {
      if (!button->getIsMine()) {
        auto adjacentButtons = button->getAdjacentButtons();
        int value = 0;
        for (const auto& button : adjacentButtons) {
          if (button->getIsMine()) {
            value++;
          }
        }
        button->setAdjacentMines(value);
      }
    }
  }

  //finally, create hidden sprites onto a map
  for (int x = 0; x < boardWidth; ++x) {
    std::vector<sf::Sprite*> tileHiddenSpriteInner;
    for (int y = 0; y < boardHeight; ++y) {

        // Create a new sprite for the tile  
        auto tileHiddenSprite = new sf::Sprite;
        tileHiddenSprite->setTexture(tileHiddenTexture);
        tileHiddenSprite->setPosition(x, y);

        // Store the sprite's memory address in the map
        tileHiddenSpriteInner.push_back(tileHiddenSprite);

    }
    tileHiddenSprites.push_back(tileHiddenSpriteInner);
  }
  setTileHiddenSprites(tileHiddenSprites);
}

inline void Board::removeTiles(std::vector<std::vector<sf::Sprite*>>& tileHiddenSprites, std::vector<std::vector<ButtonTexture*>>& allButtons, int xFloor, int yFloor, int& tileWinCounter)
{
  auto& buttonPtr = allButtons[yFloor][xFloor];
  auto& spritePtr = tileHiddenSprites[xFloor][yFloor];
  spritePtr->setPosition(-100, -100);
  ++tileWinCounter;
  
  auto adjacentButtons = buttonPtr->getAdjacentButtons();

  if ((buttonPtr->getAdjacentMines() > 0) || (buttonPtr->getIsMine())){
    return;
  }

  for (const auto& button : adjacentButtons) {
    if (!(button->getIsMine())){
        int xPos = button->getXPos()/32;
        int yPos = button->getYPos()/32;

        //recursive
        if (tileHiddenSprites[xPos][yPos]->getPosition().x != -100 && !(button->getIsFlagged())){
           removeTiles(tileHiddenSprites, allButtons, xPos, yPos, tileWinCounter);
        }
    }
  }

}

inline void Board::revealMines(std::vector<std::vector<sf::Sprite*>>& tileHiddenSprites, std::vector<std::vector<ButtonTexture*>>& allButtons, int xFloor, int yFloor)
{
  for (const auto& buttons : allButtons) {
    for (const auto& button : buttons) {
      if (button->getIsMine()){
          int xPos = button->getXPos()/32;
          int yPos = button->getYPos()/32;
          button->setIsFlagged(false);
          //removes the tile at that location
          auto& buttonPtr = allButtons[yPos][xPos];
          auto& spritePtr = tileHiddenSprites[xPos][yPos];
          spritePtr->setPosition(-100, -100);
      }
    }
  }
}