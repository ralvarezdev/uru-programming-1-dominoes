#include<iostream>
#include<string>
#include<vector>
//#include<windows.h>
#include"input.h"
#include"cards.h"

using std::cout; using std::cin;
using std::string;
using std::vector;

void clearAndTitle(string title){
    system("cls");
    cout<<title;
}

void Board::boardSize(int x, int y){
    if(x%2==0){
        x++;
    }
    
    if(y%2==0){
        y++;
    }

    xSize = x;
    ySize = y;

    xCenter = (xSize/2); // We're counting position 0
    yCenter = (ySize/2);

    boardCellsOccupied.resize(ySize);
    boardCellsContent.resize(ySize);

    for(y=0;y<ySize;y++){
        boardCellsOccupied[y].resize(xSize);
        boardCellsContent[y].resize(xSize);
    }
}

void Board::printTitleBar(string title, int numberPlayers){
    clearAndTitle(title);

    if(numberPlayers==3){
        cout<<"\n"<<players[0]<<" ("<<points[0]<<") vs "<<players[1]<<" ("<<points[1]<<") vs "<<players[2]<<" ("<<points[2]<<")\n";
    }else if(numberPlayers==4){
        cout<<"\nTeam "<<players[0]<<" ("<<points[0]<<") vs Team "<<players[1]<<" ("<<points[1]<<")\n";
    }
}

void Board::printBoard(){
    int x, y, cellContent;

    cout<<"\n";

    for(y=0;y<ySize;y++){
        for(x=0;x<xSize;x++){
            if(boardCellsOccupied[y][x]==false){
                //system("Color 07");
                cout<<"   ";
            }else{
                //system("Color 70");
                cellContent = boardCellsContent[y][x];

                // 0:0, 1:1, 2:2, 3:3, 4:4, 5:5, 6:6, 7:|, 8:-
                if(cellContent<7){
                    cout<<"["<<cellContent<<"]";
                }else if(cellContent==7){
                    cout<<" | ";
                }else if(cellContent==8){
                    cout<<" - ";
                }
            }
        }
        cout<<"\n";
    }
}

bool Dominoes::convertIntToCard(bool newGame, int numberPlayers, int cardNumber){
    if(cardNumber<7){
        if(numberPlayers==3&&cardNumber==0){
            sideA = 6;
            sideB = 6;

            if(newGame==true){
                return true;
            }

        }else{
            sideA = 0;
            sideB = cardNumber;
        }

    }else if(cardNumber<13){
        sideA = 1;
        sideB = cardNumber - 6;

    }else if(cardNumber<18){
        sideA = 2;
        sideB = cardNumber - 11;

    }else if(cardNumber<22){
        sideA = 3;
        sideB = cardNumber - 15;

    }else if(cardNumber<25){
        sideA = 4;
        sideB = cardNumber - 18;

    }else{
        switch(cardNumber){
            case 25:
                sideA = 5;
                sideB = 5;
                break;
            case 26:
                sideA = 5;
                sideB = 6;
                break;
            case 27:
                sideA = 6;
                sideB = 6;
                
                if(newGame==true){
                    return true;
                }
        }
    }
    return false;
}

void Board::changeCellContent(bool equalTiles, bool lieDown, int row, int col, int sideFree, int free, int sideConnected, int connected, int separator){
    int x;
    
    if(equalTiles==true){
        if(lieDown==false){
            boardCellsContent[free][col] = sideFree;
            boardCellsContent[free+1][col] = 8;
            boardCellsContent[free+2][col] = sideFree;

            for(x=0;x<3;x++){
                boardCellsOccupied[free+x][col] = true;
            }
        }else{
            boardCellsContent[row][free] = sideFree;
            boardCellsContent[row][free+1] = 7;
            boardCellsContent[row][free+2] = sideFree;

            for(x=0;x<3;x++){
                boardCellsOccupied[row][free+x] = true;
            }
        }

    }else{
        if(lieDown==true){
            boardCellsContent[row][free] = sideFree;
            boardCellsOccupied[row][free] = true;
            boardCellsContent[row][connected] = sideConnected;
            boardCellsOccupied[row][connected] = true;

            if(separator!=-1){
                boardCellsContent[row][separator] = 7;
                boardCellsOccupied[row][separator] = true;
            }
        }else{
            boardCellsContent[free][col] = sideFree;
            boardCellsOccupied[free][col] = true;
            boardCellsContent[connected][col] = sideConnected;
            boardCellsOccupied[connected][col] = true;
            
            if(separator!=-1){
                boardCellsContent[separator][col] = 8;
                boardCellsOccupied[separator][col] = true;
            }
        }
    }   
}

