#include <iostream>
#include <raylib.h>
#include "study_in_pink2.h"
#include <raymath.h>

using namespace std;
Color dark = {230, 216, 181, 255};
Color white = {30, 19, 9, 255};

static int COUNT = 0;
// int cellCount = 20;
int numRows = 0;
int numCols = 0;
int cellSize = 840 / 20;
int offset = 70;

double lastUpdateTime = 0;

bool eventTriggered(double interval){
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval){
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}


void DrawHpMpBar(int x, int y, int width, int height, int currentHP, int maxHP, bool showText)
{
    if (maxHP <= 0) maxHP = 1;
    // clamp currentHP
    if (currentHP < 0) currentHP = 0;
    if (currentHP > maxHP) currentHP = maxHP;

    float ratio = (float)currentHP / (float)maxHP;
    int fillW = (int)(ratio * width + 0.5f);

    // background (empty bar)
    DrawRectangle(x, y, width, height, WHITE);

    // color thay đổi theo mức HP (xanh -> vàng -> đỏ)

    Color fillColor;
    if (ratio > 0.6f) fillColor = GREEN;
    else if (ratio > 0.3f) fillColor = YELLOW;
    else fillColor = RED;

    // phần đầy
    if (fillW > 0) DrawRectangle(x, y, fillW, height, fillColor);

    // viền
    DrawRectangleLines(x, y, width, height, BLACK);

    // chữ phần trăm ở giữa
    if (showText) {
        char text[32];
        int perc = (int)(ratio * 100.0f + 0.5f);
        sprintf(text, "%d / %d (%d%%)", currentHP, maxHP, perc);
        int txtW = MeasureText(text, height - 4); // dùng height làm size chữ
        int txtX = x + (width - txtW) / 2;
        int txtY = y + (height - (height - 4)) / 2 - 1;
        DrawText(text, txtX, y + (height - (height - 4))/2, height - 4, BLACK);
    }
}

//Read Config File


//Sherlock
//Watson
//Criminal
//RobotC
//RobotS
//RobotW
//RobotSW
//Wall
//Path
//FakeWall
//MagicBook
//EnergyDrink
//FirstAid
//ExcemptionCard
//PassingCard


class Game{
public:
    StudyPinkProgram *program;
    string configFile;
    bool running = true;
    string sherlockDiraction = "";
    string watsonDiraction = "";

    Image wall;
    Image fake_wall;
    Image sherlock;
    Image watson;
    Image criminal;
    Image robotC;
    Image robotS;
    Image robotW;
    Image robotSW;

    Texture2D wallTexture;
    Texture2D fakeWallTexture;
    Texture2D sherlockTexture;
    Texture2D watsonTexture;
    Texture2D criminalTexture;
    Texture2D robotCTexture;
    Texture2D robotSTexture;
    Texture2D robotWTexture;
    Texture2D robotSWTexture;


    Game(){

        wall = LoadImage("resources/wall.jpeg");
        fake_wall = LoadImage("resources/fakewall.jpeg");
        sherlock = LoadImage("resources/sherlock.png");
        watson = LoadImage("resources/watson.png");
        criminal = LoadImage("resources/criminal.png");
        robotC = LoadImage("resources/robotC.png");
        robotS = LoadImage("resources/robotS.png");
        robotW = LoadImage("resources/robotW.png");
        robotSW = LoadImage("resources/robotSW.png");


        wallTexture = LoadTextureFromImage(wall);
        fakeWallTexture = LoadTextureFromImage(fake_wall);
        sherlockTexture = LoadTextureFromImage(sherlock);
        watsonTexture = LoadTextureFromImage(watson);
        criminalTexture = LoadTextureFromImage(criminal);
        robotCTexture = LoadTextureFromImage(robotC);
        robotSTexture = LoadTextureFromImage(robotS);
        robotWTexture = LoadTextureFromImage(robotW);
        robotSWTexture = LoadTextureFromImage(robotSW);

        UnloadImage(wall);
        UnloadImage(fake_wall);
        UnloadImage(sherlock);
        UnloadImage(watson);
        UnloadImage(criminal);
        UnloadImage(robotC);
        UnloadImage(robotS);
        UnloadImage(robotW);
        UnloadImage(robotSW);

    };

