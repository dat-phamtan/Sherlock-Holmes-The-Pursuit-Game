#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// deque<string> logString;

int randomInt(int min, int max) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

const Position Position::npos = Position(-1, -1);
/*
 * CLASS: MapElement
 */
MapElement::MapElement(ElementType in_type)
{
    this -> type = in_type;
}
MapElement::~MapElement() {}
ElementType MapElement::getType() const
{
    return type;
}
/*
 * CLASS: Path
 */
Path::Path()
    : MapElement(PATH) {}
/*
 * CLASS: Wall
 */
Wall::Wall()
    : MapElement(WALL) {}
/*
 * CLASS: FakeWall
 */
FakeWall::FakeWall(int in_req_exp)
    : MapElement(ElementType::FAKE_WALL)
{
    this -> req_exp = in_req_exp;
}
int FakeWall::getReqExp() const
{
    return req_exp;
}
/*
 * CLASS: Position
 */
Position::Position(int r, int c)
{
    this -> r = r;
    this -> c = c;
}
Position::Position(const string &str_pos)
{
    string xpos = str_pos.substr(str_pos.find("(") + 1, str_pos.find(",") - str_pos.find("(") - 1 );
    stringstream xpos2(xpos);
    xpos2 >> this -> r;
    string ypos = str_pos.substr(str_pos.find(",") + 1, str_pos.find(")") - str_pos.find(",") - 1);
    stringstream ypos2(ypos);
    ypos2 >> this -> c;
}
int Position::getRow() const
{
    return r;
}
int Position::getCol() const
{
    return c;
}
void Position::setRow(int r)
{
    this -> r = r;
}
void Position::setCol(int c)
{
    this -> c = c;
}
string Position::str() const
{
    string str = "(" + to_string(r) + "," + to_string(c) + ")";
    return str;
}
bool Position::isEqual(Position position) const
{
    bool equal = ((this -> r == position.getRow()) and (this -> c == position.getCol()));
    return equal;
}
bool Position::isEqual(int in_r, int in_c) const{
    return (in_r == this->getRow() and in_c == this->getCol());
}
/*
 * CLASS: MovingObject
 */
MovingObject::MovingObject(int index, const Position pos, Map *map, const string &name)
{
    this -> index = index;
    this -> pos = pos;
    this -> map = map;
    this -> name = name;
}
MovingObject::~MovingObject(){}
Position MovingObject::getCurrentPosition() const
{
    return pos;
}
/*
 * CLASS: Character kế thừa class MovingObject
 */
Character::Character(int index, const Position pos, Map *map, const string &name)
    : MovingObject(
        index, pos, map, name
      )
{
    this -> index = index;
    this -> pos = pos;
    this -> map = map;
    this -> name = name;
}
/*
 * CLASS: Sherlock kế thừa class Character
 */
Sherlock::Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp)
    : Character(
          index, pos, map, name
      )
{
    this -> index = index;
    this -> map = map;
    this -> name = "Sherlock";
    this -> moving_rule = moving_rule;
    this -> pos = init_pos;
    this -> hp = init_hp;
    this -> exp = init_exp;
    this -> submoving_rule = moving_rule;
    this -> index_moving_rule = 0;
    bag = new SherlockBag(this);
}
Sherlock::~Sherlock(){
    delete bag;
}
Position Sherlock::getNextPosition()
{
    if (submoving_rule.find("U") == 0){
        Position upos = Position(pos.getRow() - 1 , pos.getCol());
        if(map->isValid(upos, this))
            return upos;
        else
            return Position::npos;
    }
    else if (submoving_rule.find("L") == 0){
        Position upos = Position(pos.getRow() , pos.getCol() - 1);
        if(map->isValid(upos, this))
            return upos;
        else
            return Position::npos;
    }
    else if (submoving_rule.find("D") == 0){
        Position upos = Position(pos.getRow() + 1 , pos.getCol());
        if(map->isValid(upos, this))
            return upos;
        else
            return Position::npos;
    }
    else if (submoving_rule.find("R") == 0){
        Position upos = Position(pos.getRow() , pos.getCol() + 1);
        if(map->isValid(upos, this))
            return upos;
        else
            return Position::npos;
    }
    return Position::npos;
}
void Sherlock::move()
{
    if(this->getEXP() != 0){
        if(Position::npos.isEqual(this->getNextPosition())){
            this->getNextPosition();
        }
        else{
            pos = this->getNextPosition();
            this->getNextPosition();
        }
    }
    submoving_rule = submoving_rule + submoving_rule[0];
    submoving_rule.erase(submoving_rule.begin());
}
string Sherlock::str() const
{
    string Sher ="Sherlock[index="+ to_string(this->index) +";pos=" + this->pos.str() + ";moving_rule=" + this->moving_rule + "]";
    return Sher;
}
MovingObjectType Sherlock::getObjectType() const
{
    return MovingObjectType::SHERLOCK;
}
int Sherlock::getHP() const
{
    if (hp >= 0 and hp <= 500)
        return hp;
    else if (hp > 500){
        hp == 500;
        return hp;
    }
    else{
        hp == 0;
        return hp;
    }
}
int Sherlock::getEXP() const
{
    if (exp >= 0 and exp <= 3000)
        return exp;
    else if (exp > 3000){
        exp == 3000;
        return exp;
    }
    else{
        exp == 0;
        return exp;
    }
}
void Sherlock::setHP(int hp)
{
    if (hp >= 0 and hp <= 500)
        this -> hp = hp;
    else if (hp > 500){
        hp = 500;
        this -> hp = hp;
    }
    else{
        hp = 0;
        this -> hp = hp;
    }
}
void Sherlock::setEXP(int exp)
{
    if (exp >= 0 and exp <= 3000)
        this -> exp = exp;
    else if (exp > 3000){
        exp = 3000;
        this -> exp = exp;
    }
    else{
        exp = 0;
        this -> exp = exp;
    }
}
BaseBag *Sherlock::getBag() const{
    return bag;
}

/*
 * CLASS: Watson kế thừa class Character
 */
Watson::Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp)
    : Character(
        index, pos, map, name
      )
{
    this -> index = index;
    this -> map = map;
    this -> name = "Watson";
    this -> moving_rule = moving_rule;
    this -> pos = init_pos;
    this -> hp = init_hp;
    this -> exp = init_exp;
    this -> submoving_rule = moving_rule;
    this -> index_moving_rule = 0;
    bag = new WatsonBag(this);
    this -> prev_pos = Position::npos;
}
Watson::~Watson(){
    delete bag;
}
Position Watson::getNextPosition()
{
    std::vector<Position> candidates;

    Position up(pos.getRow() - 1, pos.getCol());
    Position left(pos.getRow(), pos.getCol() - 1);
    Position down(pos.getRow() + 1, pos.getCol());
    Position right(pos.getRow(), pos.getCol() + 1);

    // Kiểm tra từng hướng hợp lệ
    if (map->isValid(up, this)) candidates.push_back(up);
    if (map->isValid(left, this)) candidates.push_back(left);
    if (map->isValid(down, this)) candidates.push_back(down);
    if (map->isValid(right, this)) candidates.push_back(right);

    if (candidates.empty()) {
        return Position::npos; // Không có hướng hợp lệ
    }

    int ranNum = randomInt(1, candidates.size());
    return candidates[ranNum - 1];

    // if(ranNum == 1){
    //     Position upos = Position(pos.getRow() - 1 , pos.getCol());
    //     if(map->isValid(upos, this)){
    //         return upos;
    //     }
    //     else{
    //         return this->getNextPosition();
    //     }
    // }
    // else if(ranNum == 2){
    //     Position upos = Position(pos.getRow() , pos.getCol() - 1);
    //     if(map->isValid(upos, this)){
    //         return upos;
    //     }
    //     else{
    //         return this->getNextPosition();
    //     }
    // }
    // else if(ranNum == 3){
    //     Position upos = Position(pos.getRow() + 1 , pos.getCol());
    //     if(map->isValid(upos, this)){
    //         return upos;
    //     }
    //     else{
    //         return this->getNextPosition();
    //     }
    // }
    // else if(ranNum == 4){
    //     Position upos = Position(pos.getRow() , pos.getCol() + 1);
    //     if(map->isValid(upos, this)){
    //         return upos;
    //     }
    //     else{
    //         return this->getNextPosition();
    //     }
    // }

    // if (submoving_rule.find("U") == 0){
    //     Position upos = Position(pos.getRow() - 1 , pos.getCol());
    //     if(map->isValid(upos, this))
    //         return upos;
    //     else
    //         return Position::npos;
    // }
    // else if (submoving_rule.find("L") == 0){
    //     Position upos = Position(pos.getRow() , pos.getCol() - 1);
    //     if(map->isValid(upos, this))
    //         return upos;
    //     else
    //         return Position::npos;
    // }
    // else if (submoving_rule.find("D") == 0){
    //     Position upos = Position(pos.getRow() + 1 , pos.getCol());
    //     if(map->isValid(upos, this))
    //         return upos;
    //     else
    //         return Position::npos;
    // }
    // else if (submoving_rule.find("R") == 0){
    //     Position upos = Position(pos.getRow() , pos.getCol() + 1);
    //     if(map->isValid(upos, this))
    //         return upos;
    //     else
    //         return Position::npos;
    // }
    // return Position::npos;
}
void Watson::move()
{
    if (this->getEXP() == 0) return;

    Position next = this->getNextPosition();
    if (!Position::npos.isEqual(next)) {
        pos = next;  // luôn di chuyển 1 bước hợp lệ
    }
}

string Watson::str() const
{
    string Wat ="Watson[index="+ to_string(this->index) +";pos=" + this->pos.str() + ";moving_rule=" + this->moving_rule + "]";
    return Wat;
}
MovingObjectType Watson::getObjectType() const
{
    return MovingObjectType::WATSON;
}
int Watson::getHP() const
{
    if (hp >= 0 and hp <= 700)
        return hp;
    else if (hp > 700){
        hp == 700;
        return hp;
    }
    else{
        hp == 0;
        return hp;
    }
}
int Watson::getEXP() const
{
    if (exp >= 0 and exp <= 2500)
        return exp;
    else if (exp > 2500){
        exp == 2500;
        return exp;
    }
    else{
        exp == 0;
        return exp;
    }
}
void Watson::setHP(int hp)
{
    if (hp >= 0 and hp <= 700)
        this -> hp = hp;
    else if (hp > 700){
        hp = 700;
        this -> hp = hp;
    }
    else{
        hp = 0;
        this -> hp = hp;
    }
}
void Watson::setEXP(int exp)
{
    if (exp >= 0 and exp <= 2500)
        this -> exp = exp;
    else if (exp > 2500){
        exp = 2500;
        this -> exp = exp;
    }
    else{
        exp = 0;
        this -> exp = exp;
    }
}
BaseBag *Watson::getBag() const{
    return bag;
}

