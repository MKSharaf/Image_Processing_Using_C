#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    /*
    Divide each pixel value with the result of image size to make the total
    value of the sum = 1
    */
    for(int w = 0; w < im.w; w++){
        for(int h = 0; h < im.h; h++){
            for(int c = 0; c < im.c; c++){
                set_pixel(im, w, h, c, get_pixel(im, w, h, c)/(1.0*im.w*im.h));
            }
        }
    }
}

image make_box_filter(int w)
{
    //Making an image and filling it's every pixel with the value of 1
    image n_img = make_image(w,w,1);
    for(int i = 0; i < w * w; i++){
        n_img.data[i] = 1;
    }
    l1_normalize(n_img);
    return n_img;
}

image convolve_image(image im, image filter, int preserve)
{
    /*
    We first make sure that the amount of channels in the filter image is either 0 or the same amount of channels
    as the original image, and then we calculate the amount of pixels that we will move
    */
    assert(filter.c == im.c || filter.c == 1);
    int v = (filter.h-1)/2;
    image n_img;
    /*
    There are 4 possible scenarios that we will get:-
        1) Both the original image, and the filter box will have the same amount of channels, and we want to preserve the colors
        2) Both the original image, and the filter box will have the same amount of channels, and we don't want to preserve the colors
        3) The number of channels between the original image, and the filter box are different, and we want to preserve the colors
        4) The number of channels between the original image, and the filter box are different, and we don't want to preserve the colors
    */
    if (im.c == filter.c && preserve != 1){
    float q = 0;
    n_img = make_image(im.w, im.h, 1);
        for (int h = 0; h < im.h; h++){
            for (int w = 0; w < im.w; w++){
                //We calculate the q which will be the sum of doing cross-correlation on every channel
                q = 0;
                for (int c = 0; c < im.c; c++){
                    for (int f_h = v * -1; f_h <= v; f_h++){
                        for (int f_w = v * -1; f_w <= v; f_w++){
                            q = q + get_pixel(im, w + f_w, h + f_h, c) * get_pixel(filter, f_w + v, f_h + v, c);
                        }
                    }
                }
            set_pixel(n_img, w, h, 0, q);
            }
        }
    }
    else if (im.c == filter.c && preserve == 1){
    float q = 0;
    n_img = make_image(im.w, im.h, im.c);
        for (int h = 0; h < im.h; h++){
            for (int w = 0; w < im.w; w++){
                for (int c = 0; c < im.c; c++){
                //We calculate the q which will be the result of doing cross-correlation on each channel
                q = 0;
                    for (int f_h = v * -1; f_h <= v; f_h++){
                        for (int f_w = v * -1; f_w <= v; f_w++){
                            q = q + get_pixel(im, w + f_w, h + f_h, c) * get_pixel(filter, f_w + v, f_h + v, c);
                        }
                    }
                set_pixel(n_img, w, h, c, q);
                }
            }
        }
    }
    else if (im.c > filter.c && preserve != 1){
    float q = 0;
    n_img = make_image(im.w, im.h, 1);
        for (int h = 0; h < im.h; h++){
            for (int w = 0; w < im.w; w++){
                q = 0;
                for (int c = 0; c < im.c; c++){
                    for (int f_h = v * -1; f_h <= v; f_h++){
                        for (int f_w = v * -1; f_w <= v; f_w++){
                            q = q + get_pixel(im, w + f_w, h + f_h, c) * get_pixel(filter, f_w + v, f_h + v, 0);
                        }
                    }
                }
                set_pixel(n_img, w, h, 0, q);
            }
        }
    }
    else if (im.c > filter.c && preserve == 1){
    float q = 0;
    n_img = make_image(im.w, im.h, im.c);
        for (int h = 0; h < im.h; h++){
            for (int w = 0; w < im.w; w++){
                for (int c = 0; c < im.c; c++){
                q = 0;
                    for (int f_h = v * -1; f_h <= v; f_h++){
                        for (int f_w = v * -1; f_w <= v; f_w++){
                            q = q + get_pixel(im, w + f_w, h + f_h, c) * get_pixel(filter, f_w + v, f_h + v, 0);
                        }
                    }
                    set_pixel(n_img, w, h, c, q);
                }
            }
        }
    }
    return n_img;
}

image make_highpass_filter()
{
    image filter = make_box_filter(3);
    set_pixel(filter, 0, 0, 0, 0);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 4);
    set_pixel(filter, 2, 1, 0, -1);
    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 2, 2, 0, 0);
    return filter;
}

image make_sharpen_filter()
{
    image filter = make_box_filter(3);
    set_pixel(filter, 0, 0, 0, 0);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 5);
    set_pixel(filter, 2, 1, 0, -1);
    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 2, 2, 0, 0);
    return filter;
}

