#ifndef ROBOT_H
#define ROBOT_H

#include <iostream>
#include <chrono>
#include "elma.h"
#include <string>
#include <vector>
#include <thread>
#include <stdlib.h>
#include <time.h> 
#include <cstdlib>
#include <ctime>
/*****************************************************
*This Robot h file contains several classes that makes 
*up the entire project. It is a simulation for a robot
*to wandering aournd in a given map. And will execute 
*different behaviors when different events been met. 
*behaviors including: Wandering around in the map;
*find intruder and eliminate it; Going back to the 
*charging stattion when it is out of battery.
*
*Below is the list of consist classes:
* - Robot states (State) | <line 34 - 125>
* - Intruder class (Process) | <line 131 - 141>
* - Obj class | <line 150 - 171>
* - Map class (Process) | <line 176 - 433>
* - Robot class (State Machine) | <line 435 - 487>
*
*The code will print a map on the console for 2-D visulization
*******************************************************/

using namespace elma;
using namespace std;
int num_stp = 0; //count numbers of robot has moved.
bool is_wander = false; //global variable to tell if 
                        //robot is in wander state.

/*Robot States : Idle, Recharge, FindRechargeStation
*                Wander, Kill, MakeNoise, Evade
*All states will be utilized when different events
*happened. More detials of state transition could be
*found at the Robot class.*/
class Idle : public State {
public:
    Idle() : State("Idle") {}
    void entry(const Event& e) {}
    void during() {} 
    void exit(const Event& e) {}

};
class ReChar : public State {
public:
    ReChar() : State("Recharge") {}
    void entry(const Event& e) {}
    void during() {} 
    void exit(const Event& e) {}

};
class FindRechSta : public State {
public:
    FindRechSta() : State("Find Recharge Station") {}
    void entry(const Event& e) {}
    void during() {} 
    void exit(const Event& e) {}

};

class Wander : public State {
public:
    Wander() : State("Wander") {}
    void entry(const Event& e) {
         //initial state wander generate robot 
         cout<<"> Entering wander state"<<"\n";
         //Randomly generate a robot if it has not been generated
         if (!is_robot) {
            emit(Event("generate robot"));
            is_robot = true;
         }
         is_wander = true;
    }
    void during() {
        //Wandering in the map
        cout<<"> I am wandering around"<<"\n";
    } 
    void exit(const Event& e) {
        if (e.name() == "intruder detected") {
            std::cout<<"> Intruder detected, make some noise!\n";
        }
        //When intruder detected, robot no longer behaves wandering.
        is_wander = false;
    }
    private:
    bool is_robot = false;
};

class Kill : public State {
public:
    Kill() : State("Kill") {}
    void entry(const Event& e) {
        //emit event HA, will trigger map to emit "eliminate"
        //Robot states will work along with map class to deliver event propagation.
        emit(Event("Ha"));

    }
    void during() {} 
    void exit(const Event& e) {
        if (e.name() == "eliminated") {
            std::cout<<"> Intruder eliminated, make some noise!\n";
        }
    }

};

class MakeNoise : public State {
public:
    MakeNoise() : State("Make Noise") {}
    void entry(const Event& e) {
        emit(Event("making noise"));
        
    }
    void during() {
        cout<<"> Approaching target"<<"\n";
    } 
    void exit(const Event& e) {}

};

class Evade : public State {
public:
    Evade() : State("Evade") {}
    void entry(const Event& e) {}
    void during() {} 
    void exit(const Event& e) {}

};
//*****************End of robot states**************

/*This is the indtruder class. It is in charge of emitting events for
*generating intruders. In the test file, intruder will be generated
*every two seconds.*/
class Intruder : public Process {
    public: 
    Intruder() : Process("intruder") {}
    void init() {}
    void start() {}
    void update() {
        emit(Event("generate intruder"));
    }
    void stop() {}
};

//*****************End of Intruder class**************

/*Object class will serve the map class. Objects will
*be passed around in the map. Every object: robot,
*intruder, and charging station is an object in the
*map. Will see detailed use of Object inside the map
*class.(e.g. updateMap)*/
class Obj { //: StateMachine //add transisiton
//objects in the map
  public:
  int row, col;
  Obj() {}

  void random() {
    row = rand()%10;
    if (row == 4) {
      random();
    }
    col = rand()%10;
    if (col == 0) {
      random();
    }
  }
  void set(int ir, int ic) {
    row = ir;
    col = ic;
  }
};
//*****************End of Obj class**************//

