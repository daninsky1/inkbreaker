#pragma once
/*
* Tries to mimic very porly css properties.
* This is not a real css implementation, just a way to store some style
* properties, inspired by css.
*/
namespace css {
struct Margin {
    double top;
    double right;
    double bottom;
    double left;
    
    Margin(double all)
        : top(all), right(all), bottom(all), left(all) {}
    Margin(double tb, double lr)
        : top(tb), right(lr), bottom(tb), left(lr) {}
    Margin(double t, double r, double b, double l)
        : top(t), right(r), bottom(b), left(l) {}
};

struct Padding {
    double top;
    double right;
    double bottom;
    double left;
    Padding(double all)
        : top(all), right(all), bottom(all), left(all) {}
    Padding(double tb, double lr)
        : top(tb), right(lr), bottom(tb), left(lr) {}
    Padding(double t, double r, double b, double l)
        : top(t), right(r), bottom(b), left(l) {}
};
}