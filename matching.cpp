#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cctype>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <unistd.h>
#include <fstream>
#include <algorithm>
using namespace std;

//functions
//is in functions for future if statements
//strings
bool isIn(vector <string> list, string word){
    bool in=false;
    for (int i=0; i<list.size(); i++){
        if (list[i]==word){
            in=true;
        }
    }
    return in;
}
//ints
bool isIn(vector <int> list, int num){
    bool in=false;
    for (int i=0; i<list.size(); i++){
        if (list[i]==num){
            in=true;
        }
    }
    return in;
}

//checking for people entering letters for no reason
bool includesNotNumbers(string word){
    int counter=0;
    char numbers[10]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    //going through the letters of the the word
    for (int i=0; i<word.length(); i++){
        for (int j=0; j<10; j++){
            if (word[i]==numbers[j]){
                counter++;
            }
        }
    }
    //checking
    if (counter!=word.length()){
        return true;
    } else {
        return false;
    }
}

//instructions
void instructions(){
    cout<<"\nThis is a normal game of memory where you choose which the number of cards, and whether it is a 1 player, 2 player, or against the computer game.";
    cout<<"\nYour goal is to match the emojis to themselves (eg. 🥧 and 🥧). ";
    cout<<"\nYou can choose which cards to flip over but 2 seconds after the second one is flipped, unless they are a match, they both will be unflipped.\n\n";
    usleep(5000000);
}

//menu
int menu(int &numCards, string playAgain){
    string numPlayers, answer, cardAmount;

    //having it only happen the first time
    if (playAgain=="no"){
        cout<<"Hello and welcome to the matching game! \n";

        //getting instructions
        do{
            cout<<"Enter 1 to get instructions, enter 2 to continue: ";
            cin>>answer;
            //checking for errors
            if (includesNotNumbers(answer)){
                answer="200";
            }
        } while (stoi(answer)!=1&&stoi(answer)!=2);
        if (stoi(answer)==1){
            instructions();
        }
    }

    //showing options
    cout<<"\nPlease enter the number for the one you want:";
    cout<<"\n1. One player game";
    cout<<"\n2. Two player game";
    cout<<"\n3. One player game against the computer";
    cout<<"\nWhich do you want? ";
    cin>>numPlayers;

    //more error checking
    if (includesNotNumbers(numPlayers)){
        numPlayers="200";
    }
    //stopping from entering stupid answers
    while(stoi(numPlayers)>3||stoi(numPlayers)<1){
        cout<<"\nThat isn't an option. Please try again: ";
        cin>>numPlayers;
        if (includesNotNumbers(numPlayers)){
            numPlayers="200";
        }
    }

    //number of cards
    cout<<"\n\nHow many cards do you want, 6, 12, 24, 36, or 48? ";
    cin>>cardAmount;
    //error checking
    if (includesNotNumbers(cardAmount)){
        cardAmount="200";
    }
    while (stoi(cardAmount)!=6&&stoi(cardAmount)!=12&&stoi(cardAmount)!=24&&stoi(cardAmount)!=36&&stoi(cardAmount)!=48){
        cout<<"\nThat isn't an option, sorry. Please reenter it: ";
        cin>>cardAmount;
        if (includesNotNumbers(cardAmount)){
            cardAmount="200";
        }
    }
    numCards=stoi(cardAmount);
    return stoi(numPlayers);
}

//randomizing order
vector <string> randomOrder(vector <string> cards){
    vector <string> randomCards;
    int num;
    for (int i=0; i<cards.size(); i++){
        do{
            num=rand()%cards.size();
        } while (cards[num]==" ");
        randomCards.push_back(cards[num]);
        cards[num]=" ";
    }
    return randomCards;
}

//setting the cards
vector <string> settingCards(int numCards){
    vector <string> cards, totalCards={"🍮","🍮","🍨","🍨","🍪","🍪","🍰","🍰","🧁","🧁","🎂","🎂","🍩","🍩","🥧","🥧","🧇","🧇","🥞","🥞","🍦","🍦","🍭","🍭","🍬","🍬","🍫","🍫","🍓","🍓","🍋","🍋","🥭","🥭","🍍","🍍","🍐","🍐","🍇","🍇","🍑","🍑","🍒","🍒","🫐","🫐","🍎","🍎"};
    
    //actually updating the cards
    for (int i=0; i<numCards; i++){
        cards.push_back(totalCards[i]);
    }
    cards=randomOrder(cards);
    return cards;
}

