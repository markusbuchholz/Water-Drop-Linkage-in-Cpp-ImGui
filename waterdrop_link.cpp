// Markus Buchholz. 2023

#include <iostream>
#include <tuple>
#include <vector>
#include <math.h>
#include <cmath>

#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

//----------- system dynamic parameters --------------------

float a = 1.0;
float Ox = 0.0;
float Oy = 0.0;
float Cx = 0.0;
float Cy = -2.0 * a;

//---------------------------------------------------------------

void plot2D(std::vector<float> X, std::vector<float> Y)
{

    plt::title("water drop linkage");
    plt::named_plot("water drop linkage", X, Y);
    plt::xlabel("pos X");
    plt::ylabel("pos Y");
    plt::legend();
    plt::xlabel("pos X");
    plt::ylabel("pos Y");
    plt::show();
}

//---------------------------------------------------------------

std::tuple<std::vector<float>, std::vector<float>> simulation()
{

    std::vector<float> x;
    std::vector<float> y;
    float dt = M_PI / 180.0;
    for (float t = 0.0; t < 2 * M_PI; t = t + dt)
    {

        float dist = a * std::sqrt(5.0 + 4.0 * std::cos(t));

        float cos_alfa = (float)(dist * dist + 3.0 * a * a) / (4.0 * a * dist);

        float sin_alfa = ((0 <= t) && (t <= M_PI)) ? std::sqrt(1 - cos_alfa * cos_alfa) : -std::sqrt(1 - cos_alfa * cos_alfa);
        int d = ((0 <= t) && (t <= M_PI)) ? 1 : 0;
        std::cout << "dec : " << d << "\n";

        float Ax = -a * std::sin(t);
        float Ay = a * std::cos(t);

        float Bx = Cx - 4.0 * a * cos_alfa * sin_alfa;

        float By = Cy + 2.0 * a * (2 * cos_alfa * cos_alfa - 1);

        float Px = 4.0 * Ax - 3.0 * Bx;
        float Py = 4.0 * Ay - 3.0 * By;

        std::cout << Px << " : " << Py << "\n";
        x.push_back(Px);
        y.push_back(Py);
    }
    return std::make_tuple(x, y);
}

int main()
{
    auto sim = simulation();
    plot2D(std::get<0>(sim), std::get<1>(sim));
}
