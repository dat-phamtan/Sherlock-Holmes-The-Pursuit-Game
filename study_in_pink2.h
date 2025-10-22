/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 1
 * Programming Fundamentals Spring 2023
 * Author: Vu Van Tien
 * Date: 02.02.2023
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"
////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class Position;
class Configuration;
class Map;

class Criminal;
class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;
class Character;
class Watson;
class Sherlock;

class TestStudyInPink;

enum ItemType
{
    MAGIC_BOOK,
    ENERGY_DRINK,
    FIRST_AID,
    EXCEMPTION_CARD,
    PASSING_CARD,
    NONE
};
enum ElementType
{
    PATH,
    WALL,
    FAKE_WALL
};
enum RobotType
{
    C = 0,
    S,
    W,
    SW
};
enum MovingObjectType
{
    SHERLOCK,
    WATSON,
    CRIMINAL,
    ROBOT
};

// extern deque<string> logString;

// void insertLogString(string aLog){
//     if(aLog == "") return;
//     if(logString.size() > 15){
//         logString.pop_front();
//     }
//     logString.push_back(aLog);
// }
class MapElement
{
    friend class TestStudyInPink;

protected:
    ElementType type;

public:
    MapElement(ElementType in_type);
    virtual ~MapElement();
    virtual ElementType getType() const;
};

class Path : public MapElement
{
    friend class TestStudyInPink;

public:
    Path();
};

class Wall : public MapElement
{
    friend class TestStudyInPink;

public:
    Wall();
};

class FakeWall : public MapElement
{
    friend class TestStudyInPink;

private:
    int req_exp;

public:
    FakeWall(int in_req_exp);
    int getReqExp() const;
};

class Map
{
    friend class TestStudyInPink;

private:
    int num_rows, num_cols;
    MapElement ***map;

public:
    int getNumRows() const;
    int getNumCols() const;
    ElementType getElementType(int i, int j) const;
    Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls);
    ~Map();
    bool isValid(const Position &pos, MovingObject *mv_obj) const;
};

class Position
{
    friend class TestStudyInPink;

private:
    int r, c;

public:
    static const Position npos;
    Position(int r = 0, int c = 0);
    Position(const string &str_pos);
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const;
    bool isEqual(int in_r, int in_c) const;
    bool isEqual(Position pos) const;
};

class MovingObject
{
    friend class TestStudyInPink;

protected:
    int index;
    Position pos;
    Map *map;
    string name;
    // deque<string> logString;

public:
    MovingObject(int index, const Position pos, Map *map, const string &name = "");
    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual MovingObjectType getObjectType() const = 0;
    // void insertLogString(string aLog){
    //     if(aLog == "") return;
    //     if(logString.size() > 15){
    //         logString.pop_front();
    //     }
    //     logString.push_back(aLog);
    // }
};

class Character : public MovingObject
{
    friend class TestStudyInPink;

public:
    Character(int index, const Position pos, Map *map, const string &name = "");
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual MovingObjectType getObjectType() const = 0;
};

class Sherlock : public Character
{
    friend class TestStudyInPink;

private:
    int hp, exp;
    string submoving_rule;
    string moving_rule;
    int index_moving_rule;
    BaseBag *bag;

public:
    Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    Position getNextPosition();
    void move();
    string str() const;
    MovingObjectType getObjectType() const;
    BaseBag *getBag() const;
    ~Sherlock();
    bool meet(RobotS *robotS);
    bool meet(RobotW *robotW);
    bool meet(RobotSW *robotSW);
    bool meet(RobotC *robotC);
    bool meet(Watson *watson);
    int getHP() const;
    int getEXP() const;
    void setHP(int hp);
    void setEXP(int exp);
    void setPos(Position pos);
    
};

class Watson : public Character
{
    friend class TestStudyInPink;

private:
    int hp, exp;
    string moving_rule;
    string submoving_rule;
    int index_moving_rule;
    BaseBag *bag;

public:
    Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    Position getNextPosition();
    void move();
    string str() const;
    MovingObjectType getObjectType() const;
    BaseBag *getBag() const;
    ~Watson();
    bool meet(RobotS *robotS);
    bool meet(RobotW *robotW);
    bool meet(RobotSW *robotSW);
    bool meet(RobotC *robotC);
    bool meet(Sherlock *sherlock);
    int getHP() const;
    int getEXP() const;
    void setHP(int hp);
    void setEXP(int exp);
    void setPos(Position pos);
    
};