/*
 * CLASS: Map
 */
Map::Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls)
{
    this -> num_cols = num_cols;
    this -> num_rows = num_rows;
    if(num_cols == 0 or num_rows == 0)  {
        return;
    }
    else{
        map = new MapElement**[num_rows];
        for (int i = 0; i < num_rows; i++){
            map[i] = new MapElement*[num_cols];
        }
        for (int i = 0; i < num_rows; i++){
            for (int j = 0; j < num_cols; j++){
                map[i][j] = new Path();
            }
        }
        for (int i = 0; i < num_fake_walls; i++){
            if((0 <= array_fake_walls[i].getRow()) and (array_fake_walls[i].getRow() < num_rows) and (0 <= array_fake_walls[i].getCol()) and (array_fake_walls[i].getCol()< num_cols)){
                int x = array_fake_walls[i].getRow();
                int y = array_fake_walls[i].getCol();
                delete map[x][y];
                map[x][y] = new FakeWall((x*257 + y*139 + 89)%1000 + 1);
            }
        }
        for (int i = 0; i < num_walls; i++){
            if (((0 <= array_walls[i].getRow()) and (array_walls[i].getRow() < num_rows)) and ((0 <= array_walls[i].getCol()) and (array_walls[i].getCol()< num_cols))){
                int x = array_walls[i].getRow();
                int y = array_walls[i].getCol();
                delete map[x][y];
                map[x][y] = new Wall();
            }
        }
    }
}
Map::~Map()
{
    if(num_cols == 0 or num_rows == 0) {
        return;
    }
    else{
    for (int i = 0; i < num_rows; i++){
        for (int j = 0; j < num_cols; j++){
            delete map[i][j];
            }
        }
    for (int i = 0; i < num_rows; i++){
        delete []map[i];
    }
    delete []map;
    }
}
int Map::getNumRows() const
{
    return num_rows;
}
int Map::getNumCols() const
{
    return num_cols;
}
ElementType Map::getElementType(int i, int j) const
{
    return map[i][j]->getType();
}
bool Map::isValid(const Position &pos, MovingObject *mv_obj) const
{
    int i = pos.getRow();
    int j = pos.getCol();
    if((i >= 0) and (i < num_rows) and (j >= 0) and (j < num_cols)){
        if (mv_obj->getObjectType() == WATSON){
            if(map[i][j]->getType() == 0)
                return true;
            else if(map[i][j]->getType() == 2){
                Watson *mv_obj2 = dynamic_cast<Watson*>(mv_obj);
                if (((i*257 + j*139 + 89)%1000 + 1) >= mv_obj2->getEXP() )
                    return false;
                else
                    return true;
            }
        }
        else if (mv_obj->getObjectType() == SHERLOCK or mv_obj->getObjectType() == CRIMINAL or mv_obj->getObjectType() == ROBOT){
            if (map[i][j]->getType() == 0 or map[i][j]->getType() == 2) 
                return true;
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
return false;
}
/*
 * CLASS: Criminal kế thừa class Character
 */
Criminal::Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson)
    : Character(
        index, pos, map, name 
    )
{
    this -> index = index;
    this -> pos = init_pos;
    this -> map = map;
    this -> name = "Criminal";
    this -> count = 0;
    this -> sherlock = sherlock;
    this -> watson = watson;
    this -> prev_pos = Position::npos;
}
Position Criminal::getNextPosition()
{
    int arrdis[4]={};
    int arrdisb[4]={};
    int out = -1;
    int dem = 0;
    Position upos = Position(pos.getRow() - 1, pos.getCol());
    arrdis[0] = manhattanDistance(upos, sherlock->getCurrentPosition()) + manhattanDistance(upos, watson->getCurrentPosition());
    Position dpos = Position(pos.getRow() + 1, pos.getCol());
    arrdis[1] = manhattanDistance(dpos, sherlock->getCurrentPosition()) + manhattanDistance(dpos, watson->getCurrentPosition());
    Position lpos = Position(pos.getRow(), pos.getCol() - 1);
    arrdis[2] = manhattanDistance(lpos, sherlock->getCurrentPosition()) + manhattanDistance(lpos, watson->getCurrentPosition());
    Position rpos = Position(pos.getRow(), pos.getCol() + 1);
    arrdis[3] = manhattanDistance(rpos, sherlock->getCurrentPosition()) + manhattanDistance(rpos, watson->getCurrentPosition());
    for (int i =0; i < 4; i++){
        arrdisb[i] = arrdis[i];
    }
    for (int i = 0; i < 3; i++){
        for (int j = i+1; j < 4; j++){
            if (arrdisb[i] < arrdisb[j]){
                int temp = arrdisb[i];
                arrdisb[i] = arrdisb[j];
                arrdisb[j] = temp;
            }
        }
    }
    while((out == -1) and (dem < 4)){
        int max = arrdisb[dem];
        if (map->isValid(upos, this) and (max == arrdis[0])){out = 0;}
        else if (map->isValid(lpos, this) and (max == arrdis[2])){out = 1;}
        else if (map->isValid(dpos, this) and (max == arrdis[1])){out = 2;}
        else if (map->isValid(rpos, this) and (max == arrdis[3])){out = 3;}
        dem++;
    }
    if (out == 0){return upos;}
    else if (out == 1){return lpos;}
    else if (out == 2){return dpos;}
    else if (out == 3){return rpos;}
    else if (out == -1){return Position::npos;}
    return Position::npos;
}
void Criminal::move()
{
    if(Position::npos.isEqual(this->getNextPosition())){
        this->prev_pos = pos;
        this->getNextPosition();
    }
    else{
        this->prev_pos = pos;
        pos = this->getNextPosition();
        this->getNextPosition();
        count++;
    }
    
}
Position Criminal::getPrevPosition()const{
    return prev_pos;
}
int Criminal::manhattanDistance(const Position &pos1, const Position &pos2) const
{
    return (abs(pos1.getRow() - pos2.getRow()) + abs(pos1.getCol() - pos2.getCol()));
}
string Criminal::str() const
{
    string Cri ="Criminal[index="+ to_string(this->index) +";pos=" + this->pos.str() + "]";
    return Cri;
}
bool Criminal::createRobotC(){
    if (count == 3){
        return true;
    }
    else{
        return false;
    }
}

MovingObjectType Criminal::getObjectType() const
{
    return MovingObjectType::CRIMINAL;
}
int Criminal::getCount() const
{
    return count;
}
/*
 * CLASS: Robot kế thừa class MovingObject
 */
Robot::Robot(int index, const Position &pos, Map *map, Criminal *criminal, const string &name)
    : MovingObject(
        index, pos, map, name 
      )
{
    this -> index = index;
    this -> pos = pos;
    this -> map = map;
    this -> name = name;
    this -> criminal = criminal;
    if(criminal != nullptr){
        i = criminal->getPrevPosition().getRow();
        j = criminal->getPrevPosition().getCol();
    }

    int p = i*j;
    int s = -1;
    if(p >= 0){
        s = p;
        while(s >= 10){
            int n = 0;
            while(s > 0){
                n += s % 10;
                s /= 10;
            }
            s = n;
        }
    }
    if (s == 0 or s == 1){
        itemhold = new MagicBook;
    }
    if (s == 2 or s == 3){
        itemhold = new EnergyDrink;
    }
    if (s == 4 or s == 5){
        itemhold = new FirstAid;
    }
    if (s == 6 or s == 7){
        itemhold = new ExcemptionCard;
    }
    if (s == 8 or s == 9){
        itemhold = new PassingCard(i, j);
    }
    
}
Robot::~Robot(){
    delete itemhold;
}
RobotType Robot::getType(){
    return robot_type;
}
int Robot::getDistance()const{
    return 0;
};
Robot *Robot::create(int index, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson)
{
    if (criminal->createRobotC()){
        return new RobotC(index, criminal->getPrevPosition(), map, criminal);
    }
    else{
        return new RobotS(index, criminal->getPrevPosition(), map, criminal, sherlock);
        int randomNum = randomInt(0, 2);
        if (randomNum == 0){
            return new RobotS(index, criminal->getPrevPosition(), map, criminal, sherlock);
        }
        else if (randomNum == 1){
            return new RobotW(index, criminal->getPrevPosition(), map, criminal, watson);
        }
        else if (randomNum == 2){
            return new RobotSW(index, criminal->getPrevPosition(), map, criminal, sherlock, watson);
        }
        // return new RobotSW(index, criminal->getPrevPosition(), map, criminal, sherlock, watson);
        // int sherdis = criminal->manhattanDistance(sherlock->getCurrentPosition(), criminal->getPrevPosition());
        // int watdis = criminal->manhattanDistance(watson->getCurrentPosition(),criminal->getPrevPosition());
        // if(sherdis < watdis){
        //     return new RobotS(index, criminal->getPrevPosition(), map, criminal, sherlock);
        // }
        // else if (sherdis > watdis){
        //     return new RobotW(index, criminal->getPrevPosition(), map, criminal, watson);
        // }
        // else if (sherdis == watdis){
        //     return new RobotSW(index, criminal->getPrevPosition(), map, criminal, sherlock, watson);
        // }
    }
    return nullptr;
}
MovingObjectType Robot::getObjectType() const
{
    return MovingObjectType::ROBOT;
}
/*
 *CLASS: RobotC kế thừa class Robot
 */
RobotC::RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal)
    : Robot(
        index, pos, map, criminal
      )
{
    
    this -> pos = init_pos;
    this->robot_type = C;
    i = init_pos.getRow();
    j = init_pos.getCol();
    int p = i*j;
    int s = -1;
    if(p >= 0){
        s = p;
        while(s >= 10){
            int n = 0;
            while(s > 0){
                n += s % 10;
                s /= 10;
            }
            s = n;
        }
    }
    if (s == 0 or s == 1){
        item = new MagicBook;
    }
    if (s == 2 or s == 3){
        item = new EnergyDrink;
    }
    if (s == 4 or s == 5){
        item = new FirstAid;
    }
    if (s == 6 or s == 7){
        item = new ExcemptionCard;
    }
    if (s == 8 or s == 9){
        item = new PassingCard(i, j);
    }
}
int RobotC::getDistance(Sherlock *sherlock)
{
    return criminal->manhattanDistance(pos, sherlock->getCurrentPosition());
}
int RobotC::getDistance(Watson *watson)
{
    return criminal->manhattanDistance(pos, watson->getCurrentPosition());
}
Position RobotC::getNextPosition()
{
    return criminal->getPrevPosition();
}
void RobotC::move()
{
    if(map->isValid(this->getNextPosition(), this)){
        pos = this -> getNextPosition();
    }
    else{}
}
string RobotC::str() const
{
    string RBC ="Robot[pos="+ pos.str() + ";type=C;dist="  + "]";
    return RBC;
}
RobotType RobotC::getType() const
{
    return C;
}
/*
 * CLASS: RobotW kế thừa class Robot
 */
