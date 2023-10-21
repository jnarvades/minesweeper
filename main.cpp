#include "createboard.cpp"
#include "loadtextures.cpp"

int main()
{
    //initialize loaded textures
    std::map<std::string, sf::Texture> textures;
    loadTextures(textures);
    sf::Texture nullTexture;
    sf::Texture debugTexture = textures.find("debug.png")->second;
    sf::Texture digitsTexture = textures.find("digits.png")->second;
    sf::Texture faceHappyTexture = textures.find("face_happy.png")->second;
    sf::Texture faceLoseTexture = textures.find("face_lose.png")->second;
    sf::Texture faceWinTexture = textures.find("face_win.png")->second;
    sf::Texture flagTexture = textures.find("flag.png")->second;
    sf::Texture mineTexture = textures.find("mine.png")->second;
    sf::Texture test1Texture = textures.find("test_1.png")->second;
    sf::Texture test2Texture = textures.find("test_2.png")->second;
    sf::Texture test3Texture = textures.find("test_3.png")->second;
    sf::Texture tileHiddenTexture = textures.find("tile_hidden.png")->second;
    sf::Texture tileRevealedTexture = textures.find("tile_revealed.png")->second;

    //load flag numbers
    std::vector<sf::Texture> digits = loadFlagNumbers();

   //load board info from config [width, height, mines]
    int boardInfo[3];
    loadBoardInfo(boardInfo);
    int boardWidth = boardInfo[0];
    int boardHeight = boardInfo[1];
    int mineNumber = boardInfo[2];

    //load test boards [1, 2, 3]
    std::vector<std::vector<bool>> testBoard;

    //Initialize createBoard out
    std::vector<std::vector<ButtonTexture*>> allButtons;
    std::vector<sf::Vector2f> mineXYCoordinates;
    std::vector<ButtonTexture*> adjacentButtons;
    std::vector<std::vector<sf::Sprite*>> tileHiddenSprites;

    //render window
    sf::RenderWindow window(sf::VideoMode(boardWidth * 32, (boardHeight * 32) + 100), L"Minesweeper ☆*:.｡.o(づ ◕‿◕ )づo.｡.:*☆", sf::Style::Titlebar | sf::Style::Close);
    sf::RectangleShape background(sf::Vector2f(boardWidth * 32, (boardHeight * 32) + 100));
        //game section of screen
    sf::RectangleShape theFunZone(sf::Vector2f(boardWidth * 32, boardHeight * 32));
    background.setFillColor(sf::Color::White);

    //initialize utils and sprites
    bool boardDrawn = false;
    bool needBoard = true;
    bool testingBoard = false;
    bool cheat = false;
    bool gameLost = false;
    bool gameWon = false;
    int flagcount = mineNumber;
    int winCounter = 0;
    int tileWinCounter = 0;
    int boardArea = 0;

    sf::Texture numberTexture;
    std::string adjacentMinesString;
    std::string filename;
    sf::Sprite smileySprite;
    sf::Sprite digitsSprite;
    sf::Sprite cheatSprite;
    sf::Sprite test1Sprite;
    sf::Sprite test2Sprite;
    sf::Sprite test3Sprite;
    sf::Sprite adjacentMinesSprite;
    sf::Sprite mineSprite;
    sf::Sprite tileRevealedSprite;
    sf::Sprite flagSprite;

    test1Sprite.setTexture(test1Texture);
    test2Sprite.setTexture(test2Texture);
    test3Sprite.setTexture(test3Texture);
    cheatSprite.setTexture(debugTexture);
    smileySprite.setTexture(faceHappyTexture);
    tileRevealedSprite.setTexture(tileRevealedTexture);
    mineSprite.setTexture(mineTexture);
    flagSprite.setTexture(flagTexture);

    //create board
    auto board = new Board;

    while (window.isOpen())
    {        

        //intitialize board
        if (!boardDrawn){
            window.clear();

            // reinitialize window
            window.setSize(sf::Vector2u(boardWidth * 32, (boardHeight * 32) + 100));
            background.setSize(sf::Vector2f(boardWidth * 32, (boardHeight * 32) + 100));
            theFunZone.setSize(sf::Vector2f(boardWidth * 32, boardHeight * 32));

            // Set the viewport to match the current window size
            float windowWidth = window.getSize().x;
            float windowHeight = window.getSize().y;
            sf::View view(sf::FloatRect(0, 0, windowWidth, windowHeight));
            window.setView(view);

            // Set the viewport to match the current window size
            float windowAspectRatio = static_cast<float>(windowWidth) / windowHeight;
            float viewAspectRatio = view.getSize().x / view.getSize().y;
            float viewportWidth = 1.f;
            float viewportHeight = 1.f;
            float viewportOffsetX = 0.f;
            float viewportOffsetY = 0.f;

            if (windowAspectRatio < viewAspectRatio) {
                // The window is taller than it is wide
                viewportWidth = windowAspectRatio / viewAspectRatio;
                viewportOffsetX = (1.f - viewportWidth) / 2.f;
            } else {
                // The window is wider than it is tall
                viewportHeight = viewAspectRatio / windowAspectRatio;
                viewportOffsetY = (1.f - viewportHeight) / 2.f;
            }

            view.setViewport(sf::FloatRect(viewportOffsetX, viewportOffsetY, viewportWidth, viewportHeight));
            window.setView(view);

            window.draw(background);

            if(needBoard){
                // create everything
                board->createBoard(boardWidth, boardHeight, mineNumber, tileHiddenTexture, mineTexture, false, allButtons, mineXYCoordinates, adjacentButtons, testBoard);
                flagcount = board->getMineNumber();
                boardArea = boardWidth * boardHeight;
            
            }

            if(testingBoard){
                // create test board
                board->createBoard(boardWidth, boardHeight, mineNumber, tileHiddenTexture, mineTexture, true, allButtons, mineXYCoordinates, adjacentButtons, testBoard);
                flagcount = board->getMineNumber();
                boardArea = boardWidth * boardHeight;
                if (mineNumber > boardArea) {
                    mineNumber = 50; //initialize mine number for new random boards (edge case that initialized board has minenumber > boardarea of testboard ~ causing crash)
                }
            }

            //initialize
            allButtons = board->getButtons();
            tileHiddenSprites = board->getTileHiddenSprites();
            

            for (const auto& buttons : allButtons){
                for (const auto& button : buttons) {

                    //load coordinates
                    float x = button->getXPos();
                    float y = button->getYPos();

                    //set board background
                    tileRevealedSprite.setPosition(x, y);
                    window.draw(tileRevealedSprite);

                    // check if mine, if so, add texture
                    if (button->getIsMine())
                    {
                        mineSprite.setPosition(x, y);
                        window.draw(mineSprite);

                    } else {
                        // check how many mines around button
                        int adjacentMinesCount = button->getAdjacentMines();
                        if (adjacentMinesCount > 0)
                        {
                            adjacentMinesString = std::to_string(adjacentMinesCount);
                            filename = "number_" + adjacentMinesString + ".png";
                            numberTexture = textures.find(filename)->second;
                            adjacentMinesSprite.setTexture(numberTexture);
                            adjacentMinesSprite.setPosition(x, y);
                            window.draw(adjacentMinesSprite);
                        }
                    }

                    // hide buttons and surrounding empty buttons on board
                    auto getHideTile = tileHiddenSprites.at(x/32).at(y/32);
                    if (getHideTile->getPosition().x != -100) {
                        getHideTile->setPosition(x, y);
                        window.draw(*getHideTile);
                    }

                    if (button->getIsMine() && gameWon)
                    {
                        button->setIsFlagged(true);
                    }

                    //check if flag, if so, draw it
                    if (button->getIsFlagged())
                    {
                        flagSprite.setPosition(x, y);
                        window.draw(flagSprite);
                    }

                    if (button->getIsMine() && cheat)
                    {
                        mineSprite.setPosition(x, y);
                        window.draw(mineSprite);
                    }

                }
            }

            for (int i = 1; i < 4; i++) {
                digitsSprite.setTexture(digits[0]);
                digitsSprite.setPosition(i * 21, boardHeight * 32);
                window.draw(digitsSprite);
            }
           
            int num;
            int iterWidth = 3;
            if (!(gameWon)) {   

                num = abs(flagcount);
                if (flagcount < 0) {
                digitsSprite.setTexture(digits[10]);
                digitsSprite.setPosition(0, boardHeight * 32);
                window.draw(digitsSprite);
                }

            } else {

                num = 0;

            }
                while (num / 10 + 1 && num != 0) {
                    digitsSprite.setTexture(digits[num % 10]);
                    digitsSprite.setPosition(iterWidth * 21, boardHeight * 32);
                    window.draw(digitsSprite);
                    --iterWidth;
                    num = num / 10;
                }

            
            boardDrawn = true;
            test1Sprite.setPosition((boardWidth * 32) - 192, (boardHeight * 32));
            test2Sprite.setPosition((boardWidth * 32) - 128, (boardHeight * 32));
            test3Sprite.setPosition((boardWidth * 32) - 64, (boardHeight * 32));
            cheatSprite.setPosition((boardWidth * 32) - 256, (boardHeight * 32));
            smileySprite.setPosition((boardWidth * 16) - 32, (boardHeight * 32));
            window.draw(test1Sprite);
            window.draw(test2Sprite);
            window.draw(test3Sprite);
            window.draw(cheatSprite);

            if (!(gameWon || gameLost)){
                smileySprite.setTexture(faceHappyTexture);

            } else if (gameWon) {
                smileySprite.setTexture(faceWinTexture);

            } else if (gameLost) {
                smileySprite.setTexture(faceLoseTexture);
            }

            window.draw(smileySprite);
            window.display();

        }

        // // allows program to breathe
        sf::sleep(sf::milliseconds(20));
        sf::Event event;

        while (window.pollEvent(event))
        {

            switch (event.type)
            {

            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::MouseButtonPressed:

                if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        
                        //initialize coordinates
                        int x = event.mouseButton.x;
                        int y = event.mouseButton.y;
                        int xFloor = (x - x % 32) / 32;
                        int yFloor = (y - y % 32) / 32;
                        auto& buttonPtr = allButtons[yFloor][xFloor];
                        auto& spritePtr = tileHiddenSprites[xFloor][yFloor];
                        

                        if (smileySprite.getGlobalBounds().contains(x, y))
                        {
                            needBoard = true;
                            boardDrawn = false;
                            testingBoard = false;
                            cheat = false;
                            gameWon = false;
                            gameLost = false;
                            winCounter = 0;
                            tileWinCounter = 0;
                            board->destroyBoard();

                        }

                        else if (theFunZone.getGlobalBounds().contains(x, y) && !(gameWon || gameLost))
                        {
                            if (!buttonPtr->getIsFlagged()) {

                                if (buttonPtr->getIsMine()){

                                    board->revealMines(tileHiddenSprites, allButtons, xFloor, yFloor);
                                    gameLost = true;

                                } else {
                                    
                                    if  (spritePtr->getPosition().x != -100) {
                                        board->removeTiles(tileHiddenSprites, allButtons, xFloor, yFloor, tileWinCounter);
                                    }

                                    if (tileWinCounter == boardArea - board->getMineNumber())
                                    {
                                        gameWon = true;                                
                                    }
                                }

                                //update the hidden tile sprites
                                board->setTileHiddenSprites(tileHiddenSprites);
                                needBoard = false;
                                boardDrawn = false;

                            }
                        }

                        else if (cheatSprite.getGlobalBounds().contains(x, y))
                        {
                            //toggles debug button
                            if (!cheat) {cheat = true;}
                            else {cheat = false;}                            
                            needBoard = false;
                            boardDrawn = false;
                            testingBoard = false;
                        }

                        else if (test1Sprite.getGlobalBounds().contains(x, y))
                        {
                            //toggles debug button
                            board->destroyBoard();
                            testBoard = loadTestBoard(testBoard, "boards/testboard1.brd", boardWidth, boardHeight, mineNumber); 
                            winCounter = 0;       
                            tileWinCounter = 0;             
                            testingBoard = true;
                            needBoard = false;
                            boardDrawn = false;
                            cheat = false;
                            gameWon = false;
                            gameLost = false;
                            
                        }

                        else if (test2Sprite.getGlobalBounds().contains(x, y))
                        {
                            //toggles debug button
                            board->destroyBoard();
                            testBoard = loadTestBoard(testBoard, "boards/testboard2.brd", boardWidth, boardHeight, mineNumber);
                            winCounter = 0;
                            tileWinCounter = 0;
                            testingBoard = true;
                            needBoard = false;
                            boardDrawn = false;
                            cheat = false;
                            gameWon = false;
                            gameLost = false;
                        }

                        else if (test3Sprite.getGlobalBounds().contains(x, y))
                        {
                            //toggles debug button
                            board->destroyBoard();
                            testBoard = loadTestBoard(testBoard, "boards/testboard3.brd", boardWidth, boardHeight, mineNumber);
                            winCounter = 0;
                            tileWinCounter = 0;
                            testingBoard = true;
                            needBoard = false;
                            boardDrawn = false;
                            cheat = false;
                            gameWon = false;
                            gameLost = false;

                        }

                    }

                if (event.mouseButton.button == sf::Mouse::Right && !(gameWon || gameLost))
                    {
                        //initialize coordinates
                        int x = event.mouseButton.x;
                        int y = event.mouseButton.y;
                        int xFloor = (x - x % 32) / 32;
                        int yFloor = (y - y % 32) / 32;

                        if (theFunZone.getGlobalBounds().contains(x, y)) {
                            auto& buttonPtr = allButtons[yFloor][xFloor];
                            auto& spritePtr = tileHiddenSprites[xFloor][yFloor];

                            if (!buttonPtr->getIsFlagged() && (spritePtr->getPosition().x != -100) /*if cover is here*/){
                                if (buttonPtr->getIsMine())
                                {
                                    ++winCounter;
                                }
                                buttonPtr->setIsFlagged(true);
                                --flagcount;

                            } else if (buttonPtr->getIsFlagged() && spritePtr->getPosition().x != -100) {
                                if (buttonPtr->getIsMine())
                                {
                                    --winCounter;
                                }
                                buttonPtr->setIsFlagged(false);
                                ++flagcount;
                            }                           
                        }

                        needBoard = false;
                        boardDrawn = false;
                        testingBoard = false;
                    }

                    break;

            default:
                break;

            }
            
        }

    }

    return 0;
}