class Criminal : public Character
{
    friend class TestStudyInPink;

private:
    Sherlock *sherlock;
    Watson *watson;
    int count;
    Position prev_pos;

public:
    Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson);
    Position getNextPosition();
    void move();
    string str() const;
    int manhattanDistance(const Position &pos1, const Position &pos2) const;
    MovingObjectType getObjectType() const;
    int getCount() const;
    bool createRobotC();
    Position getPrevPosition() const;
};

class ArrayMovingObject
{
    friend class TestStudyInPink;

private:
    MovingObject **arr_mv_objs;
    int count;
    int capacity;
    // deque<string> logString;

public:
    ArrayMovingObject(int capacity);
    ~ArrayMovingObject();
    bool isFull() const;
    bool add(MovingObject *mv_obj);
    string str() const;
    bool checkMeet(int index) const;
    MovingObject *get(int index) const;
    int size() const;
    // void insertLogString(string aLog);
    // deque<string> getLogString(){
    //     return this->logString;
    // }
};

class Configuration
{
    friend class TestStudyInPink;
    friend class StudyPinkProgram;

private:
    int map_num_rows;
    int map_num_cols;
    int max_num_moving_objects;
    int num_walls;
    Position *arr_walls;
    int num_fake_walls;
    Position *arr_fake_walls;
    string sherlock_moving_rule;
    Position sherlock_init_pos;
    int sherlock_init_hp;
    int sherlock_init_exp;
    string watson_moving_rule;
    Position watson_init_pos;
    int watson_init_hp;
    int watson_init_exp;
    Position criminal_init_pos;
    int num_steps;

public:
    Configuration(const string &filepath);
    ~Configuration();
    string str() const;
};

// Robot, BaseItem, BaseBag,...
class Robot : public MovingObject
{
    friend class TestStudyInPink;

protected:
    RobotType robot_type;
    BaseItem *item;
    Criminal *criminal;
    BaseItem *itemhold;
    int i, j;

public:
    Robot(int index,
          const Position &pos,
          Map *map,
          Criminal *criminal,
          const string &name = "");
    MovingObjectType getObjectType() const;
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual RobotType getType();
    static Robot *create(int index, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson);
    virtual int getDistance() const;
    BaseItem *NewItem();
    ~Robot();
};

class RobotC : public Robot
{
    friend class TestStudyInPink;

private:
public:
    RobotC(int index,
           const Position &init_pos,
           Map *map,
           Criminal *criminal);
    Position getNextPosition();
    void move();
    int getDistance(Sherlock *sherlock);
    int getDistance(Watson *watson);
    string str() const;
    RobotType getType() const;
};

class RobotS : public Robot
{
    friend class TestStudyInPink;

private:
    Sherlock *sherlock;

public:
    RobotS(int index,
           const Position &init_pos,
           Map *map,
           Criminal *criminal,
           Sherlock *sherlock);

    Position getNextPosition();
    void move();
    int getDistance() const;
    string str() const;
    RobotType getType() const;
};

class RobotW : public Robot
{
    friend class TestStudyInPink;

private:
    Watson *watson;

public:
    RobotW(int index,
           const Position &init_pos,
           Map *map,
           Criminal *criminal,
           Watson *watson);
    Position getNextPosition();
    void move();
    int getDistance() const;
    string str() const;
    RobotType getType() const;
};

class RobotSW : public Robot
{
    friend class TestStudyInPink;

private:
    Sherlock *sherlock;
    Watson *watson;

public:
    RobotSW(int index,
            const Position &init_pos,
            Map *map,
            Criminal *criminal,
            Sherlock *sherlock,
            Watson *watson);

    Position getNextPosition();
    string str() const;
    int getDistance() const;
    void move();
    RobotType getType() const;
};