    ~Game(){
        
        UnloadTexture(wallTexture);
        UnloadTexture(fakeWallTexture);
        UnloadTexture(sherlockTexture);
        UnloadTexture(watsonTexture);
        UnloadTexture(criminalTexture);
        UnloadTexture(robotCTexture);
        UnloadTexture(robotSTexture);
        UnloadTexture(robotWTexture);
        UnloadTexture(robotSWTexture);
        delete program;
    };

    void GameInit(string configFile){
        
        this->configFile = configFile;
        program = new StudyPinkProgram(configFile);
        numCols = this->program->map->getNumCols();
        numRows = this->program->map->getNumRows();
        COUNT = 0;  
    };

    void DrawMap() {
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {

                Rectangle source = {0.0f, 0.0f, (float)wall.width, (float)wall.height};
                Rectangle dest = { (float)(offset + j * cellSize), (float)(offset + i * cellSize), (float)cellSize, (float)cellSize };
                Vector2 origin = { 0.0f, 0.0f };
                ElementType et = this->program->map->getElementType(i, j);

                if (et == PATH) {
                    // Rectangle aPath = Rectangle{offset + j*cellSize, offset + i*cellSize, (float)cellSize, (float)cellSize};
                    // DrawRectangleRounded(aPath, 0.5, 10, WHITE);
                    // //DrawRectangle(offset + j * cellSize, offset + i * cellSize, cellSize, cellSize, DARKGRAY);
                }
                else if (et == WALL) {   
                    DrawTexturePro(wallTexture, source, dest, origin, 0.0 , WHITE);
                }
                else if (et == FAKE_WALL) {
                    DrawTexturePro(fakeWallTexture, source, dest, origin, 0.0 , WHITE);
                }
            }
        }
        for (int i = 0; i <= numRows; i++) {
            DrawLine(offset, offset + i*cellSize, offset + cellSize*numCols, offset + i*cellSize, white);
            
        }
        for (int i = 0; i < numCols; i++) {
            DrawLine(offset + i*cellSize, offset, offset + i*cellSize, offset + cellSize*numRows, white);
        }
    };

    void DrawACharacter(MovingObject *obj) {
        int r = obj->getCurrentPosition().getRow();
        int c = obj->getCurrentPosition().getCol();
        if (obj->getObjectType() == SHERLOCK) {
            float target = (float)cellSize/sherlockTexture.height;
            // DrawTexturePro(sherlockTexture, source, dest, origin, 0.0 , WHITE);
            DrawTextureEx(sherlockTexture, (Vector2){(float)(offset + c * cellSize + 10), (float)(offset + r * cellSize )}, 0.0, target, WHITE);
        }
        else if (obj->getObjectType() == WATSON) {
            float target = (float)cellSize/watsonTexture.height;
            // DrawTexturePro(watsonTexture, source, dest, origin, 0.0 , WHITE);
            DrawTextureEx(watsonTexture, (Vector2){(float)(offset + c * cellSize + 10), (float)(offset + r * cellSize)}, 0.0, target, WHITE);
        }
        else if (obj->getObjectType() == CRIMINAL) {
            float target = (float)cellSize/criminalTexture.height;
            // DrawTexturePro(criminalTexture, source, dest, origin, 0.0 , WHITE);
            DrawTextureEx(criminalTexture, (Vector2){(float)(offset + c * cellSize ), (float)(offset + r * cellSize)}, 0.0, target, WHITE);
        }
        else if (obj->getObjectType() == ROBOT){
            Robot *robot = dynamic_cast<Robot*>(obj);
            if(robot->getType() == 0){
                float target = (float)cellSize/robotCTexture.height;
                DrawTextureEx(robotCTexture, (Vector2){(float)(offset + c * cellSize + 7), (float)(offset + r * cellSize)}, 0.0, target, WHITE);
            }
            else if(robot->getType() == 1){
                float target = (float)cellSize/robotSTexture.height;
                DrawTextureEx(robotSTexture, (Vector2){(float)(offset + c * cellSize + 7), (float)(offset + r * cellSize)}, 0.0, target, WHITE);
            }
            else if(robot->getType() == 2){
                float target = (float)cellSize/robotWTexture.height;
                DrawTextureEx(robotWTexture, (Vector2){(float)(offset + c * cellSize + 7), (float)(offset + r * cellSize)}, 0.0, target, WHITE);
            }
            else if(robot->getType() == 3){
                float target = (float)cellSize/robotSWTexture.height;
                DrawTextureEx(robotSWTexture, (Vector2){(float)(offset + c * cellSize + 7), (float)(offset + r * cellSize)}, 0.0, target, WHITE);
            }
        }
    };

    void DrawCharacters() {
        for (int i = 0; i < program->arr_mv_objs->size(); i++) {
            DrawACharacter(program->arr_mv_objs->get(i));
        }
    };

    void DrawDetailBoard() {
        // DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)(cellSize*cellCount) + 10 , (float)(cellSize*cellCount) + 10 }, 5, white);
        DrawRectangleLinesEx(Rectangle{1350, (float)offset - 5, 385, 300}, 5, white);
        string Round = "Round: " + to_string(COUNT);
        DrawText(Round.c_str(), 1370, 75, 30, white);

        DrawText("Sherlock HP", 1370, 115, 20, white);
        DrawHpMpBar(1520, 117, 150, 15, program->sherlock->getHP(), 500, true);
        DrawText("Sherlock EXP", 1370, 135, 20, white);
        DrawHpMpBar(1520, 137, 150, 15, program->sherlock->getEXP(), 400, true);
        string sherlockBag = "Bag: " + program->sherlock->getBag()->str();
        DrawText(sherlockBag.c_str(), 1370, 155, 20, white);



        DrawText("Watson HP", 1370, 215, 20, white);
        DrawHpMpBar(1520, 217, 150, 15, program->watson->getHP(), 400, true);
        DrawText("Watson EXP", 1370, 235, 20, white);
        DrawHpMpBar(1520, 237, 150, 15, program->watson->getEXP(), 400, true);
        string watsonBag = "Bag: " + program->watson->getBag()->str();
        DrawText(watsonBag.c_str(), 1370, 255, 20, white);



        // string Sher = "SHERLOCK HP: " + to_string(program->sherlock->getHP()) + " | EXP: " + to_string(program->sherlock->getEXP());
        // string sherBag = "Bag: " + program->sherlock->getBag()->str();
        // string Wat = "WATSON HP: " + to_string(program->watson->getHP()) + " | EXP: " + to_string(program->watson->getEXP());
        // DrawText(Round.c_str(), 1370, 75, 30, white);
        // DrawText(Sher.c_str(), 1370, 115, 20, white);
        // DrawText(Wat.c_str(), 1370, 135, 20, white);
    };

    void SherlockMove() {
        if (sherlockDiraction == "D") {
            Position nextPos = program->sherlock->getCurrentPosition();
            nextPos.setRow(nextPos.getRow() + 1);
            if (program->map->isValid(nextPos, program->sherlock)) {
                program->sherlock->setPos(nextPos);
            }
        }
        else if (sherlockDiraction == "U") {
            Position nextPos = program->sherlock->getCurrentPosition();
            nextPos.setRow(nextPos.getRow() - 1);
            if (program->map->isValid(nextPos, program->sherlock)) {
                program->sherlock->setPos(nextPos);
            }
        }
        else if (sherlockDiraction == "L") {
            Position nextPos = program->sherlock->getCurrentPosition();
            nextPos.setCol(nextPos.getCol() - 1);
            if (program->map->isValid(nextPos, program->sherlock)) {
                program->sherlock->setPos(nextPos);
            }
        }
        else if (sherlockDiraction == "R") {
            Position nextPos = program->sherlock->getCurrentPosition();
            nextPos.setCol(nextPos.getCol() + 1);
            if (program->map->isValid(nextPos, program->sherlock)) {
                program->sherlock->setPos(nextPos);
            }
        }
        this->program->arr_mv_objs->checkMeet(1);
        if (this->CheckStop()){
            GameOver();
        }
        sherlockDiraction = "";
    };

    void WatsonMove() {
        if (watsonDiraction == "D") {
            Position nextPos = program->watson->getCurrentPosition();
            nextPos.setRow(nextPos.getRow() + 1);
            if (program->map->isValid(nextPos, program->watson)) {
                program->watson->setPos(nextPos);
            }
        }
        else if (watsonDiraction == "U") {
            Position nextPos = program->watson->getCurrentPosition();
            nextPos.setRow(nextPos.getRow() - 1);
            if (program->map->isValid(nextPos, program->watson)) {
                program->watson->setPos(nextPos);
            }
        }
        else if (watsonDiraction == "L") {
            Position nextPos = program->watson->getCurrentPosition();
            nextPos.setCol(nextPos.getCol() - 1);
            if (program->map->isValid(nextPos, program->watson)) {
                program->watson->setPos(nextPos);
            }
        }
        else if (watsonDiraction == "R") {
            Position nextPos = program->watson->getCurrentPosition();
            nextPos.setCol(nextPos.getCol() + 1);
            if (program->map->isValid(nextPos, program->watson)) {
                program->watson->setPos(nextPos);
            }
        }
        this->program->arr_mv_objs->checkMeet(2);
        if (this->CheckStop()){
            GameOver();
        }
        watsonDiraction = "";
    };


    void Update() {
        for(int i = 0; i < this->program->arr_mv_objs->size(); i++){
            if (i != 1 && i != 2){
                if (this->CheckStop()){
                    GameOver();
                    break;
                }
                this->program->arr_mv_objs->get(i)->move();
                this->program->arr_mv_objs->checkMeet(i);
                if (this->CheckStop()){
                    GameOver();
                    break;
                }
                MovingObject *robot = nullptr;
                if (this->program->arr_mv_objs->get(i)->getObjectType() == MovingObjectType::CRIMINAL)
                {
                    robot = Robot::create(this->program->arr_mv_objs->size(), this->program->map, this->program->criminal, this->program->sherlock, this->program->watson);
                }
                if (robot != nullptr)
                {
                    if (this->program->criminal->getCount() % 3 == 0 && this->program->criminal->getCount() > 0)
                    {
                        this->program->arr_mv_objs->add(robot);
                    }
                    else
                    {
                        delete robot;
                    }
                }
            }
        }
    };

    bool CheckStop(){
        if (this->program->sherlock->getHP() <= 10 || this->program->watson->getHP() <= 10){
            return true;
        }
        else if (this->program->sherlock->getEXP() <= 10 || this->program->watson->getEXP() <= 10){
            return true;
        }
        else if (this->program->criminal->getCurrentPosition().isEqual(this->program->sherlock->getCurrentPosition()) || this->program->criminal->getCurrentPosition().isEqual(this->program->watson->getCurrentPosition())){
            return true;
        }
        return false;
    };

    void DrawSherlockDiractionArrow() {
        int sherlockRow = program->sherlock->getCurrentPosition().getRow();
        int sherlockCol = program->sherlock->getCurrentPosition().getCol();
        Rectangle dest;
        if (sherlockDiraction == "D") {
            dest = { (float)(offset + sherlockCol * cellSize + 5),(float)(offset + (sherlockRow + 1) * cellSize + 5), (float)cellSize - 10, (float)cellSize - 10};
        };
        if (sherlockDiraction == "U") {
            dest = { (float)(offset + sherlockCol * cellSize + 5),(float)(offset + (sherlockRow - 1) * cellSize + 5), (float)cellSize - 10, (float)cellSize - 10};
        };
        if (sherlockDiraction == "L") {
           dest = { (float)(offset + (sherlockCol - 1) * cellSize + 5),(float)(offset + sherlockRow * cellSize + 5), (float)cellSize - 10, (float)cellSize - 10};
        };
        if (sherlockDiraction == "R") {
            dest = { (float)(offset + (sherlockCol + 1) * cellSize + 5),(float)(offset + sherlockRow * cellSize + 5), (float)cellSize - 10, (float)cellSize - 10};
        };
        DrawRectangleLinesEx(dest, 5, RED);
    }

    void DrawWatsonDiractionArrow() {
        int watsonRow = program->watson->getCurrentPosition().getRow();
        int watsonCol = program->watson->getCurrentPosition().getCol();
        Rectangle dest;
        if (watsonDiraction == "D") {
            dest = { (float)(offset + watsonCol * cellSize + 7),(float)(offset + (watsonRow + 1) * cellSize + 7), (float)cellSize - 14, (float)cellSize - 14};
        };
        if (watsonDiraction == "U") {
            dest = { (float)(offset + watsonCol * cellSize + 7),(float)(offset + (watsonRow - 1) * cellSize + 7), (float)cellSize - 14, (float)cellSize - 14};
        };
        if (watsonDiraction == "L") {
           dest = { (float)(offset + (watsonCol - 1) * cellSize + 7),(float)(offset + watsonRow * cellSize + 7), (float)cellSize - 14, (float)cellSize - 14};
        };
        if (watsonDiraction == "R") {
            dest = { (float)(offset + (watsonCol + 1) * cellSize + 7),(float)(offset + watsonRow * cellSize + 7), (float)cellSize - 14, (float)cellSize - 14};
        };
        DrawRectangleLinesEx(dest, 5, BLUE);
    }

    bool checkValidMove(string dir, string character) {
        int row, col;
        if (character == "Sherlock") {
            row = program->sherlock->getCurrentPosition().getRow();
            col = program->sherlock->getCurrentPosition().getCol();
        }
        else if (character == "Watson") {
            row = program->watson->getCurrentPosition().getRow();
            col = program->watson->getCurrentPosition().getCol();
        }
        if (dir == "D") {
            return this->program->map->isValid(Position(row + 1, col), program->sherlock);
        }
        else if (dir == "U") {
            return this->program->map->isValid(Position(row - 1, col), program->sherlock);
        }
        else if (dir == "L") {
            return this->program->map->isValid(Position(row, col - 1), program->sherlock);
        }
        else if (dir == "R") {
            return this->program->map->isValid(Position(row, col + 1), program->sherlock);
        }
        return false;
    };

    void GameOver() {
        delete program;
        // program = new StudyPinkProgram(configFile);
        // COUNT = 0;
        GameInit(configFile);
        sherlockDiraction = "";
        watsonDiraction = "";
    };
};



