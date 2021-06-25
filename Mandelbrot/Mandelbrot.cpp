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
    auto x = z.x;
    auto y = z.y;
    return Complex{x*x - y*y, 2*x*y};
}


// Main -------------------------------------
int main()
{
    RgbFrame frame;
    frame.width = 600;
    frame.height = 400;
    frame.buffer = new uint8_t[frame.width * frame.height * 3];

    auto ptr = frame.buffer;
    for (int row = 0; row < frame.height; ++row)
        for (int col = 0; col < frame.width; ++col) {
            *(ptr++) = 255;
            *(ptr++) = 0;
            *(ptr++) = 0;
        }

    dumpAsPpm(frame.buffer, frame.width, frame.height, "test.ppm");

    delete[] frame.buffer;
    return 0;
}
