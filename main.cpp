//
//  Hockey Program
//
//  Created on 25/08/2017.
//  Copyright © 2017 Anthony Kattuman. All rights reserved.

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>

#define PI 3.14159265358979323846

#define PASS 1
#define BALL_WON 2
#define BALL_LOST 3
#define FREE_WON 4
#define PENALTY_CONCEDED 5
#define SHOT 6
#define D_ENTRY 7
#define DEFENSIVE_ERROR 8
#define ELIMINATION 9
#define SUBSTITUTION 10
#define PENALTY_CORNER 11
#define PENALTY_STROKE 12
#define END_OF_D_ENTRY 13
#define END_OF_PC 14
#define OPPO_D_ENTRY 15
#define OPPO_SHOT 16
#define OPPO_GOAL 17
#define ATT_BALL_OUT_OF_PLAY 18
#define OPPO_BALL_OUT_OF_PLAY 20

#define DURING_D_ENTRY 9
#define DURING_PC 10
#define OUT_OF_D 14
#define GOAL 15
#define ON_TARGET 11
#define INJECTION_ON_TARGET 18
#define TRAPPED 19
#define TYPE 12
#define INTO_D 13
#define TYPE_2 20
#define TYPE_3 21

#define YES 1
#define NO 2
#define LEFT 1
#define RIGHT 2
#define OPEN_PLAY 5

#define D_ENTRY_ID 16
#define PC_ID 17

int pitchWidth = 550;
int pitchHeight = 914;
int pos_x = pitchWidth / 2;
int pos_y = pitchHeight / 2;
int passCoordinates[4] = {0};
int locationCoordinates[2] = {0};
int shotCoordinates[2] = {0};

const int GOALWIDTH = 366;
const int GOALHEIGHT = 214;
const int POSTWIDTH = 5;
const int BACKBOARDHEIGHT = 46;
const float goalSizeMultiplier = 2;
const int goalWidth = GOALWIDTH * goalSizeMultiplier;
const int goalHeight = GOALHEIGHT * goalSizeMultiplier;
const int postWidth = POSTWIDTH * goalSizeMultiplier;
const int backboardHeight = BACKBOARDHEIGHT * goalSizeMultiplier;
const int goalClearance = 200;
const int backboardLineWidth = 3;
const int shotWindowWidth = goalWidth + 2 * postWidth+ 2 * goalClearance;
const int shotWindowHeight = goalHeight + postWidth + goalClearance;
const int FPS = 60;
const int circleRadius = 7;
const int lineWidth = 5;
const int passLineWidth = 2;
const int dRadius = 150;
const int carryCircleRadius = 1.5 * circleRadius;
const int dMapWidth = 2 * pitchWidth;
const int dMapHeight = pitchHeight/2;

float screenMultiplier = 1;

const int eventsColumns = 22;
const int eventsRows = 5000;

int events[eventsRows][eventsColumns] = {0};
int counter = 0;
int dEntryCounter = 0;
int penaltyCornerCounter = 0;
int dEntryPoint = 0;
int PCEntryPoint = 0;
std::string foldername = "";

const int highestNumber = 199;
const int maxPlayers = 25;
int numberOfPlayers = 0;

bool shirtNumbers[highestNumber + 1] = {false};
int playerNumbers[maxPlayers] = {0};

bool activeD = false;
bool activePC = false;
int ballOutOfPlayMarker = 0;

void error();
void outputFile();
void outputData();
void createFolders();
void printBallWon();
void printDefensiveErrors();
void printEliminations();
void printPasses();
void summarySheet();
void printPlayerSheets();
void printBallLost();
void printFreesWon();
void printPenaltiesConceded();
void printDEntryPoints();
void printShotsGoalMap();
void printShotsDMap();
void printPCWins();
void printPSWins();
void printShotsDMapLarge();
void printPCWinsLarge();
void printPSWinsLarge();
void printFullPlayerImages();

void drawImages();

void printArray();
int stringToInt(std::string input);
bool isNumber(const std::string& s);

void drawPitchMap();
void drawDMap();
void drawGoalMap();

void pass();
void dEntry();
void ballWon();
void ballLost();
void freeWon();
void freeLost();
void goalConceded();
void defensiveError();
void elimination();
void penaltyCorner();
void penaltyStroke();
void oppositionDEntry();
void oppositionShot();
void mainMenu();
void dEntryMenu();
void penaltyCornerMenu();
void shot();
void endOfDEntry(int entryPoint);
void endOfPC(int entryPoint);
void dEntryCarry();
void ballOutOfPlay();

bool checkInsideD(int posX, int posY);
bool checkOnTarget(int posX, int posY);

void findPlayerNumbers();

float getLength(int x1, int y1, int x2, int y2);
float getAngle(int x1, int y1, int x2, int y2);

void passWindow();
void locationWindow();
void shotWindow();
void dWindow();
void carryWindow();
void sidelineWindow();
void longCornerWindow();

void clearPassCoordinates();
void clearLocationCoordinates();
void clearShotCoordinates();

bool isNumber(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}
int stringToInt(std::string input)
{
    if(isNumber(input) == true)
        return atoi(input.c_str());
    else
        return -1;
}

