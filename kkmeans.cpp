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
    for (int i = 0; i < 120; ++i)
    {
        x = -100 + rand()/(double)RAND_MAX*200;
        y = -100 + rand()/(double)RAND_MAX*200;
        f << x << ";" << y << endl;
    }

    for (int i = 0; i < 40; ++i)
    {
        x = -80 + rand()/(double)RAND_MAX*20;
        y = -60 + rand()/(double)RAND_MAX*20;
        f << x << ";" << y << endl;
    }

    for (int i = 0; i < 40; ++i)
    {
        x = 50 + rand()/(double)RAND_MAX*20;
        y = 70 + rand()/(double)RAND_MAX*20;
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
        sscanf(line.c_str(), "%lf;%lf", &x, &y);
        data.emplace_back(x, y);

        MY_DEBUG_ONLY( cout << "original: " << line << endl; ) // just echo
        MY_DEBUG_ONLY( cout << "check:    " << get<0>(data.back()) << " --- " << get<1>(data.back()) << endl; ) // just echo
    }
}

void Task::TrainAndDo(double g, double tolerance, unsigned long d_size)
{
    typedef dlib::matrix<double,2,1> sample_type;
    typedef dlib::radial_basis_kernel<sample_type> kernel_type;

    dlib::kcentroid<kernel_type> kc(kernel_type(g), tolerance, d_size);
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
    f.close();

    const int w = 200;
    const int h = 200;

    bmpHeaderType bmpHeader;
    memset(&bmpHeader, 0, sizeof(bmpHeader));
    bmpHeader.Type = 0x4d42;
    bmpHeader.OffsetBits = 54;
    bmpHeader.SizeH = 40;
    bmpHeader.Width = w;
    bmpHeader.Height = h;
    bmpHeader.Planes = 1;
    bmpHeader.BitCount = 24;
    bmpHeader.SizeImage = 3*bmpHeader.Width*bmpHeader.Height;
    bmpHeader.Size = bmpHeader.SizeImage + 54;

    MY_DEBUG_ONLY( cout << "bmpHeader.SizeImage = " << bmpHeader.SizeImage << endl;)

    unsigned char *buff = new unsigned char[bmpHeader.SizeImage];

    fstream f_bmp_points("result_points.bmp", ios::out | ios::binary);
    memset(buff, 0, bmpHeader.SizeImage);

    f_bmp_points.write((const char*)&bmpHeader, sizeof(bmpHeader));

    for (auto & sample : samples)
    {
        int x = 100 + (int)sample(0);
        int y = 100 + (int)sample(1);

        MY_DEBUG_ONLY(
            if (x < 0)
                cout << "x < 0" << endl;
            if (y < 0)
                cout << "y < 0" << endl;
            if (x > 199)
                cout << "x > 199" << endl;
            if (y > 199)
                cout << "y > 199" << endl;
        )

        MY_DEBUG_ONLY( if ( (h-y-1)*w*3 + x*3 + 2 > (int)bmpHeader.SizeImage )
            cout << "points:  (h-y-1)*w*3 + x*3 + 2 > bmpHeader.SizeImage" << endl; )

        int res = test(sample);
        switch(res)
        {
        case 0:
            buff[(h-y-1)*w*3 + x*3] = 200;
        break;
        case 1:
            buff[(h-y-1)*w*3 + x*3 + 1] = 200;
        break;
        case 2:
            buff[(h-y-1)*w*3 + x*3 + 2] = 200;
        break;
        default:
            buff[(h-y-1)*w*3 + x*3] = 127;
            buff[(h-y-1)*w*3 + x*3 + 1] = 127;
            buff[(h-y-1)*w*3 + x*3 + 2] = 127;
        }
    }

    f_bmp_points.write((const char*)buff, bmpHeader.SizeImage);
    //f_bmp_points.flush();
    f_bmp_points.close();

    fstream f_bmp_all("result_all.bmp", ios::out | ios::binary);
    f_bmp_all.write((const char*)&bmpHeader, sizeof(bmpHeader));
    memset(buff, 0, bmpHeader.SizeImage);

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            MY_DEBUG_ONLY (if ( (h-y-1)*w*3 + x*3 + 2 > (int)bmpHeader.SizeImage )
                cout << "all: (h-y-1)*w*3 + x*3 + 2 > bmpHeader.SizeImage" << endl; )

            m(0) = x-100;
            m(1) = y-100;
            int res = test(m);

            switch(res)
            {
            case 0:
                buff[(h-y-1)*w*3 + x*3] = 200;
            break;
            case 1:
                buff[(h-y-1)*w*3 + x*3 + 1] = 200;
            break;
            case 2:
                buff[(h-y-1)*w*3 + x*3 + 2] = 200;
            break;
            default:
                buff[(h-y-1)*w*3 + x*3] = 127;
                buff[(h-y-1)*w*3 + x*3 + 1] = 127;
                buff[(h-y-1)*w*3 + x*3 + 2] = 127;
            }
        }
    }

    f_bmp_all.write((const char*)buff, bmpHeader.SizeImage);
    //f_bmp_all.flush();
    f_bmp_all.close();

    delete[]buff;
}