RobotW::RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson *watson)
    : Robot(
          index, pos, map , criminal
      )
{
    this -> index = 0;
    this -> pos = init_pos;
    this -> watson = watson;
    this->robot_type = W;
    i = init_pos.getRow();
    j = init_pos.getCol();
    int p = i*j;
    int s = -1;
    if(p >= 0){
        s = p;
        while(s >= 10){
            int n = 0;
            while(s > 0){
                n += s % 10;
                s /= 10;
            }
            s = n;
        }
    }
    if (s == 0 or s == 1){
        item = new MagicBook;
    }
    if (s == 2 or s == 3){
        item = new EnergyDrink;
    }
    if (s == 4 or s == 5){
        item = new FirstAid;
    }
    if (s == 6 or s == 7){
        item = new ExcemptionCard;
    }
    if (s == 8 or s == 9){
        item = new PassingCard(i, j);
    }
}

Position RobotW::getNextPosition()
{
    int arrdis[4]={};
    int arrdisb[4]={};
    int out = -1;
    int dem = 0;
    Position upos = Position(pos.getRow() - 1, pos.getCol());
    arrdis[0] = criminal->manhattanDistance(upos, watson->getCurrentPosition());
    Position dpos = Position(pos.getRow() + 1, pos.getCol());
    arrdis[1] = criminal->manhattanDistance(dpos, watson->getCurrentPosition());
    Position lpos = Position(pos.getRow(), pos.getCol() - 1);
    arrdis[2] = criminal->manhattanDistance(lpos, watson->getCurrentPosition());
    Position rpos = Position(pos.getRow(), pos.getCol() + 1);
    arrdis[3] = criminal->manhattanDistance(rpos, watson->getCurrentPosition());
    for (int i =0; i < 4; i++){
        arrdisb[i] = arrdis[i];
        
    }
    for (int i = 0; i < 3; i++){
        for (int j = i+1; j < 4; j++){
            if (arrdisb[i] > arrdisb[j]){
                int temp = arrdisb[i];
                arrdisb[i] = arrdisb[j];
                arrdisb[j] = temp;
            }
        }
    }
        while((out == -1) and (dem < 4)){
            int max = arrdisb[dem];
            
            if (map->isValid(upos, this) and (max == arrdis[0])){out = 0;}
            else if (map->isValid(rpos, this) and (max == arrdis[3])){out = 3;}
            else if (map->isValid(dpos, this) and (max == arrdis[1])){out = 2;}
            else if (map->isValid(lpos, this) and (max == arrdis[2])){out = 1;}
            dem++;
        }
    if (out == 0){return upos;}
    else if (out == 1){return lpos;}
    else if (out == 2){return dpos;}
    else if (out == 3){return rpos;}
    else if (out == -1){return Position::npos;}
    return Position::npos;
}
void RobotW::move()
{
    if(Position::npos.isEqual(this->getNextPosition())){
        this->getNextPosition();
    }
    else{
        pos = this->getNextPosition();
        this->getNextPosition();
    }
}
string RobotW::str() const
{
    string RBW ="Robot[pos="+ pos.str() + ";type=W;dist=" + to_string(this->getDistance()) + "]";
    return RBW;
}
RobotType RobotW::getType() const
{
    return W;
}
int RobotW::getDistance() const
{
    return criminal->manhattanDistance(pos, watson->getCurrentPosition());
}
/*
 * CLASS: RobotS kế thừa class Robot
 */
RobotS::RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock)
    : Robot(
        index, pos, map, criminal
    )
{
    this -> pos = init_pos;
    this -> sherlock = sherlock;
    this->robot_type = S;
    i = init_pos.getRow();
    j = init_pos.getCol();
    int p = i*j;
    int s = -1;
    if(p >= 0){
        s = p;
        while(s >= 10){
            int n = 0;
            while(s > 0){
                n += s % 10;
                s /= 10;
            }
            s = n;
        }
    }
    if (s == 0 or s == 1){
        item = new MagicBook;
    }
    if (s == 2 or s == 3){
        item = new EnergyDrink;
    }
    if (s == 4 or s == 5){
        item = new FirstAid;
    }
    if (s == 6 or s == 7){
        item = new ExcemptionCard;
    }
    if (s == 8 or s == 9){
        item = new PassingCard(i, j);
    }
}

Position RobotS::getNextPosition()
{
    int arrdis[4]={};
    int arrdisb[4]={};
    int out = -1;
    int dem = 0;
    Position upos = Position(pos.getRow() - 1, pos.getCol());
    arrdis[0] = criminal->manhattanDistance(upos, sherlock->getCurrentPosition());
    Position dpos = Position(pos.getRow() + 1, pos.getCol());
    arrdis[1] = criminal->manhattanDistance(dpos, sherlock->getCurrentPosition());
    Position lpos = Position(pos.getRow(), pos.getCol() - 1);
    arrdis[2] = criminal->manhattanDistance(lpos, sherlock->getCurrentPosition());
    Position rpos = Position(pos.getRow(), pos.getCol() + 1);
    arrdis[3] = criminal->manhattanDistance(rpos, sherlock->getCurrentPosition());
    for (int i =0; i < 4; i++){
        arrdisb[i] = arrdis[i];
    }
    for (int i = 0; i < 3; i++){
        for (int j = i+1; j < 4; j++){
            if (arrdisb[i] > arrdisb[j]){
                int temp = arrdisb[i];
                arrdisb[i] = arrdisb[j];
                arrdisb[j] = temp;
            }
        }
    }
    while((out == -1) and (dem < 4)){
        int min = arrdisb[dem];
        if (map->isValid(upos, this) and (min == arrdis[0])){out = 0;}
        else if (map->isValid(rpos, this) and (min == arrdis[3])){out = 3;}
        else if (map->isValid(dpos, this) and (min == arrdis[1])){out = 2;}
        else if (map->isValid(lpos, this) and (min == arrdis[2])){out = 1;}
        dem++;
    }
    if (out == 0){return upos;}
    else if (out == 1){return lpos;}
    else if (out == 2){return dpos;}
    else if (out == 3){return rpos;}
    else if (out == -1){return Position::npos;}
    return Position::npos;
}
void RobotS::move()
{
    if(Position::npos.isEqual(this->getNextPosition())){
        this->getNextPosition();
    }
    else{
        pos = this->getNextPosition();
        this->getNextPosition();
    }
}
string RobotS::str() const
{
    string RBS ="Robot[pos="+ pos.str() + ";type=S;dist=" + to_string(this->getDistance()) + "]";
    return RBS;
}
RobotType RobotS::getType() const
{
    return S;
}
int RobotS::getDistance() const
{
    return criminal->manhattanDistance(pos, sherlock->getCurrentPosition());
}
/*
 * CLASS: RobotSW kế thừa class Robot
 */
RobotSW::RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson)
    : Robot(
        index, pos , map, criminal
      )
{
    this -> index = 0;
    this -> pos = init_pos;
    this -> sherlock = sherlock;
    this -> watson = watson;
    this->robot_type = SW;
   i = init_pos.getRow();
    j = init_pos.getCol();
    int p = i*j;
    int s = -1;
    if(p >= 0){
        s = p;
        while(s >= 10){
            int n = 0;
            while(s > 0){
                n += s % 10;
                s /= 10;
            }
            s = n;
        }
    }
    if (s == 0 or s == 1){
        item = new MagicBook;
    }
    if (s == 2 or s == 3){
        item = new EnergyDrink;
    }
    if (s == 4 or s == 5){
        item = new FirstAid;
    }
    if (s == 6 or s == 7){
        item = new ExcemptionCard;
    }
    if (s == 8 or s == 9){
        item = new PassingCard(i, j);
    }
}

Position RobotSW::getNextPosition()
{
    int arrdis[8]={};
    int arrdisb[8]={};
    int out = -1;
    int dem = 0;
    Position upos = Position(pos.getRow() - 2, pos.getCol());
    arrdis[0] = criminal->manhattanDistance(upos, sherlock->getCurrentPosition()) + criminal->manhattanDistance(upos, watson->getCurrentPosition());
    Position dpos = Position(pos.getRow() + 2, pos.getCol());
    arrdis[1] = criminal->manhattanDistance(dpos, sherlock->getCurrentPosition()) + criminal->manhattanDistance(dpos, watson->getCurrentPosition());
    Position lpos = Position(pos.getRow(), pos.getCol() - 2);
    arrdis[2] = criminal->manhattanDistance(lpos, sherlock->getCurrentPosition()) + criminal->manhattanDistance(lpos, watson->getCurrentPosition());
    Position rpos = Position(pos.getRow(), pos.getCol() + 2);
    arrdis[3] = criminal->manhattanDistance(rpos, sherlock->getCurrentPosition()) + criminal->manhattanDistance(rpos, watson->getCurrentPosition());
    Position urpos = Position(pos.getRow() - 1, pos.getCol() + 1);
    arrdis[4] = criminal->manhattanDistance(urpos, sherlock->getCurrentPosition()) + criminal->manhattanDistance(urpos, watson->getCurrentPosition());
    Position drpos = Position(pos.getRow() + 1, pos.getCol() + 1);
    arrdis[5] = criminal->manhattanDistance(drpos, sherlock->getCurrentPosition()) + criminal->manhattanDistance(drpos, watson->getCurrentPosition());
    Position dlpos = Position(pos.getRow() + 1, pos.getCol() - 1);
    arrdis[6] = criminal->manhattanDistance(dlpos, sherlock->getCurrentPosition()) + criminal->manhattanDistance(dlpos, watson->getCurrentPosition());
    Position ulpos = Position(pos.getRow() - 1, pos.getCol() - 1);
    arrdis[7] = criminal->manhattanDistance(ulpos, sherlock->getCurrentPosition()) + criminal->manhattanDistance(ulpos, watson->getCurrentPosition());
    for (int i = 0; i < 8; i++){
        arrdisb[i] = arrdis[i];
    }
    for (int i = 0; i < 7; i++){
        for (int j = i+1; j < 8; j++){
            if (arrdisb[i] > arrdisb[j]){
                int temp = arrdisb[i];
                arrdisb[i] = arrdisb[j];
                arrdisb[j] = temp;
            }
        }
    }
    while((out == -1) and (dem < 8)){
        int min = arrdisb[dem];
        if (map->isValid(upos, this) and (min == arrdis[0])){out = 0;}
        else if (map->isValid(urpos, this) and (min == arrdis[4])){out = 1;}
        else if (map->isValid(rpos, this) and (min == arrdis[3])){out = 2;}
        else if (map->isValid(drpos, this) and (min == arrdis[5])){out = 3;}
        else if (map->isValid(dpos, this) and (min == arrdis[1])){out = 4 ;}
        else if (map->isValid(dlpos, this) and (min == arrdis[6])){out = 5;}
        else if (map->isValid(lpos, this) and (min == arrdis[2])){out = 6;}
        else if (map->isValid(ulpos, this) and (min == arrdis[7])){out = 7;}
        dem++;
    }
    if (out == 0){return upos;}
    else if (out == 1){return urpos;}
    else if (out == 2){return rpos;}
    else if (out == 3){return drpos;}
    else if (out == 4){return dpos;}
    else if (out == 5){return dlpos;}
    else if (out == 6){return lpos;}
    else if (out == 7){return ulpos;}
    else if (out == -1){return Position::npos;}
    return Position::npos;
}
string RobotSW::str() const
{
    string RBSW ="Robot[pos="+ pos.str() + ";type=SW;dist=" + to_string(this->getDistance()) + "]";
    return RBSW;
}
RobotType RobotSW::getType() const
{
    return RobotType::SW;
}
int RobotSW::getDistance() const
{
    return criminal->manhattanDistance(pos, sherlock->getCurrentPosition()) + criminal->manhattanDistance(pos, watson->getCurrentPosition());
}
void RobotSW::move()
{
    if(Position::npos.isEqual(this->getNextPosition())){
        this->getNextPosition();
    }
    else{
        pos = this->getNextPosition();
        this->getNextPosition();
    }
}
/*
 * CLASS: ArrayMovingObject
 */
