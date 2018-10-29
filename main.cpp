#include <iostream>
#include "Node.h"
#include <queue>
#include <memory>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <map>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stack>


matrix Goal ={{1,2,3,4},
                  {5,6,7,8},
                  {9,10,11,12},
                  {13,14,15,0}};
/*
 * Fajna funkcja haszujaca wiem
 */
int BFS (std::shared_ptr<Board> BoardState, matrix resoult, std::vector<std::string> &pattern)
{
    if(BoardState->getBoard()==Goal){
        std::cout<<"Found Solution, loaded matrix is equal to final matrix";
        return 1;
    }
    auto start = std::make_shared<Node>("Error", nullptr, BoardState->getPossibleMoves(), BoardState->getBoard(), pattern);
    std::shared_ptr<Node> curNode;
    std::shared_ptr<Node>Children;
    std::queue<std::shared_ptr<Node>> open_list;
    std::unordered_set<size_t> explored;
    open_list.push(start);
    while(!open_list.empty()){
        curNode = open_list.front();
        explored.insert(curNode->getSeed());
        open_list.pop();
        if(BoardState->getBoard()==Goal){
                std::cout << curNode->getCounter() << std::endl;
                std::cout << curNode->getPath() ;
                return 1;
        }
        else {
            for (const auto &it:curNode->getPossibleMovesForNode()){
                BoardState->setBoardSize(curNode->getState());
                BoardState->setCoordinates();
                BoardState->takeAction(it);
                Children = std::make_shared<Node> (it,curNode, BoardState->getPossibleMoves() , BoardState->getBoard(), pattern);
                if(explored.find(Children->getSeed())  != explored.end())
                    continue;
                if(BoardState->getBoard()==Goal) {
                    std::cout << Children->getCounter() << std::endl;
                    std::cout << Children->getPath() ;
                    return 1;
                }
               open_list.push(Children);
            }
        }
    }
    return 15;
}

int DFS (std::shared_ptr<Board> BoardState, matrix resoult, std::vector<std::string> &pattern, int rec)
{
    if(BoardState->getBoard()==Goal){
        std::cout<<"Found Solution, loaded matrix is equal to final matrix";
        return 1;
    }
    auto start = std::make_shared<Node>("Error", nullptr, BoardState->getPossibleMoves(), BoardState->getBoard(), pattern);
    std::shared_ptr<Node> curNode;
    std::shared_ptr<Node>Children;
    std::stack<std::shared_ptr<Node>> open_list;
    std::unordered_set<size_t> explored;
    open_list.push(start);
    while(!open_list.empty()){
        curNode = open_list.top();
        explored.insert(curNode->getSeed());
        open_list.pop();
        if(BoardState->getBoard()==Goal){
            std::cout << curNode->getCounter() << std::endl;
            std::cout << curNode->getPath() ;
            return 1;
        }
        else {
            for (const auto &it:curNode->getPossibleMovesForNode()){
                BoardState->setBoardSize(curNode->getState());
                BoardState->setCoordinates();
                BoardState->takeAction(it);
                Children = std::make_shared<Node> (it,curNode, BoardState->getPossibleMoves() , BoardState->getBoard(), pattern);
                if(explored.find(Children->getSeed() ) != explored.end())
                    continue;
                if(Children->getPath().length()>=rec)
                    continue;
                if(BoardState->getBoard()==Goal) {
                    std::cout << Children->getCounter() << std::endl;
                    std::cout << Children->getPath() ;
                    return 1;
                }
                open_list.push(Children);
            }
        }

    }
    return 15;
}
const int Hamming(matrix arg){
    int goal=0;
    for(int i=0; i<=3; i++)
        for(int j=0;j<=3;j++){
            if(arg[i][j]==0)
                //Zaznaczaał żeby ignorować pole z zerem
                continue;
            if(arg[i][j]!=Goal[i][j])
                goal++;
        }
        return goal;
}
const int Manhatann(matrix arg){

}


