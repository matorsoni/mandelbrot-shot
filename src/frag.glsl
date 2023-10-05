#version 400 core

out vec4 frag_color;

float WIDTH = 640.0;
float HEIGHT = 480.0;
vec3 C1 = vec3(0.4, 0.0, 0.0);
vec3 C2 = vec3(1.0, 1.0, 0.0);
int MAX_ITER = 200;

// Complex multiplication
vec2 cmul(vec2 a, vec2 b)
{
    return vec2(a.x * b.x - a.y * b.y,
                a.x * b.y + a.y * b.x);
}

// Compute fractal pixel color
// Input: position in C plane
vec3 mandelbrot(vec2 p)
{
    vec2 z_n = p;
    for (int i = 0; i < MAX_ITER; ++i) {
        // Zn+1 = Zn^2 + C
        z_n = cmul(z_n, z_n) + p;

        // Stop condition: radius > 2 guaranteed does not belong to the set
        if (length(z_n) > 2) {
            float t = float(i) / MAX_ITER;
            return (1.0-t) * C1 + t * C2;
        }
    }

    // If reaches here, the point is considered in the set
    return vec3(0.0);
}

void main()
{
    vec2 p_ = gl_FragCoord.xy / vec2(WIDTH, HEIGHT);
    vec2 p = 2.0*p_ - vec2(1.0);
    p.x *= WIDTH / HEIGHT;

    frag_color = vec4(mandelbrot(p), 1.0);
}
