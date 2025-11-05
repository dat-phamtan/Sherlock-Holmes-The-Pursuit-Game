#include <iostream>
#include <raylib.h>
#include "study_in_pink2.h"
#include <raymath.h>

using namespace std;
Color backgroundColor = {230, 216, 181, 255};
Color white = {30, 19, 9, 255};
Color itemPlaceHolderColor = {215, 195, 160, 255};
Color blurBlack = {0, 0, 0, 110};
Color normal = itemPlaceHolderColor;
Color hover = {162, 128, 97, 255};
Color transparent = {0, 0, 0, 0};


static int COUNT = 0;
int numRows = 0;
int numCols = 0;
int cellSize = 840 / 20;
int offset = 70;
double lastUpdateTime = 0;

enum GameState {
    PLAYING = 0,
    OVER,
    MAIN_MENU, 
};


bool eventTriggered(double interval){
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval){
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

bool DrawButton(Rectangle rec, string text, int fontSize, Color normal, Color hover){
    Vector2 mouse = GetMousePosition();
    bool isHover = CheckCollisionPointRec(mouse, rec);
    bool isClicked = isHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    // DrawRectangleRec(rec, isHover ? hover : normal);
    DrawRectangleRounded(rec, 0.25, 10, isHover ? hover : normal);
    DrawRectangleRoundedLinesEx(rec, 0.25, 10, 5, BLACK);

    int textWidth = MeasureText(text.c_str(), fontSize);
    int textX = rec.x + (rec.width - textWidth) / 2;
    int textY = rec.y + (rec.height - fontSize) / 2;
    DrawText(text.c_str(), textX, textY, fontSize, BLACK);

    return isClicked;
}

bool DrawClickedArea(Rectangle rec){
    Vector2 mouse = GetMousePosition();
    bool isHover = CheckCollisionPointRec(mouse, rec);
    bool isClicked = isHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    DrawRectangleRounded(Rectangle{rec.x, rec.y, rec.width, rec.height}, 0.25, 10, isHover ? transparent : blurBlack);

    return isClicked;
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
    GameState state = GameState::MAIN_MENU;
    // GameState state = GameState::PLAYING;
    // GameState state = GameState::OVER;
    string stopCase = "";
    int caseFlag = 0;
    bool doublePlayerFlag = true;

    Image wall;
    Image fake_wall;
    Image sherlock;
    Image watson;
    Image criminal;
    Image robotC;
    Image robotS;
    Image robotW;
    Image robotSW;
    Image magicBook;
    Image energyDrink;
    Image firstAid;
    Image excemptionCard;
    Image passingCard;
    Image sherlockDie;
    Image sherlockThinking;
    Image sherlockCatch;
    Image watsonDie;
    Image watsonConfuse;
    Image watsonCatch;
    // Image logo;
    Image gameOverBG;
    Image menuBG;
    Image singlePlayer;
    Image doublePlayer;

    Texture2D wallTexture;
    Texture2D fakeWallTexture;
    Texture2D sherlockTexture;
    Texture2D watsonTexture;
    Texture2D criminalTexture;
    Texture2D robotCTexture;
    Texture2D robotSTexture;
    Texture2D robotWTexture;
    Texture2D robotSWTexture;
    Texture2D magicBookTexture;
    Texture2D energyDrinkTexture;
    Texture2D firstAidTexture;
    Texture2D excemptionCardTexture;
    Texture2D passingCardTexture;
    Texture2D sherlockDieTexture;
    Texture2D sherlockThinkingTexture;
    Texture2D sherlockCatchTexture;
    Texture2D watsonDieTexture;
    Texture2D watsonConfuseTexture;
    Texture2D watsonCatchTexture;
    // Texture2D logoTexture;
    Texture2D gameOverBGTexture;
    Texture2D menuBGTexture;
    Texture2D singlePlayerTexture;
    Texture2D doublePlayerTexture;


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
        magicBook = LoadImage("resources/magicbook.png");
        energyDrink = LoadImage("resources/energydrink.png");
        firstAid = LoadImage("resources/firstaid.png");
        excemptionCard = LoadImage("resources/excemptioncard.png");
        passingCard = LoadImage("resources/passingcard.png");
        sherlockDie = LoadImage("resources/sherlock_die.png");
        sherlockThinking = LoadImage("resources/sherlock_thinking.png");
        sherlockCatch = LoadImage("resources/sherlock_catch_criminal.png");
        watsonDie = LoadImage("resources/watson_die.png");
        watsonConfuse = LoadImage("resources/watson_confuse.png");
        watsonCatch = LoadImage("resources/watson_catch_criminal.png");
        // logo = LoadImage("resources/logo.png");
        gameOverBG = LoadImage("resources/GameOverBG.png");
        menuBG = LoadImage("resources/backgroundMenu.png");
        singlePlayer = LoadImage("resources/singleplayer.png");
        doublePlayer = LoadImage("resources/doubleplayers.png");

        

        wallTexture = LoadTextureFromImage(wall);
        fakeWallTexture = LoadTextureFromImage(fake_wall);
        sherlockTexture = LoadTextureFromImage(sherlock);
        watsonTexture = LoadTextureFromImage(watson);
        criminalTexture = LoadTextureFromImage(criminal);
        robotCTexture = LoadTextureFromImage(robotC);
        robotSTexture = LoadTextureFromImage(robotS);
        robotWTexture = LoadTextureFromImage(robotW);
        robotSWTexture = LoadTextureFromImage(robotSW);
        magicBookTexture = LoadTextureFromImage(magicBook);
        energyDrinkTexture = LoadTextureFromImage(energyDrink);
        firstAidTexture = LoadTextureFromImage(firstAid);
        excemptionCardTexture = LoadTextureFromImage(excemptionCard);
        passingCardTexture = LoadTextureFromImage(passingCard);
        sherlockDieTexture = LoadTextureFromImage(sherlockDie);
        sherlockThinkingTexture = LoadTextureFromImage(sherlockThinking);
        sherlockCatchTexture = LoadTextureFromImage(sherlockCatch);
        watsonDieTexture = LoadTextureFromImage(watsonDie);
        watsonConfuseTexture = LoadTextureFromImage(watsonConfuse);
        watsonCatchTexture = LoadTextureFromImage(watsonCatch);
        // logoTexture = LoadTextureFromImage(logo);
        gameOverBGTexture = LoadTextureFromImage(gameOverBG);
        menuBGTexture = LoadTextureFromImage(menuBG);
        singlePlayerTexture = LoadTextureFromImage(singlePlayer);
        doublePlayerTexture = LoadTextureFromImage(doublePlayer);


        UnloadImage(wall);
        UnloadImage(fake_wall);
        UnloadImage(sherlock);
        UnloadImage(watson);
        UnloadImage(criminal);
        UnloadImage(robotC);
        UnloadImage(robotS);
        UnloadImage(robotW);
        UnloadImage(robotSW);
        UnloadImage(magicBook);
        UnloadImage(energyDrink);
        UnloadImage(firstAid);
        UnloadImage(excemptionCard);
        UnloadImage(passingCard);
        UnloadImage(sherlockDie);
        UnloadImage(sherlockThinking);
        UnloadImage(sherlockCatch);
        UnloadImage(watsonDie);
        UnloadImage(watsonConfuse);
        UnloadImage(watsonCatch);
        // UnloadImage(logo);
        UnloadImage(gameOverBG);
        UnloadImage(menuBG);
        UnloadImage(singlePlayer);
        UnloadImage(doublePlayer);
    

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
        UnloadTexture(magicBookTexture);
        UnloadTexture(energyDrinkTexture);
        UnloadTexture(firstAidTexture);
        UnloadTexture(excemptionCardTexture);
        UnloadTexture(passingCardTexture);
        UnloadTexture(sherlockDieTexture);
        UnloadTexture(sherlockThinkingTexture);
        UnloadTexture(sherlockCatchTexture);
        UnloadTexture(watsonDieTexture);
        UnloadTexture(watsonConfuseTexture);
        UnloadTexture(watsonCatchTexture);
        // UnloadTexture(logoTexture);
        UnloadTexture(gameOverBGTexture);
        UnloadTexture(menuBGTexture);
        UnloadTexture(singlePlayerTexture);
        UnloadTexture(doublePlayerTexture);

        delete program;
    };

    void GameInit(string configFile){
        
        this->configFile = configFile;
        program = new StudyPinkProgram(configFile);
        numCols = this->program->map->getNumCols();
        numRows = this->program->map->getNumRows();
        COUNT = 0;
        Logger::instance().add("Game started");
        // state = GameState::PLAYING;
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

    void DrawCharacterPicture(){
        float sTarget = (float)220/sherlockTexture.height;
        float wTarget = (float)220/watsonTexture.height;

        DrawTextureEx(sherlockTexture, (Vector2){(float)(1600), (float)(75)}, 0.0, sTarget, WHITE);
        DrawTextureEx(watsonTexture, (Vector2){(float)(1600), (float)(300)}, 0.0, wTarget, WHITE);
    };

    void DrawLogBoard(){
        DrawRectangleLinesEx(Rectangle{1220, (float)offset - 5 + 470, 520, 380}, 5, white);
        DrawText("Event log:", 1240, 550, 30, white);
        int logSize = Logger::instance().size();
        for(int i = 0; i < logSize ; i++){
            string log = to_string(i) + ": " + Logger::instance().get(i);
            DrawText(log.c_str(), 1240, 590 + i*27, 20, white);
        }
    };

    void DrawDetailBoard() {
        // DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)(cellSize*cellCount) + 10 , (float)(cellSize*cellCount) + 10 }, 5, white);
        DrawRectangleLinesEx(Rectangle{1220, (float)offset - 5, 520, 460}, 5, white);
        string Round = "Round: " + to_string(COUNT);
        DrawText(Round.c_str(), 1240, 75, 30, white);

        DrawText("Sherlock HP", 1240, 115, 20, white);
        DrawHpMpBar(1390, 115, 185, 20, program->sherlock->getHP(), 500, true);
        DrawText("Sherlock EXP", 1240, 145, 20, white);
        DrawHpMpBar(1390, 145, 185, 20, program->sherlock->getEXP(), 3000, true);
        string sherlockBag = "Bag : " + to_string(program->sherlock->getBag()->getSize()) + "/" + to_string(6);
        DrawText(sherlockBag.c_str(), 1240, 175, 20, white);
        DrawItemBox(1250, 205, 55, 50, 6);
        DrawItemInBag(1250, 205, 55, 50, program->sherlock->getBag(), 0, 6);


        DrawText("Watson HP", 1240, 295, 20, white);
        DrawHpMpBar(1390, 295, 185, 20, program->watson->getHP(), 700, true);
        DrawText("Watson EXP", 1240, 325, 20, white);
        DrawHpMpBar(1390, 325, 185, 20, program->watson->getEXP(), 2500, true);
        string watsonBag = "Bag: " + to_string(program->watson->getBag()->getSize()) + "/" + to_string(12);
        DrawText(watsonBag.c_str(), 1240, 355, 20, white);
        DrawItemBox(1250, 385, 55, 50, 6);
        DrawItemBox(1250, 440, 55, 50, 6);
        DrawItemInBag(1250, 385, 55, 50, program->watson->getBag(), 0, 6);
        if (program->watson->getBag()->getSize() > 6){
            DrawItemInBag(1250, 440, 55, 50, program->watson->getBag(), 6, program->watson->getBag()->getSize());
        }

        DrawCharacterPicture();
    };

    void DrawItemBox(int x, int y, int width, int height, int numItems) {
        for (int i = 0; i < numItems; i++){
            DrawRectangleRounded(Rectangle{(float)x + width*i, (float)y, (float)width - 5, (float)height}, 0.25, 10, itemPlaceHolderColor);
            DrawRectangleRoundedLinesEx(Rectangle{(float)x + width*i, (float)y, (float)width - 5, (float)height}, 0.25, 10, 1, BLACK);
        }
    };

    void DrawItemInBag(int x, int y, int width, int height, BaseBag *bag, int from=0, int to=6) {
        for (int i = from; i < to; i++){
            ItemType it = bag->getItemType(i);
            if (it == MAGIC_BOOK){
                Rectangle source = {0.0f, 0.0f, (float)magicBook.width, (float)magicBook.height};
                Rectangle dest = { (float)(x + width*(i%6) + 4), (float)(y + 5), (float)width - 10, (float)height - 10 };
                Vector2 origin = { 0.0f, 0.0f };
                DrawTexturePro(magicBookTexture, source, dest, origin, 0.0 , WHITE);
            }
            else if (it == ENERGY_DRINK){
                Rectangle source = {0.0f, 0.0f, (float)energyDrink.width, (float)energyDrink.height};
                Rectangle dest = { (float)(x + width*(i%6) + 3), (float)(y + 5), (float)width - 10, (float)height - 10 };
                Vector2 origin = { 0.0f, 0.0f };
                DrawTexturePro(energyDrinkTexture, source, dest, origin, 0.0 , WHITE);
            }
            else if (it == FIRST_AID){
                Rectangle source = {0.0f, 0.0f, (float)firstAid.width, (float)firstAid.height};
                Rectangle dest = { (float)(x + width*(i%6) + 2), (float)(y + 4), (float)width - 10, (float)height - 10 };
                Vector2 origin = { 0.0f, 0.0f };
                DrawTexturePro(firstAidTexture, source, dest, origin, 0.0 , WHITE);
            }
            else if (it == EXCEMPTION_CARD){
                Rectangle source = {0.0f, 0.0f, (float)excemptionCard.width, (float)excemptionCard.height};
                // float ratio = excemptionCard.height/excemptionCard.width;
                Rectangle dest = { (float)(x + width*(i%6)), (float)(y + 9), (float)width - 6, (float)height - 10 - 8};
                Vector2 origin = { 0.0f, 0.0f };
                DrawTexturePro(excemptionCardTexture, source, dest, origin, 0.0 , WHITE);
            }
            else if (it == PASSING_CARD){
                Rectangle source = {0.0f, 0.0f, (float)passingCard.width, (float)passingCard.height};
                // float ratio = passingCard.width/passingCard.height;
                Rectangle dest = { (float)(x + width*(i%6) + 6), (float)(y), (float)width - 15, (float)height - 2 };
                Vector2 origin = { 0.0f, 0.0f };
                DrawTexturePro(passingCardTexture, source, dest, origin, 0.0 , WHITE);
            }
        }
    };

    void DrawGameOverScreen(){

    }

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
            if(this->doublePlayerFlag && (i == 1 || i == 2)){
                continue;
            }
            if(!this->doublePlayerFlag && i==1){
                continue;
            }
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
    
    void CheckStopCase(){
        if (this->program->sherlock->getHP()  <= 10){
            this->stopCase = "Sherlock out of HP";
            this->caseFlag = 1;
        }
        else if (this->program->sherlock->getEXP() <= 10){
            this->stopCase = "Sherlock is exhausted";
            this->caseFlag = 2;
        }
        else if (this->program->watson->getHP() <= 10){
            this->stopCase = "Watson out of HP";
            this->caseFlag = 3;
        }
        else if (this->program->watson->getEXP() <= 10){
            this->stopCase = "Watson is exhausted";
            this->caseFlag = 4;
        }
        else if (this->program->criminal->getCurrentPosition().isEqual(this->program->sherlock->getCurrentPosition())){
            this->stopCase = "Sherlock caught the Criminal";
            this->caseFlag = 5;
        }
        else if (this->program->criminal->getCurrentPosition().isEqual(this->program->watson->getCurrentPosition())){
            this->stopCase = "Watson caught the Criminal";
            this->caseFlag = 6;
        }
        else{
            this->stopCase = "";
            this->caseFlag = 0;
        }
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
        CheckStopCase();
        state = GameState::OVER;
        // delete program;
        // program = new StudyPinkProgram(configFile);
        // COUNT = 0;
        Logger::instance().clear();
        // GameInit(configFile);
        sherlockDiraction = "";
        watsonDiraction = "";
        
    };
};



int main () {
    cout << "Starting the game..." << endl;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1800, 970, "Sherlock Holmes: The Pursuit Game");
    SetTargetFPS(60);
    // Texture2D background = LoadTexture("resources/treasure.jpeg");
    string configFile = "input/input.txt";
    Game game = Game();
    game.GameInit(configFile);


    

    while(WindowShouldClose() == false) {
    
        //Drawing
        BeginDrawing();

        if (game.state == GameState::PLAYING){
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
            if(game.doublePlayerFlag){
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
            }

            if (game.doublePlayerFlag && IsKeyPressed(KEY_SPACE) && game.sherlockDiraction != "" && game.watsonDiraction != ""){
                COUNT++;
                game.SherlockMove();
                game.WatsonMove();
                game.Update();
                
            }else if (!game.doublePlayerFlag && IsKeyPressed(KEY_SPACE) && game.sherlockDiraction != ""){
                COUNT++;
                game.SherlockMove();
                game.Update();
            }
            

            ClearBackground(backgroundColor);
            DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)(cellSize*numCols) + 10 , (float)(cellSize*numRows) + 10 }, 5, white);
            DrawText("Sherlock Holmes: The Potrait Game", offset - 5, 20, 40, white);
            
            game.DrawMap();
            game.DrawDetailBoard();
            game.DrawLogBoard();
            if (game.sherlockDiraction != ""){
                game.DrawSherlockDiractionArrow();
            }
            if (game.watsonDiraction != ""){
                game.DrawWatsonDiractionArrow();
            }
            game.DrawCharacters();
            

        }
        else if(game.state == GameState::OVER){
            ClearBackground(backgroundColor);
            DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)(cellSize*numCols) + 10 , (float)(cellSize*numRows) + 10 }, 5, white);
            DrawText("Sherlock Holmes: The Potrait Game", offset - 5, 20, 40, white);
            
            game.DrawMap();
            game.DrawDetailBoard();
            game.DrawLogBoard();
            DrawRectangle(0,0,1800,970,blurBlack);
           
            // Color boxBackground = {135, 89, 49, 120};
            // DrawRectangleRoundedLinesEx(Rectangle{300, (float)offset - 5, (float)(1800 - 2*300), (float)(970 - 2*offset)}, 0.25, 10, 5, white);
            // DrawRectangleRounded(Rectangle{300, (float)offset - 5, (float)(1800 - 2*300), (float)(970 - 2*offset)}, 0.25, 10, backgroundColor);
            float bgTarget = (float)1200 / game.gameOverBGTexture.width;
            DrawTextureEx(game.gameOverBGTexture, (Vector2){(float)(300), (float)offset - 5}, 0.0, bgTarget, WHITE);

            // DrawText("Sherlock Holmes: The Potrait Game", offset - 5, 20, 40, white);
            DrawText("GAME OVER", 670, 170, 80, white);

            int textWidth = MeasureText(game.stopCase.c_str(), 50);
            int x = (1800 - textWidth)/2;
            DrawText(game.stopCase.c_str(), x + 25, 630, 50, white);

            bool playAgain = DrawButton(Rectangle{600, 700, 150, 50}, "Play again", 20, normal, hover);
            bool quit = DrawButton(Rectangle{1110, 700, 150, 50}, "Quit game", 20, normal, hover);

            if (playAgain){ 
                delete game.program;
                game.GameInit(configFile);
                game.state = GameState::PLAYING;
                
            }
            if (quit){
                // delete game.program;
                break;
            }

            Texture2D caseTexture;
            
            if (game.caseFlag == 1){//complete
                caseTexture = game.sherlockDieTexture;
                float sTarget = (float)350/caseTexture.height;
                DrawTextureEx(caseTexture, (Vector2){(float)(650), (float)(240)}, 0.0, sTarget, WHITE);
            } 
            else if (game.caseFlag == 2){//complete
                caseTexture = game.sherlockThinkingTexture;
                float sTarget = (float)400/caseTexture.height;
                DrawTextureEx(caseTexture, (Vector2){(float)(780), (float)(240)}, 0.0, sTarget, WHITE);
            } 
            else if (game.caseFlag == 3){//complete
                caseTexture = game.watsonDieTexture;
                float sTarget = (float)400/caseTexture.height;
                DrawTextureEx(caseTexture, (Vector2){(float)(620), (float)(230)}, 0.0, sTarget, WHITE);
            }
            else if (game.caseFlag == 4){
                caseTexture = game.watsonConfuseTexture;
                float sTarget = (float)400/caseTexture.height;
                DrawTextureEx(caseTexture, (Vector2){(float)(780), (float)(250)}, 0.0, sTarget, WHITE);
            } 
            else if (game.caseFlag == 5){//complete
                caseTexture = game.sherlockCatchTexture;
                float sTarget = (float)370/caseTexture.height;
                DrawTextureEx(caseTexture, (Vector2){(float)(740), (float)(250)}, 0.0, sTarget, WHITE);
            }
            else if (game.caseFlag == 6){//complete
                caseTexture = game.watsonCatchTexture;
                float sTarget = (float)470/caseTexture.height;
                DrawTextureEx(caseTexture, (Vector2){(float)(760), (float)(220)}, 0.0, sTarget, WHITE);
            }

        }
        else if (game.state == GameState::MAIN_MENU){
            float bgTarget = (float)1800 / game.menuBGTexture.width;
            DrawTextureEx(game.menuBGTexture, (Vector2){0.0f, 0.0f}, 0.0, bgTarget, WHITE);
            float singleTarget = (float)78/ game.singlePlayerTexture.height;
            DrawTextureEx(game.singlePlayerTexture, (Vector2){480,570}, 0.0, singleTarget, WHITE);
            bool singleP = DrawClickedArea(Rectangle{495, 583, 314, 56});
            if(singleP){
                game.doublePlayerFlag = false;
                game.state = GameState::PLAYING;
            }

            float doubleTarget = (float)70 / game.doublePlayerTexture.height;
            DrawTextureEx(game.doublePlayerTexture, (Vector2){498, 670}, 0.0, doubleTarget, WHITE);
            bool doubleP = DrawClickedArea(Rectangle{507, 679, 294, 56});
            if(doubleP){
                game.doublePlayerFlag = true;
                game.state = GameState::PLAYING;
            }
        }

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