ArrayMovingObject::ArrayMovingObject(int capacity)
{
    // TODO: constructor
    this -> count = 0;
    this -> capacity = capacity;
    arr_mv_objs = new MovingObject*[capacity];
    for (int i = 0; i < capacity; i++){
        arr_mv_objs[i] = nullptr;
    }
}
ArrayMovingObject::~ArrayMovingObject()
{
    for (int i = 3; i < count; i++){
        delete arr_mv_objs[i];
    }
    delete []arr_mv_objs;
}
bool ArrayMovingObject::isFull() const
{
    if(count == capacity) {return true;}
    else {return false;}
}
bool ArrayMovingObject::add(MovingObject *mv_obj)
{
    if (this->isFull()){return false;}
    else{
        arr_mv_objs[count] = mv_obj;
        count++;
        return true;
    }
}
MovingObject *ArrayMovingObject::get(int index) const
{
    return arr_mv_objs[index];
}
int ArrayMovingObject::size() const
{
    return count;
}
string ArrayMovingObject::str() const
{
    string out="ArrayMovingObject[count=" + to_string(count)+";"+"capacity="+to_string(capacity) +";";
    for (int i = 0; i < count; i++){
        out = out+arr_mv_objs[i]->str();
        if (i < count -1){
            out = out+";";
        }
        else{out = out + "]";}
    }  
    return out;
}
BaseItem::BaseItem(){}
BaseItem::~BaseItem(){}
// *CLASS: MagicBook
ItemType MagicBook::getType() const
{
    return ItemType::MAGIC_BOOK;
}
string MagicBook::str() const
{
    return "MagicBook";
}
bool MagicBook::canUse(Character *obj, Robot *robot)
{
    if (robot == nullptr){
        if (obj->getObjectType() == SHERLOCK){
            Sherlock *obj2 = dynamic_cast<Sherlock*>(obj);
            if (obj2->getEXP() <= 350){return true;}
            else {return false;}
        }
        else if(obj->getObjectType() == WATSON){
            Watson * obj2 = dynamic_cast<Watson*>(obj);
            if (obj2->getEXP() <= 350){return true;}
            else {return false;}
        }
        else{return false;}
    }
    else{
        return false;
    }
}
void MagicBook::use(Character *obj, Robot *robot)
{
    if (robot == nullptr and this ->canUse(obj, robot)){
        if(obj->getObjectType() == SHERLOCK){
            Sherlock *obj2 = dynamic_cast<Sherlock*>(obj);
            obj2->setEXP(ceil((float)obj2->getEXP()*1.25));
        }
        else if (obj->getObjectType() == WATSON){
            Watson * obj2 = dynamic_cast<Watson*>(obj);
            obj2->setEXP(ceil((float)obj2->getEXP()*1.25));
        }
    }
}
// *CLASS: EnergyDrink
ItemType EnergyDrink::getType() const
{
    return ItemType::ENERGY_DRINK;
}
string EnergyDrink::str() const
{
    return "EnergyDrink";
}
bool EnergyDrink::canUse(Character *obj, Robot *robot)
{
    if (robot == nullptr){
        if (obj->getObjectType() == SHERLOCK){
            Sherlock *obj2 = dynamic_cast<Sherlock*>(obj);
            if (obj2->getHP() <= 100){return true;}
            else {return false;}
        }
        else if(obj->getObjectType() == WATSON){
            Watson * obj2 = dynamic_cast<Watson*>(obj);
            if (obj2->getHP() <= 100){return true;}
            else {return false;}
        }
        else{return false;}
    }
    else{
        return false;
    }
}
void EnergyDrink::use(Character *obj, Robot *robot)
{
    if (robot == nullptr and this ->canUse(obj, robot)){
        if(obj->getObjectType() == SHERLOCK){
            Sherlock *obj2 = dynamic_cast<Sherlock*>(obj);
            obj2->setHP(ceil((float)obj2->getHP()*1.2));
        }
        else if (obj->getObjectType() == WATSON){
            Watson * obj2 = dynamic_cast<Watson*>(obj);
            obj2->setHP(ceil((float)obj2->getHP()*1.2));
        }
    }
}
// *CLASS: FirstAid
ItemType FirstAid::getType() const
{
    return ItemType::FIRST_AID;
}
string FirstAid::str() const
{
    return "FirstAid";
}
bool FirstAid::canUse(Character *obj, Robot *robot)
{
    if (robot == nullptr){
        if (obj->getObjectType() == SHERLOCK){
            Sherlock *obj2 = dynamic_cast<Sherlock*>(obj);
            if (obj2->getHP() <= 100 or obj2->getEXP() <= 350){return true;}
            else {return false;}
        }
        else if(obj->getObjectType() == WATSON){
            Watson * obj2 = dynamic_cast<Watson*>(obj);
            if (obj2->getHP() <= 100 or obj2->getEXP() <= 350){return true;}
            else {return false;}
        }
        else{return false;}
    }
    else{
        return false;
    }
}
void FirstAid::use(Character *obj, Robot *robot)
{
    if (robot == nullptr and this ->canUse(obj, robot)){
        if(obj->getObjectType() == SHERLOCK){
            Sherlock *obj2 = dynamic_cast<Sherlock*>(obj);
            obj2->setHP(ceil((float)obj2->getHP()*1.5));
        }
        else if (obj->getObjectType() == WATSON){
            Watson * obj2 = dynamic_cast<Watson*>(obj);
            obj2->setHP(ceil((float)obj2->getHP()*1.5));
        }
    }
}
// *CLASS: ExcemptionCard
ItemType ExcemptionCard::getType() const
{
    return ItemType::EXCEMPTION_CARD;
}
string ExcemptionCard::str() const
{
    return "ExcemptionCard";
}
bool ExcemptionCard::canUse(Character *obj, Robot *robot)
{
    if (robot != nullptr){
        if (obj->getObjectType() == SHERLOCK){
            Sherlock *obj2 = dynamic_cast<Sherlock*>(obj);
            if (obj2->getHP()%2 != 0){return true;}
            else {return false;}
        }
        else {return false;}
    }
    else{return false;}
}
void ExcemptionCard::use(Character *obj, Robot *robot)
{
    // TODO: sinh viên hiện thực theo tư duy code của mình (hàm có thể rỗng)
}
// *CLASS: PassingCard
PassingCard::PassingCard(string challenge){
    this->challenge = challenge;
}
PassingCard::PassingCard(int i, int j)
{
    int t = (i*11 + j)%4;
    if (t == 0){challenge = "RobotS";}
    if (t == 1){challenge = "RobotC";}
    if (t == 2){challenge = "RobotSW";}
    if (t == 3){challenge = "all";}
}
ItemType PassingCard::getType() const
{
    return ItemType::PASSING_CARD;
}
string PassingCard::str() const
{
    return "PassingCard";
}
bool PassingCard::canUse(Character *obj, Robot *robot)
{
    if (robot != nullptr){
        if (obj->getObjectType() == WATSON){
            Watson *obj2 = dynamic_cast<Watson*>(obj);
            if ((obj2->getHP())%2 == 0){return true;}
            else {return false;}
        }
        else {return false;}
    }
    else{return false;}
    // *Trước khi đấm Robot, Watson, HP chẵn
}
void PassingCard::use(Character *obj, Robot *robot)
{
    if(this->challenge == "all"){
    }
    if(this->challenge == "RobotS"){
        if(robot->getType() == S){
        }
        else{
            if(obj->getObjectType() == SHERLOCK){
                Sherlock *obj2 = dynamic_cast<Sherlock*>(obj);
                obj2->setEXP(obj2->getEXP() - 50);
            }
            if(obj->getObjectType() == WATSON){
                Watson *obj2 = dynamic_cast<Watson*>(obj);
                obj2->setEXP(obj2->getEXP() - 50);
            }
        }
    }
    if(this->challenge == "RobotC"){
        if(robot->getType() == C){
        }
        else{
            if(obj->getObjectType() == SHERLOCK){
                Sherlock *obj2 = dynamic_cast<Sherlock*>(obj);
                obj2->setEXP(obj2->getEXP() - 50);
            }
            if(obj->getObjectType() == WATSON){
                Watson *obj2 = dynamic_cast<Watson*>(obj);
                obj2->setEXP(obj2->getEXP() - 50);
            }
        }
    }
    if(this->challenge == "RobotSW"){
        if(robot->getType() == SW){
        }
        else{
            if(obj->getObjectType() == SHERLOCK){
                Sherlock *obj2 = dynamic_cast<Sherlock*>(obj);
                obj2->setEXP(obj2->getEXP() - 50);
            }
            if(obj->getObjectType() == WATSON){
                Watson *obj2 = dynamic_cast<Watson*>(obj);
                obj2->setEXP(obj2->getEXP() - 50);
            }
        }
    }
}
// *CLASS: BaseBag
BaseBag::BaseBag(int capacity)
{
    this->capacity = capacity;
    this->size = size;
}
BaseBag::~BaseBag()
{
    Node* xoa = head;
    while (xoa != nullptr){
        Node* ke = xoa->next;
        delete xoa;
        xoa = ke;
    }
}