void error()
{
    std::cout << "Invalid input" << std::endl << std::endl;
}
void printArray()
{
    for (int i = 0; i <= counter; i++)
    {
        for (int j = 0; j < eventsColumns; j++)
        {
            std::cout << events[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}
void drawImages()
{
    drawPitchMap();
    drawDMap();
    drawGoalMap();
}

void findPlayerNumbers()
{
    int playerCounter = 0;
    for (int i = 1; i <= highestNumber; i++)
    {
        for (int j = 0; j <= counter; j++)
        {
            if (events[j][1] == i)
                shirtNumbers[i] = true;
            if (events[j][2] == i)
                shirtNumbers[i] = true;
        }
        if (shirtNumbers[i] == true)
            {
            playerNumbers[playerCounter] = i;
            playerCounter++;
            }
    }
    numberOfPlayers = playerCounter;
}

//Output
void outputData()
{
    findPlayerNumbers();
    createFolders();
    outputFile();
    summarySheet();
    std::cout << std::endl << "Processing..." << std::endl;
    printPlayerSheets();
    printBallWon();
    printBallLost();
    printFreesWon();
    printPenaltiesConceded();
    printDefensiveErrors();
    printEliminations();
    printPasses();
    printPCWins();
    printPSWins();
    printDEntryPoints();
    printShotsGoalMap();
    printShotsDMap();
    printShotsDMapLarge();
    printPCWinsLarge();
    printPSWinsLarge();
    printFullPlayerImages();
}
void outputFile()
{
    std::ofstream myfile (foldername + "/data.csv");
    if (myfile.is_open())
    {
        for (int i = 0; i <= counter; i++)
        {
            for (int j = 0; j < eventsColumns; j++)
            {
                myfile << events[i][j];
                if (j != eventsColumns - 1)
                    myfile << ",";
            }
            myfile << "\n";
        }
        myfile.close();
    }
}
void createFolders()
{
    std::cout << "Folder name (NO SPACES):" << std::endl;
    std::cin >> foldername;
    int status = 0;
    status = mkdir(foldername.c_str(), ACCESSPERMS);
	status = mkdir((foldername + "/Intermediates").c_str(), ACCESSPERMS);
    
    //Folders for each player
    std::string number;
    for (int i = 0; i <= maxPlayers; i++)
    {
        if (playerNumbers[i] != 0)
        {
            number = std::to_string(playerNumbers[i]);
            status = mkdir((foldername + "/" + number).c_str(), ACCESSPERMS);
            status = mkdir((foldername + "/" + number + "/Intermediates").c_str(), ACCESSPERMS);
        }
    }
}
void summarySheet()
{
    int goals = 0;
    int oppoShots = 0;
    int oppoDEntries = 0;
    int oppoPCs = 0;
    int oppoGoals = 0;
    int frees = 0;
    int freesAgainst = 0;
    int dEntries[5] = {0};
    float shots[2] = {0};
    float passes[2] = {0};
    int turnoverLosses[2] = {0};
    int turnoverWins[2] = {0};
    int defensiveErrors[2] = {0};
    int eliminations[2] = {0};
    int corners = 0;
    int outcomes = 0;
    int endOfDEntries[11] = {0};
    int outOfPlayTurnovers = 0;
    int PCGoals = 0;
    int injectionsOnTarget = 0;
    int PCTraps = 0;
    
    for (int i = 0; i < counter; i++)
    {
        if (events[i][0] == PASS)
        {
            passes[0]++;
            if (events[i][TYPE] == YES)
                passes[1]++;
            else if (events[i][TYPE] == NO)
                turnoverLosses[1]++;
        }
        else if (events[i][0] == D_ENTRY)
        {
            dEntries[0]++;
            if (events[i][TYPE] == 1)
                dEntries[1]++;
            else if (events[i][TYPE] == 2)
                dEntries[2]++;
            else if (events[i][TYPE] == 3)
                {
                if (events[i][TYPE_2] == 1)
                    dEntries[3]++;
                else if (events[i][TYPE_2] == 2)
                    dEntries[4]++;
                }
            
            //Count D Entries without any outcome
            int outcomeLoopCounter = i;
            bool outcomeCounting = true;
            
            while (events[outcomeLoopCounter][D_ENTRY_ID] == events[i][D_ENTRY_ID] && outcomeCounting == true)
            {
                if (events[outcomeLoopCounter][0] == SHOT || events[outcomeLoopCounter][0] == PENALTY_CORNER || events[outcomeLoopCounter][0] == PENALTY_STROKE)
                    {
                    outcomes++;
                    outcomeCounting = false;
                    }
                outcomeLoopCounter++;
            }
        }
        else if (events[i][0] == BALL_LOST)
            {
                turnoverLosses[0]++;
            }
        else if (events[i][0] == SHOT)
        {
            shots[0]++;
            if (events[i][GOAL] == 1)
                {
                goals++;
                if (events[i][DURING_PC] == YES)
                    PCGoals++;
                }
            if (events[i][ON_TARGET] == 1)
                shots[1]++;
        }
        else if (events[i][0] == BALL_WON)
            {
                if (events[i][TYPE] == 1)
                    turnoverWins[0]++;
                else
                    turnoverWins[1]++;
            }
        else if (events[i][0] == DEFENSIVE_ERROR)
        {
            if (events[i][TYPE] == 1)
                defensiveErrors[0]++;
            else
                defensiveErrors[1]++;
        }
        else if (events[i][0] == ELIMINATION)
        {
            if (events[i][TYPE] == 1)
                eliminations[0]++;
            else
                eliminations[1]++;
        }
        else if (events[i][0] == FREE_WON)
            frees++;
        else if (events[i][0] == PENALTY_CONCEDED)
            {
            if (events[i][TYPE] == 1)
                freesAgainst++;
            else if (events[i][TYPE] == 2)
                oppoPCs++;
            }
        else if (events[i][0] == OPPO_SHOT)
            oppoShots++;
        else if (events[i][0] == OPPO_D_ENTRY)
            oppoDEntries++;
        else if (events[i][0] == OPPO_GOAL)
            oppoGoals++;
        else if (events[i][0] == PENALTY_CORNER)
            {
            corners++;
            if (events[i][INJECTION_ON_TARGET] == YES)
                injectionsOnTarget++;
            if (events[i][TRAPPED] == YES)
                PCTraps++;
            }
        else if (events[i][0] == PENALTY_STROKE)
            {
            shots[0]++;
            if (events[i][ON_TARGET] == YES)
                shots[1]++;
            if (events[i][GOAL] == YES)
                goals++;
            }
        else if (events[i][0] == ATT_BALL_OUT_OF_PLAY)
            outOfPlayTurnovers++;
        else if (events[i][0] == END_OF_D_ENTRY)
            {
            if (events[i][TYPE] == 1)
                {
                if (events[i + 1][0] == ATT_BALL_OUT_OF_PLAY)
                    endOfDEntries[0]++;
                else if (events[i + 1][0] == OPPO_BALL_OUT_OF_PLAY)
                    endOfDEntries[1]++;
                }
            else if (events[i][TYPE] == 2)
                endOfDEntries[3]++;
            else if (events[i][TYPE] == 3)
                endOfDEntries[4]++;
            else if (events[i][TYPE] == 4)
                endOfDEntries[5]++;
            else if (events[i][TYPE] == 5)
                endOfDEntries[6]++;
            else if (events[i][TYPE] == 6)
                endOfDEntries[7]++;
            else if (events[i][TYPE] == 7)
                endOfDEntries[8]++;
            else if (events[i][TYPE] == 8)
                endOfDEntries[9]++;
            else if (events[i][TYPE] == 9)
                endOfDEntries[10]++;
            }
    }
    float passCompletion = 0;
    if (passes[0] != 0)
        passCompletion = (passes[1]/passes[0])*100;
    else
        passCompletion = 0;
    float shotOnTargetPercentage = 0;
    if (shots[0] != 0)
        shotOnTargetPercentage = (shots[1]/shots[0])*100;
    else
        shotOnTargetPercentage = 0;
    
    std::cout << "Team summary sheet" << std::endl << std::endl;
    std::cout << "Goals scored: " << goals << "\t\t\t\t"<< "Opposition goals: "<< oppoGoals << std::endl;
    std::cout << "D entries: " << dEntries[0] << "\t\t\t\t"<< "Opposition D entries: "<< oppoDEntries << std::endl;
    std::cout << "\t Carry: " << dEntries[1] << std::endl;
    std::cout << "\t Pass: " << dEntries[2] << std::endl;
    std::cout << "\t No outcome: " << dEntries[0] - outcomes << std::endl;
    std::cout << "End of D entries by method " << std::endl;
    std::cout << "\t Free conceded: " << endOfDEntries[3] << std::endl;
    std::cout << "\t Ball out of play" << std::endl;
    std::cout << "\t\t Retained possession: " << endOfDEntries[0] << std::endl;
    std::cout << "\t\t Opposition ball: " << endOfDEntries[2] << std::endl;
    std::cout << "\t Opposition carry out: " << endOfDEntries[5] << std::endl;
    std::cout << "\t Opposition pass out: " << endOfDEntries[4] << std::endl;
    std::cout << "\t Carry out: " << endOfDEntries[6] << std::endl;
    std::cout << "\t Pass out: " << endOfDEntries[8] << std::endl;
    std::cout << "\t Missed PS: " << endOfDEntries[10] << std::endl;
    std::cout << "\t Goal: " << endOfDEntries[9] << std::endl;
    std::cout << "\t Other: " << endOfDEntries[7] << std::endl;
    std::cout << "Shots: " << shots[0] << "\t\t\t\t\t"<< "Opposition shots: "<< oppoShots << std::endl;
    std::cout << "\t On target: " << shots[1] << "  (" << trunc(shotOnTargetPercentage) << "%)" << std::endl;
    std::cout << "Penalty Corners: " << corners << "\t\t\t"<< "Opposition PCs: "<< oppoPCs << std::endl;
    std::cout << "\t PC Goals: " << PCGoals << std::endl;
    std::cout << "\t Injections on target: " << injectionsOnTarget << std::endl;
    std::cout << "\t Successful traps: " << PCTraps << std::endl;
    std::cout << "Turnover wins: " << turnoverWins[0] + turnoverWins[1] << std::endl;
    std::cout << "\t Tackles: " << turnoverWins[0] << std::endl;
    std::cout << "\t Interceptions: " << turnoverWins[1] << std::endl;
    std::cout << "\t Opposition ball out of play: " << outOfPlayTurnovers << std::endl;
    std::cout << "Turnover losses: " << turnoverLosses[0] + turnoverLosses[1] << std::endl;
    std::cout << "\t Tackles: " << turnoverLosses[0] << std::endl;
    std::cout << "\t Interceptions: " << turnoverWins[1] << std::endl;
    std::cout << "Defensive errors: " << defensiveErrors[0] + defensiveErrors[1] << std::endl;
    std::cout << "\t Missed tackles: " << defensiveErrors[0] << std::endl;
    std::cout << "\t Mistraps: " << defensiveErrors[1] << std::endl;
    std::cout << "Eliminations: " << eliminations[0] + eliminations[1] << std::endl;
    std::cout << "\t Left: " << eliminations[0] << std::endl;
    std::cout << "\t Right: " << eliminations[1] << std::endl;
    std::cout << "Frees won: " << frees << std::endl;
    std::cout << "Frees conceded: " << freesAgainst << std::endl;
    std::cout << "Completed passes: " << passes[1] << std::endl;
    std::cout << "\t Pass completion: " << trunc(passCompletion) << " %"<<  std::endl;
    
    std::ofstream myfile (foldername + "/summary.txt");
    if (myfile.is_open())
    {
        myfile << "Team summary sheet\n\n";
        myfile << "Goals scored: " << goals << "\t\t\t\t"<< "Opposition goals: "<< oppoGoals << "\n";
        myfile << "D entries: " << dEntries[0] << "\t\t\t\t"<< "Opposition D entries: "<< oppoDEntries << "\n";
        myfile << "\t Carry: " << dEntries[1] << "\n";
        myfile << "\t Pass: " << dEntries[2] << "\n";
        myfile << "\t Tackle in D: " << dEntries[3] << "\n";
        myfile << "\t Interception in D: " << dEntries[4] << "\n";
        myfile << "\t No outcome: " << dEntries[0] - outcomes << "\n";
        myfile << "End of D entries by method " << "\n";
        myfile << "\t Free conceded: " << endOfDEntries[3] << "\n";
        myfile << "\t Ball out of play" << "\n";
        myfile << "\t\t Retained possession: " << endOfDEntries[0] << "\n";
        myfile << "\t\t Opposition ball: " << endOfDEntries[2] << "\n";
        myfile << "\t Opposition carry out: " << endOfDEntries[5] << "\n";
        myfile << "\t Opposition pass out: " << endOfDEntries[4] << "\n";
        myfile << "\t Carry out: " << endOfDEntries[6] << "\n";
        myfile << "\t Pass out: " << endOfDEntries[8] << "\n";
        myfile << "\t Missed PS: " << endOfDEntries[10] << "\n";
        myfile << "\t Goal: " << endOfDEntries[9] << "\n";
        myfile << "\t Other: " << endOfDEntries[7] << "\n";
        myfile << "Shots: " << shots[0] << "\t\t\t\t"<< "Opposition shots: "<< oppoShots << "\n";
        myfile << "\t On target: " << shots[1] << "  (" << trunc(shotOnTargetPercentage) << "%)" << "\n";
        myfile << "Penalty Corners: " << corners << "\t\t\t"<< "Opposition PCs: "<< oppoPCs << "\n";
        myfile << "\t PC Goals: " << PCGoals << "\n";
        myfile << "\t Injections on target: " << injectionsOnTarget << "\n";
        myfile << "\t Successful traps: " << PCTraps << "\n";
        myfile << "Turnover wins: " << turnoverWins[0] + turnoverWins[1] << "\n";
        myfile << "\t Tackles: " << turnoverWins[0] << "\n";
        myfile << "\t Interceptions: " << turnoverWins[1] << "\n";
        myfile << "Turnover losses: " << turnoverLosses[0] + turnoverLosses[1] << "\n";
        myfile << "\t Tackles: " << turnoverLosses[0] << "\n";
        myfile << "\t Interceptions: " << turnoverWins[1] << "\n";
        myfile << "Defensive errors: " << defensiveErrors[0] + defensiveErrors[1] << "\n";
        myfile << "\t Missed tackles: " << defensiveErrors[0] << "\n";
        myfile << "\t Mistraps: " << defensiveErrors[1] << "\n";
        myfile << "Eliminations: " << eliminations[0] + eliminations[1] << "\n";
        myfile << "\t Left: " << eliminations[0] << "\n";
        myfile << "\t Right: " << eliminations[1] << "\n";
        myfile << "Frees won: " << frees << "\n";
        myfile << "Frees conceded: " << freesAgainst << "\n";
        myfile << "Completed passes: " << passes[1] << "\n";
        myfile << "\t Pass completion: " << trunc(passCompletion) << " %"<<  "\n";
        
        myfile.close();
    }
}
void printBallWon()
{
    sf::RenderTexture texture;
    texture.create(pitchWidth, pitchHeight);
    
    sf::Texture pitchBackground;
    pitchBackground.loadFromFile("Resources/pitch.png");
    sf::Sprite sprite;
    sprite.setTexture(pitchBackground, true);
    
    sf::Texture option1Texture;
    option1Texture.loadFromFile("Resources/tackle_won.png");
    sf::Sprite option1Sprite;
    option1Sprite.setTexture(option1Texture, true);
    
    sf::Texture option2Texture;
    option2Texture.loadFromFile("Resources/interception_won.png");
    sf::Sprite option2Sprite;
    option2Sprite.setTexture(option2Texture, true);
    
    int posX = 0;
    int posY = 0;
    
    for (int i = 0; i <= counter; i++)
    {
        
        if (events[i][0] == BALL_WON)
        {
            posX = events[i][3];
            posY = events[i][4];
            
            if (events[i][TYPE] == 1)
                {
                option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                texture.draw(option1Sprite);
                }
            else
                {
                option2Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                texture.draw(option2Sprite);
                }
        }
    }
    
    sf::Texture texture2 = texture.getTexture();
    sf::Image image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/Intermediates/turnover_wins_overlay.png");
    
    texture.draw(sprite);
    sf::Texture overlayTexture;
    overlayTexture.loadFromFile(foldername + "/Intermediates/turnover_wins_overlay.png");
    sf::Sprite overlaySprite;
    overlaySprite.setTexture(overlayTexture, true);
    texture.draw(overlaySprite);
    
    texture2 = texture.getTexture();
    image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/turnover_wins.png");
    
    for (int j = 0; j < numberOfPlayers; j++)
    {
        int playerShirtNumber = playerNumbers[j];
        sf::RenderTexture texture;
        texture.create(pitchWidth, pitchHeight);
        texture.clear(sf::Color::Transparent);
        
        sf::Texture pitchBackground;
        pitchBackground.loadFromFile("Resources/pitch.png");
        sf::Sprite sprite;
        sprite.setTexture(pitchBackground, true);
        
        int posX = 0;
        int posY = 0;
        
        for (int i = 0; i <= counter; i++)
        {
            
            if (events[i][0] == BALL_WON && events[i][1] == playerShirtNumber)
            {
                posX = events[i][3];
                posY = events[i][4];
                if (events[i][TYPE] == 1)
                {
                    option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(option1Sprite);
                }
                else
                {
                    option2Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(option2Sprite);
                }
            }
        }
        
        std::string number;
        number = std::to_string(playerShirtNumber);
        
        sf::Texture texture2 = texture.getTexture();
        sf::Image image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number + "/Intermediates/turnover_wins_overlay.png");
        
        texture.draw(sprite);
        sf::Texture overlayTexture;
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/turnover_wins_overlay.png");
        sf::Sprite overlaySprite;
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        texture2 = texture.getTexture();
        image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number +"/turnover_wins.png");
    }
}
void printBallLost()
{
    sf::RenderTexture texture;
    texture.create(pitchWidth, pitchHeight);
    
    sf::Texture pitchBackground;
    pitchBackground.loadFromFile("Resources/pitch.png");
    sf::Sprite sprite;
    sprite.setTexture(pitchBackground, true);
    
    sf::Texture option1Texture;
    option1Texture.loadFromFile("Resources/ball_lost.png");
    sf::Sprite option1Sprite;
    option1Sprite.setTexture(option1Texture, true);
    
    sf::Texture option2Texture;
    option2Texture.loadFromFile("Resources/pass_incomplete.png");
    sf::Sprite option2Sprite;
    option2Sprite.setTexture(option2Texture, true);
    
    int posX = 0;
    int posY = 0;
    
    for (int i = 0; i <= counter; i++)
    {
        
        if (events[i][0] == BALL_LOST)
        {
            posX = events[i][3];
            posY = events[i][4];
            
            option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
            texture.draw(option1Sprite);
        }
        else if (events[i][0] == PASS && events[i][TYPE] == NO)
            {
                    sf::CircleShape shape(circleRadius);
                    sf::RectangleShape rectangle;
                    shape.setFillColor(sf::Color::Black);
                    rectangle.setFillColor(sf::Color::Black);
                
                    int x1 = events[i][3];
                    int y1 = events[i][4];
                    int x2 = events[i][5];
                    int y2 = events[i][6];
                
                    shape.setPosition(x1 - circleRadius, y1 - circleRadius);
                    texture.draw(shape);
                
                    float length = getLength(x1, y1, x2, y2);
                    float angle = getAngle(x1, y1, x2, y2);
                    rectangle.setSize(sf::Vector2f(length, passLineWidth));
                    rectangle.setPosition(x1, y1);
                    rectangle.setRotation(-angle);
                    texture.draw(rectangle);
                    rectangle.setPosition(x2, y2);
                    rectangle.setRotation(-angle + 180);
                    texture.draw(rectangle);
                    option2Sprite.setPosition(x2 - circleRadius, y2 - circleRadius);
                    texture.draw(option2Sprite);
            }
    }
    
    sf::Texture texture2 = texture.getTexture();
    sf::Image image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/Intermediates/turnover_losses_overlay.png");
    
    texture.draw(sprite);
    sf::Texture overlayTexture;
    overlayTexture.loadFromFile(foldername + "/Intermediates/turnover_losses_overlay.png");
    sf::Sprite overlaySprite;
    overlaySprite.setTexture(overlayTexture, true);
    texture.draw(overlaySprite);
    
    texture2 = texture.getTexture();
    image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/turnover_losses.png");
    
    for (int j = 0; j < numberOfPlayers; j++)
    {
        int playerShirtNumber = playerNumbers[j];
        sf::RenderTexture texture;
        texture.create(pitchWidth, pitchHeight);
        texture.clear(sf::Color::Transparent);
        
        sf::Texture pitchBackground;
        pitchBackground.loadFromFile("Resources/pitch.png");
        sf::Sprite sprite;
        sprite.setTexture(pitchBackground, true);
        
        int posX = 0;
        int posY = 0;
        
        for (int i = 0; i <= counter; i++)
        {
            
            if (events[i][0] == BALL_LOST && events[i][1] == playerShirtNumber)
            {
                posX = events[i][3];
                posY = events[i][4];
                option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                texture.draw(option1Sprite);
            }
            else if (events[i][0] == PASS && events[i][TYPE] == NO && events[i][1] == playerShirtNumber)
            {
                sf::CircleShape shape(circleRadius);
                sf::RectangleShape rectangle;
                shape.setFillColor(sf::Color::Black);
                rectangle.setFillColor(sf::Color::Black);
                
                int x1 = events[i][3];
                int y1 = events[i][4];
                int x2 = events[i][5];
                int y2 = events[i][6];
                
                shape.setPosition(x1 - circleRadius, y1 - circleRadius);
                texture.draw(shape);
                float length = getLength(x1, y1, x2, y2);
                float angle = getAngle(x1, y1, x2, y2);
                rectangle.setSize(sf::Vector2f(length, passLineWidth));
                rectangle.setPosition(x1, y1);
                rectangle.setRotation(-angle);
                texture.draw(rectangle);
                rectangle.setPosition(x2, y2);
                rectangle.setRotation(-angle + 180);
                texture.draw(rectangle);
                option2Sprite.setPosition(x2 - circleRadius, y2 - circleRadius);
                texture.draw(option2Sprite);
            }
            
        }
        
        std::string number;
        number = std::to_string(playerShirtNumber);
        
        sf::Texture texture2 = texture.getTexture();
        sf::Image image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number + "/Intermediates/turnover_losses_overlay.png");
        
        texture.draw(sprite);
        sf::Texture overlayTexture;
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/turnover_losses_overlay.png");
        sf::Sprite overlaySprite;
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        texture2 = texture.getTexture();
        image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number +"/turnover_losses.png");
    }
}
void printFreesWon()
{
    sf::RenderTexture texture;
    texture.create(pitchWidth, pitchHeight);
    
    sf::Texture pitchBackground;
    pitchBackground.loadFromFile("Resources/pitch.png");
    sf::Sprite sprite;
    sprite.setTexture(pitchBackground, true);
    
    sf::Texture option1Texture;
    option1Texture.loadFromFile("Resources/free_won.png");
    sf::Sprite option1Sprite;
    option1Sprite.setTexture(option1Texture, true);
    
    int posX = 0;
    int posY = 0;
    
    for (int i = 0; i <= counter; i++)
    {
        
        if (events[i][0] == FREE_WON)
        {
            posX = events[i][3];
            posY = events[i][4];
            
            option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
            texture.draw(option1Sprite);
        }
    }
    
    sf::Texture texture2 = texture.getTexture();
    sf::Image image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/Intermediates/free_wins_overlay.png");
    
    texture.draw(sprite);
    sf::Texture overlayTexture;
    overlayTexture.loadFromFile(foldername + "/Intermediates/free_wins_overlay.png");
    sf::Sprite overlaySprite;
    overlaySprite.setTexture(overlayTexture, true);
    texture.draw(overlaySprite);
    
    texture2 = texture.getTexture();
    image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/free_wins.png");
    
    for (int j = 0; j < numberOfPlayers; j++)
    {
        int playerShirtNumber = playerNumbers[j];
        sf::RenderTexture texture;
        texture.create(pitchWidth, pitchHeight);
        texture.clear(sf::Color::Transparent);
        
        sf::Texture pitchBackground;
        pitchBackground.loadFromFile("Resources/pitch.png");
        sf::Sprite sprite;
        sprite.setTexture(pitchBackground, true);
        
        int posX = 0;
        int posY = 0;
        
        for (int i = 0; i <= counter; i++)
        {
            if (events[i][0] == FREE_WON && events[i][1] == playerShirtNumber)
            {
                posX = events[i][3];
                posY = events[i][4];
                option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                texture.draw(option1Sprite);
            }
        }
        
        std::string number;
        number = std::to_string(playerShirtNumber);
        
        sf::Texture texture2 = texture.getTexture();
        sf::Image image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number + "/Intermediates/free_wins_overlay.png");
        
        texture.draw(sprite);
        sf::Texture overlayTexture;
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/free_wins_overlay.png");
        sf::Sprite overlaySprite;
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        texture2 = texture.getTexture();
        image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number +"/free_wins.png");
    }
}
void printPenaltiesConceded()
{
    sf::RenderTexture texture;
    texture.create(pitchWidth, pitchHeight);
    
    sf::Texture pitchBackground;
    pitchBackground.loadFromFile("Resources/pitch.png");
    sf::Sprite sprite;
    sprite.setTexture(pitchBackground, true);
    
    sf::Texture option1Texture;
    option1Texture.loadFromFile("Resources/free_conceded.png");
    sf::Sprite option1Sprite;
    option1Sprite.setTexture(option1Texture, true);
    
    sf::Texture option2Texture;
    option2Texture.loadFromFile("Resources/pc_conceded.png");
    sf::Sprite option2Sprite;
    option2Sprite.setTexture(option2Texture, true);
    
    sf::Texture option3Texture;
    option3Texture.loadFromFile("Resources/ps_conceded.png");
    sf::Sprite option3Sprite;
    option3Sprite.setTexture(option3Texture, true);
    
    int posX = 0;
    int posY = 0;
    
    for (int i = 0; i <= counter; i++)
    {
        if (events[i][0] == PENALTY_CONCEDED)
        {
            posX = events[i][3];
            posY = events[i][4];
            
            if (events[i][TYPE] == 1)
                {
                    option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(option1Sprite);
                }
            else if (events[i][TYPE] == 2)
            {
                option2Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                texture.draw(option2Sprite);
            }
            else if (events[i][TYPE] == 3)
                {
                    option3Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(option3Sprite);
                }
        }
    }
    
    sf::Texture texture2 = texture.getTexture();
    sf::Image image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/Intermediates/penalties_conceded_overlay.png");
    
    texture.draw(sprite);
    sf::Texture overlayTexture;
    overlayTexture.loadFromFile(foldername + "/Intermediates/penalties_conceded_overlay.png");
    sf::Sprite overlaySprite;
    overlaySprite.setTexture(overlayTexture, true);
    texture.draw(overlaySprite);
    
    texture2 = texture.getTexture();
    image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/penalties_conceded.png");
    
    for (int j = 0; j < numberOfPlayers; j++)
    {
        int playerShirtNumber = playerNumbers[j];
        sf::RenderTexture texture;
        texture.create(pitchWidth, pitchHeight);
        texture.clear(sf::Color::Transparent);
        
        sf::Texture pitchBackground;
        pitchBackground.loadFromFile("Resources/pitch.png");
        sf::Sprite sprite;
        sprite.setTexture(pitchBackground, true);
        
        int posX = 0;
        int posY = 0;
        
        for (int i = 0; i <= counter; i++)
        {
            if (events[i][0] == PENALTY_CONCEDED && events[i][1] == playerShirtNumber)
            {
                posX = events[i][3];
                posY = events[i][4];
                
                if (events[i][TYPE] == 1)
                {
                    option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(option1Sprite);
                }
                else if (events[i][TYPE] == 2)
                {
                    option2Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(option2Sprite);
                }
                else if (events[i][TYPE] == 3)
                {
                    option3Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(option3Sprite);
                }
            }
        }
        
        std::string number;
        number = std::to_string(playerShirtNumber);
        
        sf::Texture texture2 = texture.getTexture();
        sf::Image image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number + "/Intermediates/penalties_conceded_overlay.png");
        
        texture.draw(sprite);
        sf::Texture overlayTexture;
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/penalties_conceded_overlay.png");
        sf::Sprite overlaySprite;
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        texture2 = texture.getTexture();
        image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number +"/penalties_conceded.png");
    }
}
void printDefensiveErrors()
{
    sf::RenderTexture texture;
    texture.create(pitchWidth, pitchHeight);
    
    sf::Texture pitchBackground;
    pitchBackground.loadFromFile("Resources/pitch.png");
    sf::Sprite sprite;
    sprite.setTexture(pitchBackground, true);
    
    sf::Texture option1Texture;
    option1Texture.loadFromFile("Resources/missed_tackle.png");
    sf::Sprite option1Sprite;
    option1Sprite.setTexture(option1Texture, true);
    
    sf::Texture option2Texture;
    option2Texture.loadFromFile("Resources/mistrap.png");
    sf::Sprite option2Sprite;
    option2Sprite.setTexture(option2Texture, true);
    
    int posX = 0;
    int posY = 0;
    
    for (int i = 0; i <= counter; i++)
    {
        
        if (events[i][0] == DEFENSIVE_ERROR)
        {
            posX = events[i][3];
            posY = events[i][4];
            
            if (events[i][TYPE] == 1)
            {
                option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                texture.draw(option1Sprite);
            }
            else
            {
                option2Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                texture.draw(option2Sprite);
            }
        }
    }
    
    sf::Texture texture2 = texture.getTexture();
    sf::Image image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/Intermediates/defensive_errors_overlay.png");
    
    texture.draw(sprite);
    sf::Texture overlayTexture;
    overlayTexture.loadFromFile(foldername + "/Intermediates/defensive_errors_overlay.png");
    sf::Sprite overlaySprite;
    overlaySprite.setTexture(overlayTexture, true);
    texture.draw(overlaySprite);
    
    texture2 = texture.getTexture();
    image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/defensive_errors.png");
    
    for (int j = 0; j < numberOfPlayers; j++)
    {
        int playerShirtNumber = playerNumbers[j];
        sf::RenderTexture texture;
        texture.create(pitchWidth, pitchHeight);
        texture.clear(sf::Color::Transparent);
        
        sf::Texture pitchBackground;
        pitchBackground.loadFromFile("Resources/pitch.png");
        sf::Sprite sprite;
        sprite.setTexture(pitchBackground, true);
        
        int posX = 0;
        int posY = 0;
        
        for (int i = 0; i <= counter; i++)
        {
            
            if (events[i][0] == DEFENSIVE_ERROR && events[i][1] == playerShirtNumber)
            {
                posX = events[i][3];
                posY = events[i][4];
                if (events[i][TYPE] == 1)
                {
                    option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(option1Sprite);
                }
                else
                {
                    option2Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(option2Sprite);
                }
            }
        }
        
        std::string number;
        number = std::to_string(playerShirtNumber);
        
        sf::Texture texture2 = texture.getTexture();
        sf::Image image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number + "/Intermediates/defensive_errors_overlay.png");
        
        texture.draw(sprite);
        sf::Texture overlayTexture;
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/defensive_errors_overlay.png");
        sf::Sprite overlaySprite;
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        texture2 = texture.getTexture();
        image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number +"/defensive_errors.png");
    }
}
void printEliminations()
{
    sf::RenderTexture texture;
    texture.create(pitchWidth, pitchHeight);
    
    sf::Texture pitchBackground;
    pitchBackground.loadFromFile("Resources/pitch.png");
    sf::Sprite sprite;
    sprite.setTexture(pitchBackground, true);
    
    sf::Texture option1Texture;
    option1Texture.loadFromFile("Resources/elimination_right.png");
    sf::Sprite option1Sprite;
    option1Sprite.setTexture(option1Texture, true);
    
    sf::Texture option2Texture;
    option2Texture.loadFromFile("Resources/elimination_left.png");
    sf::Sprite option2Sprite;
    option2Sprite.setTexture(option2Texture, true);
    
    int posX = 0;
    int posY = 0;
    
    for (int i = 0; i <= counter; i++)
    {
        
        if (events[i][0] == ELIMINATION)
        {
            posX = events[i][3];
            posY = events[i][4];
            
            if (events[i][TYPE] == RIGHT)
            {
                option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                texture.draw(option1Sprite);
            }
            else
            {
                option2Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                texture.draw(option2Sprite);
            }
        }
    }
    
    sf::Texture texture2 = texture.getTexture();
    sf::Image image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/Intermediates/eliminations_overlay.png");
    
    texture.draw(sprite);
    sf::Texture overlayTexture;
    overlayTexture.loadFromFile(foldername + "/Intermediates/eliminations_overlay.png");
    sf::Sprite overlaySprite;
    overlaySprite.setTexture(overlayTexture, true);
    texture.draw(overlaySprite);
    
    texture2 = texture.getTexture();
    image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/eliminations.png");
    
    for (int j = 0; j < numberOfPlayers; j++)
    {
        int playerShirtNumber = playerNumbers[j];
        sf::RenderTexture texture;
        texture.create(pitchWidth, pitchHeight);
        texture.clear(sf::Color::Transparent);
        
        sf::Texture pitchBackground;
        pitchBackground.loadFromFile("Resources/pitch.png");
        sf::Sprite sprite;
        sprite.setTexture(pitchBackground, true);
        
        int posX = 0;
        int posY = 0;
        
        for (int i = 0; i <= counter; i++)
        {
            
            if (events[i][0] == ELIMINATION && events[i][1] == playerShirtNumber)
            {
                posX = events[i][3];
                posY = events[i][4];
                if (events[i][TYPE] == RIGHT)
                {
                    option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(option1Sprite);
                }
                else
                {
                    option2Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(option2Sprite);
                }
            }
        }
        
        std::string number;
        number = std::to_string(playerShirtNumber);
        
        sf::Texture texture2 = texture.getTexture();
        sf::Image image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number + "/Intermediates/eliminations_overlay.png");
        
        texture.draw(sprite);
        sf::Texture overlayTexture;
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/eliminations_overlay.png");
        sf::Sprite overlaySprite;
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        texture2 = texture.getTexture();
        image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number +"/eliminations.png");
    }
}
void printPasses()
{
    sf::RenderTexture texture;
    texture.create(pitchWidth, pitchHeight);
    
    sf::Texture pitchBackground;
    pitchBackground.loadFromFile("Resources/pitch.png");
    sf::Sprite sprite;
    sprite.setTexture(pitchBackground, true);
    
    sf::Texture arrowTexture;
    arrowTexture.loadFromFile("Resources/pass_arrowhead.png");
    sf::Sprite arrowSprite;
    arrowSprite.setTexture(arrowTexture, true);
    
    sf::Texture incompleteTexture;
    incompleteTexture.loadFromFile("Resources/pass_incomplete.png");
    sf::Sprite incompleteSprite;
    incompleteSprite.setTexture(incompleteTexture, true);
    
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    
    sf::CircleShape shape(circleRadius);
    sf::RectangleShape rectangle;
    shape.setFillColor(sf::Color::Black);
    rectangle.setFillColor(sf::Color::Black);
    
    for (int i = 0; i <= counter; i++)
    {
        
        if (events[i][0] == PASS)
        {
            x1 = events[i][3];
            y1 = events[i][4];
            x2 = events[i][5];
            y2 = events[i][6];
            
            shape.setPosition(x1 - circleRadius, y1 - circleRadius);
            texture.draw(shape);
            float length = getLength(x1, y1, x2, y2);
            float angle = getAngle(x1, y1, x2, y2);
            rectangle.setSize(sf::Vector2f(length, passLineWidth));
            rectangle.setPosition(x1, y1);
            rectangle.setRotation(-angle);
            texture.draw(rectangle);
            rectangle.setPosition(x2, y2);
            rectangle.setRotation(-angle + 180);
            texture.draw(rectangle);
            
            if (events[i][TYPE] == 1)
            {
                arrowSprite.setPosition(x2 - circleRadius * sin(angle * PI/180), y2 - circleRadius * cos (angle * PI/180));
                arrowSprite.setRotation(-angle + 90);
                texture.draw(arrowSprite);
            }
            else
            {
                incompleteSprite.setPosition(x2 - circleRadius, y2 - circleRadius);
                texture.draw(incompleteSprite);
            }
        }
    }
    
    sf::Texture texture2 = texture.getTexture();
    sf::Image image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/Intermediates/passes_overlay.png");
    
    texture.draw(sprite);
    sf::Texture overlayTexture;
    overlayTexture.loadFromFile(foldername + "/Intermediates/passes_overlay.png");
    sf::Sprite overlaySprite;
    overlaySprite.setTexture(overlayTexture, true);
    texture.draw(overlaySprite);
    
    texture2 = texture.getTexture();
    image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/passes.png");
    
    for (int j = 0; j < numberOfPlayers; j++)
    {
        int playerShirtNumber = playerNumbers[j];
        sf::RenderTexture texture;
        texture.create(pitchWidth, pitchHeight);
        texture.clear(sf::Color::Transparent);
        
        sf::Texture pitchBackground;
        pitchBackground.loadFromFile("Resources/pitch.png");
        sf::Sprite sprite;
        sprite.setTexture(pitchBackground, true);
        
        int x1 = 0;
        int y1 = 0;
        int x2 = 0;
        int y2 = 0;
        
        for (int i = 0; i <= counter; i++)
        {
            
            if (events[i][0] == PASS && events[i][1] == playerShirtNumber)
            {
                x1 = events[i][3];
                y1 = events[i][4];
                x2 = events[i][5];
                y2 = events[i][6];
                
                shape.setPosition(x1 - circleRadius, y1 - circleRadius);
                texture.draw(shape);
                float length = getLength(x1, y1, x2, y2);
                float angle = getAngle(x1, y1, x2, y2);
                rectangle.setSize(sf::Vector2f(length, passLineWidth));
                rectangle.setPosition(x1, y1);
                rectangle.setRotation(-angle);
                texture.draw(rectangle);
                rectangle.setPosition(x2, y2);
                rectangle.setRotation(-angle + 180);
                texture.draw(rectangle);
                
                if (events[i][TYPE] == 1)
                {
                    arrowSprite.setPosition(x2 - circleRadius * sin(angle * PI/180), y2 - circleRadius * cos (angle * PI/180));
                    arrowSprite.setRotation(-angle + 90);
                    texture.draw(arrowSprite);
                }
                else
                {
                    incompleteSprite.setPosition(x2 - circleRadius, y2 - circleRadius);
                    texture.draw(incompleteSprite);
                }
            }
        }
        
        std::string number;
        number = std::to_string(playerShirtNumber);
        
        sf::Texture texture2 = texture.getTexture();
        sf::Image image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number + "/Intermediates/passes_overlay.png");
        
        texture.draw(sprite);
        sf::Texture overlayTexture;
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/passes_overlay.png");
        sf::Sprite overlaySprite;
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        texture2 = texture.getTexture();
        image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number +"/passes.png");
    }
}
void printPlayerSheets()
{
    int goals = 0;
    int frees = 0;
    int freesAgainst = 0;
    int dEntries[5] = {0};
    float shots[2] = {0};
    float passes[2] = {0};
    int eliminations[2] = {0};
    int defensiveErrors[2] = {0};
    int turnovers = 0;
    int turnoverWins[3] = {0};
    int cornersWon = 0;
    int strokesWon = 0;
    int receipts = 0;
    int deflections = 0;
    int lostBall = 0;
    
    for (int j = 0; j < numberOfPlayers; j++)
        {
            goals = 0;
            frees = 0;
            freesAgainst = 0;
            dEntries[0] = 0;
            dEntries[1] = 0;
            dEntries[2] = 0;
            dEntries[3] = 0;
            dEntries[4] = 0;
            shots[0] = 0;
            shots[1] = 0;
            passes[0] = 0;
            passes[1] = 0;
            turnovers = 0;
            turnoverWins[0] = 0;
            turnoverWins[1] = 0;
            turnoverWins[2] = 0;
            eliminations[0] = 0;
            eliminations[1] = 0;
            defensiveErrors[0] = 0;
            defensiveErrors[1] = 0;
            cornersWon = 0;
            strokesWon = 0;
            receipts = 0;
            deflections = 0;
            lostBall = 0;
            
            for (int i = 0; i <= counter; i++)
                {
                if (events[i][1] == playerNumbers[j])
                    {
                        if (events[i][0] == PASS)
                        {
                            passes[0]++;
                            if (events[i][TYPE] == YES)
                                passes[1]++;
                            else if (events[i][TYPE] == NO)
                                turnovers++;
                        }
                        else if (events[i][0] == D_ENTRY)
                        {
                            dEntries[0]++;
                            if (events[i][TYPE] == 1)
                                dEntries[1]++;
                            else if (events[i][TYPE] == 2)
                                dEntries[2]++;
                            else if (events[i][TYPE] == 3)
                                {
                                if (events[i][TYPE_2] == 1)
                                    dEntries[3]++;
                                else
                                    dEntries[4]++;
                                }
                        }
                        else if (events[i][0] == BALL_LOST)
                            lostBall++;
                        else if (events[i][0] == SHOT)
                        {
                            shots[0]++;
                            if (events[i][GOAL] == 1)
                                goals++;
                            if (events[i][ON_TARGET] == 1)
                                shots[1]++;
                        }
                        else if (events[i][0] == BALL_WON)
                            {
                            turnoverWins[0]++;
                            if (events[i][TYPE] == 1)
                                turnoverWins[1]++;
                            else if (events[i][TYPE] == 2)
                                turnoverWins[2]++;
                            }
                        else if (events[i][0] == FREE_WON)
                            frees++;
                        else if (events[i][0] == ELIMINATION)
                            {
                            if (events[i][TYPE] == LEFT)
                                eliminations[0]++;
                            else if (events[i][TYPE] == RIGHT)
                                eliminations[1]++;
                            }
                        else if (events[i][0] == DEFENSIVE_ERROR)
                        {
                            if (events[i][TYPE] == 1)
                                defensiveErrors[0]++;
                            else if (events[i][TYPE] == 2)
                                defensiveErrors[1]++;
                        }
                        else if (events[i][0] == PENALTY_CONCEDED)
                        {
                            if (events[i][TYPE] == 1)
                                freesAgainst++;
                        }
                        else if (events[i][0] == PENALTY_STROKE)
                        {
                            shots[0]++;
                            if (events[i][ON_TARGET] == YES)
                                shots[1]++;
                            if (events[i][GOAL] == YES)
                                goals++;
                        }
                    }
                    if (events[i][2] == playerNumbers[j])
                    {
                        if (events[i][0] == PENALTY_CORNER)
                            cornersWon++;
                        else if (events[i][0] == PENALTY_STROKE)
                            strokesWon++;
                        else if (events[i][0] == PASS)
                            receipts++;
                        else if (events[i][0] == SHOT)
                            deflections++;
                        
                    }
                }
            
            float passCompletion = 0;
            if (passes[0] != 0)
                passCompletion = (passes[1]/passes[0])*100;
            else
                passCompletion = 0;
            float shotOnTargetPercentage = 0;
            if (shots[0] != 0)
                shotOnTargetPercentage = (shots[1]/shots[0])*100;
            else
                shotOnTargetPercentage = 0;
            
            std::string number;

                if (playerNumbers[j] != 0)
                {
                    number = std::to_string(playerNumbers[j]);
                    std::ofstream myfile (foldername + "/" + number + "/summary.txt");
                    if (myfile.is_open())
                    {
                        myfile << "Summary sheet: #" + number + "\n\n";
                        myfile << "Goals scored: " << goals << "\n";
                        myfile << "D Entries: " << dEntries[0] << "\n";
                        myfile << "\t Carry: " << dEntries[1] << "\n";
                        myfile << "\t Pass: " << dEntries[2] << "\n";
                        myfile << "\t Tackle in D: " << dEntries[3] << "\n";
                        myfile << "\t Interception in D: " << dEntries[4] << "\n";
                        myfile << "Penalty Corners Won: " << cornersWon << "\n";
                        myfile << "Penalty Strokes Won: " << strokesWon << "\n";
                        myfile << "Shots: " << shots[0] << "\n";
                        myfile << "\t On target: " << shots[1] << "\n";
                        myfile << "Deflections: " << deflections << "\n";
                        myfile << "Turnover wins: " << turnoverWins[0] << "\n";
                        myfile << "\t Tackle: " << turnoverWins[1] << "\n";
                        myfile << "\t Interception: " << turnoverWins[2] << "\n";
                        myfile << "Turnover losses: " << turnovers + lostBall << "\n";
                        myfile << "\t Tackle: " << lostBall << "\n";
                        myfile << "\t Interception: " << turnovers << "\n";
                        myfile << "Defensive Errors: " << defensiveErrors[0] + defensiveErrors[1] << "\n";
                        myfile << "\t Missed tackles: " << defensiveErrors[0] << "\n";
                        myfile << "\t Mistraps: " << defensiveErrors[1] << "\n";
                        myfile << "Eliminations: " << eliminations[0] + eliminations[1] << "\n";
                        myfile << "\t Left: " << eliminations[0] << "\n";
                        myfile << "\t Right: " << eliminations[1] << "\n";
                        myfile << "Frees won: " << frees << "\n";
                        myfile << "Frees conceded: " << freesAgainst << "\n";
                        myfile << "Passes: " << passes[0] << "\n";
                        myfile << "\t Completed: " << passes[1] << " ("<< passCompletion << "%)"<<  "\n";
                        myfile << "Receipts: " << receipts << "\n";
                        
                        myfile.close();
                    }
                }
        }
}
void printDEntryPoints()
{
    sf::RenderTexture texture;
    texture.create(pitchWidth, pitchHeight/2);
    
    sf::Texture pitchBackground;
    pitchBackground.loadFromFile("Resources/pitch.png");
    sf::Sprite sprite;
    sprite.setTexture(pitchBackground, true);
    
    sf::Texture option1Texture;
    option1Texture.loadFromFile("Resources/tackle_won.png");
    sf::Sprite option1Sprite;
    option1Sprite.setTexture(option1Texture, true);
    
    sf::Texture option2Texture;
    option2Texture.loadFromFile("Resources/interception_won.png");
    sf::Sprite option2Sprite;
    option2Sprite.setTexture(option2Texture, true);
    
    sf::Texture arrowTexture;
    arrowTexture.loadFromFile("Resources/pass_arrowhead.png");
    sf::Sprite arrowSprite;
    arrowSprite.setTexture(arrowTexture, true);
    
    sf::CircleShape carryShape(circleRadius);
    carryShape.setFillColor(sf::Color::Red);
    
    sf::CircleShape shape(circleRadius);
    sf::RectangleShape rectangle;
    shape.setFillColor(sf::Color::Black);
    rectangle.setFillColor(sf::Color::Black);
    
    int posX = 0;
    int posY = 0;
    
    for (int i = 0; i <= counter; i++)
    {
        
        if (events[i][0] == D_ENTRY)
        {
            posX = events[i][3];
            posY = events[i][4];
            
            if (events[i][TYPE] == 1)
            {
                carryShape.setPosition(posX - circleRadius, posY - circleRadius);
                texture.draw(carryShape);
            }
            else if (events[i][TYPE] == 2)
            {
                int x1 = events[i][3];
                int y1 = events[i][4];
                int x2 = events[i][5];
                int y2 = events[i][6];
                
                shape.setPosition(x1 - circleRadius, y1 - circleRadius);
                texture.draw(shape);
                float length = getLength(x1, y1, x2, y2);
                float angle = getAngle(x1, y1, x2, y2);
                rectangle.setSize(sf::Vector2f(length, passLineWidth));
                rectangle.setPosition(x1, y1);
                rectangle.setRotation(-angle);
                texture.draw(rectangle);
                rectangle.setPosition(x2, y2);
                rectangle.setRotation(-angle + 180);
                texture.draw(rectangle);
                
                arrowSprite.setPosition(x2 - circleRadius * sin(angle * PI/180), y2 - circleRadius * cos (angle * PI/180));
                arrowSprite.setRotation(-angle + 90);
                texture.draw(arrowSprite);
            }
            else if (events[i][TYPE] == 3)
            {
                if (events[i][TYPE_2] == 1)
                    {
                        option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                        texture.draw(option1Sprite);
                    }
                else if (events[i][TYPE_2] == 2)
                    {
                        option2Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                        texture.draw(option2Sprite);
                    }
            }
        }
    }
    
    sf::Texture texture2 = texture.getTexture();
    sf::Image image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/Intermediates/d_entries_overlay.png");
    
    texture.draw(sprite);
    sf::Texture overlayTexture;
    overlayTexture.loadFromFile(foldername + "/Intermediates/d_entries_overlay.png");
    sf::Sprite overlaySprite;
    overlaySprite.setTexture(overlayTexture, true);
    texture.draw(overlaySprite);
    
    texture2 = texture.getTexture();
    image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/d_entries.png");
    
    for (int j = 0; j < numberOfPlayers; j++)
    {
        int playerShirtNumber = playerNumbers[j];
        sf::RenderTexture texture;
        texture.create(pitchWidth, pitchHeight/2);
        texture.clear(sf::Color::Transparent);
        
        sf::Texture pitchBackground;
        pitchBackground.loadFromFile("Resources/pitch.png");
        sf::Sprite sprite;
        sprite.setTexture(pitchBackground, true);
        
        int posX = 0;
        int posY = 0;
        
        for (int i = 0; i <= counter; i++)
        {
            
        if (events[i][0] == D_ENTRY && events[i][1] == playerShirtNumber)
        {
            posX = events[i][3];
            posY = events[i][4];

            if (events[i][TYPE] == 1)
                {
                carryShape.setPosition(posX - circleRadius, posY - circleRadius);
                texture.draw(carryShape);
                }
            else if (events[i][TYPE] == 2)
                {
                int x1 = events[i][3];
                int y1 = events[i][4];
                int x2 = events[i][5];
                int y2 = events[i][6];

                shape.setPosition(x1 - circleRadius, y1 - circleRadius);
                texture.draw(shape);
                float length = getLength(x1, y1, x2, y2);
                float angle = getAngle(x1, y1, x2, y2);
                rectangle.setSize(sf::Vector2f(length, passLineWidth));
                rectangle.setPosition(x1, y1);
                rectangle.setRotation(-angle);
                texture.draw(rectangle);
                rectangle.setPosition(x2, y2);
                rectangle.setRotation(-angle + 180);
                texture.draw(rectangle);

                arrowSprite.setPosition(x2 - circleRadius * sin(angle * PI/180), y2 - circleRadius * cos (angle * PI/180));
                arrowSprite.setRotation(-angle + 90);
                texture.draw(arrowSprite);
                }
            else if (events[i][TYPE] == 3)
                {
                if (events[i][TYPE_2] == 1)
                    {
                    option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(option1Sprite);
                    }
                else if (events[i][TYPE_2] == 2)
                    {
                    option2Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(option2Sprite);
                    }
                }
            }
            else if (events[i][0] == D_ENTRY && events[i][2] == playerShirtNumber)
                {
                
                if (events[i][TYPE] == 2)
                    {
                    int x1 = events[i][3];
                    int y1 = events[i][4];
                    int x2 = events[i][5];
                    int y2 = events[i][6];
                    
                    shape.setPosition(x1 - circleRadius, y1 - circleRadius);
                    texture.draw(shape);
                    float length = getLength(x1, y1, x2, y2);
                    float angle = getAngle(x1, y1, x2, y2);
                    rectangle.setSize(sf::Vector2f(length, passLineWidth));
                    rectangle.setPosition(x1, y1);
                    rectangle.setRotation(-angle);
                    texture.draw(rectangle);
                    rectangle.setPosition(x2, y2);
                    rectangle.setRotation(-angle + 180);
                    texture.draw(rectangle);
                    
                    shape.setFillColor(sf::Color::Green);
                    shape.setPosition(x2 - circleRadius, y2 - circleRadius);
                    texture.draw(shape);
                    shape.setFillColor(sf::Color::Black);
                    
                    arrowSprite.setPosition(x2 - circleRadius * sin(angle * PI/180), y2 - circleRadius * cos (angle * PI/180));
                    arrowSprite.setRotation(-angle + 90);
                    texture.draw(arrowSprite);
                    }
                }
            }
        std::string number;
        number = std::to_string(playerShirtNumber);
     
        sf::Texture texture2 = texture.getTexture();
        sf::Image image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number + "/Intermediates/d_entries_overlay.png");
     
        texture.draw(sprite);
        sf::Texture overlayTexture;
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/d_entries_overlay.png");
        sf::Sprite overlaySprite;
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        texture2 = texture.getTexture();
        image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number +"/d_entries.png");
    }
}
void printShotsGoalMap()
{
    sf::RenderTexture texture;
    texture.create(shotWindowWidth, shotWindowHeight);
    
    sf::Texture goalBackground;
    goalBackground.loadFromFile("Resources/goal.png");
    sf::Sprite sprite;
    sprite.setTexture(goalBackground, true);
    
    sf::Texture option1Texture;
    option1Texture.loadFromFile("Resources/hit_goal.png");
    sf::Sprite option1Sprite;
    option1Sprite.setTexture(option1Texture, true);
    
    sf::Texture option2Texture;
    option2Texture.loadFromFile("Resources/flick_goal.png");
    sf::Sprite option2Sprite;
    option2Sprite.setTexture(option2Texture, true);
    
    sf::Texture option3Texture;
    option3Texture.loadFromFile("Resources/push_goal.png");
    sf::Sprite option3Sprite;
    option3Sprite.setTexture(option3Texture, true);
    
    sf::Texture option4Texture;
    option4Texture.loadFromFile("Resources/slap_goal.png");
    sf::Sprite option4Sprite;
    option4Sprite.setTexture(option4Texture, true);
    
    sf::Texture option5Texture;
    option5Texture.loadFromFile("Resources/hit_no_goal.png");
    sf::Sprite option5Sprite;
    option5Sprite.setTexture(option5Texture, true);
    
    sf::Texture option6Texture;
    option6Texture.loadFromFile("Resources/flick_no_goal.png");
    sf::Sprite option6Sprite;
    option6Sprite.setTexture(option6Texture, true);
    
    sf::Texture option7Texture;
    option7Texture.loadFromFile("Resources/push_no_goal.png");
    sf::Sprite option7Sprite;
    option7Sprite.setTexture(option7Texture, true);
    
    sf::Texture option8Texture;
    option8Texture.loadFromFile("Resources/slap_no_goal.png");
    sf::Sprite option8Sprite;
    option8Sprite.setTexture(option8Texture, true);
    
    int posX = 0;
    int posY = 0;
    
    for (int i = 0; i <= counter; i++)
    {
        
        if (events[i][0] == SHOT && events[i][TYPE_2] == NO)
        {
            posX = events[i][7];
            posY = events[i][8];
            
            //check if deflected
            if (events[i][TYPE_3] == YES)
                {
                    int deflectionClearance = 3;
                    
                    sf::RectangleShape rectangle;
                    rectangle.setFillColor(sf::Color::Black);
                    
                    rectangle.setSize(sf::Vector2f(2 * (circleRadius + deflectionClearance), 2 * (circleRadius + deflectionClearance)));
                    rectangle.setPosition(posX - (circleRadius + deflectionClearance), posY - (circleRadius + deflectionClearance));
                    texture.draw(rectangle);
                    
                    rectangle.setFillColor(sf::Color::White);
                    rectangle.setSize(sf::Vector2f(2 * circleRadius, 2 * circleRadius));
                    rectangle.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(rectangle);
                    
                    rectangle.setFillColor(sf::Color::Black);
                }
            
            if (events[i][GOAL] == YES)
            {
                if (events[i][TYPE] == 1)
                {
                    option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(option1Sprite);
                }
                else if (events[i][TYPE] == 2)
                {
                    option2Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(option2Sprite);
                }
                else if (events[i][TYPE] == 3)
                {
                    option3Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(option3Sprite);
                }
                else if (events[i][TYPE] == 4)
                {
                    option4Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(option4Sprite);
                }
            }
            else
            {
                if (events[i][TYPE] == 1)
                {
                    option5Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(option5Sprite);
                }
                else if (events[i][TYPE] == 2)
                {
                    option6Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(option6Sprite);
                }
                else if (events[i][TYPE] == 3)
                {
                    option7Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(option7Sprite);
                }
                else if (events[i][TYPE] == 4)
                {
                    option8Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(option8Sprite);
                }
            }
        }
    }
    
    sf::Texture texture2 = texture.getTexture();
    sf::Image image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/Intermediates/shots_goal_map_overlay.png");
    
    texture.draw(sprite);
    sf::Texture overlayTexture;
    overlayTexture.loadFromFile(foldername + "/Intermediates/shots_goal_map_overlay.png");
    sf::Sprite overlaySprite;
    overlaySprite.setTexture(overlayTexture, true);
    texture.draw(overlaySprite);
    
    texture2 = texture.getTexture();
    image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/shots_goal_map.png");
    
    for (int j = 0; j < numberOfPlayers; j++)
    {
        int playerShirtNumber = playerNumbers[j];
        sf::RenderTexture texture;
        texture.create(shotWindowWidth, shotWindowHeight);
        texture.clear(sf::Color::Transparent);
        
        sf::Texture goalBackground;
        goalBackground.loadFromFile("Resources/goal.png");
        sf::Sprite sprite;
        sprite.setTexture(goalBackground, true);
        
        int posX = 0;
        int posY = 0;
        
        for (int i = 0; i <= counter; i++)
        {
            if (events[i][0] == SHOT && events[i][TYPE_2] == NO && events[i][1] == playerShirtNumber)
            {
                posX = events[i][7];
                posY = events[i][8];
                
                //check if deflected
                if (events[i][TYPE_3] == YES)
                {
                    int deflectionClearance = 3;
                    
                    sf::RectangleShape rectangle;
                    rectangle.setFillColor(sf::Color::Black);
                    
                    rectangle.setSize(sf::Vector2f(2 * (circleRadius + deflectionClearance), 2 * (circleRadius + deflectionClearance)));
                    rectangle.setPosition(posX - (circleRadius + deflectionClearance), posY - (circleRadius + deflectionClearance));
                    texture.draw(rectangle);
                    
                    rectangle.setFillColor(sf::Color::White);
                    rectangle.setSize(sf::Vector2f(2 * circleRadius, 2 * circleRadius));
                    rectangle.setPosition(posX - circleRadius, posY - circleRadius);
                    texture.draw(rectangle);
                    
                    rectangle.setFillColor(sf::Color::Black);
                }
                
                if (events[i][GOAL] == YES)
                {
                    if (events[i][TYPE] == 1)
                    {
                        option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                        texture.draw(option1Sprite);
                    }
                    else if (events[i][TYPE] == 2)
                    {
                        option2Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                        texture.draw(option2Sprite);
                    }
                    else if (events[i][TYPE] == 3)
                    {
                        option3Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                        texture.draw(option3Sprite);
                    }
                    else if (events[i][TYPE] == 4)
                    {
                        option4Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                        texture.draw(option4Sprite);
                    }
                }
                else
                {
                    if (events[i][TYPE] == 1)
                    {
                        option5Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                        texture.draw(option5Sprite);
                    }
                    else if (events[i][TYPE] == 2)
                    {
                        option6Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                        texture.draw(option6Sprite);
                    }
                    else if (events[i][TYPE] == 3)
                    {
                        option7Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                        texture.draw(option7Sprite);
                    }
                    else if (events[i][TYPE] == 4)
                    {
                        option8Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                        texture.draw(option8Sprite);
                    }
                }
            }

        }
        
        std::string number;
        number = std::to_string(playerShirtNumber);
        
        sf::Texture texture2 = texture.getTexture();
        sf::Image image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number + "/Intermediates/shots_goal_map_overlay.png");
        
        texture.draw(sprite);
        sf::Texture overlayTexture;
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/shots_goal_map_overlay.png");
        sf::Sprite overlaySprite;
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        texture2 = texture.getTexture();
        image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number +"/shots_goal_map.png");
    }

}
void printShotsDMap()
{
    sf::RenderTexture texture;
    texture.create(dMapWidth, dMapHeight);
    
    sf::Texture goalBackground;
    goalBackground.loadFromFile("Resources/d.png");
    sf::Sprite sprite;
    sprite.setTexture(goalBackground, true);
    
    sf::Texture option1Texture;
    option1Texture.loadFromFile("Resources/hit_goal.png");
    sf::Sprite option1Sprite;
    option1Sprite.setTexture(option1Texture, true);
    
    sf::Texture option2Texture;
    option2Texture.loadFromFile("Resources/flick_goal.png");
    sf::Sprite option2Sprite;
    option2Sprite.setTexture(option2Texture, true);
    
    sf::Texture option3Texture;
    option3Texture.loadFromFile("Resources/push_goal.png");
    sf::Sprite option3Sprite;
    option3Sprite.setTexture(option3Texture, true);
    
    sf::Texture option4Texture;
    option4Texture.loadFromFile("Resources/slap_goal.png");
    sf::Sprite option4Sprite;
    option4Sprite.setTexture(option4Texture, true);
    
    sf::Texture option5Texture;
    option5Texture.loadFromFile("Resources/hit_no_goal.png");
    sf::Sprite option5Sprite;
    option5Sprite.setTexture(option5Texture, true);
    
    sf::Texture option6Texture;
    option6Texture.loadFromFile("Resources/flick_no_goal.png");
    sf::Sprite option6Sprite;
    option6Sprite.setTexture(option6Texture, true);
    
    sf::Texture option7Texture;
    option7Texture.loadFromFile("Resources/push_no_goal.png");
    sf::Sprite option7Sprite;
    option7Sprite.setTexture(option7Texture, true);
    
    sf::Texture option8Texture;
    option8Texture.loadFromFile("Resources/slap_no_goal.png");
    sf::Sprite option8Sprite;
    option8Sprite.setTexture(option8Texture, true);
    
    sf::Texture deflectionTexture;
    deflectionTexture.loadFromFile("Resources/deflection.png");
    sf::Sprite deflectionSprite;
    deflectionSprite.setTexture(deflectionTexture, true);
    
    int posX = 0;
    int posY = 0;
    
    for (int i = 0; i <= counter; i++)
    {
        
        if (events[i][0] == SHOT)
        {
            posX = events[i][3] * 2;
            posY = events[i][4] * 2;
            
            if (events[i][TYPE_3] == YES)
                {
                    sf::RectangleShape rectangle;
                    rectangle.setFillColor(sf::Color::Black);
                    
                    int x2 = events[i][5] * 2;
                    int y2 = events[i][6] * 2;
                    
                    float length = getLength(posX, posY, x2, y2);
                    float angle = getAngle(posX, posY, x2, y2);
                    rectangle.setSize(sf::Vector2f(length, passLineWidth));
                    rectangle.setPosition(posX, posY);
                    rectangle.setRotation(-angle);
                    texture.draw(rectangle);
                    rectangle.setPosition(x2, y2);
                    rectangle.setRotation(-angle + 180);
                    texture.draw(rectangle);
                    deflectionSprite.setPosition(x2 - circleRadius, y2 - circleRadius);
                    texture.draw(deflectionSprite);
                    
                    if (events[i][GOAL] == YES)
                    {
                        if (events[i][TYPE] == 1)
                        {
                            option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option1Sprite);
                        }
                        else if (events[i][TYPE] == 2)
                        {
                            option2Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option2Sprite);
                        }
                        else if (events[i][TYPE] == 3)
                        {
                            option3Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option3Sprite);
                        }
                        else if (events[i][TYPE] == 4)
                        {
                            option4Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option4Sprite);
                        }
                    }
                    else
                    {
                        if (events[i][TYPE] == 1)
                        {
                            option5Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option5Sprite);
                        }
                        else if (events[i][TYPE] == 2)
                        {
                            option6Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option6Sprite);
                        }
                        else if (events[i][TYPE] == 3)
                        {
                            option7Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option7Sprite);
                        }
                        else if (events[i][TYPE] == 4)
                        {
                            option8Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option8Sprite);
                        }
                    }
                }
            else
            {
                if (events[i][GOAL] == YES)
                {
                    if (events[i][TYPE] == 1)
                    {
                        option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                        texture.draw(option1Sprite);
                    }
                    else if (events[i][TYPE] == 2)
                    {
                        option2Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                        texture.draw(option2Sprite);
                    }
                    else if (events[i][TYPE] == 3)
                    {
                        option3Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                        texture.draw(option3Sprite);
                    }
                    else if (events[i][TYPE] == 4)
                    {
                        option4Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                        texture.draw(option4Sprite);
                    }
                }
                else
                {
                    if (events[i][TYPE] == 1)
                    {
                        option5Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                        texture.draw(option5Sprite);
                    }
                    else if (events[i][TYPE] == 2)
                    {
                        option6Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                        texture.draw(option6Sprite);
                    }
                    else if (events[i][TYPE] == 3)
                    {
                        option7Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                        texture.draw(option7Sprite);
                    }
                    else if (events[i][TYPE] == 4)
                    {
                        option8Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                        texture.draw(option8Sprite);
                    }
                }
            }
        }
    }
    
    sf::Texture texture2 = texture.getTexture();
    sf::Image image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/Intermediates/shots_d_map_overlay.png");
    
    texture.draw(sprite);
    sf::Texture overlayTexture;
    overlayTexture.loadFromFile(foldername + "/Intermediates/shots_d_map_overlay.png");
    sf::Sprite overlaySprite;
    overlaySprite.setTexture(overlayTexture, true);
    texture.draw(overlaySprite);
    
    texture2 = texture.getTexture();
    image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/shots_d_map.png");
    
    for (int j = 0; j < numberOfPlayers; j++)
    {
        int playerShirtNumber = playerNumbers[j];
        sf::RenderTexture texture;
        texture.create(dMapWidth, dMapHeight);
        texture.clear(sf::Color::Transparent);
        
        sf::Texture goalBackground;
        goalBackground.loadFromFile("Resources/d.png");
        sf::Sprite sprite;
        sprite.setTexture(goalBackground, true);
        
        int posX = 0;
        int posY = 0;
        
        for (int i = 0; i <= counter; i++)
        {
            if (events[i][0] == SHOT && events[i][1] == playerShirtNumber)
            {
                posX = events[i][3] * 2;
                posY = events[i][4] * 2;
                
                if (events[i][TYPE_3] == YES)
                {
                    sf::RectangleShape rectangle;
                    rectangle.setFillColor(sf::Color::Black);
                    
                    int x2 = events[i][5] * 2;
                    int y2 = events[i][6] * 2;
                    
                    float length = getLength(posX, posY, x2, y2);
                    float angle = getAngle(posX, posY, x2, y2);
                    rectangle.setSize(sf::Vector2f(length, passLineWidth));
                    rectangle.setPosition(posX, posY);
                    rectangle.setRotation(-angle);
                    texture.draw(rectangle);
                    rectangle.setPosition(x2, y2);
                    rectangle.setRotation(-angle + 180);
                    texture.draw(rectangle);
                    deflectionSprite.setPosition(x2 - circleRadius, y2 - circleRadius);
                    texture.draw(deflectionSprite);
                    
                    if (events[i][GOAL] == YES)
                    {
                        if (events[i][TYPE] == 1)
                        {
                            option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option1Sprite);
                        }
                        else if (events[i][TYPE] == 2)
                        {
                            option2Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option2Sprite);
                        }
                        else if (events[i][TYPE] == 3)
                        {
                            option3Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option3Sprite);
                        }
                        else if (events[i][TYPE] == 4)
                        {
                            option4Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option4Sprite);
                        }
                    }
                    else
                    {
                        if (events[i][TYPE] == 1)
                        {
                            option5Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option5Sprite);
                        }
                        else if (events[i][TYPE] == 2)
                        {
                            option6Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option6Sprite);
                        }
                        else if (events[i][TYPE] == 3)
                        {
                            option7Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option7Sprite);
                        }
                        else if (events[i][TYPE] == 4)
                        {
                            option8Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option8Sprite);
                        }
                    }
                }
                else
                {
                    if (events[i][GOAL] == YES)
                    {
                        if (events[i][TYPE] == 1)
                        {
                            option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option1Sprite);
                        }
                        else if (events[i][TYPE] == 2)
                        {
                            option2Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option2Sprite);
                        }
                        else if (events[i][TYPE] == 3)
                        {
                            option3Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option3Sprite);
                        }
                        else if (events[i][TYPE] == 4)
                        {
                            option4Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option4Sprite);
                        }
                    }
                    else
                    {
                        if (events[i][TYPE] == 1)
                        {
                            option5Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option5Sprite);
                        }
                        else if (events[i][TYPE] == 2)
                        {
                            option6Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option6Sprite);
                        }
                        else if (events[i][TYPE] == 3)
                        {
                            option7Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option7Sprite);
                        }
                        else if (events[i][TYPE] == 4)
                        {
                            option8Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option8Sprite);
                        }
                    }
                }
            }
        }
        
        std::string number;
        number = std::to_string(playerShirtNumber);
        
        sf::Texture texture2 = texture.getTexture();
        sf::Image image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number + "/Intermediates/shots_d_map_overlay.png");
        
        texture.draw(sprite);
        sf::Texture overlayTexture;
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/shots_d_map_overlay.png");
        sf::Sprite overlaySprite;
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        texture2 = texture.getTexture();
        image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number +"/shots_d_map.png");
    }
    
}
void printPCWins()
{
    sf::RenderTexture texture;
    texture.create(dMapWidth, dMapHeight);
    
    sf::Texture pitchBackground;
    pitchBackground.loadFromFile("Resources/d.png");
    sf::Sprite sprite;
    sprite.setTexture(pitchBackground, true);
    
    sf::Texture option1Texture;
    option1Texture.loadFromFile("Resources/pc_won.png");
    sf::Sprite option1Sprite;
    option1Sprite.setTexture(option1Texture, true);
    
    sf::CircleShape circle(circleRadius);
    circle.setFillColor(sf::Color::Green);
    
    int posX = 0;
    int posY = 0;
    
    for (int i = 0; i <= counter; i++)
    {
        
        if (events[i][0] == PENALTY_CORNER)
        {
            posX = events[i][3];
            posY = events[i][4];

            option1Sprite.setPosition(posX * 2, posY * 2);
            texture.draw(option1Sprite);
        }
    }
    
    sf::Texture texture2 = texture.getTexture();
    sf::Image image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/Intermediates/pc_wins_overlay.png");
    
    texture.draw(sprite);
    sf::Texture overlayTexture;
    overlayTexture.loadFromFile(foldername + "/Intermediates/pc_wins_overlay.png");
    sf::Sprite overlaySprite;
    overlaySprite.setTexture(overlayTexture, true);
    texture.draw(overlaySprite);
    
    texture2 = texture.getTexture();
    image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/pc_wins.png");
    
    for (int j = 0; j < numberOfPlayers; j++)
    {
        int playerShirtNumber = playerNumbers[j];
        sf::RenderTexture texture;
        texture.create(dMapWidth, dMapHeight);
        texture.clear(sf::Color::Transparent);
        
        sf::Texture pitchBackground;
        pitchBackground.loadFromFile("Resources/d.png");
        sf::Sprite sprite;
        sprite.setTexture(pitchBackground, true);
        
        int posX = 0;
        int posY = 0;
        
        for (int i = 0; i <= counter; i++)
        {
            
            if (events[i][0] == PENALTY_CORNER && events[i][2] == playerShirtNumber)
            {
                posX = events[i][3];
                posY = events[i][4];
                
                option1Sprite.setPosition(posX * 2, posY * 2);
                texture.draw(option1Sprite);
            }
        }
        
        std::string number;
        number = std::to_string(playerShirtNumber);
        
        sf::Texture texture2 = texture.getTexture();
        sf::Image image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number + "/Intermediates/pc_wins_overlay.png");
        
        texture.draw(sprite);
        sf::Texture overlayTexture;
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/pc_wins_overlay.png");
        sf::Sprite overlaySprite;
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        texture2 = texture.getTexture();
        image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number +"/pc_wins.png");
    }
}
void printPSWins()
{
    sf::RenderTexture texture;
    texture.create(dMapWidth, dMapHeight);
    
    sf::Texture pitchBackground;
    pitchBackground.loadFromFile("Resources/d.png");
    sf::Sprite sprite;
    sprite.setTexture(pitchBackground, true);
    
    sf::Texture option1Texture;
    option1Texture.loadFromFile("Resources/ps_won.png");
    sf::Sprite option1Sprite;
    option1Sprite.setTexture(option1Texture, true);
    
    sf::CircleShape circle(circleRadius);
    circle.setFillColor(sf::Color::Green);
    
    int posX = 0;
    int posY = 0;
    
    for (int i = 0; i <= counter; i++)
    {
        
        if (events[i][0] == PENALTY_STROKE)
        {
            posX = events[i][3];
            posY = events[i][4];
            
            option1Sprite.setPosition(posX * 2, posY * 2);
            texture.draw(option1Sprite);
        }
    }
    
    sf::Texture texture2 = texture.getTexture();
    sf::Image image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/Intermediates/ps_wins_overlay.png");
    
    texture.draw(sprite);
    sf::Texture overlayTexture;
    overlayTexture.loadFromFile(foldername + "/Intermediates/ps_wins_overlay.png");
    sf::Sprite overlaySprite;
    overlaySprite.setTexture(overlayTexture, true);
    texture.draw(overlaySprite);
    
    texture2 = texture.getTexture();
    image = texture2.copyToImage();
    image.flipVertically();
    image.saveToFile(foldername + "/ps_wins.png");
    
    for (int j = 0; j < numberOfPlayers; j++)
    {
        int playerShirtNumber = playerNumbers[j];
        sf::RenderTexture texture;
        texture.create(dMapWidth, dMapHeight);
        texture.clear(sf::Color::Transparent);
        
        sf::Texture pitchBackground;
        pitchBackground.loadFromFile("Resources/d.png");
        sf::Sprite sprite;
        sprite.setTexture(pitchBackground, true);
        
        int posX = 0;
        int posY = 0;
        
        for (int i = 0; i <= counter; i++)
        {
            
            if (events[i][0] == PENALTY_STROKE && events[i][2] == playerShirtNumber)
            {
                posX = events[i][3];
                posY = events[i][4];
                
                option1Sprite.setPosition(posX * 2, posY * 2);
                texture.draw(option1Sprite);
            }
        }
        
        std::string number;
        number = std::to_string(playerShirtNumber);
        
        sf::Texture texture2 = texture.getTexture();
        sf::Image image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number + "/Intermediates/ps_wins_overlay.png");
        
        texture.draw(sprite);
        sf::Texture overlayTexture;
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/ps_wins_overlay.png");
        sf::Sprite overlaySprite;
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        texture2 = texture.getTexture();
        image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number +"/ps_wins.png");
    }
}
void printShotsDMapLarge()
{
    sf::Texture option1Texture;
    option1Texture.loadFromFile("Resources/hit_goal.png");
    sf::Sprite option1Sprite;
    option1Sprite.setTexture(option1Texture, true);
    
    sf::Texture option2Texture;
    option2Texture.loadFromFile("Resources/flick_goal.png");
    sf::Sprite option2Sprite;
    option2Sprite.setTexture(option2Texture, true);
    
    sf::Texture option3Texture;
    option3Texture.loadFromFile("Resources/push_goal.png");
    sf::Sprite option3Sprite;
    option3Sprite.setTexture(option3Texture, true);
    
    sf::Texture option4Texture;
    option4Texture.loadFromFile("Resources/slap_goal.png");
    sf::Sprite option4Sprite;
    option4Sprite.setTexture(option4Texture, true);
    
    sf::Texture option5Texture;
    option5Texture.loadFromFile("Resources/hit_no_goal.png");
    sf::Sprite option5Sprite;
    option5Sprite.setTexture(option5Texture, true);
    
    sf::Texture option6Texture;
    option6Texture.loadFromFile("Resources/flick_no_goal.png");
    sf::Sprite option6Sprite;
    option6Sprite.setTexture(option6Texture, true);
    
    sf::Texture option7Texture;
    option7Texture.loadFromFile("Resources/push_no_goal.png");
    sf::Sprite option7Sprite;
    option7Sprite.setTexture(option7Texture, true);
    
    sf::Texture option8Texture;
    option8Texture.loadFromFile("Resources/slap_no_goal.png");
    sf::Sprite option8Sprite;
    option8Sprite.setTexture(option8Texture, true);
    
    sf::Texture deflectionTexture;
    deflectionTexture.loadFromFile("Resources/deflection.png");
    sf::Sprite deflectionSprite;
    deflectionSprite.setTexture(deflectionTexture, true);
    
    for (int j = 0; j < numberOfPlayers; j++)
    {
        int playerShirtNumber = playerNumbers[j];
        sf::RenderTexture texture;
        texture.create(pitchWidth, pitchHeight/4);
        texture.clear(sf::Color::Transparent);
        
        int posX = 0;
        int posY = 0;
        
        for (int i = 0; i <= counter; i++)
        {
            if (events[i][0] == SHOT && events[i][1] == playerShirtNumber)
            {
                posX = events[i][3];
                posY = events[i][4];
                
                if (events[i][TYPE_3] == YES)
                {
                    sf::RectangleShape rectangle;
                    rectangle.setFillColor(sf::Color::Black);
                    
                    int x2 = events[i][5];
                    int y2 = events[i][6];
                    
                    float length = getLength(posX, posY, x2, y2);
                    float angle = getAngle(posX, posY, x2, y2);
                    rectangle.setSize(sf::Vector2f(length, passLineWidth));
                    rectangle.setPosition(posX, posY);
                    rectangle.setRotation(-angle);
                    texture.draw(rectangle);
                    rectangle.setPosition(x2, y2);
                    rectangle.setRotation(-angle + 180);
                    texture.draw(rectangle);
                    deflectionSprite.setPosition(x2 - circleRadius, y2 - circleRadius);
                    texture.draw(deflectionSprite);
                    
                    if (events[i][GOAL] == YES)
                    {
                        if (events[i][TYPE] == 1)
                        {
                            option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option1Sprite);
                        }
                        else if (events[i][TYPE] == 2)
                        {
                            option2Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option2Sprite);
                        }
                        else if (events[i][TYPE] == 3)
                        {
                            option3Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option3Sprite);
                        }
                        else if (events[i][TYPE] == 4)
                        {
                            option4Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option4Sprite);
                        }
                    }
                    else
                    {
                        if (events[i][TYPE] == 1)
                        {
                            option5Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option5Sprite);
                        }
                        else if (events[i][TYPE] == 2)
                        {
                            option6Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option6Sprite);
                        }
                        else if (events[i][TYPE] == 3)
                        {
                            option7Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option7Sprite);
                        }
                        else if (events[i][TYPE] == 4)
                        {
                            option8Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option8Sprite);
                        }
                    }
                }
                else
                {
                    if (events[i][GOAL] == YES)
                    {
                        if (events[i][TYPE] == 1)
                        {
                            option1Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option1Sprite);
                        }
                        else if (events[i][TYPE] == 2)
                        {
                            option2Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option2Sprite);
                        }
                        else if (events[i][TYPE] == 3)
                        {
                            option3Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option3Sprite);
                        }
                        else if (events[i][TYPE] == 4)
                        {
                            option4Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option4Sprite);
                        }
                    }
                    else
                    {
                        if (events[i][TYPE] == 1)
                        {
                            option5Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option5Sprite);
                        }
                        else if (events[i][TYPE] == 2)
                        {
                            option6Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option6Sprite);
                        }
                        else if (events[i][TYPE] == 3)
                        {
                            option7Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option7Sprite);
                        }
                        else if (events[i][TYPE] == 4)
                        {
                            option8Sprite.setPosition(posX - circleRadius, posY - circleRadius);
                            texture.draw(option8Sprite);
                        }
                    }
                }
            }
        }
        
        std::string number;
        number = std::to_string(playerShirtNumber);
        
        sf::Texture texture2 = texture.getTexture();
        sf::Image image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number + "/Intermediates/shots_d_map_large_overlay.png");
    }
}
void printPCWinsLarge()
{
    
    sf::Texture option1Texture;
    option1Texture.loadFromFile("Resources/pc_won.png");
    sf::Sprite option1Sprite;
    option1Sprite.setTexture(option1Texture, true);
    
    sf::CircleShape circle(circleRadius);
    circle.setFillColor(sf::Color::Green);
    
    for (int j = 0; j < numberOfPlayers; j++)
    {
        int playerShirtNumber = playerNumbers[j];
        sf::RenderTexture texture;
        texture.create(pitchWidth, pitchHeight/4);
        texture.clear(sf::Color::Transparent);
        
        int posX = 0;
        int posY = 0;
        
        for (int i = 0; i <= counter; i++)
        {
            
            if (events[i][0] == PENALTY_CORNER && events[i][2] == playerShirtNumber)
            {
                posX = events[i][3];
                posY = events[i][4];
                
                option1Sprite.setPosition(posX, posY);
                texture.draw(option1Sprite);
            }
        }
        
        std::string number;
        number = std::to_string(playerShirtNumber);
        
        sf::Texture texture2 = texture.getTexture();
        sf::Image image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number + "/Intermediates/pc_wins_large_overlay.png");
    }
}
void printPSWinsLarge()
{
    
    sf::Texture option1Texture;
    option1Texture.loadFromFile("Resources/ps_won.png");
    sf::Sprite option1Sprite;
    option1Sprite.setTexture(option1Texture, true);
    
    sf::CircleShape circle(circleRadius);
    circle.setFillColor(sf::Color::Green);
    
    
    for (int j = 0; j < numberOfPlayers; j++)
    {
        int playerShirtNumber = playerNumbers[j];
        sf::RenderTexture texture;
        texture.create(pitchWidth, pitchHeight/4);
        texture.clear(sf::Color::Transparent);
        
        int posX = 0;
        int posY = 0;
        
        for (int i = 0; i <= counter; i++)
        {
            
            if (events[i][0] == PENALTY_STROKE && events[i][2] == playerShirtNumber)
            {
                posX = events[i][3];
                posY = events[i][4];
                
                option1Sprite.setPosition(posX, posY);
                texture.draw(option1Sprite);
            }
        }
        
        std::string number;
        number = std::to_string(playerShirtNumber);
        
        sf::Texture texture2 = texture.getTexture();
        sf::Image image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number + "/Intermediates/ps_wins_large_overlay.png");
    }
}
void printFullPlayerImages()
{
    sf::RenderTexture texture;
    
    for (int j = 0; j < numberOfPlayers; j++)
    {
        int playerShirtNumber = playerNumbers[j];
        sf::RenderTexture texture;
        texture.create(pitchWidth, pitchHeight);
        texture.clear(sf::Color::Transparent);
        
        sf::Texture pitchBackground;
        pitchBackground.loadFromFile("Resources/pitch.png");
        sf::Sprite sprite;
        sprite.setTexture(pitchBackground, true);
        texture.draw(sprite);
        
        std::string number;
        number = std::to_string(playerShirtNumber);
        
        sf::Texture overlayTexture;
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/passes_overlay.png");
        sf::Sprite overlaySprite;
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/turnover_wins_overlay.png");
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/turnover_losses_overlay.png");
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/free_wins_overlay.png");
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/penalties_conceded_overlay.png");
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/eliminations_overlay.png");
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/defensive_errors_overlay.png");
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/d_entries_overlay.png");
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/pc_wins_large_overlay.png");
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/ps_wins_large_overlay.png");
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        overlayTexture.loadFromFile(foldername + "/" + number +"/Intermediates/shots_d_map_large_overlay.png");
        overlaySprite.setTexture(overlayTexture, true);
        texture.draw(overlaySprite);
        
        sf::Texture texture2 = texture.getTexture();
        sf::Image image = texture2.copyToImage();
        image.flipVertically();
        image.saveToFile(foldername + "/" + number +"/summary_pitch_map.png");
    }
}

