#include <iostream>
#include <raylib.h>
using namespace std;

Color green = Color{38,185,154,255};//here color is an data type in the raylib library
Color dark_green = Color{20,160,133,255};//here first we enter the rgb value for which the color will be obtained
Color light_green = Color{129,204,184,155};//and then we will enter the transprancy
Color yellow = Color{243,213,91,255};

int player_score = 0; // globalising the scores of the player and the cpu
int cpu_score = 0;

class Ball
{

public:
    float x, y;
    int speed_x, speed_y;
    int radius;
    void draw_ball()
    {                                    // this function will takecare of teh figure of the ball
        DrawCircle(x, y, radius, yellow); // here we enter the center coordinates and then radius and then colour
    }

    void update_ball()
    { // this function will take care of the movement of the ball
        x += speed_x;
        y += speed_y;

        // here make sure the equality does not hold well it might glitch out of the screen due to the different speed or pixel
        if (y + radius >= GetScreenHeight() || y - radius <= 0) // basically rough idea of the collision with the wall
        {                                                       // the function getscreenheight() is from the raylib lib which automatically returns the screen height we decided
            speed_y *= -1;                                      // this will make sure that the ball will not leave the screen
        }
        if (x + radius >= GetScreenWidth())
        {
            cpu_score++;
            resetball();
        }

        if (x - radius <= 0) // basically rough idea of the collision with the wall
        {                    // the function getscreenwidth() is from the raylib lib which automatically returns the screen width we decided
            player_score++;  // this will make sure that the ball will not leave the screen
            resetball();
        }
    }

    void resetball()//this function will make sure that if the function once goes out then it will come and get reset again
    {
        x= GetScreenWidth()/2;
        y= GetScreenHeight()/2;

        int speed_choices[2] = {-1,1};//here the function getrandomvalue will choose an random value from the given option
        speed_x *= speed_choices[GetRandomValue(0,1)];
        speed_y *= speed_choices[GetRandomValue(0,1)];
    }
};

class paddle
{
protected:
    void limitmoment()
    {
        if (y <= 0) // the below two function are used to make sure the paddle does not leave the screen
        {
            y = 0;
        }
        if (y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;

    void draw_paddle()
    {
        // DrawRectangle(x, y, width, height, WHITE);  //this is for the proper random shape
        DrawRectangleRounded(Rectangle{x,y,width,height},0.8,0,WHITE);//arguments are rec,roundness,segment,color
    }

    void update()
    {
        if (IsKeyDown(KEY_UP)) // these function will take any keyboard input from the user and detect it
        {
            y = y - speed;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            y = y + speed;
        }

        limitmoment();
    }
};

// now for creating paddle which will be used by the computer will be using inheritence
class cpupaddle : public paddle
{
public:
    void update(int ball_y)
    {

        if (y + height / 2 > ball_y) // this will be so that the ball center must match with the center of the paddle
        {                            // which will make sure that the paddle move automatically
            y = y - speed;
        }
        if (y + height / 2 <= ball_y)
        {
            y = y + speed;
        }

        limitmoment();
    }
};

Ball ball;
paddle player;
cpupaddle cpu;

int main()
{
    cout << "starting the game" << endl;

    // creating an black screen for our game and game loop
    const int screen_width = 1280; // here using the const since i will not be changing these variables from now onwards
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "my pong game! "); // this takes three variable size of the screen which will appear
    // add the title which will be written on the top of the screen
    SetTargetFPS(60); // this decides how fast will our game run or how fast game tun
    // if here we do not mention the speed then the computer will try to run the game as fast as it can

    // instantiating the variables of the object
    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;

    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = screen_height / 2 - player.height / 2;
    cpu.speed = 6;

    while (WindowShouldClose() == false) // here the function returns true whenever someone presses esc or the close icon on the window
    {
        BeginDrawing(); // this creates an black canves so that we can draw over objects on that

        // updating part of the game loop(movement part of the game)
        ball.update_ball();
        player.update();
        cpu.update(ball.y);

        // checking for collisions with the paddles and continuing our game
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})) // be very careful of the the capital letters in the function
        {                                                                                                                              // and here the function takes the following arguments respectively
            ball.speed_x *= -1;
        }
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})) // be very careful of the the capital letters in the function
        {                                                                                                                  // and here the function takes the following arguments respectively
            ball.speed_x *= -1;
        }
        // below is the complete drawing part of the game
        ClearBackground(dark_green); // this means clear the whole background and just leave black colour screen
        // without this any movement in the program will leave back their traces and will look really messsed up
        ball.draw_ball();
        // DrawRectangle(10,screen_height/2 - 60,25,120,WHITE);//here coordinates then width and height and colour
        cpu.draw_paddle();
        DrawCircle(screen_width/2,screen_height/2,150,light_green);
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
        player.draw_paddle();

        //printing the score on the screen
        DrawText(TextFormat("%i",cpu_score),screen_width / 4 -20,20,80,WHITE);//arguments are text,xpos,ypos,fontsize,colour.
        DrawText(TextFormat("%i",player_score),3*screen_width / 4 -20,20,80,WHITE);
        //here be careful with the cpu_format is int but in the case of text it must be an textformate
        //so for doing so we can use the function from the raylib library

        EndDrawing(); // this function endes the canvas which was created initialy
    }

    CloseWindow(); // since we created an screen then at the end we must destroy the screen as well
    return 0;
}