//showing it
void board(vector <string> cards, int position1, int position2, vector <int> &positionsMatched){
    cout<<"\n\033[0;95m|-----------------------------|\n| ";
    //going through all cards
    for (int i=0; i<cards.size(); i++){
        //seeeing if it should be shown
        if (isIn(positionsMatched, i)){
            printf("\033[0;92m%2s",cards[i].c_str());
        } else if (i==position1||i==position2){
            printf("\033[0;97m%2s",cards[i].c_str());
        } else {
            printf("%2i", i+1);
        }
        cout<<"\033[0;95m | ";

        //getting it to go down a line sometimes
        if (i%6==5){
            cout<<"\n|-----------------------------|\n| \033[0;95m";
        }
    }
    cout<<"\r\r  \n";
}

//checking if it is a match
bool isMatch(int numCards, vector <string> cards, int &playerPoints, vector <int> &positionsMatched, int position1, int position2){
    if (cards[position1]==cards[position2]){
        cout<<"\nIt's a match!";
        playerPoints++;
        positionsMatched.push_back(position1);
        positionsMatched.push_back(position2);
        return true;
    } else {
        cout<<"\nNot a match :(";
        return false;
    }
}

//remembering cards
void remember(vector <string> &cardsRemembered, vector <string> cards, int position){
    int chance=rand()%2;

    //actually remembering
    if (chance==0&&(!isIn(cardsRemembered, to_string(position)))){
        cardsRemembered.push_back(to_string(position));
        cardsRemembered.push_back(cards[position]);
    }
}

//choose positions
void positions(int numCards, vector <string> cards, int playerNum, vector <int> &positionsMatched, int &playerPoints, vector <string> &cardsRemembered){
    //variables
    string position1="-1", position2="-1";
    board(cards, stoi(position1), stoi(position2), positionsMatched);

    //first position
    cout<<"\nPlayer "<<playerNum;
    cout<<". Which position do you want to reveal? ";
    cin>>position1;
    //checking
    if (includesNotNumbers(position1)){
        position1="200";
    }
    while (stoi(position1)>numCards||stoi(position1)<1||isIn(positionsMatched, stoi(position1)-1)){
        cout<<"That isn't an option, please reenter: ";
        cin>>position1;
        if (includesNotNumbers(position1)){
            position1="200";
        }
    }

    //showing board
    system("clear");
    board(cards, stoi(position1)-1, stoi(position2)-1, positionsMatched);

    //second postions
    cout<<"\nWhich other position do you want to reveal? ";
    cin>>position2;
    //checking
    if (includesNotNumbers(position2)){
        position2="200";
    }
    while (stoi(position2)>numCards||stoi(position2)<1||isIn(positionsMatched, stoi(position2)-1)||position2==position1){
        cout<<"That isn't an option, please reenter: ";
        cin>>position2;
        if (includesNotNumbers(position2)){
            position2="200";
        }
    }
    //computer remembering
    remember(cardsRemembered, cards, stoi(position1)-1);
    remember(cardsRemembered, cards, stoi(position2)-1);

    //showing
    system("clear");
    isMatch(numCards, cards, playerPoints, positionsMatched, stoi(position1)-1, stoi(position2)-1);
    board(cards, stoi(position1)-1, stoi(position2)-1, positionsMatched);
    cout<<"\n";
    usleep(2000000);
    system("clear");
}

//computer being somewhat smart
void computerSmart(int &position1, int &position2, vector <string> &cardsRemembered, vector <string> cards, vector <int> &positionsMatched){
    position1=-10;
    position2=-10;
    for (int i=1; i<cardsRemembered.size(); i+=2){
        for (int j=1; j<cardsRemembered.size(); j+=2){
            if (j!=i&&cardsRemembered[i]==cardsRemembered[j]){
                position1=stoi(cardsRemembered[i-1]);
                if (!isIn(positionsMatched, position1)){
                    position2=stoi(cardsRemembered[j-1]);
                    i=100;
                    j=100;
                }
            }
        }
    }
    //if can't be smart
    if (position2==-10){
        do {
            position1=rand()%cards.size();
        } while (isIn(positionsMatched, position1));
        //randomly
        do {
            position2=rand()%cards.size();
        } while (isIn(positionsMatched, position2)||position2==position1);

        //check if position 1 helps
        for (int j=1; j<cardsRemembered.size(); j+=2){
            if (j!=position1&&to_string(position1)==cardsRemembered[j]){
                position2=stoi(cardsRemembered[j-1]);
            }
        }
        
    }
    remember(cardsRemembered, cards, position1);
    remember(cardsRemembered, cards, position2);
}

//computer choosing
void computerPositions(int numCards, vector <string> cards, int &computerPoints, vector <int> &positionsMatched, vector <string> &cardsRemembered){
    int position1, position2;
    //getting placements
    computerSmart(position1, position2, cardsRemembered, cards, positionsMatched);

    //output results
    cout<<"The computer chose positions "<<position1+1<<"  "<<position2+1;
    isMatch(numCards, cards, computerPoints, positionsMatched, position1, position2);
    board(cards, position1, position2, positionsMatched);
    cout<<"\n";
    usleep(2000000);
    system("clear");
}