void clearPassCoordinates()
{
    passCoordinates[0] = 0;
    passCoordinates[1] = 0;
    passCoordinates[2] = 0;
    passCoordinates[3] = 0;
}
void clearLocationCoordinates()
{
    locationCoordinates[0] = 0;
    locationCoordinates[1] = 0;
}
void clearShotCoordinates()
{
    shotCoordinates[0] = 0;
    shotCoordinates[1] = 0;
}

bool checkInsideD(int posX, int posY)
{
    if (((posX - pitchWidth/2)*(posX - pitchWidth/2) + (posY)*(posY)) < dRadius*dRadius)
        return true;
    else
        return false;
}
bool checkOnTarget(int posX, int posY)
{
    if (posX > goalClearance && posX < goalClearance + goalWidth + postWidth*2 && posY > goalClearance -postWidth)
        return true;
    else
        return false;
}

float getLength(int x1, int y1, int x2, int y2)
{
    float rise = y2 - y1;
    float run = x2 - x1;
    float length = sqrt(rise*rise + run*run);
    return length;
}
float getAngle(int x1, int y1, int x2, int y2)
{
    float rise = -(y2 - y1);
    float run = x2 - x1;
    float angle = 0;
    if (run >= 0)
        angle = atan(rise/run) * 180/PI;
    else
        angle = atan(rise/run) * 180/PI + 180;
    return angle;
}