bool BaseBag::insert(BaseItem *item)
{
    
    Node* node = new Node(item, nullptr);
    if(size < this->capacity){
        if (head == nullptr){
            head = node;
            size++;
            return true;
        }
        else{
            node->next = head;
            head = node;
            size++;
            return true;
        }
    }
    else{
        delete node;
        return false;
    }
}
BaseItem *BaseBag::get(ItemType itemType)
{
    if(head != nullptr){
        if(head->item->getType() == itemType){
            Node* tam = head;
            head = head->next;
            BaseItem* out = tam->item;
            delete tam;
            size--;
            return out;
        }
        else{
            Node* tim = head;
            
            while(tim != nullptr and tim->item->getType() != itemType){
                tim = tim->next;
            }
            
            if(tim == nullptr){
                return nullptr;
            }
            else{
                BaseItem* out = tim->item;
                tim->item = head->item;
                head->item = out;
                head = head->next;
                size--;
                return out;
            }
        }
    }
    else{return nullptr;}
}
string BaseBag::str() const
{   
    Node* check = head;
    string strout = "Bag[count=" + to_string(this->size) + ";";
    while(check != nullptr){
        strout = strout + check->item->str();
        if(check->next != nullptr){
            strout = strout +",";
        }
        check = check->next;
    }
    strout = strout + "]";
    return strout;
}
bool BaseBag::checkItem(ItemType itemType){
    Node* tim = head;
    while(tim != nullptr){
        if(tim->item->getType() == itemType){
            return true;
        }
        tim = tim->next;
    }
    return false;
}
BaseItem *BaseBag::get(){
    Node* sheritem = head;
    if(obj->getObjectType() == SHERLOCK){
        Sherlock *obj2 = dynamic_cast<Sherlock*>(obj);
        while(sheritem != nullptr and !(sheritem->item->canUse(obj2, nullptr))){
            sheritem = sheritem->next;
        }
        if (sheritem == nullptr){return nullptr;}
        else{
            BaseItem* out = sheritem->item;
            sheritem->item = head->item;
            head->item = out;
            head = head->next;
            size--;
            return out;
        }
    }
    else if(obj->getObjectType() == WATSON){
        Watson *obj2 = dynamic_cast<Watson*>(obj);
        while(sheritem != nullptr and !(sheritem->item->canUse(obj2, nullptr))){
            sheritem = sheritem->next;
        }
        if (sheritem == nullptr){return nullptr;}
        else{
            BaseItem* out = sheritem->item;
            sheritem->item = head->item;
            head->item = out;
            head = head->next;
            size--;
            return out;
        }
    }
    else{
        return nullptr;
    }
}
// *CLASS: SherlockBag
SherlockBag::SherlockBag(Sherlock *character)
    : BaseBag(
        capacity
    )
{
    this->size = 0;
    this->head = nullptr;
    this->capacity = 6;
    this->sherlock = character;
}
BaseItem *SherlockBag::get()
{
    
    Node* sheritem = head;
    while(sheritem != nullptr and !(sheritem->item->canUse(sherlock, nullptr))){
        sheritem = sheritem->next;
    }
    if (sheritem == nullptr){return nullptr;}
    else{
        BaseItem* out = sheritem->item;
        sheritem->item = head->item;
        head->item = out;
        head = head->next;
        size--;
        return out;
    }
}
// *CLASS: WatsonBag
WatsonBag::WatsonBag(Watson *character)
    : BaseBag(
        capacity
      )
{
    this->size = 0;
    this->head = nullptr;
    this->capacity = 12;
    this->watson = character;
}
BaseItem *WatsonBag::get()
{
    Node* sheritem = head;
    while(sheritem != nullptr and !(sheritem->item->canUse(watson, nullptr))){
        sheritem = sheritem->next;
    }
    if (sheritem == nullptr){return nullptr;}
    else{
        BaseItem* out = sheritem->item;
        sheritem->item = head->item;
        head->item = out;
        head = head->next;
        size--;
        return out;
    }
}
/*
 * CLASS: Configuration
 */
Configuration::Configuration(const string &filepath)
{
    num_fake_walls = 0;
    num_walls = 0;
    sherlock_init_exp = 0;
    sherlock_moving_rule = "";
    watson_init_exp = 0;
    watson_moving_rule = "";
    sherlock_init_hp = 0;
    watson_init_hp = 0;
    sherlock_init_pos = Position::npos;
    watson_init_pos = Position::npos;
    criminal_init_pos = Position::npos;
    arr_fake_walls = nullptr;
    arr_walls = nullptr;
    map_num_cols = 0;
    map_num_rows = 0;
    max_num_moving_objects = 0;

    ifstream myfile(filepath);
    string line;
    while(getline(myfile, line)){
    if (line.find("ROWS") != -1){
        string numrow = line.substr(line.find("=") + 1);
        stringstream numrow2(numrow); 
        numrow2 >> map_num_rows;
    }else if (line.find("COLS") != -1){
        string numcol = line.substr(line.find("=") + 1);
        stringstream numcol2(numcol); 
        numcol2 >> map_num_cols;
    }else if (line.find("OBJECTS") != -1){
        string numo = line.substr(line.find("=") + 1);
        stringstream numo2(numo); 
        numo2 >> max_num_moving_objects;
    }else if (line.find("STEPS") != -1){
        string numst = line.substr(line.find("=") + 1);
        stringstream numst2(numst); 
        numst2 >> num_steps;
    }else if (line.find("RULE") != -1){
        if (line.find("SHERLOCK") != -1){
            sherlock_moving_rule = line.substr(line.find("=") + 1);
        }
        else{
            watson_moving_rule = line.substr(line.find("=") + 1);
        }
    }else if (line.find("HP") != -1){
        if (line.find("SHERLOCK") != -1){
            string sherhp = line.substr(line.find("=") + 1);
            stringstream sherhp2(sherhp);
            sherhp2 >> sherlock_init_hp;
        }
        else{
            string wathp = line.substr(line.find("=") + 1);
            stringstream wathp2(wathp);
            wathp2 >> watson_init_hp;
        }
    }else if (line.find("EXP") != -1){
        if (line.find("SHERLOCK") != -1){
            string sherexp = line.substr(line.find("=") + 1);
            stringstream sherexp2(sherexp);
            sherexp2 >> sherlock_init_exp;
        }
        else{
            string watexp = line.substr(line.find("=") + 1);
            stringstream watexp2(watexp);
            watexp2 >> watson_init_exp;
        }
    }else if (line.find("ARRAY") != -1){
        if (line.find("FAKE_WALLS") != -1){
            int countfwall = 0;
            string linesub = line.substr(line.find("=") + 1);
            for (int i = line.find("=") + 1;i <= line.length();i++ ){
                if (line[i] == 40)
                    countfwall ++;
            }
            num_fake_walls = countfwall;
            int dem = 0;
            int hang, cot;
            if (num_fake_walls == 0){}
            else if (num_fake_walls == 1){
                arr_fake_walls = new Position[1];
                for (int i = 0; i < linesub.length();i++){
                    if(linesub[i] == 40){
                        string xfwall = linesub.substr(i+1,linesub.find(",")-1);
                        stringstream xfwall2(xfwall);
                        xfwall2 >> hang;
                        arr_fake_walls[0].setRow(hang);
                        string yfwall = linesub.substr(linesub.find(",")+1, linesub.find(")")-1);
                        stringstream yfwall2(yfwall);
                        yfwall2 >> cot;
                        arr_fake_walls[0].setCol(cot);
                        dem ++;
                    }
                }
            }
            else{
                int hang, cot;
                arr_fake_walls = new Position[countfwall];
                for (int i = 0; i < linesub.length();i++){
                    if(linesub[i] == 40){
                        string xfwall = linesub.substr(i+1,linesub.find(",")-1);
                        stringstream xfwall2(xfwall);
                        xfwall2 >> hang;
                        arr_fake_walls[dem].setRow(hang);
                        linesub.replace(i,linesub.find(",") -i," ");
                        string yfwall = linesub.substr(linesub.find(",") + 1, linesub.find(")") - 1);
                        stringstream yfwall2(yfwall);
                        yfwall2 >> cot;
                        arr_fake_walls[dem].setCol(cot);
                        linesub.replace(linesub.find(","),linesub.find(")")- linesub.find(",") + 1," ");
                        dem ++;
                    }
                    
                }
            }
        }
        else{
            int countwall = 0;
            string linesub = line.substr(line.find("=") + 1);
            for (int i = line.find("=") + 1;i <= line.length();i++ ){
                if (line[i] == 40)
                    countwall ++;
            }
            num_walls = countwall;
            int dem = 0;
            int hang, cot;
            if(num_walls == 0){}
            else if (num_walls == 1){
                arr_walls = new Position[1];
                for (int i = 0; i < linesub.length();i++){
                    if(linesub[i] == 40){
                        string xwall = linesub.substr(i+1,linesub.find(",")-1);
                        stringstream xwall2(xwall);
                        xwall2 >> hang;
                        arr_walls[0].setRow(hang);
                        string ywall = linesub.substr(linesub.find(",")+1, linesub.find(")")-1);
                        stringstream ywall2(ywall);
                        ywall2 >> cot;
                        arr_walls[0].setCol(cot);
                        dem ++;
                    }
                }
            }
            else{
                arr_walls = new Position[countwall];
                int dem = 0;
                int hang, cot;
                for (int i = 0; i < linesub.length();i++){
                    if(linesub[i] == 40){
                        string xwall = linesub.substr(i+1,linesub.find(",")-1);
                        stringstream xwall2(xwall);
                        xwall2 >> hang;
                        arr_walls[dem].setRow(hang);
                        linesub.replace(i,linesub.find(",") -i," ");
                        string ywall = linesub.substr(linesub.find(",") + 1, linesub.find(")") - 1);
                        stringstream ywall2(ywall);
                        ywall2 >> cot;
                        arr_walls[dem].setCol(cot);
                        linesub.replace(linesub.find(","),linesub.find(")")- linesub.find(",") + 1," ");
                        dem ++;
                    }
                }
            }
        }
    }else if (line.find("POS") != -1){
        if (line.find("SHERLOCK") != -1){
            int hang, cot;
            string xsherpos = line.substr(line.find("(") + 1,line.find(",") - 1);
            stringstream xsherpos2(xsherpos);
            xsherpos2 >> hang;
            sherlock_init_pos.setRow(hang);
            string ysherpos = line.substr(line.find(",") + 1,line.find(")") - 1);
            stringstream ysherpos2(ysherpos);
            ysherpos2 >> cot;
            sherlock_init_pos.setCol(cot);
        }else if (line.find("WATSON") != -1){
            int hang, cot;
            string xwatpos = line.substr(line.find("(") + 1,line.find(",") - 1);
            stringstream xwatpos2(xwatpos);
            xwatpos2 >> hang;
            watson_init_pos.setRow(hang);
            string ywatpos = line.substr(line.find(",") + 1,line.find(")") - 1);
            stringstream ywatpos2(ywatpos);
            ywatpos2 >> cot;
            watson_init_pos.setCol(cot);
        }else{
            int hang, cot;
            string xcripos = line.substr(line.find("(") + 1,line.find(",") - 1);
            stringstream xcripos2(xcripos);
            xcripos2 >> hang;
            criminal_init_pos.setRow(hang);
            string ycripos = line.substr(line.find(",") + 1,line.find(")") - 1);
            stringstream ycripos2(ycripos);
            ycripos2 >> cot;
            criminal_init_pos.setCol(cot);
        }
    }
}
myfile.close();
}
Configuration::~Configuration()
{
    if(num_fake_walls != 0)
    delete []arr_fake_walls;
    if(num_walls != 0)
    delete []arr_walls;
}
string Configuration::str() const
{
    string arrwall = "[";
    string arrfwall = "[";
    if (num_walls == 0){arrwall = arrwall + "]";}
    else{
        for (int i = 0; i < num_walls; i++){
            if (i != num_walls -1){
                arrwall = arrwall + arr_walls[i].str() + ";";
            }
            else{
                arrwall = arrwall + arr_walls[i].str() + "]";
            }
        }
    }
    if (num_fake_walls == 0){arrfwall = arrfwall + "]";}
    else{
        for (int i = 0; i < num_fake_walls; i++){
            if (i != num_fake_walls -1){
                arrfwall = arrfwall + arr_fake_walls[i].str() + ";";
            }
            else{
                arrfwall = arrfwall + arr_fake_walls[i].str() + "]";
            }
        }
    }
    string out="Configuration[\n"  "MAP_NUM_ROWS=" + to_string(map_num_rows)+"\nMAP_NUM_COLS=" + to_string(map_num_cols) + "\nMAX_NUM_MOVING_OBJECTS=" + to_string(max_num_moving_objects) + "\nNUM_WALLS=" + to_string(num_walls) + "\nARRAY_WALLS=" + arrwall + "\nNUM_FAKE_WALLS=" + to_string(num_fake_walls) + "\nARRAY_FAKE_WALLS=" + arrfwall + "\nSHERLOCK_MOVING_RULE=" + sherlock_moving_rule + "\nSHERLOCK_INIT_POS=" + sherlock_init_pos.str() + "\nSHERLOCK_INIT_HP=" + to_string(sherlock_init_hp) + "\nSHERLOCK_INIT_EXP=" + to_string(sherlock_init_exp) + "\nWATSON_MOVING_RULE=" + watson_moving_rule + "\nWATSON_INIT_POS=" + watson_init_pos.str() + "\nWATSON_INIT_HP=" + to_string(watson_init_hp) + "\nWATSON_INIT_EXP=" + to_string(watson_init_exp) + "\nCRIMINAL_INIT_POS=" + criminal_init_pos.str() + "\nNUM_STEPS=" + to_string(num_steps) + "\n]";
    return out;
}

