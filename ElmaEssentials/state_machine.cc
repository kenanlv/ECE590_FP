#include <iostream>
#include "elma.h"
#include <unordered_set>
#include <vector>


using namespace std;

namespace elma {

    int State::_id_counter = 0;

    StateMachine& StateMachine::set_initial(State& s) {
        _initial = &s;
        return *this;
    }

    StateMachine& StateMachine::add_transition(string event_name, State& from, State& to) {
        _transitions.push_back(Transition(event_name, from, to));
        to._state_machine_ptr = this;
        from._state_machine_ptr = this;
        return *this;
    }

    void StateMachine::init() {
        for (auto transition : _transitions ) {
            watch(transition.event_name(), [this, transition](Event& e) {
                if ( _current->id() == transition.from().id() ) {
                    _current->exit(e);
                    _current = &transition.to();
                    _current->entry(e);
                    if ( !_propagate ) {
                      e.stop_propagation();
                    }
                }
            //}, 0);
            });
        }
    }

    void StateMachine::start() {
        if ( _initial == NULL ) { 
            throw(Exception("State machine started without an initial state (call set_initial(...) first)"));
        }
        _current = _initial;
        _current->entry(Event("start"));
    }

    void StateMachine::update() {
        _current->during();
    }

    void StateMachine::stop() {}

    json StateMachine::to_json() {
        //take from transition for every states.
        json j;

        vector<string> states;
        for (auto s : _transitions) {
            states.push_back(s.to().name());
            states.push_back(s.from().name());
        }
        std::unordered_set<string> set;
        for (const auto &i : states) {
            set.insert(i);
        }
        vector<map<string, string>> totalTrans;
        map<string, string> tran;
        for(auto transition : _transitions) {
            tran.insert (pair<string,string>("from",transition.from().name() ) );
            tran.insert (pair<string,string>("to",transition.to().name() ) );
            tran.insert (pair<string,string>("when",transition.event_name()) );
            totalTrans.push_back(tran);
        } 

        json value = {{"name", name()},{"states", set},{"transitions", totalTrans}};

        return value;
    }

};