void Board::addCard(Dominoes domino){
    bool answer, equalTiles, lieDown, separator;
    char cardPosition;
    int x, y, z, row, col, cardPlayed, sideConnected, sideFree, freePosition, connectedPosition;  
    int separatorPosition = -1;

    if(cardsPlayed!=0){
        if((cardPlayedUp==cardPlayedDown)||(domino.sideA==cardPlayedUp&&domino.sideB==cardPlayedDown)||(domino.sideB==cardPlayedUp&&domino.sideA==cardPlayedDown)){
            answer = booleanQuestion("Where do you want to put the card 'Yes' for Up/Left, 'No' for Down/Right?");

            if(answer==true){
                cardPosition = 'u';
            }else{
                cardPosition = 'd';
            }

        }else if(domino.sideA==cardPlayedUp||domino.sideB==cardPlayedUp){
            cardPosition = 'u';

        }else if(domino.sideA==cardPlayedDown||domino.sideB==cardPlayedDown){
            cardPosition = 'd';
        }
    }else{
        upSegment = 1;
        downSegment = 1;
        col = xCenter;
        colLeft = col;
        colRight = col;
        row = yCenter;
        rowUp = row;
        rowDown = row;

        for(y=0;y<ySize;y++){
            for(x=0;x<xSize;x++){
                boardCellsOccupied[y][x] = false;
                boardCellsContent[y][x] = -1;
            }
        }
    }

    if(domino.sideA==domino.sideB){
        equalTiles = true;
        sideFree = domino.sideA;
    }else{
        if(cardPosition=='u'){
            cardPlayed = cardPlayedUp;
        }else{
            cardPlayed = cardPlayedDown;
        }

        if(domino.sideA==cardPlayed){
            sideConnected = domino.sideA;
            sideFree = domino.sideB;
        }else{
            sideConnected = domino.sideB;
            sideFree = domino.sideA;
        }
    }

    if(cardsPlayed==0){
        cardPlayedUp = domino.sideA;
        cardPlayedDown = domino.sideB;

        if(equalTiles==true){
            colLeft--;
            colRight++;
        }else{
            colLeft--;
            colRight+=2;
            lieDown = true;
        }
    }else if(cardPosition=='u'){
        checkUpPosition(equalTiles, &row, &col, &lieDown, &separator);
        cardPlayedUp = sideFree;
    }else if(cardPosition=='d'){
        checkDownPosition(equalTiles, &row, &col, &lieDown, &separator);
        cardPlayedDown = sideFree;
    }

    if(equalTiles==true){
        if(lieDown==false){
            changeCellContent(true, false, row, col, sideFree, row-1);
        }else{
            changeCellContent(true, true, row, col, sideFree, col-1);
        }
        
        if(cardPosition=='u'){
            equalTilesUp = true;
        }else if(cardPosition=='d'){
            equalTilesDown = true;
        }else{
            equalTilesUp = true;
            equalTilesDown = true;
        }

    }else{
        if(cardsPlayed==0){
            equalTilesUp = false;
            equalTilesDown = false;
            changeCellContent(false, true, row, col, domino.sideA, xCenter, domino.sideB, xCenter+1);

        }else{
            if(lieDown==false){
                freePosition = row;
            }else{
                freePosition = col;
            }

            if(cardPosition=='u'){
                if(upSegment<3){
                    connectedPosition = freePosition+1;
                    if(equalTilesUp==false){
                        separatorPosition = freePosition+2;
                    }
                }else{
                    connectedPosition = freePosition-1;
                    if(equalTilesUp==false){
                        separatorPosition = freePosition-2;
                    }
                }
                equalTilesUp = false;
            }else if(cardPosition=='d'){
                if(downSegment<3){
                    connectedPosition = freePosition-1;
                    if(equalTilesDown==false){
                        separatorPosition = freePosition-2;
                    }
                }else{
                    connectedPosition = freePosition+1;
                    if(equalTilesDown==false){
                        separatorPosition = freePosition+2;
                    }
                }
                equalTilesDown = false;
            }
            changeCellContent(false, lieDown, row, col, sideFree, freePosition, sideConnected, connectedPosition, separatorPosition);        
        }
    }
    cardsPlayed++;
}

