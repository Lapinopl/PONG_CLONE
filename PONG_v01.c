// PONG CLONE WRITTEN IN SDL Version 0.1
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <time.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HIGHT 720
#define GAME_SPEED 100

int main(void)
{

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("ERROR INITILAZING SDL: %s\n",SDL_GetError());
        return 1;
    }

    SDL_Window * game_window = SDL_CreateWindow("PONG 0.1",SDL_WINDOWPOS_CENTERED,
                                                SDL_WINDOWPOS_CENTERED,
                                                WINDOW_WIDTH,WINDOW_HIGHT,0);

    if(!game_window)
    {
        printf("ERROR CREATING THE WINDOW: %s\n",SDL_GetError());
        SDL_Quit();
        return 2;
    }
    SDL_Renderer* my_renderer = SDL_CreateRenderer(game_window,0,SDL_RENDERER_ACCELERATED |
                                     SDL_RENDERER_PRESENTVSYNC);

    if (!my_renderer)
    {
        printf("ERROR CREATING THE RENDERER: %s\n",SDL_GetError());
        SDL_DestroyWindow(game_window);
        SDL_Quit();
        return 3;
    }

    SDL_Surface* ball = IMG_Load("ASSETS/Ball.png");

    if (!ball)
    {
        printf("Error creating surface: %s",SDL_GetError());
        SDL_DestroyRenderer(my_renderer);
        SDL_DestroyWindow(game_window);
        SDL_Quit();
        return 4;

    }

    SDL_Surface* paddle = IMG_Load("ASSETS/paddle.png");

     if (!paddle)
    {
        printf("Error creating surface: %s",SDL_GetError());
        SDL_DestroyRenderer(my_renderer);
        SDL_DestroyWindow(game_window);
        SDL_Quit();
        return 4;
    }

    SDL_Texture* ball_texture = SDL_CreateTextureFromSurface(my_renderer,ball);
    SDL_Texture* paddle_texture = SDL_CreateTextureFromSurface(my_renderer,paddle);
    SDL_FreeSurface(ball);
    SDL_FreeSurface(paddle);
    if(!ball_texture || !paddle_texture)
    {

        printf("Error creating textures: %s",SDL_GetError());
        SDL_DestroyRenderer(my_renderer);
        SDL_DestroyWindow(game_window);
        SDL_Quit();
        return 5;
    }

   SDL_Rect ball_rect, paddle1_rect, paddle2_rect;
   int paddle_h, paddle_w;

   SDL_QueryTexture(ball_texture,NULL,NULL,&ball_rect.w, &ball_rect.h);
   ball_rect.h = ball_rect.w /= 4;                                                   // Ball is a square, so it's easy to scale down.
   SDL_QueryTexture(paddle_texture,NULL,NULL,&paddle_w,&paddle_h);
   paddle1_rect.h = paddle2_rect.h = paddle_h/4;                                     // Paddles are not square so scaling down is done in two lines.
   paddle1_rect.w = paddle2_rect.w = paddle_w/8;
    // Position the object in the play-field
    float ball_x_pos, ball_y_pos, ball_y_vel, ball_x_vel, paddle1_x_pos, paddle1_y_pos,
    paddle2_x_pos, paddle2_y_pos;

    paddle1_y_pos = paddle2_y_pos = (WINDOW_HIGHT - paddle1_rect.h)/2;              // Put both paddles in the centre of the y axis(height)
    paddle1_x_pos = paddle1_rect.w ;                                                //  Put first paddle one length from the left screen edge.
    paddle2_x_pos = WINDOW_WIDTH - ((paddle2_rect.w)*2) ;                            // Second one one one length from right screen edge
    paddle1_rect.y = paddle2_rect.y = paddle2_y_pos;                               // Apply the y coordinates to the actual rects.

    paddle1_rect.x = paddle1_x_pos;                                                // Update the actual position
    paddle2_rect.x = paddle2_x_pos;

    ball_y_pos = (WINDOW_HIGHT - ball_rect.h)/2;
    ball_x_pos = (WINDOW_WIDTH - ball_rect.w)/2;
    ball_rect.x = (int)ball_x_pos;
    ball_rect.y = (int)ball_y_pos;


    SDL_Event game_event;
    int window_close = 0;
    int paddle1_up, paddle1_down, paddle2_up, paddle2_down;
    paddle1_up = paddle1_down = paddle2_up = paddle2_down = 0;
    // Random number to set ball starting direction
    time_t t;
    srand(time(&t));
    int dir = (rand()%2);
    if (dir) ball_x_vel = GAME_SPEED/20;                                        // Which way will the ball go.
    else ball_x_vel = -GAME_SPEED/20;
    if (!dir) ball_y_vel = -GAME_SPEED/20;
    else ball_y_vel = GAME_SPEED/20;
    while (!window_close)
    {
        while(SDL_PollEvent(&game_event))
        {
            if(game_event.type == SDL_QUIT || game_event.key.keysym.scancode
               == SDL_SCANCODE_ESCAPE)
            {
                window_close = 1;
            }

           switch (game_event.type){                                                // indentation is super inconsistent. I need to work on that.

            case SDL_KEYDOWN:

                    switch(game_event.key.keysym.scancode){

                        case SDL_SCANCODE_W:
                            paddle1_up = 1;
                            break;
                        case SDL_SCANCODE_S:
                            paddle1_down = 1;
                            break;
                        case SDL_SCANCODE_UP:
                            paddle2_up = 1;
                            break;
                        case SDL_SCANCODE_DOWN:
                            paddle2_down = 1;
                            break;


                    }break;

            case SDL_KEYUP:

                    switch (game_event.key.keysym.scancode){
                        case SDL_SCANCODE_W:
                            paddle1_up = 0;
                            break;
                        case SDL_SCANCODE_S:
                            paddle1_down = 0;
                            break;
                        case SDL_SCANCODE_UP:
                            paddle2_up = 0;
                            break;
                        case SDL_SCANCODE_DOWN:
                            paddle2_down = 0;
                            break;
                    }break;

           }

        }


        if(paddle1_down && !paddle1_up) paddle1_y_pos += GAME_SPEED / 10;           // Paddles Position updates.
        if(!paddle1_down && paddle1_up) paddle1_y_pos -= GAME_SPEED / 10;

         if(paddle2_down && !paddle2_up) paddle2_y_pos += GAME_SPEED / 10;
        if(!paddle2_down && paddle2_up) paddle2_y_pos -= GAME_SPEED / 10;

        if (paddle1_y_pos <= 0) paddle1_y_pos = 0;                                  // Border checks!
        if (paddle1_y_pos >= WINDOW_HIGHT - paddle1_rect.h) paddle1_y_pos =
        WINDOW_HIGHT - paddle1_rect.h;
        if (paddle2_y_pos <= 0 ) paddle2_y_pos = 0;
        if (paddle2_y_pos >= WINDOW_HIGHT - paddle2_rect.h) paddle2_y_pos =
            WINDOW_HIGHT - paddle2_rect.h;





        // Ball logic
        if(ball_x_pos <= 0)
        {
            ball_x_pos = (WINDOW_WIDTH - ball_rect.w)/2;
            ball_x_vel = -ball_x_vel;
            SDL_Delay(500);
        }
        else if (ball_x_pos >= WINDOW_WIDTH - ball_rect.w)
        {
            ball_x_pos = (WINDOW_WIDTH - ball_rect.w)/2;
            ball_x_vel = -ball_x_vel;
            SDL_Delay(500);
        }

        if((ball_x_pos < paddle1_x_pos + paddle1_rect.w) && (ball_x_pos > paddle1_x_pos))                                   // Took me like 30min to figure out that I'm missing ball size compensation
        {

            if(ball_y_pos + ball_rect.h  > paddle1_y_pos && ball_y_pos < paddle1_y_pos + paddle1_rect.h)
            {
                ball_x_vel = -ball_x_vel;
            }
        }
        if((ball_x_pos + ball_rect.w >= paddle2_x_pos) && (ball_x_pos + ball_rect.w <= paddle2_x_pos + paddle2_rect.w))
        {

            if((ball_y_pos+ ball_rect.h > paddle2_y_pos) && (ball_y_pos < paddle2_y_pos + paddle2_rect.h ))
            {
                ball_x_vel = -ball_x_vel;
            }
        }

        if(ball_y_pos <= 0)
        {
            ball_y_pos = 0;
            ball_y_vel = -ball_y_vel;
        }

        else if(ball_y_pos >= WINDOW_HIGHT - ball_rect.h)
        {
            ball_y_pos = WINDOW_HIGHT - ball_rect.h;
            ball_y_vel = -ball_y_vel;
        }



        ball_x_pos += ball_x_vel;
        ball_y_pos += ball_y_vel;

        paddle1_rect.y = paddle1_y_pos;
        paddle2_rect.y = paddle2_y_pos;
        ball_rect.x = ball_x_pos;
        ball_rect.y = ball_y_pos;


    SDL_RenderClear(my_renderer);
    SDL_RenderCopy(my_renderer,ball_texture,NULL,&ball_rect);
    SDL_RenderCopy(my_renderer,paddle_texture,NULL,&paddle1_rect);
    SDL_RenderCopy(my_renderer,paddle_texture,NULL,&paddle2_rect);
    SDL_RenderPresent(my_renderer);
    SDL_Delay(16);







    }



    SDL_DestroyRenderer(my_renderer);
    SDL_DestroyWindow(game_window);
    SDL_Quit();

    return 0;

}