int main () {
    cout << "Starting the game..." << endl;
    InitWindow(1800, 970, "Sherlock Holmes: The Pursuit Game");
    SetTargetFPS(60);
    // Texture2D background = LoadTexture("resources/treasure.jpeg");
    string configFile = "input/input.txt";
    Game game = Game();
    game.GameInit(configFile);

    

    while(WindowShouldClose() == false) {
    
        //Drawing
        BeginDrawing();

        //ControlSherlock
        if (IsKeyPressed(KEY_DOWN) && game.checkValidMove("D", "Sherlock")){
            game.sherlockDiraction = "D";
        } 
        if (IsKeyPressed(KEY_UP) && game.checkValidMove("U", "Sherlock")){
            game.sherlockDiraction = "U";
        }
        if (IsKeyPressed(KEY_LEFT) && game.checkValidMove("L", "Sherlock")){
            game.sherlockDiraction = "L";
        }
        if (IsKeyPressed(KEY_RIGHT) && game.checkValidMove("R", "Sherlock")){
            game.sherlockDiraction = "R";
        }

        //ControlWatson
        if (IsKeyPressed(KEY_S) && game.checkValidMove("D", "Watson")){
            game.watsonDiraction = "D";
        } 
        if (IsKeyPressed(KEY_W) && game.checkValidMove("U", "Watson")){
            game.watsonDiraction = "U";
        }
        if (IsKeyPressed(KEY_A) && game.checkValidMove("L", "Watson")){
            game.watsonDiraction = "L";
        }
        if (IsKeyPressed(KEY_D) && game.checkValidMove("R", "Watson")){
            game.watsonDiraction = "R";
        }

        if (IsKeyPressed(KEY_SPACE) && game.sherlockDiraction != "" && game.watsonDiraction != ""){
            COUNT++;
            game.SherlockMove();
            game.WatsonMove();
            game.Update();
            
        }


        ClearBackground(dark);
        DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)(cellSize*numCols) + 10 , (float)(cellSize*numRows) + 10 }, 5, white);
        //DrawText("Sherlock Holmes: The Potrait Game", offset - 5, 20, 40, white);
        
        game.DrawMap();
        game.DrawDetailBoard();
        if (game.sherlockDiraction != ""){
            game.DrawSherlockDiractionArrow();
        }
        if (game.watsonDiraction != ""){
            game.DrawWatsonDiractionArrow();
        }
        game.DrawCharacters();
        


        EndDrawing();
        
    }
    CloseWindow();
    cout << "Game closed!" << endl;
    return 0;
}

// Chỉnh logic kiểm tra gặp mặt
// Vẽ túi đồ cho Sherlock và Watson
// Vẽ khung log các sự kiện xảy ra
// Vẽ thêm map