//Drawing backgrounds for windows
void drawPitchMap()
{
    sf::RenderTexture texture;
    texture.create(pitchWidth, pitchHeight);
    //draw pitch
    
    //set background to white
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(pitchWidth, pitchHeight));
    rectangle.setFillColor(sf::Color::White);
    rectangle.setPosition(0, 0);
    texture.draw(rectangle);
    
    //attacking D
    sf::CircleShape circle(dRadius);
    circle.setFillColor(sf::Color::White);
    circle.setOutlineThickness(lineWidth);
    circle.setOutlineColor(sf::Color::Black);
    circle.setPosition(pitchWidth/2 - dRadius, -dRadius);
    texture.draw(circle);
    //defensive D
    circle.setFillColor(sf::Color::White);
    circle.setOutlineThickness(lineWidth);
    circle.setOutlineColor(sf::Color::Black);
    circle.setPosition(pitchWidth/2 - dRadius, pitchHeight - dRadius);
    texture.draw(circle);
    
    //top line
    rectangle.setSize(sf::Vector2f(pitchWidth, lineWidth));
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setPosition(0, 0);
    texture.draw(rectangle);
    //attacking 23 line
    rectangle.setSize(sf::Vector2f(pitchWidth, lineWidth));
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setPosition(0, (pitchHeight - lineWidth)/4);
    texture.draw(rectangle);
    //halfway line
    rectangle.setSize(sf::Vector2f(pitchWidth, lineWidth));
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setPosition(0, (pitchHeight - lineWidth)/2);
    texture.draw(rectangle);
    //defensive 23 line
    rectangle.setSize(sf::Vector2f(pitchWidth, lineWidth));
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setPosition(0, 3*(pitchHeight - lineWidth)/4);
    texture.draw(rectangle);
    //bottom line
    rectangle.setSize(sf::Vector2f(pitchWidth, lineWidth));
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setPosition(0, pitchHeight - lineWidth);
    texture.draw(rectangle);
    //left sideline
    rectangle.setSize(sf::Vector2f(lineWidth, pitchHeight));
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setPosition(0, 0);
    texture.draw(rectangle);
    //right sideline
    rectangle.setSize(sf::Vector2f(lineWidth, pitchHeight));
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setPosition(pitchWidth - lineWidth, 0);
    texture.draw(rectangle);
    
    sf::Texture texture2 = texture.getTexture();
    sf::Image image = texture2.copyToImage();
    image.sf::Image::flipVertically();
    image.saveToFile("Resources/pitch.png");
}
void drawDMap()
{
    sf::RenderTexture texture;
    texture.create(dMapWidth, dMapHeight);
    
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(dMapWidth, dMapHeight));
    rectangle.setFillColor(sf::Color::White);
    rectangle.setPosition(0, 0);
    texture.draw(rectangle);
    
    //attacking D
    sf::CircleShape circle(dRadius * 2);
    circle.setFillColor(sf::Color::White);
    circle.setOutlineThickness(lineWidth * 2);
    circle.setOutlineColor(sf::Color::Black);
    circle.setPosition(pitchWidth - dRadius * 2, -dRadius * 2);
    texture.draw(circle);
    
    //top line
    rectangle.setSize(sf::Vector2f(dMapWidth, lineWidth * 2));
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setPosition(0, 0);
    texture.draw(rectangle);
    //attacking 23 line
    rectangle.setSize(sf::Vector2f(dMapWidth, lineWidth * 2));
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setPosition(0, dMapHeight - lineWidth * 2);
    texture.draw(rectangle);
    //left sideline
    rectangle.setSize(sf::Vector2f(lineWidth * 2, dMapHeight));
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setPosition(0, 0);
    texture.draw(rectangle);
    //right sideline
    rectangle.setSize(sf::Vector2f(lineWidth * 2, dMapHeight));
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setPosition((pitchWidth - lineWidth) * 2, 0);
    texture.draw(rectangle);
    
    //flip
    sf::Texture texture2 = texture.getTexture();
    sf::Image image = texture2.copyToImage();
    image.sf::Image::flipVertically();
    image.saveToFile("Resources/d.png");
}
void drawGoalMap()
{
    sf::RenderTexture texture;
    texture.create(shotWindowWidth, shotWindowHeight);
    
    //set background to white
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(shotWindowWidth, shotWindowHeight));
    rectangle.setFillColor(sf::Color::White);
    rectangle.setPosition(0, 0);
    texture.draw(rectangle);
    
    //draw goal
    
    //cross bar
    rectangle.setSize(sf::Vector2f(goalWidth + 2*postWidth, postWidth));
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setPosition(goalClearance, goalClearance);
    texture.draw(rectangle);
    //left post
    rectangle.setSize(sf::Vector2f(postWidth, goalHeight));
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setPosition(goalClearance, goalClearance + postWidth);
    texture.draw(rectangle);
    //right post
    rectangle.setSize(sf::Vector2f(postWidth, goalHeight));
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setPosition(goalClearance + postWidth + goalWidth, goalClearance + postWidth);
    texture.draw(rectangle);
    //backboard
    rectangle.setSize(sf::Vector2f(goalWidth, backboardLineWidth));
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setPosition(goalClearance + postWidth, goalClearance + postWidth + goalHeight - backboardHeight);
    texture.draw(rectangle);
    
    //flip
    sf::Texture texture2 = texture.getTexture();
    sf::Image image = texture2.copyToImage();
    image.sf::Image::flipVertically();
    image.saveToFile("Resources/goal.png");
}