/*Map class: this class serves for the visulization
*of the simulation. It draws robot, intruder, and 
*charging station. And updates them acoordingly.*/
class Map : public Process {
    friend Wander;
    public:
    
    Map() : Process("robot_map") {}
    //Functoin for robot to move in the map
    void Up(int &row) {
    row--;
    num_stp++;
    }
    void Down(int &row) {
    row++;
    num_stp++;
    }
    void Left(int &col) {
    col--;
    num_stp++;
    }
    void Right(int &col) {
    col++;
    num_stp++;
    }

    //Initialize the map by creating a row x col matrix
    vector<vector<char>> initialMap(int row, int col) {
        vector<vector<char>> map(row);

        for ( int i = 0 ; i < row ; i++ ) {
            map[i].resize(col);
            for (int j = 0; j < col; j++) {
            map[i][j] = '_';
            }
        }
        map[4][0] = '0';
        postMap(map);
        return map;
    }

    //Print out the map onto consol screen
    void postMap(vector<vector<char>> map) {
        
        cout<<"************MAP****************"<<"\n";
        for ( int i = 0 ; i < map.size(); i++ ) {
            cout<<"\t";
            for (int j = 0; j < map[0].size(); j++) {
            cout<<map[i][j];
            }
            cout<<"\n";
        }
        cout<<"*******************************"<<"\n";
        cout<<"\n";
        std::this_thread::sleep_for (std::chrono::milliseconds(700));
    }

    //Update the entire map. Including positions for robots and
    //intruders.
    void updateMap(vector<vector<char>> &map, Obj &r, Obj &c) {
        //Comparing the positions of r and c to decide move up or down
        if(r.row > c.row) {
            for (int i = r.row; i > c.row + 1; i--) {
                map[r.row][r.col] = '_';
                Up(r.row);
                
                cout<<"> Up"<<"\n";
                map[r.row][r.col] = '^';
                map[4][0] = '0';
                postMap(map);
                std::this_thread::sleep_for (std::chrono::milliseconds(200));
                std::cout<<"\n";
            }
        } else{
            for (int i = r.row; i < c.row - 1; i++) {
                map[r.row][r.col] = '_';
                Down(r.row);

                cout<<"> Down"<<"\n";
                map[r.row][r.col] = '^';
                map[4][0] = '0';
                postMap(map);
                std::this_thread::sleep_for (std::chrono::milliseconds(200));
                std::cout<<"\n";
            }
        }
        //move left or right
        if(r.col > c.col && r.row != c.row) {
            for (int i = r.col; i > c.col; i--) {
                map[r.row][r.col] = '_';
                Left(r.col);
                cout<<"> Left"<<"\n";
                map[r.row][r.col] = '^';
                map[4][0] = '0';
                postMap(map);
                std::this_thread::sleep_for (std::chrono::milliseconds(200));
                std::cout<<"\n"; // && r.row != c.row
            }
        } else if (r.col < c.col && r.row != c.row) {
            for (int i = r.col; i < c.col; i++) {
                map[r.row][r.col] = '_';
                Right(r.col);
                cout<<"> Right"<<"\n";
                map[r.row][r.col] = '^';
                map[4][0] = '0';
                postMap(map);
                std::this_thread::sleep_for (std::chrono::milliseconds(200));
                // clear();
                // ClearScreen();
                std::cout<<"\n";
            }
        }
    }


    /*Special map function for Wander sate.
    *Will keep track of the state robot is currently in. If its in 
    *the Wander state, robot will wandering around. randomly moving
    *around in the map*/

