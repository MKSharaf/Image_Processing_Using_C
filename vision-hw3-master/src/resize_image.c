#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    int xi = round(x);
    int yi = round(y);
    return get_pixel(im, xi, yi, c);
}

image nn_resize(image im, int w, int h)
{
    image new_im = make_image(w,h,im.c);
    float a1 = (0.5 + ((float)im.w - 0.5)) / (0.5 + ((float)w - 0.5));
    float b1 = -0.5 - (a1 * -0.5);
    float a2 = (0.5 + ((float)im.h - 0.5)) / (0.5 + ((float)h - 0.5));
    float b2 = -0.5 - (a2 * -0.5);
    for (int c = 0; c < im.c; c++){
        for (int i = 0; i < h; i++){
            for(int j = 0; j < w; j++){
                set_pixel(new_im, j, i, c, nn_interpolate(im, (a1 * j + b1), (a2 * i + b2), c));
            }
        }
    }
    return new_im;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    //Pixel P1 coordinate
    int x1 = floor(x), y1 = floor(y);
    //Pixel P2 coordinate
    int x2 = ceil(x), y2 = floor(y);
    //Pixel P3 coordinate
    int x3 = floor(x), y3 = ceil(y);
    //Pixel P4 coordinate
    int x4 = ceil(x), y4 = ceil(y);
    //Distance between P1, P, and P3 or P2, P, and P4
    float d1 = y - (int) y;
    float d2 = 1 - d1;
    //Distance between P1, P, and P2 or P3, P, and P4
    float d3 = x - (int) x;
    float d4 = 1 - d3;
    //Calculating q1, and q2 values
    float q1 = d1 * get_pixel(im, x3, y3, c) + d2 * get_pixel(im, x1, y1, c);
    float q2 = d1 * get_pixel(im, x4, y4, c) + d2 * get_pixel(im, x2, y2, c);
    //Calculating q3
    float q3 = d3 * q2 + d4 * q1;
    return q3;
}

image bilinear_resize(image im, int w, int h)
{
    image new_im = make_image(w,h,im.c);
    float a1 = (0.5 + ((float)im.w - 0.5)) / (0.5 + ((float)w - 0.5));
    float b1 = -0.5 - (a1 * -0.5);
    float a2 = (0.5 + ((float)im.h - 0.5)) / (0.5 + ((float)h - 0.5));
    float b2 = -0.5 - (a2 * -0.5);
    for (int c = 0; c < im.c; c++){
        for (int i = 0; i < h; i++){
            for(int j = 0; j < w; j++){
                set_pixel(new_im, j, i, c, bilinear_interpolate(im, (a1 * j + b1), (a2 * i + b2), c));
            }
        }
    }
    return new_im;
}

