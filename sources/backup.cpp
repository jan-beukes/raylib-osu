#include <raylib.h>
#include <time.h>
#include <raymath.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <sstream>

int main()
{
    const int window_height = 720;
    const int window_width = 1280;
    InitWindow(window_width, window_height, "osu");
    SetTargetFPS(120);
    DisableCursor();

    // Game Variables
    float x = 400.0;
    float y = 300.0;
    int radius = 10;
    float dampening = -0.6;

    float gravity = 490.0; // scale of 1m = 50 pixels
    float impulse = 15000;
    float impulse_time = 0.1;
    float cooldown = 2.0;
    int can_force = 1;
    int low_speed = 20; // when to disable gravity and apply friction

    const char *text = "BALL";
    int font_size = 30;

    Vector2 pos = {x, y};
    Vector2 velocity = Vector2Zero();
    Vector2 accel = {0, gravity};

    double force_start_time;
    double cooldown_start_time;

    int score = 0;

    // Define a Circle struct
    struct Circle
    {
        Vector2 position;
        float radius;
        Color color;
        float elapsed_time;
        float max_time;
    };

    // Create a list of circles
    std::vector<Circle> circles;

    // Clear the circles vector
    circles.clear();

    // Read circle data from a text file
    std::ifstream circleFile("circles.txt");
    std::string line;
    while (std::getline(circleFile, line))
    {
        std::istringstream iss(line);
        float x, y, time;
        if (!(iss >> x >> y >> time))
        {
            break;
        } // Error handling

        Circle circle;
        circle.position = {x, y};
        circle.radius = 30.0f; // Set a default radius
        circle.color = (Color){
            (unsigned char)GetRandomValue(50, 255),
            (unsigned char)GetRandomValue(50, 255),
            (unsigned char)GetRandomValue(50, 255),
            0 // Start with alpha = 0
        };
        circle.elapsed_time = 0.0f;
        circle.max_time = time;
        circles.push_back(circle);
    }

    // Generate random circles
    // for (int i = 0; i < 10; i++)
    // {
    //     Circle circle;
    //     circle.position = {(float)GetRandomValue(0, window_width), (float)GetRandomValue(0, window_height)};
    //     circle.radius = 30; //(float)GetRandomValue(20, 50);
    //     circle.color = (Color){
    //         (unsigned char)GetRandomValue(50, 255),
    //         (unsigned char)GetRandomValue(50, 255),
    //         (unsigned char)GetRandomValue(50, 255),
    //         0 // Start with alpha = 0
    //     };
    //     circle.elapsed_time = 0.0f;
    //     circle.max_time = (float)GetRandomValue(3, 6); // Time until fully opaque
    //     circles.push_back(circle);
    // }

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // Update circles
        for (auto it = circles.begin(); it != circles.end();)
        {
            it->elapsed_time += dt;
            if (it->elapsed_time >= it->max_time)
            {
                it = circles.erase(it);
            }
            else
            {
                // Update alpha based on elapsed time
                float alpha = (it->elapsed_time / it->max_time) * 255;
                it->color.a = (unsigned char)alpha;
                ++it;
            }
        }

        // Check for click and remove circles
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mouse = GetMousePosition();
            for (auto it = circles.begin(); it != circles.end();)
            {
                if (CheckCollisionPointCircle(mouse, it->position, it->radius))
                {
                    it = circles.erase(it);
                    score += 10;
                }
                else
                {
                    ++it;
                }
            }
        }

        // Drawing
        BeginDrawing();
        {
            ClearBackground((Color){30, 30, 30, 255});

            // Draw all circles
            for (const auto &circle : circles)
            {
                // Draw the circle
                DrawCircleV(circle.position, circle.radius, circle.color);

                // Calculate ring radius
                float t = circle.elapsed_time / circle.max_time;
                float ring_radius = circle.radius + circle.radius * 0.5f * (1.0f - t); // Starts at 1.5x radius

                // Draw the ring
                DrawCircleLines((int)circle.position.x, (int)circle.position.y, ring_radius, WHITE);
            }

            // Draw score
            DrawText(TextFormat("My Score: %d", score), 10, 10, 20, WHITE);

            // Draw mouse cursor
            Vector2 mouse = GetMousePosition();
            DrawCircle((int)mouse.x, (int)mouse.y, radius, WHITE);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
