#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    if (x < 0){
    	x = 0;
    }
    if (x >= im.w){
    	x = im.w - 1;
    }
    if (y < 0){
    	y = 0;
    }
    if (y >= im.h){
    	y = im.h - 1;
    }
    if (c < 0){
    	c = 0;
    }
    if (c >= im.c){
    	c = im.c - 1;
    }
    return im.data[x + y * im.w + c * im.h * im.w];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    if ((x >= 0 && x < im.w) && (y >= 0 && y < im.h) && (c < im.c && c >= 0)){
        im.data[x + y * im.w + c * im.h * im.w] = v;
    }
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    int pixels = im.w * im.h * im.c;
    for (int i = 0; i < pixels; i++){
        copy.data[i] = im.data[i];
    }
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    float value = 0;
    for (int w = 0; w < im.w * im.h; w++){
        for (int c = 0; c < im.c; c++){
            value = im.data[w + c * im.w * im.h];
            switch (c)
            {
            case 0:
                gray.data[w] = gray.data[w] + 0.299 * value;
                break;
            case 1:
                gray.data[w] = gray.data[w] + 0.587 * value;
                break;
            case 2:
                gray.data[w] = gray.data[w] + 0.114 * value;
                break;
            }
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    for (int i = 0; i < im.w * im.h; i++){
        im.data[i + c * im.w * im.h] = im.data[i + c * im.w * im.h] + v;
    }
}

void clamp_image(image im)
{
    for (int w = 0; w < im.w * im.h; w++){
        for (int c = 0; c < im.c; c++){
            switch (c)
            {
            case 0:
                if (im.data[w + c * im.w * im.h] > 1){
                    im.data[w + c * im.w * im.h] = 1;
                } else if (im.data[w + c * im.w * im.h] < 0){
                    im.data[w + c * im.w * im.h] = 0;
                }
                break;
            case 1:
                if (im.data[w + c * im.w * im.h] > 1){
                    im.data[w + c * im.w * im.h] = 1;
                } else if (im.data[w + c * im.w * im.h] < 0){
                    im.data[w + c * im.w * im.h] = 0;
                }
                break;
            case 2:
                if (im.data[w + c * im.w * im.h] > 1){
                    im.data[w + c * im.w * im.h] = 1;
                } else if (im.data[w + c * im.w * im.h] < 0){
                    im.data[w + c * im.w * im.h] = 0;
                }
                break;
            }
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    float H_, H, S, V, C;
    for (int i = 0; i < im.w * im.h; i++){
        float M = three_way_max(im.data[i], im.data[i + 1 * im.w * im.h], im.data[i + 2 * im.w * im.h]);
        float m = three_way_min(im.data[i], im.data[i + 1 * im.w * im.h], im.data[i + 2 * im.w * im.h]);
        C = M - m;
        if (M == 0){
            S = 0;
        } else {
            S = C / M;
        }
        if (C == 0){
            H_ = 0;
        } else {
            if (M == im.data[i]){
                H_ = (im.data[i + 1 * im.w * im.h] - im.data[i + 2 * im.w * im.h]) / C;
            }
            else if (M == im.data[i + 1 * im.w * im.h]){
                H_ = ((im.data[i + 2 * im.w * im.h] - im.data[i]) / C) + 2;
            }
            else if (M == im.data[i + 2 * im.w * im.h]){
                H_ = ((im.data[i] - im.data[i + 1 * im.w * im.h]) / C) + 4;
            }
        }
        if (H_ < 0){
            H = (H_ / 6) + 1;
        } else {
            H = H_ / 6;
        }
        V = M;
        im.data[i] = H;
        im.data[i + 1 * im.w * im.h] = S;
        im.data[i + 2 * im.w * im.h] = V;
    }
}

void hsv_to_rgb(image im)
{
    float H_, H, S, V, C, X, m;
    float R_, G_, B_;
    for (int i = 0; i < im.w * im.h; i++){
        H = im.data[i];
        S = im.data[i + 1 * im.w * im.h];
        V = im.data[i + 2 * im.w * im.h];
        float M = V;
        C = S * M;
        m = V - C;
        H_ = H * 6;
        X = C * (1 - fabs(fmodf(H_,  2) - 1));
        if ((H_ >= 0 && H_ < 1)  || H_ == 6){
            R_ = C;
            G_ = X;
            B_ = 0;
        }
        else if (H_ >= 1 && H_ < 2){
            R_ = X;
            G_ = C;
            B_ = 0;
        }
        else if (H_ >= 2 && H_ < 3){
            R_ = 0;
            G_ = C;
            B_ = X;
        }
        else if (H_ >= 3 && H_ < 4){
            R_ = 0;
            G_ = X;
            B_ = C;
        }
        else if (H_ >= 4 && H_ < 5){
            R_ = X;
            G_ = 0;
            B_ = C;
        }
        else if (H_ >= 5 && H_ < 6){
            R_ = C;
            G_ = 0;
            B_ = X;
        }
        im.data[i] = R_ + m;
        im.data[i + 1 * im.w * im.h] = G_ + m;
        im.data[i + 2 * im.w * im.h] = B_ + m;
    }
}
