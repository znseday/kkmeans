#include <iostream>
#include <cassert>
#include <fstream>
#include <chrono>
#include <thread>

#include "kkmeans.h"

using namespace std;

bool IsDebugOutput = false;

void Task::GenDemoData(const std::string &fn)
{
    dlib::rand rnd;
    ofstream f(fn);
    double x, y;
    // we will make 50 points from each class
    const long num = 50;

    // make some samples near the origin
    double radius = 0.5;
    for (long i = 0; i < num; ++i)
    {
        double sign = 1;
        if (rnd.get_random_double() < 0.5)
            sign = -1;
        x = 2*radius*rnd.get_random_double()-radius;
        y = sign*sqrt(radius*radius - x*x);

        f << x << ";" << y << endl;
    }

    // make some samples in a circle around the origin but far away
    radius = 10.0;
    for (long i = 0; i < num; ++i)
    {
        double sign = 1;
        if (rnd.get_random_double() < 0.5)
            sign = -1;
        x = 2*radius*rnd.get_random_double()-radius;
        y = sign*sqrt(radius*radius - x*x);

        // add this sample to our set of samples we will run k-means
        f << x << ";" << y << endl;
    }


    // make some samples in a circle around the point (25,25)
    radius = 4.0;
    for (long i = 0; i < num; ++i)
    {
       double sign = 1;
       if (rnd.get_random_double() < 0.5)
           sign = -1;
       x = 2*radius*rnd.get_random_double()-radius;
       y = sign*sqrt(radius*radius - x*x);

       // translate this point away from the origin
       x += 25;
       y += 25;

       // add this sample to our set of samples we will run k-means
       f << x << ";" << y << endl;
    }
}

void Task::GenRealData(const std::string &fn)
{
    ofstream f(fn);
    double x, y;
    for (int i = 0; i < 100; ++i)
    {
        x = -100 + rand()/(double)RAND_MAX*200;
        y = -100 + rand()/(double)RAND_MAX*200;
        f << x << ";" << y << endl;
    }

    for (int i = 0; i < 50; ++i)
    {
        x = -80 + rand()/(double)RAND_MAX*40;
        y = -80 + rand()/(double)RAND_MAX*40;
        f << x << ";" << y << endl;
    }

    for (int i = 0; i < 50; ++i)
    {
        x = 40 + rand()/(double)RAND_MAX*40;
        y = 40 + rand()/(double)RAND_MAX*40;
        f << x << ";" << y <<  endl;
    }
}

void Task::LoadDataFromInputStream()
{
    #if (defined WIN32) || (defined WIN64)
        cout << MY_P_FUNC << endl;                  // for debugging
        ifstream i_stream = ifstream("my_own_test.txt");
        if (!i_stream)
        {
            cout << "My error: the input file not found" << endl;
            exit(0);
        }
    #else
        istream &i_stream = cin;
    #endif

    string line;
    while (getline(i_stream, line))
    {
        double x, y;
        sscanf(line.c_str(), "%f;%f", &x, &y);
        data.emplace_back(x, y);

        MY_DEBUG_ONLY( cout << line << endl; ) // just echo
        MY_DEBUG_ONLY( cout << get<0>(data.back()) << " --- " << get<1>(data.back()) << endl; ) // just echo
    }
}

void Task::TrainAndDo()
{
    typedef dlib::matrix<double,2,1> sample_type;
    typedef dlib::radial_basis_kernel<sample_type> kernel_type;

    dlib::kcentroid<kernel_type> kc(kernel_type(0.05),0.005, 8);
    dlib::kkmeans<kernel_type> test(kc);

    vector<sample_type> samples;
    vector<sample_type> initial_centers;

    sample_type m;

    for (auto point : data)
    {
        m(0) = get<0>(point);
        m(1) = get<1>(point);
        samples.push_back(m);
    }

    test.set_number_of_centers(3);
    pick_initial_centers(3, initial_centers, samples, test.get_kernel());

    test.train(samples, initial_centers);

    ofstream f("result.txt");

    for (auto & sample : samples)
    {
        double x = sample(0);
        double y = sample(1);
        int res = test(sample);
        f << x << ";" << y << ";" << res << endl;
    }
}