class BaseItem
{
    friend class TestStudyInPink;

public:
    virtual bool canUse(Character *obj, Robot *robot) = 0;
    virtual void use(Character *obj, Robot *robot) = 0;
    BaseItem();
    ~BaseItem();
    virtual ItemType getType() const = 0;
    virtual string str() const = 0;
    
};

class MagicBook : public BaseItem
{
    friend class TestStudyInPink;

public:
    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
    ItemType getType() const;
    string str() const;
};

class EnergyDrink : public BaseItem
{
    friend class TestStudyInPink;

public:
    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
    ItemType getType() const;
    string str() const;
};

class FirstAid : public BaseItem
{
    friend class TestStudyInPink;

public:
    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
    ItemType getType() const;
    string str() const;
};

class ExcemptionCard : public BaseItem
{
    friend class TestStudyInPink;

public:
    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
    ItemType getType() const;
    string str() const;
};

class PassingCard : public BaseItem
{
    friend class TestStudyInPink;

private:
    string challenge;

public:
    PassingCard(int i, int j);
    PassingCard(string challenge);
    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
    ItemType getType() const;
    string str() const;
};

class BaseBag
{
    friend class TestStudyInPink;

protected:
    class Node
    {
    public:
        BaseItem *item;
        Node *next;

    public:
        Node(BaseItem *item, Node *next = nullptr) : item(item), next(next) {}
    };

protected:
    int size;
    int capacity;
    Node *head;
    Character *obj;

public:
    virtual bool insert(BaseItem *item);
    virtual BaseItem *get();
    virtual BaseItem *get(ItemType itemType);
    virtual string str() const;
    BaseBag(int capacity);
    virtual ~BaseBag();
    bool checkItem(ItemType itemType);
    int getSize() const
    {
        return size;
    }
    ItemType getItemType(int pos) const
    {
        if (pos < 0 || pos >= size)
        {
            return ItemType::NONE;
        }
        int check = 0;
        Node *tim = head;
        while (tim != nullptr)
        {
            if (check == pos)
            {
                return tim->item->getType();
            }
            check++;
            tim = tim->next;
        }
        return ItemType::NONE;
    }
};
class SherlockBag : public BaseBag
{
    friend class TestStudyInPink;

private:
    Sherlock *sherlock;

public:
    SherlockBag(Sherlock *character);
    BaseItem *get();
};
class WatsonBag : public BaseBag
{
    friend class TestStudyInPink;

private:
    Watson *watson;

public:
    WatsonBag(Watson *character);
    BaseItem *get();
};

class StudyPinkProgram
{
    friend class TestStudyInPink;

public:
    Criminal *criminal;
    Sherlock *sherlock;
    Watson *watson;
    Configuration *config;
    Map *map;
    ArrayMovingObject *arr_mv_objs;
    bool var;
    // Logger *logger;
    // vector<string> logString;

    StudyPinkProgram(){};
    StudyPinkProgram(const string &config_file_path);
    bool isStop() const;
    void printResult() const
    {
        // if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
        // {
        //     cout << "Sherlock caught the criminal" << endl;
        // }
        // else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
        // {
        //     cout << "Watson caught the criminal" << endl;
        // }
        // else
        // {
        //     cout << "The criminal escaped" << endl;
        // }
    }

    void printStep(int si) const
    {
        // cout << "Step: " << setw(4) << setfill('0') << si
        //      << "--"
        //      << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }
    void run(bool verbose);
    ~StudyPinkProgram();
    // void insertLogString();
    // vector<string> getLogString(){
    //     return this->logString;
    // };
    
};

class Logger {
private:
    std::deque<std::string> logs;
    Logger() {} // constructor private để không thể tạo instance khác

public:
    static Logger& instance() {
        static Logger instance; // duy nhất trong toàn chương trình
        return instance;
    }

    void add(const std::string& message) {
        if (message == "") return;
        if (logs.size() > 10)
            logs.pop_front();
        logs.push_back(message);
    }

    string get(int i) const {
        return logs.at(i);
    }

    int size(){return (int)logs.size();}

    void clear() { logs.clear(); }
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
