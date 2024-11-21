//Author: Andrew Kerr
//Date: 04/27/2022

#include "graph.hpp"
#include <list>
#include <algorithm>

UndirectedPath::UndirectedPath(Computer& a, Computer& b, int cost) : _a(a), _b(b), _cost(cost) {}

PathList::PathList(bool own_paths) {
    //Sets the default variables.
    this->_head = NULL;
    this->_tail = NULL;
    this->_owner_of_paths = own_paths;
}

PathList::~PathList() {
    //Destroyes the list.
    if(!_head) {
        PathNode* node = _head;
        PathNode* next = NULL;
        while(node) {
            next = node->_next;
            if(node->_path && _owner_of_paths)
                delete node->_path;
            delete node;
            node = next;
        }
    }
    _head = NULL;
    _tail = NULL;
}

void PathList::operator+=(UndirectedPath* path) {
    this->push(path); //Pushes the node onto the list.
}

void PathList::operator+=(PathList& list) {
    //Our other list's connections.
    PathNode* other = list._head;
    if(!other) 
        return;

    //Inserts the first node if necessary.
    if(!this->_head) {
        _head = new PathNode();
        if(!_head) 
            return;
        _head->_path = other->_path;
        _head->_next = NULL;
        _tail = _head;
        other = other->_next;
    }

    //Starts at the end.
    PathNode* current = this->_head;

    //Loop through all of our external nodes.
    while(other && current) {
        //Loop through our current till we find an index to place into.
        if(current) {
            PathNode* node;
            do {
                //Don't append any duplicate pathing.
                if(current->_path == other->_path) break;
                //Insert into the list if possible, otherwise search for an insert point.
                if(current->_path->cost() > other->_path->cost()) {
                    node = new PathNode();
                    if(!node) return;

                    //Set path and next node.
                    node->_path = other->_path;
                    node->_next = current;

                    //If our "current" node has a previous node, update its next to be the new node.
                    if(current->_prev) current->_prev->_next = node;
                    else _head = node;

                    //Set our new previous nodes to reflect the new relationship.
                    node->_prev = current->_prev;
                    current->_prev = node;
                    break; //Success, break out of inner loop to advance to next other node.
                }
                //Advance to next node.
                current = current->_next;
            } while(current);

            //We didn't find a place to insert into, break our loop and append remaining to end of list.
            if(!node) break;
        }

        //Move to next node.
        other = other->_next;
    }

    //If we hit this loop, it means we hit the end of our list. Simple append to tail any remaining other nodes.
    while(other) {
        PathNode* node = new PathNode();
        if(!node) return;
        node->_path = other->_path;
        node->_next = NULL;
        node->_prev = _tail;
        _tail->_next = node;
        _tail = node;

        other = other->_next;
    }
}

bool PathList::push(UndirectedPath* path) {
    if(!_head){ //If head doesn't exist then we need to begin our list.
        _head = new PathNode();
        if(!_head) return false;
        _head->_path = path;
        _head->_next = NULL;
        _head->_prev = NULL;
        _tail = _head;
    }
    else {
        //List exists, lets attempt to insert into the list at the end.
        if(_tail->_path->cost() <= path->cost()) { //We have a new tail to append to.
            PathNode* node = new PathNode();
            if(!node) return false;
            node->_path = path;
            node->_next = NULL;
            node->_prev = _tail;
            _tail->_next = node;
            _tail = node;
        }
        else {
            //We need insert into our list somewhere.
            PathNode* cur = _head;
            while(cur) {
                if(cur->_path->cost() >= path->cost()) {

                    PathNode* node = new PathNode();
                    if(!node) return false;
                    node->_path = path;

                    if(!cur->_prev){ //Prepend to the list.
                        node->_next = _head;
                        node->_prev = NULL;
                        _head = node;
                    }
                    else { //Insert into the list.
                        if(cur->_prev)
                            cur->_prev->_next = node;
                        node->_prev = cur;
                        node->_next = cur;
                        cur->_prev = node;
                    }
                    break;
                }
                cur = cur->_next;
            }
            if(!cur) return false; //Shouldn't ever reach this condition since the logic should either insert it or just ignore it.
        }
        return true;
    }
    //Done, we succeeded at it.
    return true;
}

UndirectedPath* PathList::pop() {
    if(!_head)
        return NULL;

    //Remove the front most node.
    PathNode* node = _head;
    _head = node->_next;
    if(node->_next) node->_next->_prev = NULL;
    if(_tail == node) _tail = NULL;

    //Get path and delete the node.
    UndirectedPath* path = node->_path;
    delete node;

    //Return the path.
    return path;
}

