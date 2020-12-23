#include <iostream>
#include <optional>
using namespace std;

struct Image {
  virtual ~Image() = default;
  virtual void draw() = 0;
};

class Bitmap : public Image {
 public:
  explicit Bitmap(string_view filename) : filename_{filename} { cout << "Loading image from " << filename << "\n"; }
  void draw() override { cout << "Drawing image " << filename_ << "\n"; }

 private:
  string filename_;
};

class LazyBitmap : public Image {
 public:
  explicit LazyBitmap(string_view filename) : filename_{filename} {}
  void draw() override {
    if (!bmp_)
      bmp_ = Bitmap{filename_};
    bmp_.value().draw();
  }

 private:
  string filename_;
  optional<Bitmap> bmp_{nullopt};
};

void draw_image(Image& img) {
  cout << "About to draw the image\n";
  img.draw();
  cout << "Done drawing the image\n";
}

void test() {
  LazyBitmap img{ "pokemon.png" };
  draw_image(img);
}

int main() {
  test();
  return 0;
}