void Board::checkUpPosition(bool equalTiles, int *row, int *col, bool *lieDown, bool *separator){
    bool ready;
    int compare, x;

    while(ready==false){
        *row = rowUp;
        *col = colLeft;

        if(upSegment==4){
            cout<<"\nError: Cannot add more dominoes in one of the sides. Please increase the size of the board on the cpp...";
            enterCharToContinue("\nPress '1' to Continue", "1");
        }
        switch(upSegment){
            case 1:
                compare = colLeft;
                break;
            case 2:
                compare = rowUp;
                break;
            case 3:
                compare = xSize-colLeft;
                break;
        }

        if(equalTilesUp==false){
            compare--;
        }

       if(compare>1&&equalTiles==true){
            *separator = true;

            switch(upSegment){
                case 1:
                    colLeft--;
                    break;
                case 2:
                    rowUp--;
                    *lieDown = true;
                    break;
                case 3:
                    colLeft++;
                    break;
            }
            ready = true;
        }else if(compare>3&&equalTiles==false){
            if(equalTilesUp==false){
                x = 2;
                *separator = true;
            }else{
                x = 1;
            }

            switch(upSegment){
                case 1:
                    *col-=x;
                    colLeft-=x+1;
                    *lieDown = true;
                    break;
                case 2:
                    *row-=x;
                    rowUp-=x+1;
                    break;
                case 3:
                    *col+=x;
                    colLeft+=x+1; 
                    *lieDown = true;               
                    break;
            }
            ready = true;
        }else{
            switch(upSegment){
                case 1:
                    rowUp--;
                    colLeft++;
                    break;
                case 2:
                    rowUp++;
                    colLeft++;
                    break;
            }
            
            while(boardCellsOccupied[rowUp][colLeft]==true){
                switch(upSegment){
                    case 1:
                        rowUp--;
                        break;
                    case 2:
                        colLeft++;
                        break;
                }
            }
            upSegment++;
        }
    }
}

void Board::checkDownPosition(bool equalTiles, int *row, int *col, bool *lieDown, bool *separator){
    bool ready;
    int compare, x;

    while(ready==false){
        *row = rowDown;
        *col = colRight;

        if(downSegment==4){
            cout<<"\nError: Cannot add more dominoes in one of the sides. Please increase the size of the board on the cpp...";
            enterCharToContinue("\nPress '1' to Continue", "1");
        }
        switch(downSegment){
            case 1:
                compare = xSize-colRight;
                break;
            case 2:
                compare = ySize-rowDown;
                break;
            case 3:
                compare = colRight;
                break;
        }

        if(equalTilesDown==false){
            compare--;
        }

        if(compare>1&&equalTiles==true){
            *separator = true;

            switch(downSegment){
                case 1:
                    colRight++;
                    break;
                case 2:
                    rowDown++;
                    *lieDown = true;
                    break;
                case 3:
                    colRight--;
                    break;
            }
            ready = true;

        }else if(compare>3&&equalTiles==false){
            if(equalTilesDown==false){
                x = 2;
                *separator = true;
            }else{
                x = 1;
            }

            switch(downSegment){
                case 1:
                    *col+=x;
                    colRight+=x+1;
                    *lieDown = true;
                    break;
                case 2:
                    *row+=x;
                    rowDown+=x+1;
                    break;
                case 3:
                    *col-=x;
                    colRight-=x+1;
                    *lieDown = true;               
                    break;
            }
            ready = true;
        
        }else{
            switch(downSegment){
                case 1:
                    colRight--;
                    rowDown++;
                    break;
                case 2:
                    colRight--;
                    rowDown--;
                    break;
            }
            
            while(boardCellsOccupied[rowDown][colRight]==true){
                switch(downSegment){
                    case 1:
                        rowDown++;
                        break;
                    case 2:
                        colRight--;
                        break;
                }
            }
            downSegment++;
        }
    }
}

