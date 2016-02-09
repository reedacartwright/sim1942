#ifndef CARTWRIGHT_WORKER_H
#define CARTWRIGHT_WORKER_H

#include <gtkmm.h>

#include <atomic>
#include <memory>
#include <vector>
#include <set>

#include <boost/timer/timer.hpp>

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
  {0.2352941176470588,0.2352941176470588,0.2352941176470588,1.0000000000000000},

  /* black for null cells */
  {0,0,0,1.0000000000000000},
  {0,0,0,1.0000000000000000}
};
constexpr size_t num_colors = sizeof(col_set)/sizeof(color_rgb);
constexpr size_t num_alleles = num_colors-2;
constexpr size_t null_allele = num_colors-1;

#define CELL_TYPE_MASK UINT64_C(0xFF)
#define CELL_FITNESS_MASK (~CELL_TYPE_MASK)

static_assert(null_allele < num_colors && null_allele <= CELL_TYPE_MASK , "Null allele is invalid.");

union cell {
    cell() {
        fitness = 1.0;
        constexpr uint64_t color = 10;
        static_assert(color < num_alleles, "Default color is invalid.");
        type = (type & CELL_FITNESS_MASK) | (color & CELL_TYPE_MASK);
    };
    double fitness;
    uint64_t type;

    bool is_null() const {
        return (color() == null_allele);
    }
    bool is_fertile() const {
        return (color() < null_allele-1);
    }
    void toggle_on() {
        type = (type & CELL_FITNESS_MASK) | null_allele;
    }
    void toggle_off() {
        type = (type & CELL_FITNESS_MASK) | (null_allele-1);
    }
    void toggle() {
        if(is_null()) {
            toggle_off();
        } else {
            toggle_on();
        }
    }

    uint64_t color() const {
        return type & CELL_TYPE_MASK;
    }

    bool operator<(cell other) {
        return fitness < other.fitness;
    }
};

typedef std::vector<cell> pop_t;

class Worker
{
public:
    Worker(int width, int height, double mu, int delay=0);

    // Thread function.
    void do_work(SimCHCG* caller);

    std::pair<pop_t,unsigned long long> get_data();

    void swap_buffers();

    void stop();

    // Synchronizes access to member data.
    void do_next_generation();
    void do_clear_nulls();

    void toggle_cell(int x, int y, bool on);

protected:
    void apply_toggles();

private:
    Glib::Timer timer_;

    std::atomic<bool> go_{false};

    int width_;
    int height_;
    double mu_;
    unsigned long long gen_;
    int delay_;

    std::unique_ptr<pop_t> pop_a_;
    std::unique_ptr<pop_t> pop_b_;

    xorshift64 rand;

    Glib::Threads::Cond sync_;
    Glib::Threads::Mutex sync_mutex_, toggle_mutex_;
    Glib::Threads::RWLock data_lock_;

    bool next_generation_{false};
    bool clear_all_nulls_{false};

    typedef std::map<std::pair<int,int>,bool> toggle_map_t;
    toggle_map_t toggle_map_;

    typedef std::set<std::pair<int,int>> null_cells_t;
    null_cells_t null_cells_;
};

#endif // GTKMM_EXAMPLEWORKER_H