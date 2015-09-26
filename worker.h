#ifndef CARTWRIGHT_WORKER_H
#define CARTWRIGHT_WORKER_H

#include <gtkmm.h>

#include <atomic>
#include <memory>
#include <vector>

#include "xorshift64.h"

class SimCHCG;

struct color_rgb {
  double red;
  double green;
  double blue;
  double alpha;
};

constexpr color_rgb col_set[] = {
  {1.0000000000000000,0.6901960784313725,0.6235294117647059,1.0000000000000000},
  {0.7137254901960784,0.7882352941176470,0.8901960784313725,1.0000000000000000},
  {0.7372549019607844,0.8784313725490196,0.7098039215686275,1.0000000000000000},
  {0.8431372549019608,0.7176470588235294,0.8549019607843137,1.0000000000000000},
  {1.0000000000000000,0.8000000000000000,0.6352941176470588,1.0000000000000000},
  {1.0000000000000000,1.0000000000000000,0.7176470588235294,1.0000000000000000},
  {1.0000000000000000,0.8078431372549020,0.8980392156862745,1.0000000000000000},
  {0.8352941176470589,0.8352941176470589,0.8352941176470589,1.0000000000000000},
  {0.9921568627450981,0.5294117647058824,0.4470588235294118,1.0000000000000000},
  {0.5647058823529412,0.6862745098039216,0.8313725490196079,1.0000000000000000},
  {0.6039215686274509,0.8156862745098039,0.5686274509803921,1.0000000000000000},
  {0.7647058823529411,0.5803921568627451,0.7843137254901961,1.0000000000000000},
  {1.0000000000000000,0.7019607843137254,0.4588235294117647,1.0000000000000000},
  {1.0000000000000000,1.0000000000000000,0.5725490196078431,1.0000000000000000},
  {0.9960784313725490,0.7098039215686275,0.8470588235294118,1.0000000000000000},
  {0.7568627450980392,0.7568627450980392,0.7568627450980392,1.0000000000000000},
  {0.9529411764705882,0.3568627450980392,0.2784313725490196,1.0000000000000000},
  {0.4078431372549020,0.5882352941176471,0.7764705882352941,1.0000000000000000},
  {0.4627450980392157,0.7490196078431373,0.4313725490196079,1.0000000000000000},
  {0.6823529411764706,0.4431372549019608,0.7098039215686275,1.0000000000000000},
  {1.0000000000000000,0.6000000000000000,0.2745098039215687,1.0000000000000000},
  {1.0000000000000000,1.0000000000000000,0.4117647058823529,1.0000000000000000},
  {0.9843137254901960,0.6117647058823530,0.8000000000000000,1.0000000000000000},
  {0.6784313725490196,0.6784313725490196,0.6784313725490196,1.0000000000000000},
  {0.8941176470588236,0.1019607843137255,0.1098039215686274,1.0000000000000000},
  {0.2156862745098039,0.4941176470588236,0.7215686274509804,1.0000000000000000},
  {0.3019607843137255,0.6862745098039216,0.2901960784313726,1.0000000000000000},
  {0.5960784313725490,0.3058823529411765,0.6392156862745098,1.0000000000000000},
  {1.0000000000000000,0.4980392156862745,0.0000000000000000,1.0000000000000000},
  {1.0000000000000000,1.0000000000000000,0.2000000000000000,1.0000000000000000},
  {0.9686274509803922,0.5058823529411764,0.7490196078431373,1.0000000000000000},
  {0.6000000000000000,0.6000000000000000,0.6000000000000000,1.0000000000000000},
  {0.7019607843137254,0.1176470588235294,0.0980392156862745,1.0000000000000000},
  {0.1882352941176471,0.3921568627450980,0.5647058823529412,1.0000000000000000},
  {0.2509803921568627,0.5372549019607843,0.2352941176470588,1.0000000000000000},
  {0.4705882352941176,0.2470588235294118,0.5019607843137255,1.0000000000000000},
  {0.7803921568627451,0.3960784313725490,0.0509803921568627,1.0000000000000000},
  {0.7803921568627451,0.7764705882352941,0.1803921568627451,1.0000000000000000},
  {0.7568627450980392,0.4000000000000000,0.5843137254901961,1.0000000000000000},
  {0.4705882352941176,0.4705882352941176,0.4705882352941176,1.0000000000000000},
  {0.5215686274509804,0.1176470588235294,0.0823529411764706,1.0000000000000000},
  {0.1568627450980392,0.2901960784313726,0.4156862745098039,1.0000000000000000},
  {0.1960784313725490,0.4000000000000000,0.1843137254901961,1.0000000000000000},
  {0.3490196078431372,0.1921568627450981,0.3725490196078431,1.0000000000000000},
  {0.5764705882352941,0.2980392156862745,0.0666666666666667,1.0000000000000000},
  {0.5725490196078431,0.5647058823529412,0.1568627450980392,1.0000000000000000},
  {0.5529411764705883,0.3019607843137255,0.4313725490196079,1.0000000000000000},
  {0.3490196078431372,0.3490196078431372,0.3490196078431372,1.0000000000000000},
  {0.3450980392156863,0.0980392156862745,0.0705882352941176,1.0000000000000000},
  {0.1215686274509804,0.2000000000000000,0.2784313725490196,1.0000000000000000},
  {0.1450980392156863,0.2666666666666667,0.1333333333333333,1.0000000000000000},
  {0.2352941176470588,0.1372549019607843,0.2470588235294118,1.0000000000000000},
  {0.3803921568627451,0.2039215686274510,0.0666666666666667,1.0000000000000000},
  {0.3764705882352941,0.3686274509803922,0.1215686274509804,1.0000000000000000},
  {0.3607843137254902,0.2078431372549020,0.2862745098039216,1.0000000000000000},
  {0.2352941176470588,0.2352941176470588,0.2352941176470588,1.0000000000000000}
};
constexpr size_t num_alleles = sizeof(col_set)/sizeof(color_rgb);

union cell {
  cell() {
    // Get the color of the parent
    fitness = 1.0;
    constexpr uint64_t color = 10;
    static_assert(color < num_alleles, "Default color is invalid.");
    type = (type & 0xFFFFFFFFFFFFFF00) | color;
  };
  double fitness;
  uint64_t type;

  bool operator<(cell other) {
    return fitness < other.fitness;
  }
};

typedef std::vector<cell> pop_t;

class Worker
{
public:
  Worker(int width, int height, double mu);

  // Thread function.
  void do_work(SimCHCG* caller);

  const pop_t& get_data() const;
  unsigned long long get_gen() const { return gen_; }

  void swap_buffers();

  void stop();

  // Synchronizes access to member data.
  mutable Glib::Threads::Cond sync_;
  mutable Glib::Threads::Mutex mutex_;

private:
  Glib::Timer timer_;

  // Data used by both GUI thread and worker thread.
  std::atomic<bool> go;

  int width_;
  int height_;
  double mu_;
  unsigned long long gen_;

  std::unique_ptr<pop_t> pop_a_;
  std::unique_ptr<pop_t> pop_b_;

  xorshift64 rand;
};

#endif // GTKMM_EXAMPLEWORKER_H