//Pop-up Windows
void passWindow()
{
    bool pressed = false;
    
    sf::RenderWindow window(sf::VideoMode(pitchWidth / screenMultiplier, pitchHeight / screenMultiplier), "");
    window.setMouseCursorVisible(false);
    window.setFramerateLimit(FPS);
    window.requestFocus();
    
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    
    while (window.isOpen())
    {
        sf::Event event;
        
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == event.MouseButtonPressed)
            {
                pressed = !pressed;
                if (pressed == true)
                {
                    sf::Vector2i location = sf::Mouse::getPosition(window);
                    passCoordinates[0] = location.x * screenMultiplier;
                    passCoordinates[1] = location.y * screenMultiplier;
                }
                else if (pressed == false)
                {
                    sf::Vector2i location = sf::Mouse::getPosition(window);
                    passCoordinates[2] = location.x * screenMultiplier;
                    passCoordinates[3] = location.y * screenMultiplier;
                    window.close();
                }
            }
            else
            {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                pos_x = position.x;
                pos_y = position.y;
            }
        }
        // clear the window
        window.clear(sf::Color::White);
        
        //draw pitch
        sf::Texture texture;
        texture.loadFromFile("Resources/pitch.png");
        sf::Sprite sprite;
        sprite.setTexture(texture, true);
		sprite.setScale(1 / screenMultiplier, 1 / screenMultiplier);
        window.draw(sprite);
        
        sf::RectangleShape rectangle;
        
        //draw mouse position
        sf::CircleShape shape(circleRadius / screenMultiplier);
		shape.setPosition(pos_x - circleRadius / screenMultiplier, pos_y - circleRadius / screenMultiplier);
        shape.setFillColor(sf::Color::Black);
        window.draw(shape);
        
        //if passing spot recorded, draw circle and line.
        if (pressed == true)
        {
            //draw position of pass
            shape.setPosition((passCoordinates[0] - circleRadius) / screenMultiplier, (passCoordinates[1] - circleRadius) / screenMultiplier);
            shape.setFillColor(sf::Color::Black);
            window.draw(shape);
            
            //draw line to current mouse position
            float angle = getAngle(passCoordinates[0] / screenMultiplier, passCoordinates[1] / screenMultiplier, pos_x, pos_y);
            float length = getLength(passCoordinates[0] / screenMultiplier, passCoordinates[1] / screenMultiplier, pos_x, pos_y);
            
            rectangle.setSize(sf::Vector2f(length, passLineWidth/2));
            rectangle.setFillColor(sf::Color::Black);
            rectangle.setPosition(passCoordinates[0] / screenMultiplier, passCoordinates[1] / screenMultiplier);
            rectangle.setRotation(-angle);
            window.draw(rectangle);
            rectangle.setSize(sf::Vector2f(length, passLineWidth/2));
            rectangle.setFillColor(sf::Color::Black);
            rectangle.setPosition(pos_x, pos_y);
            rectangle.setRotation(-angle + 180);
            window.draw(rectangle);
            
        }
        
        // end the current frame
        window.display();
    }
}
void locationWindow()
{
    sf::RenderWindow window(sf::VideoMode(pitchWidth / screenMultiplier, pitchHeight / screenMultiplier), "");
    window.setMouseCursorVisible(false);
    window.setFramerateLimit(FPS);
    window.requestFocus();
    
    while (window.isOpen())
    {
        sf::Event event;
        
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == event.MouseButtonPressed)
            {
                sf::Vector2i location = sf::Mouse::getPosition(window);
                locationCoordinates[0] = location.x * screenMultiplier;
                locationCoordinates[1] = location.y * screenMultiplier;
                window.close();
            }
            else
            {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                pos_x = position.x;
                pos_y = position.y;
            }
        }
        // clear the window
        window.clear(sf::Color::White);
        
        //draw pitch
        sf::Texture texture;
        texture.loadFromFile("Resources/pitch.png");
        sf::Sprite sprite;
        sprite.setTexture(texture, true);
		sprite.setScale(1 / screenMultiplier, 1 / screenMultiplier);
        window.draw(sprite);
        
        //draw mouse position
        sf::CircleShape shape(circleRadius / screenMultiplier);
        shape.setPosition(pos_x - circleRadius / screenMultiplier, pos_y - circleRadius / screenMultiplier);
        shape.setFillColor(sf::Color::Black);
        window.draw(shape);
        
        // end the current frame
        window.display();
    }
}
void shotWindow()
{
    sf::RenderWindow window(sf::VideoMode(shotWindowWidth / screenMultiplier, shotWindowHeight / screenMultiplier), "");
    window.setMouseCursorVisible(false);
    window.setFramerateLimit(FPS);
    window.requestFocus();
    
    while (window.isOpen())
    {
        sf::Event event;
        
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == event.MouseButtonPressed)
            {
                sf::Vector2i location = sf::Mouse::getPosition(window);
                shotCoordinates[0] = location.x * screenMultiplier;
                shotCoordinates[1] = location.y * screenMultiplier;
                window.close();
            }
            else
            {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                pos_x = position.x;
                pos_y = position.y;
            }
        }
        // clear the window
        window.clear(sf::Color::White);
        
        //draw goal
        sf::Texture texture;
        texture.loadFromFile("Resources/goal.png");
        sf::Sprite sprite;
        sprite.setTexture(texture, true);
		sprite.setScale(1 / screenMultiplier, 1 / screenMultiplier);
        window.draw(sprite);
        
        //draw mouse position
        sf::CircleShape shape(circleRadius * 2 / screenMultiplier);
        shape.setPosition(pos_x - (circleRadius*2)/screenMultiplier, pos_y - (circleRadius*2)/screenMultiplier);
        shape.setFillColor(sf::Color::Black);
        window.draw(shape);
        
        // end the current frame
        window.display();
    }
}
void dWindow()
{
    sf::RenderWindow window(sf::VideoMode(dMapWidth / screenMultiplier, dMapHeight / screenMultiplier), "");
    window.setMouseCursorVisible(false);
    window.setFramerateLimit(FPS);
    window.requestFocus();
    
    while (window.isOpen())
    {
        sf::Event event;
        
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == event.MouseButtonPressed)
            {
                sf::Vector2i location = sf::Mouse::getPosition(window);
                locationCoordinates[0] = (location.x)/2 * screenMultiplier;
                locationCoordinates[1] = (location.y)/2 * screenMultiplier;
                window.close();
            }
            else
            {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                pos_x = position.x;
                pos_y = position.y;
            }
        }
        // clear the window
        window.clear(sf::Color::White);
        
        //draw pitch
        sf::Texture texture;
        texture.loadFromFile("Resources/d.png");
        sf::Sprite sprite;
        sprite.setTexture(texture, true);
		sprite.setScale(1 / screenMultiplier, 1 / screenMultiplier);
        window.draw(sprite);
        
        //draw mouse position
        sf::CircleShape shape(circleRadius / screenMultiplier);
        shape.setPosition(pos_x - circleRadius / screenMultiplier, pos_y - circleRadius / screenMultiplier);
        shape.setFillColor(sf::Color::Black);
        window.draw(shape);
        
        // end the current frame
        window.display();
    }
}
void carryWindow()
{
    sf::RenderWindow window(sf::VideoMode(dMapWidth / screenMultiplier, dMapHeight / screenMultiplier), "");
    window.setMouseCursorVisible(false);
    window.setFramerateLimit(FPS);
    window.requestFocus();
    float entryY = 0;
    
    while (window.isOpen())
    {
        sf::Event event;
        
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == event.MouseButtonPressed)
            {
                sf::Vector2i location = sf::Mouse::getPosition(window);
                locationCoordinates[0] = (location.x)/2 * screenMultiplier;
                
                //find y coordinate of entry point
				entryY = sqrt((4 * dRadius*dRadius) / (screenMultiplier*screenMultiplier) - ((dMapWidth / 2) / screenMultiplier - location.x) * ((dMapWidth / 2) / screenMultiplier - location.x));
				locationCoordinates[1] = (entryY)/2 * screenMultiplier;
                window.close();
            }
            else
            {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                pos_x = position.x;
                pos_y = position.y;
            }
        }
        // clear the window
        window.clear(sf::Color::White);
        
        //draw pitch
        sf::Texture texture;
        texture.loadFromFile("Resources/d.png");
        sf::Sprite sprite;
        sprite.setTexture(texture, true);
		sprite.setScale(1 / screenMultiplier, 1 / screenMultiplier);
        window.draw(sprite);
        
        sf::RectangleShape rectangle;
        sf::CircleShape shape(carryCircleRadius / screenMultiplier);
        
        //draw mouse position
        shape.setPosition(pos_x - carryCircleRadius / screenMultiplier, pos_y - carryCircleRadius / screenMultiplier);
        shape.setFillColor(sf::Color::Black);
        window.draw(shape);
        
        //draw vertical line
        rectangle.setSize(sf::Vector2f(passLineWidth * 2 / screenMultiplier, dMapHeight / screenMultiplier));
        rectangle.setFillColor(sf::Color::Black);
        rectangle.setPosition(pos_x - passLineWidth / screenMultiplier, 0);
        window.draw(rectangle);
        
        //find y coordinate of entry point
        entryY = sqrt((4 * dRadius*dRadius)/(screenMultiplier*screenMultiplier) - ((dMapWidth/2)/screenMultiplier - pos_x) * ((dMapWidth/2)/screenMultiplier - pos_x));
        
        //draw entry position
        shape.setPosition(pos_x - carryCircleRadius / screenMultiplier, entryY);
        shape.setFillColor(sf::Color::Red);
        window.draw(shape);
        
        // end the current frame
        window.display();
    }
}

