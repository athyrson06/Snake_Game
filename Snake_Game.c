#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 255

void init(void);
void display(void);
void keyPressed(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void rectangle(float x1, float y1, float x2, float y2);
void gameWindow();
void foodSpawn();
void updatebody();
void colisionCheck();
void colisionWallCheck();
void changeDirection(int dir);
void movimentSnake(int dir);
void progressBar();
void snake();
void restartSnake();
void moveSnakeAuto(int n);

float posX1[MAX],posX2[MAX],posY1[MAX],posY2[MAX]; //COORDINATES OF THE SNAKE
float posX1f,posY1f; //INITIAL COORDINATES OF THE FOOD
int game_size; //SIZE OF THE WINDOW
int snake_size; //SIZE OF THE SNAKE
int direction; //DIRECTION OF THE SNAKE (FROM 0 TO 3 -> UP, RIGTH, DOWN, LEFT)
int move; //IF MOVE == 0 GAME IS PAUSED
int speed; //SPEED OF THE GAME
int moved; //IF MOVED == 0 THE SNAKE HASN'T YET CHANGED ITS COORDINATES
int r,g,b; //TO DEFINE THE COLOUR SCHEME OF THE SNAKE

//DEFINES THE KEYS COMMANDS
void keyPressed(unsigned char key, int x, int y){
    switch(key){
    case 'i':
        snake_size++; //CHEAT FOR INCREASING THE SIZE OF THE SNAKE
        break;
    case 's':
        speed = 300; //CHEAT FOR DECREASING THE SPEED OF THE GAME
        break;
    case 'w':
        speed = 100; //CHEAT FOR INCREASING THE SPEED OF THE GAME
        break;
    case 'r':
        r = (-1) * (r - 1); //RED ON/OFF
        break;
    case 'g':
        g = (-1) * (g - 1); //GREEN ON/OFF
        break;
    case 'b':
        b = (-1) * (b - 1); //BLUE ON/OFF
        break;
    case 32: //ENTER KEY
        move = (-1) * (move - 1); //PAUSES/RETURNS THE GAME
        break;
    case 13: //SPACE KEY
        restartSnake(); //RESTARTS THE GAME
        break;
    }

    glutPostRedisplay();
}

//DIRECTIONAL KEYS CHANGES THE DIRECTION OF THE SNAKE
void specialKeys(int key, int x, int y){
    switch(key){
        case GLUT_KEY_UP:
            changeDirection(0);
            break;
        case GLUT_KEY_RIGHT:
            changeDirection(1);
            break;
        case GLUT_KEY_DOWN:
            changeDirection(2);
            break;
        case GLUT_KEY_LEFT:
            changeDirection(3);
            break;
    }
}

//DRAWN A RECTANGLE GIVEN THE THE COORDINATES OF TWO POINTS (x1,y1) AND (x2,y2)
void rectangle(float x1, float y1, float x2, float y2){
    glBegin(GL_POLYGON);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
}

//DRAWN THE WINDOW WHERE THE GAME TAKES PLACE
void gameWindow(){
    int wX1,wY1,wX2,wY2; //VARIABLES TO STORE THE COORDINATES OF THE  WALLS

    wX1 = 0;
    wX2 = (game_size*10) + 10;
    wY1 = 0;
    wY2 = (game_size*10) + 10;

    //WALL
    glColor3ub(124, 133, 255);
    rectangle(wX1,wY1,wX2,wY2);

    //FREE TO PLAY AREA
    glColor3ub(56, 58, 96);
    rectangle(wX1+10,wY1+10,wX2-10,wY2-10);
}

//SPAWNS THE FOOD THROUGH THE FREE TO PLAY AREA
void foodSpawn(){
    //CHECKS IF THE SNAKE'S HEAD IS IN THE SAME COORDINATES AS THE FOOD, IF SO, THEN SPAWNS THE FOOD IN ANOTHER RANDOWN LOCATION
    if((posX1[0] == posX1f)&&(posX2[0] == posX1f+10)&&(posY1[0] == posY1f)&&(posY2[0] == posY1f+10)){
        posX1f = (rand()%(game_size-1))*10+10;
        posY1f = (rand()%(game_size-1))*10+10;
        snake_size++; //INCREASES THE SIZE OF THE SNAKE BY ONE
    }
}

//CHANGES THE COORDINATES OF THE EACH PART OF THE BODY OF THE SNAKE TO THE COORDINATES WHERE THE PREVIUSLY PART WHERE
void updatebody(){
    for(int j = snake_size - 1; j >= 1; j--){
        posX1[j] = posX1[j-1];
        posX2[j] = posX2[j-1];
        posY1[j] = posY1[j-1];
        posY2[j] = posY2[j-1];
    }

    //CHANGES THE VALUE OF VARIABLE MOVED TO INDICATE THE SNAKE HAS CHANGED ITS COORDINATES
    moved = 1;
}

//CHECKS IF THE SNAKES HAS BITED ITSELF :)
void colisionCheck(){
    for(int i = 1; i < snake_size; i++){
        //CHECKS IF THE COORDINATES OF THE HEAD OF THE SNAKE ARE THE SAME COORDINATES OF ANY PART OF ITS BODY
        if((posX1[i] == posX1[0])&&(posX2[i] == posX2[0])&&(posY1[i] == posY1[0])&&(posY2[i] == posY2[0])){
            //CHANGES THE COLOUR OF THE HEAD OF THE SNAKE TO RED
            glColor3f(1.0, 0.0, 0.0);
            rectangle(posX1[0],posY1[0],posX2[0],posY2[0]);
            //THEN STOPS THE GAME
            move = 0;
        }
    }
}

//CHECKS IF THE SNAKE HAVE HIT THE WALLS
void colisionWallCheck(){
    int wX1,wY1,wX2,wY2; //VARIABLES TO STORE THE COORDINATES OF THE FREE TO PLAY AREA

    wX1 = 10;
    wX2 = (game_size*10);
    wY1 = 10;
    wY2 = (game_size*10);

    //CHECKS IF THE SNAKE HEAD IN INSIDE THE FREE TO PLAY AREA
    if((posX1[0] >= wX1)&&(posX2[0] <= wX2)&&(posY1[0] >= wY1)&&(posY2[0] <= wY2)){

    }
    else{
         //CHANGES THE COLOUR OF THE HEAD OF THE SNAKE TO RED
        glColor3f(1.0, 0.0, 0.0);
        rectangle(posX1[0],posY1[0],posX2[0],posY2[0]);
        //THEN STOPS THE GAME
        move = 0;
    }
}

//CHANGES THE DIRECTION OF THE SNAKE
void changeDirection(int dir){
    //WHILE THE GAME IS PAUSED IS NOT POSSIBLE TO CHANGE THE DIRECTION OF THE SNAKE
    //IT IS ALSO NOT POSSIBLE TO INVERT THE DIRECTION OF THE SNAKE
    if((dir%2 != direction%2) && (move != 0) && moved != 0){
        direction = dir;
    }
    //CHANGES MOVED TO 0 TO INDICATES THE SNAKE HASN'T YET UPDATED ITS COORDINATES
    moved = 0;
}

//MOVIMENT THE SNAKE GIVEN A DIRECTION
void movimentSnake(int dir){
    if(dir == 1){ //RIGHT DIRECTION
        updatebody();
        posX1[0] += 10;
        posX2[0] += 10;
    }
    else if(dir == 3){ //LEFT DIRECTION
        updatebody();
        posX1[0] -= 10;
        posX2[0] -= 10;
    }
    else if(dir == 0){ //UP DIRECTION
        updatebody();
        posY1[0] += 10;
        posY2[0] += 10;
    }
    else if(dir == 2){ //DOWN DIRECTION
        updatebody();
        posY1[0] -= 10;
        posY2[0] -= 10;
    }
}

//SHOWS A PROGRESS BAR THAT INCREASES AS THE SIZE OF THE SNEAK INCREASES
void progressBar(){
    int wX1,wY1,wX2,wY2; //VARIABLES TO STORE THE COORDINATES OF THE PROGRESS BAR

    wX1 = (game_size*10) + 10;
    wX2 = (game_size*10) + 20;
    wY1 = 0;
    wY2 = (game_size*10) + 10;

    //DRAWNS AN EMPTY GREEN BAR
    glColor3ub(0, 100, 0);
    rectangle(wX1,wY1,wX2,wY2);

    //DRAWS THE INITIAL PROGRESS BAR THAT AUTO INCREASES IN PROPORTION TO THE SIZE OF THE SNAKE
    glColor3ub(124, 0, 0);
    rectangle(wX1,wY1,wX2,(snake_size-3)*5);

    //SWITCHES TO THE ADVANCED PROGRESS BAR IF THE PLAYER HAVE HIT THE INITIAL QUOTA
    if(snake_size >((game_size*2)+5)){
        glColor3ub(124, 0, 0);
        rectangle(wX1,wY1,wX2,(((game_size*2)+5)-3)*5);
        glColor3ub(124, 0, 124);
        rectangle(wX1,wY1,wX2,(snake_size%((game_size*2)+5))*5);
    }
}

//DRAWNS THE SNAKE
void snake(){
    int colour,sign,j,k; //VARIABLES RELATED TO THE COLOUR OF THE SNAKE BODY

    sign = 1;
    colour = 250;

    //CALLS THE FUCTION TO DRAWN THE PROGRES BAR
    progressBar();

    //DRAWNS THE FOOD
    glColor3ub(255, 229, 0);
    rectangle(posX1f,posY1f,posX1f+10,posY1f+10);

    //DRAWNS THE HEAD OF THE SNAKE
    glColor3ub(150, 150, 0);
    rectangle(posX1[0],posY1[0],posX2[0],posY2[0]);

    //DRAWNS THE BODY OF THE SNAKE
    for(int i = 1; i < snake_size; i++){
        //COLORIZE EACH PART OF THE BODY OF THE SNAKE WITH A SEQUENTIAL COLOUR CREATING A GRADIENT
        k = 10;
        j = i % ((250/k)*2);
        colour = 250 - (j*k);
        if(colour >= 0){
            sign = 1;
            glColor3ub(colour*sign*r,colour*sign*g,colour*sign*b);
        }
        //IF COLOUR IS SMALLER THAN ZERO THE DIRECTION OF THE GRADIENTE IS INVERTED
        else{
            sign = -1;
            glColor3ub(colour*sign*r,colour*sign*g,colour*sign*b);
        }

        rectangle(posX1[i],posY1[i],posX2[i],posY2[i]);
    }


    //CALLS THE FUNCTION TO CHECK IF THE SNAKE HAS BIT ITSELF
    colisionCheck();

    //CALLS THE FUNCTION TO CHECK IF THE SNAKE HAS HIT THE WALLS
    colisionWallCheck();

    //IMPEDE THE SNAKES TO GROW MORE THAN THE MAX VALUE
    if(snake_size >= MAX - 2){
        snake_size--;
    }
}

//START/RESTARTS THE GAME TO THE INITIAL CONDITIONS
void restartSnake(){
    direction = -1; //THE SNAKE WON'T MOVE UNTIL A DIRECTIONAL KEY IS PRESSED
    snake_size = 3; //THE SNAKE STARTS WITH A HEAD AND TWO PARTS OF BODY
    move = 1; //THE GAME STARTS NOT PAUSED
    speed = 100; //DEFINES THE INITIAL SPEED
    moved = -1; //DEFINES THAT THE SNAKE HASN'T MOVED YET

    //INITIATE THE FOOD AT RANDOM COORDINATES INSIDE THE FREE AREA
    posX1f = (rand()%(game_size-1))*10+10;
    posY1f = (rand()%(game_size-1))*10+10;

    //DEFINES THE INITIAL COORDINATES OF THE SNAKE
    for(int i = 0; i < MAX; i++){
        if(i < snake_size){
            posX1[i] = 10;
            posX2[i] = 20;
            posY1[i] = 30 - (10*i);
            posY2[i] = 40 - (10*i);
        }
        else{
            posX1[i] = 0;
            posX2[i] = 0;
            posY1[i] = 0;
            posY2[i] = 0;
        }
    }
}

//CALLBACK FUNCTION THAT ALLOWS THE SNAKE TO MOVE AUTOMATICALLY THROUGH THE FREE TO PLAY AREA
void moveSnakeAuto(int n){
        //CHECKS IF GAME IS PAUSED
        if (move != 0){
            //MOVIMENT THE SNAKE AUTOMATICALLY
            switch(direction){
                case 1:
                    movimentSnake(direction);
                    break;
                case 3:
                    movimentSnake(direction);
                    break;
                case 0:
                    movimentSnake(direction);
                    break;
                case 2:
                    movimentSnake(direction);
                    break;
            }
        }

    //CALLS THE FUNCTION TO SPAWN THE FOOD
    foodSpawn();

    glutPostRedisplay();
    glutTimerFunc(speed, moveSnakeAuto, 1);
}

int main( int argc, char** argv ){
    game_size = 20; //SIZE OF THE GAME WINDOW
    game_size++;

    r = 0; //RED
    g = 1; //GREEN
    b = 0; //BLUE

    char c; //AUXILIAR VARIABLE

    srand(time(NULL));

    //RULES ARE ANNOUNCED BEFORE THE GAME STARTS
    printf("Press Enter to Begin/Restart Game!\n");
    printf("Press Space to Pause/Return Game!\n");
    printf("Press Directional Keys to Change the Direction of the Snake!\n");
    printf("Press UP OR RIGHT KEY to Start the moviment of the Snake!\n");

    //IF ENTER KEY IS PRESSED THE GAME STARTS
    scanf("%c",&c);

    //CALLS THE FUNCTION TO INITIATE THE SNAKE
    restartSnake();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(500,100);
    glutCreateWindow("snake game");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(33, moveSnakeAuto, 1);
    glutKeyboardFunc(keyPressed);
    glutSpecialFunc(specialKeys);
    glutMainLoop();
    return 1;
}

void init(void){
    glClearColor(0.0, 0.0, 0.0, 0.1);
    glOrtho(0, game_size*10+20, 0, game_size*10+10, -1,1);
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT);

    //CALLS THE FUNCTION TO SHOW THE GAME WINDOW
    gameWindow();

    //CALLS THE FUNCTION TO SHOWS THE SNAKE
    snake();

    glFlush();
}