//fastest time stuff
void updatingTimes(vector <int> fastestTimes){

}

//getting the possible locations
vector <string> getFromFile(string fileName){
    vector <string> position;       
    ifstream file(fileName);    
    string word;                   
    
    // Read each word from file and add to set
    while (getline(file, word)) {
        position.push_back(word);             
    }
    return position;  
}

//push to file
void pushToFile(string word1, string word2, string word3, string fileName){
    ofstream file;
    file.open(fileName, std::ios::app);
    file<<endl<<word1<<"m"<<word2<<"s "<<word3;
    file.close();
}

//getting names and times
void times(vector <string> &fastestTimes, vector <string> &fastestPeople, vector <string> leaderBoard, string fileName){
    leaderBoard=getFromFile(fileName);
    //seperating stuff
    for (int i=0; i<leaderBoard.size(); i++){
        fastestTimes.push_back(leaderBoard[i].substr(0, leaderBoard[i].find(" ")));
        fastestPeople.push_back(leaderBoard[i].substr(leaderBoard[i].find(" "), leaderBoard[i].length()-1));
    }
}

//updating the leader board
void updateBoard(int minutes, int seconds, vector <string> &leaderBoard, string name, string fileName){
    pushToFile(to_string(minutes), to_string(seconds), name, fileName);
    leaderBoard=getFromFile(fileName);
    //clearing any empty spaces
    for (int i=0; i<leaderBoard.size(); i++){
        if (leaderBoard[i]==""){
            leaderBoard.erase(leaderBoard.begin()+i);
        }
    }
    //sorting
    sort(leaderBoard.begin(),leaderBoard.end());
    fileName.clear();
}

//main
int main(){
    srand(time(0));
    string playAgain="no";
    do{
        //variables
        string name, fileName;
        int numCards, numPlayers, player1Points=0, player2Points=0, computerPoints=0, p1wins=0, p2wins=0, compWins=0;
        vector <string> cards, shownCards, cardsRemembered, fastestTimes, fastestPeople, leaderBoard;
        vector <int> positionsMatched;

        //set up
        numPlayers=menu(numCards, playAgain);
        cards=settingCards(numCards);

        //dealing with best
        fileName="fastestTimes"+to_string(numCards)+".txt";
        times(fastestTimes, fastestPeople, leaderBoard, fileName);

        //timer starting
        auto t1=std::chrono::high_resolution_clock::now();

        //game
        while (positionsMatched.size()!=numCards){
            positions(numCards, cards, 1, positionsMatched, player1Points, cardsRemembered);
            //player 2
            if (positionsMatched.size()!=numCards&&numPlayers==2){
                positions(numCards, cards, 2, positionsMatched, player2Points, cardsRemembered);
            } else if (numPlayers==3&&positionsMatched.size()!=numCards){
                computerPositions(numCards, cards, computerPoints, positionsMatched, cardsRemembered);
            }
        }

        //time high score
        auto t2=std::chrono::high_resolution_clock::now();
        auto elapsed_time=std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count();
        int secondsPassed=elapsed_time/1000000;
        int minutesPassed=secondsPassed/60;
        secondsPassed=secondsPassed-minutesPassed*60;

        //showing results
        if (player1Points>player2Points){
            cout<<"\nPlayer 1 won!";
        } else if (numPlayers==2&&player1Points!=player2Points){
            cout<<"\nPlayer 2 won!";
        } else if (player1Points!=player2Points){
            cout<<"It's a tie!";
        } else {
            cout<<"\nThe computer won. With "<<computerPoints<<" matches.";
            cout<<"\nYou had "<<player1Points<<" matches.";
        }

        //showing fastest times
        if (numPlayers==1){
            cout<<"\nIt took you "<<minutesPassed<<"m"<<secondsPassed<<"s to finish the game. Good job!";
            cout<<"\nWhat do you want your name saved as? ";
            cin>>ws;
            getline(cin, name);
            updateBoard(minutesPassed, secondsPassed, leaderBoard, name, fileName);

            //showing leaderboard
            cout<<"\nCurrently the leaderboard for your number of cards is: ";
            cout<<"\n1. "<<leaderBoard[0];
            cout<<"\n2. "<<leaderBoard[1];
            cout<<"\n3. "<<leaderBoard[2];
        }
        cout<<"\n\nDo you want to play again? (y/n) \n";
        cin>>playAgain;
    } while (tolower(playAgain[0])=='y');
    cout<<"\n\n\n";
    return 0;
}