image make_emboss_filter()
{
    image filter = make_box_filter(3);
    set_pixel(filter, 0, 0, 0, -2);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 1);
    set_pixel(filter, 2, 1, 0, 1);
    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, 1);
    set_pixel(filter, 2, 2, 0, 2);
    return filter;
}

image make_gaussian_filter(float sigma)
{
    image n_img = make_box_filter(sigma * 6 + 1);
    float x, y;
    for (int h = 0; h < n_img.h; h++){
        for (int w = 0; w < n_img.w; w++){
            x = w - 3 * sigma;
            y = h - 3 * sigma;
            float z = (2 * M_PI * powf(sigma, 2.0));
            float over_z = 1.0 / z;
            float power = (powf(x, 2.0) + powf(y, 2.0)) / (2.0 * powf(sigma, 2.0));
            set_pixel(n_img, w, h, 0, over_z * powf(M_E, -1.0 * power));
        }
    }
    return n_img;
}

image add_image(image a, image b)
{
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image im = make_image(a.w, a.h, a.c);
    float pixel;
    for (int w = 0; w < im.w; w++){
        for (int h = 0; h < im.h; h++){
            for (int c = 0; c < im.c; c++){
                pixel = get_pixel(a, w, h, c) + get_pixel(b, w, h, c);
                set_pixel(im, w, h, c, pixel);
            }
        }
    }
    return im;
}

image sub_image(image a, image b)
{
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image im = make_image(a.w, a.h, a.c);
    float pixel;
    for (int w = 0; w < im.w; w++){
        for (int h = 0; h < im.h; h++){
            for (int c = 0; c < im.c; c++){
                pixel = get_pixel(a, w, h, c) - get_pixel(b, w, h, c);
                set_pixel(im, w, h, c, pixel);
            }
        }
    }
    return im;
}

image make_gx_filter()
{
    image im =  make_box_filter(3);
    set_pixel(im, 0, 0, 0, -1);
    set_pixel(im, 1, 0, 0, 0);
    set_pixel(im, 2, 0, 0, 1);
    set_pixel(im, 0, 1, 0, -2);
    set_pixel(im, 1, 1, 0, 0);
    set_pixel(im, 2, 1, 0, 2);
    set_pixel(im, 0, 2, 0, -1);
    set_pixel(im, 1, 2, 0, 0);
    set_pixel(im, 2, 2, 0, 1);
    return im;
}

image make_gy_filter()
{
    image im =  make_box_filter(3);
    set_pixel(im, 0, 0, 0, -1);
    set_pixel(im, 1, 0, 0, -2);
    set_pixel(im, 2, 0, 0, -1);
    set_pixel(im, 0, 1, 0, 0);
    set_pixel(im, 1, 1, 0, 0);
    set_pixel(im, 2, 1, 0, 0);
    set_pixel(im, 0, 2, 0, 1);
    set_pixel(im, 1, 2, 0, 2);
    set_pixel(im, 2, 2, 0, 1);
    return im;
}

void feature_normalize(image im)
{
    float max = 0, min = 0, x_;
    for (int i = 0; i < im.w * im.h * im.c; i++){
        if (i == 0){
            min = im.data[i];
            max = im.data[i];
        }
        else {
            if (min > im.data[i]){
                min = im.data[i];
            }
            if (max < im.data[i]){
                max = im.data[i];
            }
        }
    }
    for (int j = 0; j < im.w * im.h * im.c; j++){
        if (max == 0){
            x_ = 0;
        }
        else {
            x_ = (im.data[j] - min) / (max - min);
        }
        im.data[j] = x_;
    }
}

image *sobel_image(image im)
{
    image *array_image = calloc(2, sizeof(image));
    array_image[0] = make_image(im.w,im.h,1);
    array_image[1] = make_image(im.w,im.h,1);
    image gx = make_gx_filter();
    image gy = make_gy_filter();
    image im_gx = convolve_image(im, gx, 0);
    image im_gy = convolve_image(im, gy, 0);
    float g, sqrt_g, d;
    for (int w = 0; w < im.w; w++){
        for (int h = 0; h < im.h; h++){
            g = powf(get_pixel(im_gx, w, h, 0), 2) + powf(get_pixel(im_gy, w, h, 0), 2);
            sqrt_g = sqrtf(g);
            d = atan2f(get_pixel(im_gy, w, h, 0), get_pixel(im_gx, w, h, 0));
            set_pixel(array_image[0], w, h, 0, sqrt_g);
            set_pixel(array_image[1], w, h, 0, d);
        }
    }
    return array_image;
}

image colorize_sobel(image im)
{
    // TODO
    return make_image(1,1,1);
}
