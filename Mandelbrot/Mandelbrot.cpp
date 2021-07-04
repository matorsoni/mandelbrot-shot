#include <cmath>
#include <fstream>
#include <new>
#include <string>


// Dump RGB buffer as PPM image.
void dumpAsPpm(uint8_t* buffer, int width, int height, const std::string& filename)
{
    std::ofstream ofs;
    ofs.open(filename, std::ios::binary);

    // Write PPM file.
    ofs << "P6\n";
    ofs << width << " " << height << " " << 255 << "\n";
    for (int row = 0; row < height; ++row)
        for (int col = 0; col < width; ++col)
            for (int channel = 0; channel < 3; ++channel) {
                ofs << *(buffer++);
            }

    ofs.close();
    return;
}


// Complex -------------------------------------
struct Complex
{
    double x;
    double y;
};

Complex operator+(const Complex& z1, const Complex& z2)
{
    return Complex{z1.x + z2.x, z1.y + z2.y};
}

Complex square(const Complex& z)
{
    const auto x = z.x;
    const auto y = z.y;
    return Complex{x*x - y*y, 2*x*y};
}

double radius2(const Complex& z)
{
    const auto x = z.x;
    const auto y = z.y;
    return x*x + y*y;
}

void getColor(const Complex& c, uint8_t& r, uint8_t& g, uint8_t& b)
{
    const double r0 = 0.4, r1 = 1.0;
    const double g0 = 0.0, g1 = 1.0;
    const double b0 = 0.0, b1 = 0.0;
    const int MAX_ITER = 200;
    Complex z_n = c;
    // Compute Mandelbrot iteration.
    for (int i = 0; i < MAX_ITER; ++i) {
        z_n = square(z_n) + c;
        // If the radius ever reaches 2, the point does not belong to the set.
        if (radius2(z_n) > 4) {
            const double alpha = static_cast<double>(i) / MAX_ITER;
            r = static_cast<uint8_t>(255 * (r0 * (1.0 - alpha) + r1 * alpha));
            g = static_cast<uint8_t>(255 * (g0 * (1.0 - alpha) + g1 * alpha));
            b = static_cast<uint8_t>(255 * (b0 * (1.0 - alpha) + b1 * alpha));
            return;
        }
    }

    r = 0;
    g = 0;
    b = 0;
}

// Main -------------------------------------
int main()
{
    // Define rectangle on the complex plane which will be used to create the image.
    const Complex center = {-0.6, 0.0};
    double rect_width = 3.0;
    double rect_height = 2.35;
    Complex top_left = center + Complex{-rect_width / 2, rect_height / 2};

    // Create image frame buffer.
    const double aspect_ratio = rect_height / rect_width;
    const int image_resolution = 12e6;
    const int width = static_cast<int>(sqrt(image_resolution / aspect_ratio));
    const int height = static_cast<int>(aspect_ratio * width);
    uint8_t* frame_buffer = new uint8_t[width * height * 3];

    // Loop through each pixel and get the color of the corresponding point on the complex plane.
    auto ptr = frame_buffer;
    const double dx = rect_width / width;
    const double dy = -rect_height / height;
    Complex z = top_left;
    uint8_t r, g, b;
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            getColor(z, r, g, b);
            *(ptr++) = r;
            *(ptr++) = g;
            *(ptr++) = b;

            z.x += dx;
        }

        top_left.y += dy;
        z = top_left;
    }

    dumpAsPpm(frame_buffer, width, height, "Mandelbrot.ppm");

    delete[] frame_buffer;
    return 0;
}