void pass()
{
    std::string input = "";
    events[counter][0] = PASS;
    std::cout << "Passer (#)" << std::endl;
    std::cin >> input;
    events[counter][1] = stringToInt(input);
    input = "";
    std::cout << "Receiver (#) OR (i - incomplete; m - mistrap; o - out of play)" << std::endl;
    std::cin >> input;
    std::string passType = input;
    events[counter][TYPE] = YES;
    if (input == "i")
        {
        events[counter][TYPE] = NO;
        events[counter][2] = 0;
        }
    else if (input == "m")
        {
        events[counter][TYPE_2] = YES;
        input = "";
        std::cout << "Receiver (#)" << std::endl;
        std::cin >> input;
        events[counter][2] = stringToInt(input);
        }
    else if (input == "o")
        {
        events[counter][TYPE] = NO;
        events[counter][TYPE_3] = YES;
        }
    else
        events[counter][2] = stringToInt(input);
    
    if (events[counter][DURING_D_ENTRY] == YES || events[counter][DURING_PC] == YES)
        {
            passWindow();
            events[counter][3] = passCoordinates[0];
            events[counter][4] = passCoordinates[1];
            events[counter][5] = passCoordinates[2];
            events[counter][6] = passCoordinates[3];
            bool passNotOutOfD = checkInsideD(passCoordinates[2], passCoordinates[3]);
            clearPassCoordinates();
            if (passNotOutOfD == false) //if the pass goes out of the d
                {
                    events[counter][OUT_OF_D] = YES;
                    activePC = false;
                    activeD = false;
                }
        }
    else
        {
            passWindow();
            events[counter][3] = passCoordinates[0];
            events[counter][4] = passCoordinates[1];
            events[counter][5] = passCoordinates[2];
            events[counter][6] = passCoordinates[3];
            bool passIntoD = checkInsideD(passCoordinates[2], passCoordinates[3]);
            clearPassCoordinates();
            
            if (passIntoD == true && events[counter][TYPE] == YES)
            {
                counter++;
                //transfer data about method of D entry to the D entry row itself
                events[counter][1] = events[counter - 1][1];
                events[counter][2] = events[counter - 1][2];
                events[counter][3] = events[counter - 1][3];
                events[counter][4] = events[counter - 1][4];
                events[counter][5] = events[counter - 1][5];
                events[counter][6] = events[counter - 1][6];
                
                events[counter][TYPE] = 2;
                events[counter][TYPE_2] = events[counter - 1][TYPE];
                
                dEntry();
            }
        }
    if (passType == "o")
        {
            counter++;
            events[counter][0] = OPPO_BALL_OUT_OF_PLAY;
        }
    else if (passType == "m")
        {
            counter++;
            events[counter][0] = DEFENSIVE_ERROR;
            events[counter][1] = events[counter - 1][2];
            events[counter][TYPE] = 2;
            events[counter][3] = events[counter - 1][5];
            events[counter][4] = events[counter - 1][6];
        }
    counter++;
}
void mainMenu()
{
    std::cout << "1. Pass" << std::endl;
    std::cout << "2. Ball Won" << std::endl;
    std::cout << "3. Ball Lost" << std::endl;
    std::cout << "4. Free Won" << std::endl;
    std::cout << "5. Penalty Conceded" << std::endl;
    std::cout << "6. Elimination" << std::endl;
    std::cout << "7. Defensive Error" << std::endl;
    std::cout << "8. Ball out of Play" << std::endl;
    std::cout << "9. D Entry" << std::endl;
    std::cout << "10. Penalty Corner" << std::endl;
    std::cout << "11. Opposition D Entry" << std::endl;
    std::cout << "12. Opposition Shot" << std::endl;
    std::cout << "13. Undo" << std::endl;
    std::cout << "14. End" << std::endl;
}
void dEntryMenu()
{
    std::cout << "1. Pass" << std::endl;
    std::cout << "2. Shot" << std::endl;
    std::cout << "3. Ball Lost" << std::endl;
    std::cout << "4. Ball Won" << std::endl;
    std::cout << "5. Defensive Error" << std::endl;
    std::cout << "6. Penalty Corner" << std::endl;
    std::cout << "7. Penalty Stroke" << std::endl;
    std::cout << "8. End of D Entry" << std::endl;
}
void penaltyCornerMenu()
{
    std::cout << "1. Pass" << std::endl;
    std::cout << "2. Shot" << std::endl;
    std::cout << "3. Ball Lost" << std::endl;
    std::cout << "4. Ball Won" << std::endl;
    std::cout << "5. Defensive Error" << std::endl;
    std::cout << "6. Penalty Corner" << std::endl;
    std::cout << "7. Penalty Stroke" << std::endl;
    std::cout << "8. End of PC" << std::endl;
}
void dEntry()
{
    dEntryCounter++;
    dEntryPoint = counter;
    events[counter][0] = D_ENTRY;
    counter++;
    std::string input = "";
    
    activeD = true;
    
    while (activeD == true)
    {
        input = "";
        dEntryMenu();
        std::cin >> input;
        if (input == "1")
        {
            events[counter][DURING_D_ENTRY] = YES;
            pass();
        }
        else if (input == "2")
        {
            shot();
        }
        else if (input == "3")
        {
            events[counter][DURING_D_ENTRY] = YES;
            ballLost();
        }
        else if (input == "4")
        {
            events[counter][DURING_D_ENTRY] = YES;
            ballWon();
        }
        else if (input == "5")
        {
            events[counter][DURING_D_ENTRY] = YES;
            defensiveError();
        }
        else if (input == "6")
        {
            //how and where was it won
            penaltyCorner();
        }
        else if (input == "7")
        {
            activeD = false;
            penaltyStroke();
        }
        else if (input == "8")
        {
            endOfDEntry(dEntryPoint);
            activeD = false;
        }
        else
            error();
    }
    if (ballOutOfPlayMarker == ATT_BALL_OUT_OF_PLAY)
        {
        ballOutOfPlayMarker = 0;
        ballOutOfPlay();
        }
    
}
void endOfDEntry(int entryPoint)
{
    if (events[counter - 1][0] == PASS && events[counter - 1][OUT_OF_D] == YES)
        {
        events[counter][TYPE] = 7;
        }
    else if (events[counter - 1][0] == SHOT && events[counter - 1][GOAL] == YES)
    {
        events[counter][TYPE] = 8;
    }
    else if (events[counter - 2][0] == SHOT && events[counter - 2][GOAL] == YES)
    {
        events[counter][TYPE] = 8;
    }
    else if (events[counter - 1][0] == PENALTY_STROKE && events[counter - 1][GOAL] == YES)
    {
        events[counter][TYPE] = 9;
    }
    else
        {
            std::string input = "";
            input = "";
            std::cout << "1. Ball out of play" << std::endl;
            std::cout << "2. Free conceded" << std::endl;
            std::cout << "3. Opposition pass out of D" << std::endl;
            std::cout << "4. Opposition carry out of D" << std::endl;
            std::cout << "5. Carry out of D" << std::endl;
            std::cout << "6. Other" << std::endl;
            std::cin >> input;
            events[counter][TYPE] = stringToInt(input);
            if (input == "1")
                ballOutOfPlayMarker = ATT_BALL_OUT_OF_PLAY;
        }
    events[counter][0] = END_OF_D_ENTRY;
    for (int i = entryPoint; i <= counter; i++)
    {
        events[i][D_ENTRY_ID] = dEntryCounter;
    }
    counter++;
    activeD = false;
    }