BaseItem *Robot::NewItem(){
    if(itemhold != nullptr){
        if(this->itemhold->str() == "MagicBook"){
            BaseItem *out = new MagicBook;
            return out;
        }
        else if(this->itemhold->str() == "EnergyDrink"){
            BaseItem *out = new EnergyDrink;
            return out;
        }
        else if(this->itemhold->str() == "FirstAid"){
            BaseItem *out = new FirstAid;
            return out;
        }
        else if(this->itemhold->str() == "ExcemptionCard"){
            BaseItem *out = new ExcemptionCard;
            return out;
            
        }
        else if(this->itemhold->str() == "PassingCard"){
            BaseItem *out = new PassingCard(this->i, this->j);
            PassingCard *out2 = dynamic_cast<PassingCard*>(out);
            PassingCard *itemhold2 = dynamic_cast<PassingCard*>(itemhold);
            return out2;
        }
        
    }
    else{return nullptr;}
    return nullptr;

}
/*
 * CLASS: StudyPinkProgram
 */
// StudyPinkProgram::StudyPinkProgram()
// {
    
// }
StudyPinkProgram::StudyPinkProgram(const string &config_file_path)
{
    // logger = new Logger();
    config = new Configuration(config_file_path);
    map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
    arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
    sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_init_pos, map, config->sherlock_init_hp, config->sherlock_init_exp);
    watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos, map, config->watson_init_hp, config->watson_init_exp);
    criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);

    arr_mv_objs->add(criminal);
    arr_mv_objs->add(sherlock);
    arr_mv_objs->add(watson);
    var = false;
}
StudyPinkProgram::~StudyPinkProgram()
{
    // delete arr_mv_objs->get(0);
    // delete arr_mv_objs->get(1);
    // delete arr_mv_objs->get(2);
    delete map;
    delete config;
    delete arr_mv_objs;
}
void StudyPinkProgram::run(bool verbose) {
        for (int istep = 0; istep < config->num_steps; ++istep) {
        for (int i = 0; i < arr_mv_objs->size(); ++i) {
            if (isStop()) {
                break;
            }
            arr_mv_objs->get(i)->move();
            var = arr_mv_objs->checkMeet(i);
            if (isStop()) {
                printStep(istep);
                break;
            }
            MovingObject *robot = nullptr;
            if (arr_mv_objs->get(i)->getObjectType() == MovingObjectType::CRIMINAL)
            {
                robot = Robot::create(arr_mv_objs->size(), map, criminal, sherlock, watson);
            }
            if (robot != nullptr)
            {
                if (criminal->getCount() % 3 == 0 && criminal->getCount() > 0)
                {
                    arr_mv_objs->add(robot);
                }
                else
                {
                    delete robot;
                }
            }
            if (verbose) {
                printStep(istep);
            }
        }
        if (isStop()) {
            break;
        }
    }
    printResult();
}
bool StudyPinkProgram::isStop() const{
    for(int i = 0; i < arr_mv_objs->size(); i++){
        if(var or sherlock->getHP() <= 1 or watson->getHP() <= 1){
            return true;
        }
    }
    return false;
}
// *CLASS: ArrayMovingObject
bool ArrayMovingObject::checkMeet(int index) const
{
    bool check;
    if(this->get(index)->str().find("Sherlock") != -1){
        Sherlock *out = dynamic_cast<Sherlock*>(this->get(index));
        for(int i = 0; i < count; i++){
            if(this->get(index)->getCurrentPosition().isEqual(arr_mv_objs[i]->getCurrentPosition()) and i != index){
                if(arr_mv_objs[i]->getObjectType() == ROBOT){
                    Robot *robotout = dynamic_cast<Robot*>(this->get(i));
                    if(robotout->getType() == C){
                        RobotC *robotc = dynamic_cast<RobotC*>(robotout);
                        if(out->meet(robotc)){
                            for(int i = 0; i < count; i++){
                                if(this->get(i)->str().find("Criminal") != -1){
                                    Criminal *getpos = dynamic_cast<Criminal*>(this->get(i));
                                    out->setPos(getpos->getCurrentPosition());
                                }
                            }

                            return true;
                        }
                        else{check = false;}
                    }
                    if(robotout->getType() == W){
                        RobotW *robotw = dynamic_cast<RobotW*>(robotout);
                        out->meet(robotw);
                        check = false;
                    }
                    if(robotout->getType() == S){
                        RobotS *robots = dynamic_cast<RobotS*>(robotout);
                        out->meet(robots);
                        check = false;
                    }
                    if(robotout->getType() == SW){
                        RobotSW *robotsw = dynamic_cast<RobotSW*>(robotout);
                        out->meet(robotsw);
                        check = false;
                    }
                }
                if(arr_mv_objs[i]->getObjectType() == WATSON){
                    Watson *watson = dynamic_cast<Watson*>(this->get(i));
                    out->meet(watson);
                    check = false;
                }
                if(arr_mv_objs[i]->getObjectType() == CRIMINAL){
                    return true;
                }
            }
        }
        return check;
    }
    else if(this->get(index)->str().find("Watson") != -1){
        Watson *out = dynamic_cast<Watson*>(this->get(index));
        for(int i = 0; i < count; i++){
            if(this->get(index)->getCurrentPosition().isEqual(arr_mv_objs[i]->getCurrentPosition()) and i != index){  
                if(arr_mv_objs[i]->getObjectType() == ROBOT){
                    Robot *robotout = dynamic_cast<Robot*>(this->get(i));
                    if(robotout->getType() == C){
                        RobotC *robotc = dynamic_cast<RobotC*>(robotout);
                        out->meet(robotc);
                        check = false;
                    }
                    if(robotout->getType() == SW){
                        RobotSW *robotsw = dynamic_cast<RobotSW*>(robotout);
                        out->meet(robotsw);
                        check = false;
                    }
                    if(robotout->getType() == W){
                        RobotW *robotw = dynamic_cast<RobotW*>(robotout);
                        out->meet(robotw);
                        check = false;
                    }
                    if(robotout->getType() == S){
                        RobotS *robots = dynamic_cast<RobotS*>(robotout);
                        out->meet(robots);
                        check = false;
                    }
                    
                }
                if(arr_mv_objs[i]->getObjectType() == SHERLOCK){
                    Sherlock *sherlock = dynamic_cast<Sherlock*>(this->get(i));
                    out->meet(sherlock);
                    check = false;
                }
                if(arr_mv_objs[i]->getObjectType() == CRIMINAL){
                    return true;
                }
            }
        }
        return check;
    }
    else if(this->get(index)->str().find("Criminal") != -1){
        Criminal *out = dynamic_cast<Criminal*>(this->get(index));
        for(int i = 0; i < count; i++){
            if(this->get(index)->getCurrentPosition().isEqual(arr_mv_objs[i]->getCurrentPosition()) and i != index){
                if(arr_mv_objs[i]->getObjectType() == ROBOT){check = false;}
                if(arr_mv_objs[i]->getObjectType() == SHERLOCK){return true;}
                if(arr_mv_objs[i]->getObjectType() == WATSON){return true;}
            }
        }
        return check;
    }
    else if(this->get(index)->str().find("Robot") != -1){
        Robot *out = dynamic_cast<Robot*>(this->get(index));
        for(int i = 0; i < count; i++){
            if(this->get(index)->getCurrentPosition().isEqual(arr_mv_objs[i]->getCurrentPosition()) and i != index){
                if(arr_mv_objs[i]->getObjectType() == ROBOT){check = false;}
                if(arr_mv_objs[i]->getObjectType() == SHERLOCK){
                    Sherlock *sherlock = dynamic_cast<Sherlock*>(this->get(i));
                    if(out->getType() == C){
                        RobotC *robotc = dynamic_cast<RobotC*>(out);
                        if(sherlock->meet(robotc)){
                            for(int i = 0; i < count; i++){
                                if(this->get(i)->str().find("Criminal") != -1){
                                    Criminal *getpos = dynamic_cast<Criminal*>(this->get(i));
                                    sherlock->setPos(getpos->getCurrentPosition());
                                }
                            }
                            return true;
                        }
                        else{check = false;}
                    }
                    if(out->getType() == W){
                        RobotW *robotw = dynamic_cast<RobotW*>(out);
                        sherlock->meet(robotw);
                        check = false;
                    }
                    if(out->getType() == S){
                        RobotS *robots = dynamic_cast<RobotS*>(out);
                        sherlock->meet(robots);
                        check = false;
                    }
                    if(out->getType() == SW){
                        RobotSW *robotsw = dynamic_cast<RobotSW*>(out);
                        sherlock->meet(robotsw);
                        check = false;
                    }
                }
                if(arr_mv_objs[i]->getObjectType() == WATSON){
                    Watson *watson = dynamic_cast<Watson*>(this->get(i));
                    if(out->getType() == C){
                        RobotC *robotc = dynamic_cast<RobotC*>(out);
                        watson->meet(robotc);
                        check = false;
                    }
                    if(out->getType() == W){
                        RobotW *robotw = dynamic_cast<RobotW*>(out);
                        watson->meet(robotw);
                        
                        check = false;
                    }
                    if(out->getType() == S){
                        RobotS *robots = dynamic_cast<RobotS*>(out);
                        watson->meet(robots);
                        check = false;
                    }
                    if(out->getType() == SW){
                        RobotSW *robotsw = dynamic_cast<RobotSW*>(out);
                        watson->meet(robotsw);
                        check = false;
                    }
                }
                if(arr_mv_objs[i]->getObjectType() == CRIMINAL){
                    check = false;
                }
            }
        }
        return check;
    }
    return false;
}