int Player::addCards(bool newGame, vector<int> shuffledCards, int numberPlayers, int cardsPerPlayer, int startRange, int endRange){    
    bool playerStartsGame;
    int x, y, cardNumber, sixSixPosition;
    Dominoes domino;

    if(newGame==true){
        cards.resize(cardsPerPlayer);
        cardsPlayed.resize(cardsPerPlayer);
    }

    y = 0;
    sixSixPosition = -1;

    for(x=startRange;x<endRange;x++){
        cardNumber = shuffledCards[x];
        playerStartsGame = domino.convertIntToCard(newGame, numberPlayers, cardNumber);

        if(playerStartsGame){
            sixSixPosition = x - startRange;
        }

        cards[y] = domino;
        cardsPlayed[y] = false;
        y++;
    }

    if(numberPlayers==3){
        if(playerStartsGame==true){
            cout<<"\nPlayer "<<nickname<<" deck:\n- Starts the Game\n";
        }else{
            cout<<"\nPlayer "<<nickname<<" deck:\n";
        }
        
    }else if(numberPlayers==4){
        if(playerStartsGame==true){
            cout<<"\nTeam "<<nickname<<", Player "<<teamNumber<<" deck:\n- Starts the Game\n";
        }else{
            cout<<"\nTeam "<<nickname<<", Player "<<teamNumber<<" deck:\n";
        }
    }
    
    for(x=0;x<cards.size();x++){
        cout<<"\n\t("<<x+1<<")"<<" -> ["<<cards[x].sideA<<"|"<<cards[x].sideB<<"]";
    }

    return sixSixPosition;
}

bool Player::canPlay(int cardPlayedUp, int cardPlayedDown){
    int x;

    for(x=0;x<cardsPlayed.size();x++){
        if(cardsPlayed[x]==false){
            if(cards[x].sideA==cardPlayedUp||cards[x].sideA==cardPlayedDown){
                //cout<<x<<cards[x].sideA;
                return true;
            }else if(cards[x].sideB==cardPlayedUp||cards[x].sideB==cardPlayedDown){
                //cout<<x<<cards[x].sideB;
                return true;
            }
        }
    }
    return false;
}

void Player::playCard(Board *dominoesBoard, int cardsPerPlayer, int sixSixPosition){
    bool canPlay;
    int cardNumber, cardPlayedUp, cardPlayedDown;
    Dominoes domino;

    if((*dominoesBoard).cardsPlayed==0){
        cout<<"\n\tPlayer "<<nickname<<" started the game!";
    }

    if(sixSixPosition!=-1){
        cardsPlayed[sixSixPosition] = true;
        domino.sideA = 6;
        domino.sideB = 6;
    }else{
        cardPlayedUp = (*dominoesBoard).cardPlayedUp;
        cardPlayedDown = (*dominoesBoard).cardPlayedDown;

        do{
            cardNumber = intQuestion("Select a card of your deck unused", 1, cardsPerPlayer);
            domino = cards[cardNumber-1];

            if(cardsPlayed[cardNumber-1]==false){
                if(domino.sideA==cardPlayedUp||domino.sideB==cardPlayedDown||domino.sideB==cardPlayedUp||domino.sideA==cardPlayedDown){
                    canPlay = true;
                }else{
                    cout<<"\t- Cannot use card of position "<<cardNumber;
                }
            }else{
                cout<<"\t- Card of position "<<cardNumber<<" already played";
            }
        }while(canPlay==false);

        cardsPlayed[cardNumber-1] = true;
    }

    (*dominoesBoard).addCard(domino);   
}