    void wander(vector<vector<char>> &map, Obj &r) {
    int c = rand()%10;
    int a;
    if (c <= 1) {
        a = 0;
    } else if(c > 1 && c <= 3) {
        a = 1;
    } else if(c > 3 && c <= 6) {
        a = 2;
    } else {
        a = 3;
    }
    switch ( a ) {            
            case 0:
                if (r.row >= map.size() || r.col + 1 >= map[0].size()) {
                    break;
                }
                map[r.row][r.col] = '_';
                Right(r.col);
                cout<<"> Right"<<"\n";  
                map[r.row][r.col] = '^';
                map[4][0] = '0';
                postMap(map);
                std::this_thread::sleep_for (std::chrono::milliseconds(500));
                std::cout<<"\n";
                break;
            case 1:
                if (r.row >= map.size() || r.col - 1 <= 0) {
                    break;
                }
                map[r.row][r.col] = '_';
                Left(r.col);
                cout<<"> Left"<<"\n";
                map[r.row][r.col] = '^';
                map[4][0] = '0';
                postMap(map);
                std::this_thread::sleep_for (std::chrono::milliseconds(500));
                std::cout<<"\n";
                break;
            case 2:
                if (r.row + 1 >= map.size() || r.col >= map[0].size()) {
                    break;
                }
                map[r.row][r.col] = '_';
                Down(r.row);
                
                cout<<"> Down"<<"\n";
                map[r.row][r.col] = '^';
                map[4][0] = '0';
                postMap(map);
                std::this_thread::sleep_for (std::chrono::milliseconds(500));
                std::cout<<"\n";
                break;
            case 3:
                if (r.row - 1 <= 0 || r.col >= map[0].size()) {
                    break;
                }
                map[r.row][r.col] = '_';
                Up(r.row);
                cout<<"> Up"<<"\n";
                map[r.row][r.col] = '^';
                map[4][0] = '0';
                postMap(map);
                std::this_thread::sleep_for (std::chrono::milliseconds(500));
                std::cout<<"\n";
                break;
        }
    }
/*Init function, will watch for several events and update 
*coorespond data. For example, generate robot/ intruders.*/
    void init() {       
        row = 10;
        col = 10;
        map = initialMap(row, col);
        system("clear");
        cout<<"\n";
        watch("generate robot", [this](Event& e) {
            cout<<"- Randomly generating robot"<<"\n";
            r.random();
            map[r.row][r.col] = '^';
            postMap(map);
            cout<<"\n";

        });
        
        watch("generate intruder", [this](Event& e) {
            std::cout<<"- Randomly generating intruder"<<"\n";
            c.random();
            
            if (num_stp >= 8) {
                emit(Event("battery low"));
                cout<<"> Steped though: "<<num_stp<<" steps"<<"\n";
                cout<<"> Going to the charging station"<<"\n";
                re.set(4, 0);
                updateMap(map, r, re);
                emit(Event("battery full"));
                cout<<"> Battery full!"<<"\n";
                num_stp = 0;
            }
            map[c.row][c.col] = 'X';
            emit(Event("intruder detected"));
            postMap(map);
            cout<<"\n";
        });
        
        watch("intruder detected", [this](Event& e) {
            updateMap(map, r,c);
            postMap(map);
            cout<<"\n";
            emit(Event("approach"));
        });
        watch("making noise", [this](Event& e) {
            updateMap(map, r,c);
            postMap(map);
            cout<<"\n";
            emit(Event("approach"));
        });
        watch("Ha", [this](Event& e) {
            map[c.row][c.col] = '_';
            postMap(map);
            cout<<"\n";
            emit(Event("eliminated"));
        });
        
    }
    void start() {}
    void update() {
        if (is_wander) {
            wander(map, r);
            std::this_thread::sleep_for (std::chrono::milliseconds(300));
        }
    }
    void stop() {}
    
    private:
    vector<vector<char>> map;
    Wander wander_s; 
    int row, col;
    Obj r,c, re;
};
//*****************End of Map class**************//

class Robot : public StateMachine {
public:
    /*
     * Implement contructors. Constructors should instantiate states
     * and transitions AND should set initial_state so that Event('start')
     * sets the Robot to state 'Wander'
     *
     * Look at code for StateMachine.h and in examples and elma/example if you need help.
     *
     * States should have the EXACT names:
     * "Recharge"
     * "Wander"
     * "Find Recharge Station"
     * "Evade"
     * Make Noise"
     *
     * Robot should respond to events with the EXACT names:
     * "found recharge station"
     * "battery full"
     * "battery low"
     * "start"
     * "reset"
     * "intruder detected"
     * "proximity warning"
     */
    friend Map;
    Robot(std::string name) : StateMachine(name) {
        // set_propagate(1);
        set_initial(wander);
        add_transition("found recharge station", find_re_sta, recharge);
        add_transition("start", idle, wander);
        add_transition("battery full", recharge, wander);
        add_transition("reset", evade, make_noise);
        add_transition("intruder detected", wander, make_noise);
        add_transition("proximity warning", make_noise, evade);
        add_transition("reset", make_noise, wander);
        add_transition("eliminated", kill, wander);
        add_transition("approach", make_noise, kill);
        add_transition("battery low", wander, recharge);

    }
private:
     Wander wander;
     FindRechSta find_re_sta;
     ReChar recharge;
     Evade evade;
     MakeNoise make_noise;
     Idle idle;
     Kill kill;

};
//*****************End of Robot class**************//
#endif