void endOfPC(int PCentryPoint)
{
    events[counter][0] = END_OF_PC;
    for (int i = PCentryPoint; i <= counter; i++)
    {
        events[i][PC_ID] = penaltyCornerCounter;
    }
    counter++;
    activePC = false;
}
void dEntryCarry()
{
    std::string input = "";
    std::cout << "Player (#)" << std::endl;
    std::cin >> input;
    events[counter][1] = stringToInt(input);
    
    //point of entry
    carryWindow();
    events[counter][3] = locationCoordinates[0];
    events[counter][4] = locationCoordinates[1];
    
    events[counter][TYPE] = 1;
    
    dEntry();
}
void ballWon()
{
    std::string input = "";
    events[counter][0] = BALL_WON;
    std::cout << "Player (#)" << std::endl;
    std::cin >> input;
    events[counter][1] = stringToInt(input);
winTypeQ:
    input = "";
    std::cout << "1. Tackle" << std::endl;
    std::cout << "2. Interception" << std::endl;
    std::cin >> input;
    if (input != "1" && input != "2")
    {
        error();
        goto winTypeQ;
    }
    events[counter][TYPE] = stringToInt(input);
    if (events[counter][DURING_D_ENTRY] == YES)
        dWindow();
    else
        locationWindow();
    events[counter][3] = locationCoordinates[0];
    events[counter][4] = locationCoordinates[1];
    bool insideD = checkInsideD(locationCoordinates[0], locationCoordinates[1]);
    clearLocationCoordinates();
    if (insideD == true && events[counter][DURING_D_ENTRY] == 0)
    {
        counter++;
        events[counter][1] = events[counter - 1][1]; //copy across winner
        events[counter][TYPE] = 3;
        events[counter][TYPE_2] = events[counter - 1][TYPE];
        events[counter][3] = events[counter - 1][3];
        events[counter][4] = events[counter - 1][4];
        dEntry();
    }
    counter++;
}
void ballLost()
{
    std::string input = "";
    events[counter][0] = BALL_LOST;
    std::cout << "Player (#)" << std::endl;
    std::cin >> input;
    events[counter][1] = stringToInt(input);
    if (events[counter][DURING_D_ENTRY] == YES)
        dWindow();
    else
        locationWindow();
    events[counter][3] = locationCoordinates[0];
    events[counter][4] = locationCoordinates[1];
    clearLocationCoordinates();
    counter++;
}
void shot()
{
    events[counter][0] = SHOT;
    std::string input = "";
    std::cout << "Shooter #" << std::endl;
    std::cin >> input;
    events[counter][1] = stringToInt(input);
    dWindow();
    events[counter][3] = locationCoordinates[0];
    events[counter][4] = locationCoordinates[1];
    clearLocationCoordinates();
    input = "";
    std::cout << "Shot Type:" << std::endl;
    std::cout << "1. Hit" << std::endl;
    std::cout << "2. Flick" << std::endl;
    std::cout << "3. Push" << std::endl;
    std::cout << "4. Slap" << std::endl;
    std::cin >> input;
    events[counter][12] = stringToInt(input);
    input = "";
    std::cout << "Blocked?" << std::endl;
    std::cout << "1. Yes" << std::endl;
    std::cout << "2. No" << std::endl;
    std::cin >> input;
    if (input == "1")
        {
            events[counter][TYPE_2] = YES;
            input = "";
            std::cout << "On target?" << std::endl;
            std::cout << "1. Yes" << std::endl;
            std::cout << "2. No" << std::endl;
            std::cin >> input;
            if (input == "1")
                events[counter][ON_TARGET] = YES;
            else
                events[counter][ON_TARGET] = NO;
        }
    else if (input == "2")
    {
        events[counter][TYPE_2] = NO;
        input = "";
        std::cout << "Deflection?" << std::endl;
        std::cout << "1. Yes" << std::endl;
        std::cout << "2. No" << std::endl;
        std::cin >> input;
        
        if (input == "1")
            {
            events[counter][TYPE_3] = YES;
            input = "";
            std::cout << "Deflector #" << std::endl;
            std::cin >> input;
            events[counter][2] = stringToInt(input);
            dWindow();
            events[counter][5] = locationCoordinates[0];
            events[counter][6] = locationCoordinates[1];
            clearLocationCoordinates();
            }
        else
            events[counter][TYPE_3] = NO;
        shotWindow();
        events[counter][7] = shotCoordinates[0];
        events[counter][8] = shotCoordinates[1];
        bool onTarget = checkOnTarget(shotCoordinates[0], shotCoordinates[1]);
        clearShotCoordinates();
        if (onTarget == true)
            {
                events[counter][ON_TARGET] = YES;
                input = "";
                std::cout << "Goal?" << std::endl;
                std::cout << "1. Yes" << std::endl;
                std::cout << "2. No" << std::endl;
                std::cin >> input;
                if (input == "2")
                    {
                    events[counter][GOAL] = NO;
                    }
                else if (input == "1")
                    {
                    events[counter][GOAL] = YES;
                    counter++;
                    if (events[counter - 1][DURING_PC] == YES)
                        endOfPC(PCEntryPoint);
                    endOfDEntry(dEntryPoint);
                    activePC = false;
                    activeD = false;
                    }
                else
                    events[counter][GOAL] = -1;
            }
        else
            events[counter][ON_TARGET] = NO;
    }
    counter++;
}
void penaltyCorner()
{
    penaltyCornerCounter++;
    PCEntryPoint = counter;
    events[PCEntryPoint][0] = PENALTY_CORNER;
    bool openPlay = false;
    if (events[PCEntryPoint][TYPE] == OPEN_PLAY)
        openPlay = true;
    counter++;
    std::string input = "";
    
    std::cout << "Winner (#)" << std::endl;
    std::cin >> input;
    events[PCEntryPoint][2] = stringToInt(input);
    
    dWindow();
    events[PCEntryPoint][3] = locationCoordinates[0];
    events[PCEntryPoint][4] = locationCoordinates[1];
    clearLocationCoordinates();
    
    input = "";
    std::cout << "1. Injection on target" << std::endl;
    std::cout << "2. Missed injection" << std::endl;
    std::cin >> input;
    events[PCEntryPoint][INJECTION_ON_TARGET] = stringToInt(input);
    
    input = "";
    std::cout << "1. Trapped" << std::endl;
    std::cout << "2. Mistrap" << std::endl;
    std::cin >> input;
    events[PCEntryPoint][TRAPPED] = stringToInt(input);
    
    activePC = true;
    
    while (activePC == true)
    {
        input = "";
        penaltyCornerMenu();
        std::cin >> input;
        if (input == "1")
        {
            events[counter][DURING_PC] = YES;
            pass();
        }
        else if (input == "2")
        {
            events[counter][DURING_PC] = YES;
            shot();
        }
        else if (input == "3")
        {
            events[counter][DURING_PC] = YES;
            ballLost();
        }
        else if (input == "4")
        {
            events[counter][DURING_PC] = YES;
            ballWon();
        }
        else if (input == "5")
        {
            events[counter][DURING_PC] = YES;
            defensiveError();
        }
        else if (input == "6")
        {
            endOfPC(PCEntryPoint);
            if (openPlay == true)
                events[counter][TYPE] = OPEN_PLAY;
            penaltyCorner();
        }
        else if (input == "7")
        {
            endOfPC(PCEntryPoint);
            activePC = false;
            penaltyStroke();
        }
        else if (input == "8")
        {
            endOfPC(PCEntryPoint);
            if (openPlay == false)
                endOfDEntry(dEntryPoint);
            activeD = false;
            activePC = false;
        }
        else
            error();
    }
    counter++;
}
void penaltyStroke()
{
    std::string input = "";
    events[counter][0] = PENALTY_STROKE;
    std::cout << "Winner #" << std::endl;
    std::cin >> input;
    events[counter][2] = stringToInt(input);
    input = "";
    dWindow();
    events[counter][3] = locationCoordinates[0];
    events[counter][4] = locationCoordinates[1];
    clearLocationCoordinates();
    std::cout << "Taker #" << std::endl;
    std::cin >> input;
    events[counter][1] = stringToInt(input);
    shotWindow();
    events[counter][7] = shotCoordinates[0];
    events[counter][8] = shotCoordinates[1];
    bool onTarget = checkOnTarget(shotCoordinates[0], shotCoordinates[1]);
    clearShotCoordinates();
    if (onTarget == true)
    {
        events[counter][ON_TARGET] = 1;
    goalQ:
        input = "";
        std::cout << "1. Goal" << std::endl;
        std::cout << "2. Saved" << std::endl;
        std::cin >> input;
        if (input != "1" && input != "2")
        {
            error();
            goto goalQ;
        }
        events[counter][GOAL] = stringToInt(input);
    }
    counter++;
    endOfDEntry(dEntryPoint);
    activeD = false;
}
void freeWon()
{
    std::string input = "";
    events[counter][0] = FREE_WON;
    std::cout << "Player (#)" << std::endl;
    std::cin >> input;
    events[counter][1] = stringToInt(input);
    locationWindow();
    events[counter][3] = locationCoordinates[0];
    events[counter][4] = locationCoordinates[1];
    clearLocationCoordinates();
    counter++;
}
void defensiveError()
{
    std::string input = "";
    events[counter][0] = DEFENSIVE_ERROR;
    std::cout << "Player (#)" << std::endl;
    std::cin >> input;
    events[counter][1] = stringToInt(input);
    input = "";
    std::cout << "1. Missed tackle" << std::endl;
    std::cout << "2. Mistrap" << std::endl;
    std::cin >> input;
    events[counter][TYPE] = stringToInt(input);
    if (events[counter][DURING_D_ENTRY] == YES)
        dWindow();
    else
        locationWindow();
    events[counter][3] = locationCoordinates[0];
    events[counter][4] = locationCoordinates[1];
    clearLocationCoordinates();
    counter++;
}
void elimination()
{
    std::string input = "";
    events[counter][0] = ELIMINATION;
    std::cout << "Player (#)" << std::endl;
    std::cin >> input;
    events[counter][1] = stringToInt(input);
    input = "";
    std::cout << "1. Left" << std::endl;
    std::cout << "2. Right" << std::endl;
    std::cin >> input;
    events[counter][TYPE] = stringToInt(input);
    locationWindow();
    events[counter][3] = locationCoordinates[0];
    events[counter][4] = locationCoordinates[1];
    clearLocationCoordinates();
    counter++;
}
void freeLost()
{
    std::string input = "";
    events[counter][0] = PENALTY_CONCEDED;
    std::cout << "Player (#)" << std::endl;
    std::cin >> input;
    events[counter][1] = stringToInt(input);
freeType:
    input = "";
    std::cout << "1. Free Hit" << std::endl;
    std::cout << "2. Penalty Corner" << std::endl;
    std::cout << "3. Penalty Stroke" << std::endl;
    std::cin >> input;
    if (input != "1" && input != "2" && input != "3")
    {
        error();
        goto freeType;
    }
    events[counter][TYPE] = stringToInt(input);
    
        locationWindow();
        events[counter][3] = locationCoordinates[0];
        events[counter][4] = locationCoordinates[1];
        clearLocationCoordinates();
    counter++;
}
void oppositionDEntry()
{
    events[counter][0] = OPPO_D_ENTRY;
    //type
    //location
    counter++;
}
void oppositionShot()
{
    std::string input = "";
    events[counter][0] = OPPO_SHOT;
    std::cout << "1. Saved" << std::endl;
    std::cout << "2. Miss" << std::endl;
    std::cout << "3. Goal" << std::endl;
    std::cin >> input;
    if (input == "1")
        events[counter][TYPE] = 1;
    else if (input == "2")
        events[counter][TYPE] = 2;
    else if (input == "3")
        goalConceded();
    counter++;
}
void goalConceded()
{
    counter++;
    events[counter][0] = OPPO_GOAL;
goalType:
    std::string input = "";
    std::cout << "1. Field Goal" << std::endl;
    std::cout << "2. Penalty Corner" << std::endl;
    std::cout << "3. Penalty Stroke" << std::endl;
    std::cin >> input;
    if (input != "1" && input != "2" && input != "3")
    {
        error();
        goto goalType;
    }
    events[counter][TYPE] = stringToInt(input);
}
void ballOutOfPlay()
{
    std::string input = "";
type:
    std::cout << "Possession?" << std::endl;
    std::cout << "1. Us" << std::endl;
    std::cout << "2. Opposition" << std::endl;
    std::cin >> input;
    if (input != "1" && input != "2")
    {
        error();
        goto type;
    }
    if (input == "1")
        events[counter][0] = ATT_BALL_OUT_OF_PLAY;
    else
        events[counter][0] = OPPO_BALL_OUT_OF_PLAY;

    counter++;
}

int main()
{
	drawImages();

	if (sf::VideoMode::getDesktopMode().height <= 1000)
		screenMultiplier = 2;
    
    bool done = false;
    
    while (done == false)
    {
        std::string input = "";
        mainMenu();
        std::cin >> input;
        if (input == "1")
            pass();
        else if (input == "2")
            ballWon();
        else if (input == "3")
            ballLost();
        else if (input == "4")
            freeWon();
        else if (input == "5")
            freeLost();
        else if (input == "6")
            elimination();
        else if (input == "7")
            defensiveError();
        else if (input == "8")
            ballOutOfPlay();
        else if (input == "9")
            dEntryCarry();
        else if (input == "10")
            {
            events[counter][TYPE] = OPEN_PLAY;
            penaltyCorner();
            }
        else if  (input == "11")
            oppositionDEntry();
        else if  (input == "12")
            oppositionShot();
        else if (input == "13")
        {
            counter--;
            for (int i = 0; i < eventsColumns; i++)
            {
                events[counter][i] = 0;
            }
        }
        else if (input == "14")
            done = true;
        else if (input == "p")
            printArray();
        else
            error();
    }
    
    outputData();
    return 0;
}