// *CLASS: Sherlock
//tính toán kết quả khi gặp mặt
bool Sherlock::sherlockBattle(int robotExp, float counterExp, float counterPercent, int& hpLoss, int& expLoss, int& expGain){
    double randomFactor = (randomInt(95, 105)) / 100.0;
    double chance = (double)this->getEXP()/(this->getEXP() + robotExp + counterExp) * randomFactor;
    chance = chance * (1.0 - counterPercent*2.0);

    if(chance > 0.85) chance = 0.85;
    if(chance < 0) chance = 0.05;

    
    double roll = (randomInt(0, 100)) / 100.0;

    if (roll < chance){
        double expGainBase = 0.15;
        double expGainModifier = expGainBase - counterPercent;
        double counterBonus = 1;

        // if(counterPercent < 0) counterBonus = 1.1;
        // else if (counterPercent == 0);
        // else counterBonus = 0.9;

        expGain = (int)ceil(robotExp * expGainModifier * randomFactor * counterBonus);
        this->setEXP(this->getEXP() + expGain);
        return true;
    }
    else{
        double hpLossBase = robotExp/8.0;
        double hpLossFactor = 1.0 - counterPercent * 2.0; // Amplify counter effect
        if(hpLossFactor < 0.5) hpLossFactor = 0.5;   // Min 50% damage
        if(hpLossFactor > 1.5) hpLossFactor = 1.5;   // Max 150% damage


        double expLossBase = 0.08;
        double expLossModifier = expLossBase - counterPercent * 2.0; // Negative counter increases loss
        if(expLossModifier < 0.03) expLossModifier = 0.03; // Min 3% loss
        if(expLossModifier > 0.15) expLossModifier = 0.15; // Max 15% loss

        hpLoss = (int)ceil(hpLossBase * hpLossFactor * randomFactor);
        expLoss = (int)ceil(robotExp * expLossModifier * randomFactor);

        this->setHP(this->getHP() - hpLoss);
        this->setEXP(this->getEXP() - expLoss);
        return false;
    }
}

void Sherlock::setPos(Position pos)
{
    this->pos = pos;
}

/*
RobotS: Thông minh, khắc chế Sherlock nhưng bị Watson khắc chế.
- exp: 600
- counter: sherlock (50, -0.05)
- countered: watson (50, 0.05)

RobotW: Mạnh mẽ, khắc chế Watson nhưng bị Sherlock khắc chế.
- exp: 400
- counter: watson (50, -0.05)
- countered: sherlock (50, 0.05)

RobotSW: Boss, khắc chế mạnh cả Sherlock và Watson.
- exp: 800
- counter: sherlock (100, -0.1), watson (100, -0.1)
- countered: none

RobotC: Vệ sĩ cho Criminal có exp cực cao và counter cả 2.
- exp: 2000
- counter: sherlock (100, -0.1), watson (100, -0.1)
- countered: none
*/
bool Sherlock::meet(RobotS *robotS)
{
    if(this->bag->checkItem(EXCEMPTION_CARD)){
        BaseItem *k = new ExcemptionCard();
        if(k->canUse(this, robotS)){
            this->bag->get(EXCEMPTION_CARD)->use(this, robotS);

            BaseItem* q = this->bag->get();
            if(q != nullptr){
                q->use(this, nullptr);
            }
            return true;
        }
    }
    else{
        int hpLoss = 0;
        int expLoss = 0;
        int expGain = 0;
        bool result = this->sherlockBattle(800, 50, -0.05, hpLoss, expLoss, expGain);
        if(result){
            this->bag->insert(robotS->NewItem());
            string out = "Sherlock beat RobotS (+" + to_string(expGain) + " EXP), got item";
            Logger::instance().add(out);
        }
        else{
            string out = "Sherlock lost to RobotS (-" + to_string(hpLoss) + " HP, -" + to_string(expLoss) + " EXP)";
            Logger::instance().add(out);
        }
    }
    BaseItem* k = this->bag->get();
    if( k != nullptr){
        k->use(this, nullptr);
    }
    return false;

    // if(this->getEXP() > 400){
    //     this->bag->insert(robotS->NewItem());
    //     string out = "Sherlock beat RobotS and got item.";
    //     Logger::instance().add(out);
    // }
    // else{
    //     if(!t){
    //         this->setEXP(ceil((float)this->getEXP()*0.9));
    //         string out = "Sherlock lost to RobotS (-10% EXP).";
    //         Logger::instance().add(out);
    //     }
    // }
}
bool Sherlock::meet(RobotW *robotW)
{
    if(this->bag->checkItem(EXCEMPTION_CARD)){
        BaseItem *k = new ExcemptionCard();
        if(k->canUse(this, robotW)){
            this->bag->get(EXCEMPTION_CARD)->use(this, robotW);

            BaseItem* q = this->bag->get();
            if(q != nullptr){
                q->use(this, nullptr);
            }
            return true;
        }
    }
    else{
        int hpLoss = 0;
        int expLoss = 0;
        int expGain = 0;
        bool result = this->sherlockBattle(600, 50, 0.05, hpLoss, expLoss, expGain);
        if(result){
            this->bag->insert(robotW->NewItem());
            string out = "Sherlock beat RobotW (+" + to_string(expGain) + " EXP), got item";
            Logger::instance().add(out);
        }
        else{
            string out = "Sherlock lost to RobotW (-" + to_string(hpLoss) + " HP, -" + to_string(expLoss) + " EXP)";
            Logger::instance().add(out); 
        }
    }
    BaseItem* k = this->bag->get();
    if( k != nullptr){
        k->use(this, nullptr);
    }
    return false;
    // if(this->bag->checkItem(EXCEMPTION_CARD)){
    //     BaseItem *k = new ExcemptionCard();
    //     if(k->canUse(this, robotW)){
    //         this->bag->get(EXCEMPTION_CARD)->use(this, robotW);
    //     }
    // }
    // this->bag->insert(robotW->NewItem());
    // string out = "Sherlock beat RobotW and got item.";
    // Logger::instance().add(out);
    // BaseItem* k = this->bag->get();
    // if( k != nullptr){
    //     k->use(this, nullptr);
    // }
    // return false;
}
bool Sherlock::meet(RobotSW *robotSW)
{
    if(this->bag->checkItem(EXCEMPTION_CARD)){
        BaseItem *k = new ExcemptionCard();
        if(k->canUse(this, robotSW)){
            this->bag->get(EXCEMPTION_CARD)->use(this, robotSW);

            BaseItem* q = this->bag->get();
            if(q != nullptr){
                q->use(this, nullptr);
            }
            return true;
        }
    }
    else{
        int hpLoss = 0;
        int expLoss = 0;
        int expGain = 0;
        bool result = this->sherlockBattle(1000, 100, -0.1, hpLoss, expLoss, expGain);
        if(result){
            this->bag->insert(robotSW->NewItem());
            string out = "Sherlock beat RobotSW (+" + to_string(expGain) + " EXP), got item";
            Logger::instance().add(out);
        }
        else{
            string out = "Sherlock lost to RobotSW (-" + to_string(hpLoss) + " HP, -" + to_string(expLoss) + " EXP)";
            Logger::instance().add(out);
        }
    }
    BaseItem* k = this->bag->get();
    if( k != nullptr){
        k->use(this, nullptr);
    }
    return false;
    // bool t;
    // if(this->bag->checkItem(EXCEMPTION_CARD)){
    //     BaseItem *k = new ExcemptionCard();
    //     if(k->canUse(this, robotSW)){
    //         this->bag->get(EXCEMPTION_CARD)->use(this, robotSW);
    //         t = true;
    //     }
    // }
    // if(this->getEXP() > 300 and this->getHP() > 335){
    //     this->bag->insert(robotSW->NewItem());
    //     string out = "Sherlock beat RobotSW and got item.";
    //     Logger::instance().add(out);
    // }
    // else{
    //     if(!t){
    //         this->setEXP(ceil((float)this->getEXP()*0.85));
    //         this->setHP(ceil((float)this->getHP()*0.85));
    //         string out = "Sherlock lost to RobotSW (-10% EXP,HP).";
    //         Logger::instance().add(out);
    //     }
    // }
    // BaseItem* k = this->bag->get();
    // if( k != nullptr){
    //     k->use(this, nullptr);
    // }
    // return false;
}
bool Sherlock::meet(RobotC *robotC)
{
    if(this->bag->checkItem(EXCEMPTION_CARD)){
        BaseItem *k = new ExcemptionCard();
        if(k->canUse(this, robotC)){
            this->bag->get(EXCEMPTION_CARD)->use(this, robotC);

            BaseItem* q = this->bag->get();
            if(q != nullptr){
                q->use(this, nullptr);
            }
            string out = "Sherlock caught Criminal (ExcemptionCard)";
            Logger::instance().add(out);
            return true;
        }
    }
    else{
        int hpLoss = 0;
        int expLoss = 0;
        int expGain = 0;
        bool result = this->sherlockBattle(2000, 100, -0.1, hpLoss, expLoss, expGain);
        if(result){
            this->bag->insert(robotC->NewItem());
            string out = "Sherlock caught Criminal";
            Logger::instance().add(out);
            return true;
        }
        else{
            string out = "Sherlock lost RobotC (-" + to_string(hpLoss) + " HP, -" + to_string(expLoss) + " EXP)";
            Logger::instance().add(out);
        }
    }
    BaseItem* k = this->bag->get();
    if( k != nullptr){
        k->use(this, nullptr);
    }
    return false;
    // if(this->bag->checkItem(EXCEMPTION_CARD)){
    //     BaseItem *k = new ExcemptionCard();
    //     if(k->canUse(this, robotC)){
    //         this->bag->get(EXCEMPTION_CARD)->use(this, robotC);
    //         // string out = "Sherlock used Excemption Card.";
    //         // Logger::instance().add(out);
    //     }
    // }
    // if(this->getEXP() > 500){
    //     string out = "Sherlock caught Criminal.";
    //     Logger::instance().add(out);
    //     return true;
    // }
    // else{
    //     this->bag->insert(robotC->NewItem());
    //     string out = "Sherlock beat RobotC and got item.";
    //     Logger::instance().add(out);
    //     BaseItem* k = this->bag->get();
    //     if( k != nullptr){
    //         k->use(this, nullptr);
    //     }
    //     return false;
    // } 
}
bool Sherlock::meet(Watson *watson)
{
    if(this->bag->checkItem(PASSING_CARD) and watson->getBag()->checkItem(EXCEMPTION_CARD)){
        while(this->bag->checkItem(PASSING_CARD)){ 
            watson->getBag()->insert(this->bag->get(PASSING_CARD));
        }
        while(watson->getBag()->checkItem(EXCEMPTION_CARD)){
            this->bag->insert(watson->getBag()->get(EXCEMPTION_CARD));
        }
        string out = "Sherlock, Watson traded items.";
        Logger::instance().add(out);
    }
    else{
        string out = "They did not have enough items.";
        Logger::instance().add(out);
    }
    return false;
}
// *CLASS: Watson
bool Watson::watsonBattle(int robotExp, float counterExp, float counterPercent, int& hpLoss, int& expLoss, int& expGain){
    double randomFactor = (randomInt(95, 105)) / 100.0;
    double chance = (double)this->getEXP()/(this->getEXP() + robotExp + counterExp) * randomFactor;
    chance = chance * (1.0 - counterPercent*2.0);

    if(chance > 0.85) chance = 0.85;
    if(chance < 0) chance = 0.05;

    
    double roll = (randomInt(0, 100)) / 100.0;

    if (roll < chance){
        double expGainBase = 0.15;
        double expGainModifier = expGainBase - counterPercent;
        double counterBonus = 1;

        // if(counterPercent < 0) counterBonus = 1.1;
        // else if (counterPercent == 0);
        // else counterBonus = 0.9;

        expGain = (int)ceil(robotExp * expGainModifier * randomFactor * counterBonus);
        this->setEXP(this->getEXP() + expGain);
        return true;
    }
    else{
        double hpLossBase = robotExp/8.0;
        double hpLossFactor = 1.0 - counterPercent * 2.0; // Amplify counter effect
        if(hpLossFactor < 0.5) hpLossFactor = 0.5;   // Min 50% damage
        if(hpLossFactor > 1.5) hpLossFactor = 1.5;   // Max 150% damage


        double expLossBase = 0.08;
        double expLossModifier = expLossBase - counterPercent * 2.0; // Negative counter increases loss
        if(expLossModifier < 0.03) expLossModifier = 0.03; // Min 3% loss
        if(expLossModifier > 0.15) expLossModifier = 0.15; // Max 15% loss

        hpLoss = (int)ceil(hpLossBase * hpLossFactor * randomFactor);
        expLoss = (int)ceil(robotExp * expLossModifier * randomFactor);

        this->setHP(this->getHP() - hpLoss);
        this->setEXP(this->getEXP() - expLoss);
        return false;
    }
}

