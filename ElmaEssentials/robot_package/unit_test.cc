#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <stdlib.h>
#include "gtest/gtest.h"
#include "robot.h"
#include "elma.h"
#include <ncurses.h>

namespace {
    using namespace std::chrono;
    using namespace elma;


/*!!
 * Here we use your Robot class to give the robot
 * a little trial and see what state its in.
 *
 * Variants of tests will include more than one robot,
 * non-sense events, and varying event sequences.
 */
TEST(Question2, RobotEnvironment1) {
    std::srand((int) time(0));
    // create a new robot
    Robot robot = Robot("What a very nice robot");//state
    // Trans trans;//process
    Intruder intruder;//process
    Map robot_map;//process
    // init manager
    Manager m;
    m.schedule(robot, 10_ms)
    .schedule(robot_map, 10_ms)
    // .schedule(trans, 100_ms)
    .schedule(intruder, 2_s)
    .init()
    .start()
    // .emit(Event("start"))
    .run(20_s);
     

    string wander = "Wander";
    string noise = "Make Noise";
    string evade = "Evade";

    /*
     * Send signals to robot and test
     */
    std::cout << robot.current().name() << std::endl;
    // EXPECT_EQ(robot.current().name(), wander.c_str());
    // m.emit(Event("intruder detected"));
    
    // std::cout << robot.current().name() << std::endl;
    // m.emit(Event("approach"));
    // std::cout << "approach" << std::endl;
    // std::cout << robot.current().name() << std::endl;
    // std::cout<<"\n";
    // m.emit(Event("Ha"));
    // std::cout << robot.current().name() << std::endl;
    

}
}