Computer::Computer(const char* name) {
    this->_name = name; //Sets the name.
}

bool Computer::connect(Computer& other, int cost) {
    //Keeps the same computer from connecting to itself.
    if(this == &other || this->name() == other.name()) return true;

    //Creates a new path.
    UndirectedPath* path = new UndirectedPath(*this, other, cost);
    if(!path) return false;

    //Pushes the path onto this paths and the other paths.
    if(!this->_paths.push(path)) return false;
    if(!other._paths.push(path)) return false;
    return true;
}

Computers::~Computers() {
    //Deconstruct the list.
    if(!_head) {
        ListNode* node = _head;
        ListNode* next = NULL;
        while(node) {
            next = node->_next;
            if(node->_object)
                delete node->_object;
            delete node;
            node = next;
        }
    }
    _head = NULL;
}

Computers& Computers::operator+=(Computer* computer) {
    //Append the computer onto the list.
    if(!_head) {
        _head = new ListNode();
        if(_head){
            _head->_object = computer;
            _head->_next = NULL;
        }
    }
    else{
        ListNode* node = new ListNode();
        if(node){
            node->_object = computer;
            node->_next = _head;
            _head = node;
        }
    }
    return *this;
}

Computers& Computers::operator-=(Computer* computer) {
    //Removes the computer from the list if it exists.
    if(_head && computer) {
        ListNode* prev = NULL;
        ListNode* node = _head;
        //Loop through every node to find the match.
        while(node) {
            if(node->_object) {
                if(node->_object->name() == computer->name()) { //Found a matching computer.
                    if(!prev) {
                        //Update head.
                        _head = _head->_next;
                    }
                    else {
                        prev->_next = node->_next;
                    }
                    //delete node->_object;
                    delete node;
                    break;
                }
            }
            //Update and move to the next node.
            prev = node;
            node = node->_next;
        }
    }
    return *this;
}

Computer* Computers::operator[](const std::string& name) {
    //Loop through the list till we find a match.
    ListNode* node = _head;
    while(node) {
        if(node->_object && node->_object->name() == name) //Found a match.
            return node->_object;
        node = node->_next;
    }
    DBG("Couldn't find " << name << " in our existing list.");
    return NULL;
}

std::ostream& operator<<(std::ostream& o, const PathList& list) {
    o << "Paths:" << std::endl;
    PathList::PathNode* node = list._head;
    while(node) {
        if(node->_path) {
            o << "\t";
            o << node->_path->b().name() << ", ";
            o << node->_path->a().name();
            o << " ( Cost " << node->_path->cost() << ")" << std::endl;
        }
        else{
            o << "\t NULL PATH?!" << std::endl;
        }
        node = node->_next;
    }
    return o;
}

std::ostream& operator<<(std::ostream& o, const Computer& c) {
    o << c.name() << std::endl << c._paths << std::endl;
    return o;
}

std::ostream& operator<<(std::ostream& o, const Computers& c) {
    Computers::ListNode* node = c._head;
    while(node) {
        if(node->_object)
            o << *node->_object << std::endl;
        node = node->_next;
    }

    return o;
}

int solve(Computers& computers) {

    int minimum = 0;

    //Get the head of our list and remove it from the list.
    Computer* start = computers.head();

    //Append all of our possible pathing options.
    PathList avail(false);
    avail += start->paths();

    //Keep track of the already searched computers.
    std::list<std::string> searched;
    searched.push_back(start->name());

    //While we have a path to search through, search through it.
    while(!avail.isEmpty()){
        //Remove the shortest path from the list.
        UndirectedPath* path = avail.pop();
        if(path){
            //Checks if either a or b is in the list.
            bool a_find = std::find(searched.begin(), searched.end(), path->a().name()) != searched.end();
            bool b_find = std::find(searched.begin(), searched.end(), path->b().name()) != searched.end();
            if((a_find || b_find) && !(a_find && b_find)){ //Found a path that we can follow.
                DBG("Following path: " << path->a().name() << ", " << path->b().name());
                minimum += path->cost(); //Add the cost to the minimum path.
                if(!a_find) { searched.push_back(path->a().name()); avail += path->a().paths(); }
                if(!b_find) { searched.push_back(path->b().name()); avail += path->b().paths(); }
            }
        }
    }

    //Return the minimum possible cost.
    return minimum;
}