int Player::countPointsLeft(){
    int x, cardsPointsLeft = 0;

    for(x=0;x<cardsPlayed.size();x++){
        if(cardsPlayed[x]==false){
            cardsPointsLeft += cards[x].sideA + cards[x].sideB;
        }
    }
    return cardsPointsLeft;
}

void Player::printCardsLeft(int numberPlayers){
    int x;
    int y = 1;

    if(numberPlayers==3){
        cout<<"\nPlayer "<<nickname<<" cards left:\n";
        
    }else if(numberPlayers==4){
        cout<<"\nTeam "<<nickname<<", Player "<<teamNumber<<" cards left:\n";
    }

    for(x=0;x<cardsPlayed.size();x++){
        if(cardsPlayed[x]==false){
            cout<<"\n\t("<<y<<")"<<" -> ["<<cards[x].sideA<<"|"<<cards[x].sideB<<"]";
            y++;
        }
    }
}

void Player::playerWinRound(bool teams, int points){
    if(teams==false){
        cout<<"\nPlayer "<<nickname<<" won the round!";
    }else{
        cout<<"\nTeam "<<nickname<<" won the round!";
    }
    cout<<"\n- Points: "<<points;

    enterCharToContinue("\nPress '1' to Continue", "1");
}

bool Player::playerWinGame(bool teams, int pointsToWin){
    if(points>=pointsToWin){
        if(teams==false){
            cout<<"\nPlayer "<<nickname<<" won the game!";
        }else{
            cout<<"\nTeam "<<nickname<<" won the game!";
        }
        return true;

    }else{
        return false;
    }
}

void Player::winBackground(int numberPlayers){
    // To code
    enterCharToContinue("\nPress '1' to Continue", "1");
}


