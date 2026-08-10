//
//  main.cpp
//  Draughts_NEA
//
//  Created by Daniel Matveev
//

#include "Console.hpp"

int main(int argc, const char * argv[])
{ 
//    srand( static_cast<unsigned>( time( NULL ) ) );
    
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
            std::cout << "\nEnter a number" << std::endl;
            continue;
        }
        
        
        if (iMainMenuOption == 1)
        {
            for (int i = 0 ; i < 2; i++)
            {
                iPlayerSelection = 0;
                iDifficultyLevel = 0;
                
                if (i == 0)
                {
                    std::cout << "\nSelect white player" << std::endl;
                }
                else
                {
                    std::cout << "\nSelect black player" << std::endl;
                }
                
                do
                {
                    console.printPlayerSelection();
                    std::cin >> iPlayerSelection;
                    
                    if (std::cin.fail())
                    {
                        std::cin.clear();
                        
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "\nEnter a number" << std::endl;
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
                            std::cout << "\nEnter a number" << std::endl;
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
            std::cout << "\nInvalid option" << std::endl;
        }
        
    } while (iMainMenuOption != 3);
    
    return 0;

}
