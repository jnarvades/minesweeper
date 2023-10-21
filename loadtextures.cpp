#include "createboard.h"

//loads in textures from image directory, then, emplaces them onto a map
inline void loadTextures(std::map<std::string, sf::Texture>& textures){
  std::vector<std::string> filenames;
  //files
  filenames.push_back("debug.png");
  filenames.push_back("digits.png");
  filenames.push_back("face_happy.png");
  filenames.push_back("face_lose.png");
  filenames.push_back("face_win.png");
  filenames.push_back("flag.png");
  filenames.push_back("mine.png");
  filenames.push_back("number_1.png");
  filenames.push_back("number_2.png");
  filenames.push_back("number_3.png");
  filenames.push_back("number_4.png");
  filenames.push_back("number_5.png");
  filenames.push_back("number_6.png");
  filenames.push_back("number_7.png");
  filenames.push_back("number_8.png");
  filenames.push_back("test_1.png");
  filenames.push_back("test_2.png");
  filenames.push_back("test_3.png");
  filenames.push_back("tile_hidden.png");
  filenames.push_back("tile_revealed.png");

  sf::Texture texture;
  for (const auto& file : filenames)
  {
    if (!texture.loadFromFile("images/" + file))
    {
      exit(1);
    }
    textures.emplace(file, texture);    
  }
}

//load board info from config [width, height, bombs]
inline void loadBoardInfo(int boardInfo[3]) {
  std::ifstream config("boards/config.cfg");
  if (config.is_open()) {
    std::string line;
    for (int i = 0; i < 3 ; i++){

      getline(config, line);
      std::stringstream ss(line);
      std::string token;
      getline(ss, token);
      boardInfo[i] = stoi(token);

    }
  } 
}

//load test boards
inline std::vector<std::vector<bool>> loadTestBoard(std::vector<std::vector<bool>> testBoard, std::string filename, int& width, int& height, int& mineNumber) {
  testBoard.clear();

  std::ifstream board(filename);
  if (!board.is_open()) {
    exit(1);
  }

  height = 0;
  std::string line;
  while (getline(board, line)){
      ++height;
      std::vector<bool> boardline;
      width = 0;
      for (char c : line){
        bool b = static_cast<int>(c) - static_cast<int>('0') == 1;
        boardline.push_back(b);
        ++width;
      }
      testBoard.push_back(boardline);
  }
  return testBoard;
} 

inline std::vector<sf::Texture> loadFlagNumbers() {

  sf::Texture texture;
  if (!texture.loadFromFile("images/digits.png"))
  {
      exit(1);
  }

  std::vector<sf::Texture> digitTextures;

  for (int number = 0; number <= 231; number += 21) {
    // Create a texture
    sf::IntRect cropArea(number, 0, 21, 32);
    sf::Texture digitTexture;
    digitTexture.loadFromImage(texture.copyToImage(), cropArea);
    digitTextures.push_back(digitTexture);
  }

  return digitTextures;

}