int ASTAR(std::shared_ptr<Board> BoardState, matrix resoult, std::vector<std::string> &pattern){
    if(BoardState->getBoard()==Goal){
        std::cout<<"Found Solution, loaded matrix is equal to final matrix";
        return 1;
    }
    auto start = std::make_shared<Node>("Error", nullptr, BoardState->getPossibleMoves(), BoardState->getBoard(), pattern);
    std::shared_ptr<Node> curNode;
    std::shared_ptr<Node>Children;
    //Lambda przypisana do funkcji, śmieszne
    auto cmp = [](std::shared_ptr<Node> &left,std::shared_ptr<Node> &right){
        return left->getCounter() + Hamming  (left->getState() ) >= right->getCounter() + Hamming(right->getState() ) &&
               left->getCounter() + Hamming  (left->getState() ) != right->getCounter() + Hamming(right->getState() );
    };
    std::priority_queue<std::shared_ptr<Node>,std::deque<std::shared_ptr<Node>>, decltype(cmp) > open_list(cmp);
    std::unordered_set<size_t> explored;
    open_list.push(start);
    while(!open_list.empty()){
        curNode = open_list.top();
        explored.insert(curNode->getSeed());
        open_list.pop();
        if(BoardState->getBoard()==Goal){
            std::cout << curNode->getCounter() << std::endl;
            std::cout << curNode->getPath() ;
            return 1;
        }
        else {
            for (const auto &it:curNode->getPossibleMovesForNode()){
                BoardState->setBoardSize(curNode->getState());
                BoardState->setCoordinates();
                BoardState->takeAction(it);
                Children = std::make_shared<Node> (it,curNode, BoardState->getPossibleMoves() , BoardState->getBoard(), pattern);
                if(explored.find(Children->getSeed())  != explored.end())
                    continue;
                if(BoardState->getBoard()==Goal) {
                    std::cout << Children->getCounter() << std::endl;
                    std::cout << Children->getPath() ;
                    return 1;
                }
                open_list.push(Children);
            }
        }

    }
    return 15;

}

matrix parserToMatrix(std::string &arg) {
matrix Numbers;
std::ifstream infile(arg.c_str());
if(!infile.fail()) {
    std::string String;
    int a;
    while (getline(infile, String)) {
        //Kinda lazy solution
        if (String.size() > 3) {
            std::vector<int> Line;
            std::stringstream Stream(String);
            while (Stream >> a)
                Line.push_back(a);
            Numbers.push_back(Line);
        }
    }
    infile.close();
    return Numbers;
    }
    else std::cout << "FILE DOESNT EXIST!" << std::endl;
    return Goal;
}
int main(int argc, char* argv[]) {
    std::vector<std::string> pattern ;
    std::vector<std::string>  fileName;
    if(argc>1) {
        fileName.assign(argv + 1, argv + argc-1);
    }
    std::string Hold;
    for(auto &it:fileName)
        Hold.append(it);
    if(argc>2) {
        std::string str = (argv[2]);
        std::string help;
        for (unsigned int i = 0; i <= 3; i++) {
            char hold = str.at(i);
            pattern.push_back(help=hold);
        }
    }
    matrix Testuje=parserToMatrix(Hold);
    auto Test = std::make_shared<Board>(Testuje);
    Test->setCoordinates();
    int a =20;
    std::string A;
    std::cout <<"What method \n";
    std::cin >> A;
    clock_t tStart = clock();
    if(A=="BFS")
    BFS(Test, Goal, pattern);
    if(A=="DFS") {
        int rec;
        std::cout<<"NUMBER OF RECURSIONS\n";
        std::cin>>rec;
        DFS(Test, Goal, pattern, rec);
    }
    if(A=="A*"){
        ASTAR(Test, Goal, pattern);
    }
    printf("\nTime taken: %.3fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    return 0;
}