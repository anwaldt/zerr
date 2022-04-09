#ifndef LINEARINTERPOLATOR_H
#define LINEARINTERPOLATOR_H


class LinearInterpolator
{
public:

    LinearInterpolator();

    void   set_values(double start, double stop, int l);
    double get_value();
    void next_step();

private:

    double y_start;
    double y_stop;

    double L;
    int pos;

};

#endif // LINEARINTERPOLATOR_H
