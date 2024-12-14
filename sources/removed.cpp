Check force Timers if (!can_force)
{
    if (GetTime() - force_start_time >= impulse_time)
    {
        force_start_time = 0.0;
        accel.x = 0, accel.y = gravity;
    }
    if (GetTime() - cooldown_start_time >= cooldown)
    {
        can_force = 1;
    }
}
Force if ((IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) && can_force)
{
    gravity = 490;
    force_start_time = GetTime();
    cooldown_start_time = GetTime();
    // Direction to mouse
    Vector2 mouse = Vector2Subtract(GetMousePosition(), pos);
    Vector2 dir = Vector2Normalize(mouse);
    float impulse_scale = Vector2Length(mouse) / window_height;
    Vector2 force = Vector2Scale(dir, impulse * impulse_scale);
    accel = Vector2Add(accel, force);
    can_force = 0;
}
velocity = Vector2Add(velocity, Vector2Scale(accel, dt));
Colision if (pos.x + radius >= window_width && velocity.x > 0)
    velocity.x *= dampening;
if (pos.x - radius <= 0 && velocity.x < 0)
    velocity.x *= dampening;
if (pos.y + radius >= window_height && velocity.y > 0)
    velocity.y *= dampening;
if (pos.y - radius <= 0 && velocity.y < 0)
    velocity.y *= dampening;
// Friction
if (pos.y + radius >= window_height - 3 && abs(velocity.y) < low_speed)
{
    int friction = 100;
    velocity.x = velocity.x > 0 ? velocity.x - friction * dt : velocity.x + friction * dt;
}
pos.x += velocity.x * dt;
pos.y += velocity.y * dt;
char debug[64];
sprintf(debug, "speed\n\n%.0f", Vector2Length(velocity));
DrawText(debug, 5, 5, 32, WHITE);
DrawText("Cooldown", window_width - MeasureText("Cooldown", 32) - 5, 5, 32, WHITE);
float ring_prog = can_force ? 1 : (GetTime() - cooldown_start_time) / cooldown;
DrawRing((Vector2){window_width - 80, 80}, 20, 40, 0, (ring_prog * 360), 20, ORANGE);
Line int triangle_len = 15;
Vector2 mouse_dir = Vector2Normalize(Vector2Subtract(mouse, pos));
DrawLineEx(pos, mouse, 4, WHITE);
Vector2 mouse_to_point = Vector2Rotate(mouse_dir, (-7 * PI) / 6);
mouse_to_point = Vector2Scale(mouse_to_point, triangle_len);
Vector2 point1 = Vector2Add(mouse, mouse_to_point);
mouse_to_point = Vector2Rotate(mouse_dir, (7 * PI) / 6);
mouse_to_point = Vector2Scale(mouse_to_point, triangle_len);
Vector2 point2 = Vector2Add(mouse, mouse_to_point);
DrawTriangle(point1, Vector2Add(mouse, Vector2Scale(mouse_dir, 5)), point2, WHITE);
int offset = MeasureText(text, font_size) / 2;
DrawText(text, pos.x - offset, pos.y, font_size, WHITE);
