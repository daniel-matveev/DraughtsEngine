//
//  main.cpp
//  Draughts_NEA_V2
//
//  Created by Daniel Matveev on 09/10/2024.
//

#include "Console.hpp"

int main(int argc, const char * argv[])
{ 
    srand( static_cast<unsigned>( time( NULL ) ) );
    
    Console console;
    
    int iMainMenuOption;
    int iPlayerSelection;
    int iDifficultyLevel;
    
    do 
    {
        iMainMenuOption = 0;
    
        console.printStartOptions();
        std::cin >> iMainMenuOption;
        
        if (std::cin.fail())
        {
            std::cin.clear();
            
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter a number" << std::endl;
        }
        
        
        if (iMainMenuOption == 1)
        {
            for (int i = 0 ; i < 2; i++)
            {
                iPlayerSelection = 0;
                iDifficultyLevel = 0;
                
                if (i == 0)
                {
                    std::cout << "Select white player" << std::endl;
                }
                else
                {
                    std::cout << "Select black player" << std::endl;
                }
                
                do
                {
                    console.printPlayerSelection();
                    std::cin >> iPlayerSelection;
                    
                    if (std::cin.fail())
                    {
                        std::cin.clear();
                        
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Enter a number" << std::endl;
                    }
                    
                } while (iPlayerSelection > 4 || iPlayerSelection < 1);
                
                if (iPlayerSelection != 4)
                {
                    do
                    {
                        console.printDifficultyLevels();
                        std::cin >> iDifficultyLevel;
                        
                        if (std::cin.fail())
                        {
                            std::cin.clear();
                            
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Enter a number" << std::endl;
                        }
                        
                    } while (iDifficultyLevel > 3 || iDifficultyLevel < 1);
                }
                console.selectPlayer(iPlayerSelection, iDifficultyLevel);
            }
            
            console.startGame();
  
        }
        else if (iMainMenuOption == 2)
        {
            console.printGameRules();
        }
        else if (iMainMenuOption == 3)
        {
            break;
        }
        else
        {
            std::cout << "Invalid option" << std::endl;
        }
        
    } while (iMainMenuOption != 3);
    
    return 0;
    
    
//    return 0;
    
//    Game mainGame;
////    
//    Minimax mm;
//    AlphaBetaPruning abp;
//    MonteCarloTreeSearch mcts;
//    
//    
//    
//    Position toSelectPosition;
//    Position toMovePosition;
// 
//    int counter = 0;
//    
//    while (mainGame.getWinner() == NoColour) 
//    {
//        counter++;
//        mainGame.printBoard();
//        
//        std::cout << mainGame.getCurrentPlayerColour() << "'s turn" << std::endl;
//        
//        if (mainGame.getCurrentPlayerColour() == Black)
//        {
//            std::cout << "Minimax called" << std::endl;
//            mainGame = mm.getBestGameState(mainGame, 6);
//            std::cout << mm.iNumberOfLeafNodes << std::endl;
//            mm.iNumberOfLeafNodes = 0;
            
//            std::cout << "AlphaBetaPruning called" << std::endl;
//            mainGame = abp.getBestGameState(mainGame, 6);
//            std::cout << abp.iNumberOfLeafNodes << std::endl;
//            abp.iNumberOfLeafNodes = 0;
//            
//            std::cout << "MonteCarloTreeSearch called" << std::endl;
//            mainGame = mcts.getBestGameState(mainGame, mainGame.getCurrentPlayerColour(), 100);
    
//        }
//        else
//        {
//            mainGame.printBoard();
//            do
//            {
//                std::cout << "Select:" << std::endl;
//                std::cout << "X: ";
//                std::cin >> toSelectPosition.x;
//                
//                std::cout << "Y: ";
//                std::cin >> toSelectPosition.y;
//            } while (!mainGame.select(toSelectPosition));
//            
//            
//            do
//            {
//                mainGame.printBoard();
//                std::cout << "Move to:" << std::endl;
//                std::cout << "X: ";
//                std::cin >> toMovePosition.x;
//                
//                std::cout << "Y: ";
//                std::cin >> toMovePosition.y;
//            } while (!mainGame.move(toMovePosition));
//        }
        
//    }
//    
//    mainGame.getWinner();
//    
//    mainGame.printBoard();

}