int main(){
    Board dominoesBoard; 
    dominoesBoard.boardSize(37, 21); // Must be an odd number

    bool answer, gameEnded, roundEnded;
    int x, y, n, input, 
        numberPlayers, numberCards, cardsPerPlayer, gameWinner,
        maxDoubles, points, pointsToWin,
        playerTurn, hand, sixSixPosition, startRange, endRange;
    int newGame = true;
    int roundWinner = -1;
    string nickname;
    vector<int> dominoesDoubles, shuffledCards;

    string title = "*** Welcome to the Minimalist Dominoes Game ***\nmade by: blackb0x\n";

    for(x=0;x<2;x++){
        clearAndTitle(title);

        if(x==0){
            cout<<"\nRules to win a Round:\n\n\t1. Be the first player to use all of his cards\n";
            cout<<"\t2. If the game is blocked, be the player with the less points in his deck\n";
            cout<<"\t3. If the 2nd criteria applies to two players, the winner would be:\n";
            cout<<"\t\ta. The one who started the game or the one is closer to his turn\n";
            cout<<"\t\tb. ONFLY APPLIES FOR 3 PLAYERS MODE: The one who is next to the player who started the game\n";

            cout<<"\nRules to win a Game:\n\n\t1. Reach the ammount of points to win first\n";

            enterCharToContinue("Press '1' to AGREE TO THE TERMS AND CONDITIONS", "1");
        }else{
            pointsToWin = intQuestion("How much points to win?", 50, 200);
            numberPlayers = intQuestion("Number of players?", 3, 4);

            if(numberPlayers==3){
                maxDoubles = 4;
                numberCards = 27;

            }else if(numberPlayers==4){
                dominoesBoard.teams = true;
                maxDoubles = 3;
                numberCards = 28;
            }

            cardsPerPlayer = numberCards/numberPlayers;
            dominoesDoubles.push_back(0);

            y = 0;
            for(x=7;x>1;x--){            
                y += x;

                if(numberPlayers!=3||(numberPlayers==3&&y!=27)){
                    dominoesDoubles.push_back(y);
                }
            }
        }
    }

    Player players[numberPlayers];

    x = 0;
    do{
        bool rightInput = false;

        clearAndTitle(title);

        do{
            if(numberPlayers==3){
                cout<<"\nPlayer "<<x+1;

            }else if(numberPlayers==4){
                cout<<"\nTeam "<<x+1;
            }

            nickname = stringQuestion("Nickname");

            switch(x){
                case 0:
                    rightInput = true;
                    break;
                case 1:
                    if(nickname!=players[0].nickname){
                        rightInput = true;
                    }
                    break;
                case 2:
                    if(nickname!=players[0].nickname&&nickname!=players[1].nickname){
                        rightInput = true;
                    }
                    break;
            }

            if(rightInput==true){
                answer = booleanQuestion("Are you sure");
                if(answer==false){
                    rightInput = false;
                }               
            }
            
        }while(rightInput==false);

        players[x].nickname = nickname;
        dominoesBoard.players.push_back(nickname);
        dominoesBoard.points.push_back(0);
                
        if(numberPlayers==4){
            if(x==0){
                players[0].teamNumber = 1;
                players[2].nickname = nickname;
                players[2].teamNumber = 2;
            }else{
                players[1].teamNumber = 1;
                players[3].nickname = nickname;
                players[3].teamNumber = 2;
                x = 3;
            }
        }
        x++;

    }while(x<numberPlayers);

    do{        
        roundEnded = false;
        points = 0;
        dominoesBoard.cardsPlayed = 0;

        clearAndTitle(title);

        do{
            cout<<"\n*** Shuffling cards ***\n";
            shuffledCards = shuffleCards(numberCards, numberPlayers);

            for(x=0;x<numberCards-1;x++){
                if(x%cardsPerPlayer==0){
                    n = 0;
                }

                for(y=0;y<dominoesDoubles.size();y++){
                    if(shuffledCards[x]==dominoesDoubles[y]){                        
                        n++;
                    }
                }

                if(n>maxDoubles){
                    cout<<"\n\t- Too much doubles on a player deck. Shuffling cards...\n";
                    break;
                }
            }
        }while(n>maxDoubles);

        enterCharToContinue("Press '1' to Start the Game", "1");

        for(x=0;x<numberPlayers;x++){
            int hasSixSix;

            clearAndTitle(title);
            cout<<"\n*** Shuffling cards ***\n";

            startRange = x*cardsPerPlayer;
            endRange = (x+1)*cardsPerPlayer;
            hasSixSix = players[x].addCards(newGame, shuffledCards, numberPlayers, cardsPerPlayer, startRange, endRange);

            if(hasSixSix!=-1){
                playerTurn = x;
                sixSixPosition = hasSixSix;
            }

            enterCharToContinue("\nPress '1' to Continue", "1");
        }

        do{
            dominoesBoard.printTitleBar(title, numberPlayers);
            dominoesBoard.printBoard();

            if(dominoesBoard.cardsPlayed==0){
                if(newGame==true){
                    players[playerTurn].playCard(&dominoesBoard, cardsPerPlayer, sixSixPosition);
                    newGame = false;

                }else{
                    if(numberPlayers==3){
                        playerTurn = roundWinner;

                    }else if(numberPlayers==4){
                        answer = booleanQuestion("Player 1 of Winner Team wants to Start?");
                            
                        if(roundWinner==0||roundWinner==2){
                            if(answer==true){
                                playerTurn = 0;
                            }else{
                                playerTurn = 2;
                            }
                        }else{
                            if(answer==true){
                                playerTurn = 1;
                            }else{
                                playerTurn = 3;
                            }
                        }
                    }
                    roundWinner = -1;
                    hand = playerTurn;
                    players[playerTurn].playCard(&dominoesBoard, cardsPerPlayer);
                }
                n = 0;
                enterCharToContinue("\nPress '1' to Continue", "1");

            }else{
                if(numberPlayers==3){
                    cout<<"\n\tPlayer "<<players[playerTurn].nickname<<" turn:\n";

                }else if(numberPlayers==4){
                    cout<<"\n\tTeam "<<players[playerTurn].nickname<<", Player "<<players[playerTurn].teamNumber<<" turn:\n";
                }

                answer = players[playerTurn].canPlay(dominoesBoard.cardPlayedUp, dominoesBoard.cardPlayedDown);

                if(answer==true){
                    n = 0;
                    players[playerTurn].playCard(&dominoesBoard, cardsPerPlayer);
                }else{
                    n++;
                        
                    if(numberPlayers==3){
                        cout<<"\n\t- Player "<<players[playerTurn].nickname<<" cannot play in any of both sides";
                        
                    }else if(numberPlayers==4){
                        cout<<"\n\t- Team "<<players[playerTurn].nickname<<", Player "<<players[playerTurn].teamNumber<<" cannot play in any of both sides";
                    }
                    enterCharToContinue("\nPress '1' to Continue", "1");
                }
            }

            if(players[playerTurn].countPointsLeft()==0){
                roundWinner = playerTurn;

                for(x=0;x<numberPlayers;x++){
                    cout<<players[roundWinner].nickname<<" have no cards left!\n";

                    if(x!=playerTurn){
                        dominoesBoard.printTitleBar(title, numberPlayers);
                        dominoesBoard.printBoard();
                        players[x].printCardsLeft(numberPlayers);

                        points += players[x].countPointsLeft();
                        enterCharToContinue("\nPress '1' to Continue", "1");
                    }
                }
            }else if(n!=numberPlayers){
                if(playerTurn==numberPlayers-1){
                    playerTurn = 0;
                }else{
                    playerTurn++;
                }
            }else{
                cout<<"\nGame blocked: Checking who is the winner";

                bool draw; 
                int minPoints, playerPoints;
                vector<int> drawPlayers;

                for(x=0;x<numberPlayers;x++){
                    dominoesBoard.printTitleBar(title, numberPlayers);
                    dominoesBoard.printBoard();
                    players[x].printCardsLeft(numberPlayers);

                    playerPoints = players[x].countPointsLeft();
                    points += playerPoints;

                    if((x==0)||(playerPoints<minPoints)){
                        minPoints = playerPoints;
                        roundWinner = x;

                        if(draw==true){
                            draw = false;
                            drawPlayers.erase(drawPlayers.begin(), drawPlayers.end());
                        }
                            
                    }else if(playerPoints==minPoints){
                        if(draw==false){
                            draw = true;
                            drawPlayers.push_back(roundWinner);
                            roundWinner = -1;
                        }
                        drawPlayers.push_back(x);
                    }
                    enterCharToContinue("\nPress '1' to Continue", "1");
                }

                if(draw==true){
                    cout<<"\n\n\tDraw between ";

                    int y;
                    int drawPlayersSize = drawPlayers.size();
                    for(x=0;x<drawPlayersSize;x++){
                        y = drawPlayers[x];

                        if(roundWinner==-1){
                            if(y==hand){
                                roundWinner = y;
                            }else if(numberPlayers==4&&hand==y+2){
                                roundWinner = y;
                            }else if(x==drawPlayersSize-1){
                                roundWinner = hand + 1;
                            }
                        }
                            
                        if(x<drawPlayersSize-1){
                            cout<<players[y].nickname<<" and ";

                        }else{
                            cout<<players[y].nickname<<"\n";
                        }
                    }

                    for(x=0;x<drawPlayersSize;x++){
                        y = drawPlayers[x];
                        players[y].printCardsLeft(numberPlayers);
                    }
                }
            }
        }while(roundWinner==-1);

        dominoesBoard.printTitleBar(title, numberPlayers);

        if(numberPlayers==3){
            players[roundWinner].playerWinRound(false, points);
             
            players[roundWinner].points += points;
            dominoesBoard.points[roundWinner] = players[roundWinner].points;
            gameEnded = players[roundWinner].playerWinGame(false, pointsToWin);

        }else if(numberPlayers==4){
            players[roundWinner].playerWinRound(true, points);

            if(roundWinner==0||roundWinner==2){
                players[0].points += points;
                players[2].points += points;
                dominoesBoard.points[0] = players[0].points;

            }else if(roundWinner==1||roundWinner==3){
                players[1].points += points;
                players[3].points += points;
                dominoesBoard.points[1] = players[1].points;
            }
            
            gameEnded = players[roundWinner].playerWinGame(true, pointsToWin);
            gameWinner = roundWinner;
        }    
    }while(gameEnded==false);

    players[gameWinner].winBackground(numberPlayers);

    return 0;
}