void Watson::setPos(Position pos)
{
    this->pos = pos;
}

bool Watson::meet(RobotS *robotS)
{
    if(this->bag->checkItem(PASSING_CARD) and this->getHP()%2 == 0){
        BaseItem *k = new PassingCard(0,0);
        if(k->canUse(this, robotS) ){
            this->bag->get(PASSING_CARD)->use(this, robotS);

            BaseItem* q = this->bag->get();
            if(q != nullptr){
                q->use(this, nullptr);
            }
            return true;
        }
    }
    else{
        int hpLoss = 0;
        int expLoss = 0;
        int expGain = 0;
        bool result = this->watsonBattle(400, 50, 0.05, hpLoss, expLoss, expGain);
        if(result){
            this->bag->insert(robotS->NewItem());
            string out = "Watson beat RobotW (+" + to_string(expGain) + "EXP), got item";
            Logger::instance().add(out);
        }
        else{
            string out = "Watson lost to RobotW (-" + to_string(hpLoss) + " HP, -" + to_string(expLoss) + " EXP)";
            Logger::instance().add(out);
        }
    }
    
    BaseItem* k = this->bag->get();
    if( k != nullptr){
        k->use(this, nullptr);
    }
    return false;
}
bool Watson::meet(RobotW *robotW)
{
    if(this->bag->checkItem(PASSING_CARD) and this->getHP()%2 == 0){
        BaseItem *k = new PassingCard(0,0);
        if(k->canUse(this, robotW) ){
            this->bag->get(PASSING_CARD)->use(this, robotW);

            BaseItem* q = this->bag->get();
            if(q != nullptr){
                q->use(this, nullptr);
            }
            return true;
        }
    }
    else{
        int hpLoss = 0;
        int expLoss = 0;
        int expGain = 0;
        bool result = this->watsonBattle(400, 50, -0.05, hpLoss, expLoss, expGain);
        if(result){
            this->bag->insert(robotW->NewItem());
            string out = "Watson beat RobotW (+" + to_string(expGain) + "EXP), got item";
            Logger::instance().add(out);
        }
        else{
            string out = "Watson lost to RobotW (-" + to_string(hpLoss) + " HP, -" + to_string(expLoss) + " EXP)";
            Logger::instance().add(out);
        }
    }
    
    BaseItem* k = this->bag->get();
    if( k != nullptr){
        k->use(this, nullptr);
    }
    return false;
    // bool t = true;
    // if(this->bag->checkItem(PASSING_CARD) and this->getHP()%2 == 0){
    //     BaseItem *k = new PassingCard(0,0);
    //     if(k->canUse(this, robotW)){
    //         this->bag->get(PASSING_CARD)->use(this, robotW);
    //         this->bag->insert(robotW->NewItem());
    //         string out = "Watson beat RobotW and got item.";
    //         Logger::instance().add(out);
    //         t = false;
    //     }
    // }
    // else if(this->getHP() > 350){
    //     if(t){
    //         this->bag->insert(robotW->NewItem());
    //         string out = "Watson beat RobotW and got item.";
    //         Logger::instance().add(out);
    //     }
    // }
    // else{
    //     this->setHP(ceil((float)this->getHP()*0.95));
    //     string out = "Watson lost to RobotW (-5% HP).";
    //     Logger::instance().add(out);
    // }
    // BaseItem* k = this->bag->get();
    // if( k != nullptr){
    //     k->use(this, nullptr);
    // }
    // return false;
}
bool Watson::meet(RobotSW *robotSW)
{
    if(this->bag->checkItem(PASSING_CARD) and this->getHP()%2 == 0){
        BaseItem *k = new PassingCard(0,0);
        if(k->canUse(this, robotSW) ){
            this->bag->get(PASSING_CARD)->use(this, robotSW);

            BaseItem* q = this->bag->get();
            if(q != nullptr){
                q->use(this, nullptr);
            }
            return true;
        }
    }
    else{
        int hpLoss = 0;
        int expLoss = 0;
        int expGain = 0;
        bool result = this->watsonBattle(800, 100, -0.1, hpLoss, expLoss, expGain);
        if(result){
            this->bag->insert(robotSW->NewItem());
            string out = "Watson beat RobotSW (+" + to_string(expGain) + "EXP), got item";
            Logger::instance().add(out);
        }
        else{
            string out = "Watson lost to RobotSW (-" + to_string(hpLoss) + " HP, -" + to_string(expLoss) + " EXP)";
            Logger::instance().add(out);
        }
    }
    
    BaseItem* k = this->bag->get();
    if( k != nullptr){
        k->use(this, nullptr);
    }
    return false;
}
bool Watson::meet(RobotC *robotC)
{
    if(this->bag->checkItem(PASSING_CARD) and this->getHP()%2 == 0){
        BaseItem *k = new PassingCard(0,0);
        if(k->canUse(this, robotC)){
            this->bag->get(PASSING_CARD)->use(this, robotC);
            BaseItem* q = this->bag->get();
            if(q != nullptr){
                q->use(this, nullptr);
            }
            string out = "Watson caught Criminal (PassingCard)";
            Logger::instance().add(out);
            return true;
        }
    }
    else{
        int hpLoss = 0;
        int expLoss = 0;
        int expGain = 0;
        bool result = this->watsonBattle(2000, 100, -0.1, hpLoss, expLoss, expGain);
        if(result){
            this->bag->insert(robotC->NewItem());
            string out = "Watson caught Criminal";
            Logger::instance().add(out);
            return true;
        }
        else{
            string out = "Watson lost to RobotC (-" + to_string(hpLoss) + " HP, -" + to_string(expLoss) + " EXP)";
            Logger::instance().add(out);
        }
    }

    BaseItem* k = this->bag->get();
    if( k != nullptr){
        k->use(this, nullptr);
    }
    return false;
}
bool Watson::meet(Sherlock *sherlock)
{
    if(sherlock->getBag()->checkItem(PASSING_CARD) and this->bag->checkItem(EXCEMPTION_CARD)){
        while(sherlock->getBag()->checkItem(PASSING_CARD)){ 
            this->bag->insert(sherlock->getBag()->get(PASSING_CARD));
        }
        while(this->bag->checkItem(EXCEMPTION_CARD)){
            sherlock->getBag()->insert(this->bag->get(EXCEMPTION_CARD));
        }
        string out = "Sherlock, Watson traded items.";
        Logger::instance().add(out);
    }
    else{
        string out = "They did not have enough items.";
        Logger::instance().add(out);
    }
    return false;
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////