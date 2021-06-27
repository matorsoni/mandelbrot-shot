#include <fstream>
#include <new>
#include <string>

// RgbFrame -------------------------------------
struct RgbFrame
{
    uint8_t* buffer;
    int width;
    int height;
};

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

void getColor(const Complex& z, uint8_t& r, uint8_t& g, uint8_t& b)
{
    const double alpha = radius2(z);
    if (alpha > 1.0) {
        r = 0;
        g = 0;
        b = 0;
        return;
    }

    const double r0 = 1.0, r1 = 1.0;
    const double g0 = 0.0, g1 = 1.0;
    const double b0 = 1.0, b1 = 0.0;

    r = static_cast<uint8_t>(255 * (r0 * (1.0 - alpha) + r1 * alpha));
    g = static_cast<uint8_t>(255 * (g0 * (1.0 - alpha) + g1 * alpha));
    b = static_cast<uint8_t>(255 * (b0 * (1.0 - alpha) + b1 * alpha));
}

// Main -------------------------------------
int main()
{
    // Create image frame buffer.
    const int width = 800;
    const int height = 800;
    RgbFrame frame;
    frame.width = width;
    frame.height = height;
    frame.buffer = new uint8_t[width * height * 3];

    // Define rectangle on the complex plane which will be used to create the image.
    double aspect_ratio = static_cast<double>(height) / width;
    Complex top_left = {-2.0, 2.0};
    double rect_width = 4.0;
    double rect_height = aspect_ratio * rect_width;

    // Loop through each pixel and get the color of the corresponding point on the complex plane.
    auto ptr = frame.buffer;
    double dx = rect_width / frame.width;
    double dy = -rect_height / frame.height;
    Complex z = top_left;
    uint8_t r = 255, g = 0, b = 255;
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

    dumpAsPpm(frame.buffer, width, height, "test.ppm");

    delete[] frame.buffer;
    return 0;
}
