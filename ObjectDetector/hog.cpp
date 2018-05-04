/* 
To compile the program, execute:
g++ hog.cpp

To run the program, execute:
./a.out
*/

#include <cstddef>
#include <iostream>
#include <math.h>

using namespace std;

#define IMAGE_ROWS 720
#define IMAGE_COLS 1280

#define WINDOW_ROWS 128
#define WINDOW_COLS 64

#define CELLSIZE 8

#define NBINS 9
#define ANGLE_INTERVAL (180 / NBINS) // Angle assumed to be in degrees

#define HISTOGRAM88ROWS (WINDOW_ROWS / CELLSIZE)
#define HISTOGRAM88COLS (WINDOW_COLS / CELLSIZE)

#define HISTOGRAMNORMROWS (HISTOGRAM88ROWS - 1)
#define HISTOGRAMNORMCOLS (HISTOGRAM88COLS - 1)

#define HOG_DIM (4*NBINS*HISTOGRAMNORMROWS*HISTOGRAMNORMCOLS)

typedef struct PIXEL {
    int R;
    int G;
    int B;
} pixel_t;

typedef struct IMAGE {
    pixel_t pixels[IMAGE_ROWS][IMAGE_COLS];
} image_t;

typedef struct WINDOW {
    pixel_t pixels[WINDOW_ROWS][WINDOW_COLS];
} window_t;

typedef struct GRADIENT {
    int magnitude;
    int angle; // Assumes angles are in degrees
} gradient_t;

typedef struct IMAGEGRADIENT {
    gradient_t pixels[WINDOW_ROWS][WINDOW_COLS];
} imagegradient_t;

typedef struct HISTOGRAMS88 {
    float histogram_grid[HISTOGRAM88ROWS][HISTOGRAM88COLS][NBINS];
} histogram88_t;

typedef struct HISTOGRAMSNORM {
    float histogram_grid[HISTOGRAMNORMROWS][HISTOGRAMNORMCOLS][4*NBINS];
} histogramnorm_t;

typedef struct HOG_VECTOR {
    float hog_vector[HOG_DIM];
} hog_t;

void* myMalloc(size_t bytes) {
    void* ptr = malloc(bytes);
    if (ptr == NULL) {
        throw "Unable to dynamically allocate memory.";
    }
    return ptr;
}

image_t* read_image() {
    int i, j;
    image_t* image = (image_t*) myMalloc(sizeof(image_t));
    for (i = 0; i < IMAGE_ROWS; i++) {
        for (j = 0; j < IMAGE_COLS; j++) {
            pixel_t* pixel = (pixel_t*) myMalloc(sizeof(pixel_t));
            // Just populate with some "random" numbers
            pixel->R = (7*(i+1)) % 256;
            pixel->G = (5*(j+1)) % 256;
            pixel->B = (9*(i+j)) % 256;
            image->pixels[i][j] = *pixel;
        }
    }
    return image;
}

window_t* get_window(image_t* image, int startRow, int endRow, int startCol, int endCol) {
    int i, j;
    int row, col;
    window_t* window = (window_t*) myMalloc(sizeof(window_t));
    for (i = 0; i < WINDOW_ROWS; i++) {
        for (j = 0; j < WINDOW_COLS; j++) {
            window->pixels[i][j] = *(pixel_t*) myMalloc(sizeof(pixel_t));
        }
    }
    for (row = startRow; row <= endRow; row++) {
        for (col = startCol; col <= endCol; col++) {
            window->pixels[row-startRow][col-startCol].R = image->pixels[row][col].R;
            window->pixels[row-startRow][col-startCol].G = image->pixels[row][col].G;
            window->pixels[row-startRow][col-startCol].B = image->pixels[row][col].B;
        }
    }
    return window;
}

imagegradient_t* get_image_gradient(window_t* window) {
    int i, j;
    int row, col;
    imagegradient_t* imagegradient = (imagegradient_t*) myMalloc(sizeof(imagegradient_t));
    for (i = 0; i < WINDOW_ROWS; i++) {
        for (j = 0; j < WINDOW_COLS; j++) {
            imagegradient->pixels[i][j] = *(gradient_t*) myMalloc(sizeof(gradient_t));
        }
    }
    // Read in image gradient here
    int Iy[WINDOW_ROWS][WINDOW_COLS];
    int Ix[WINDOW_ROWS][WINDOW_COLS];
    int gy_r, gy_g, gy_b;
    int gx_r, gx_g, gx_b;
    int g_r, g_g, g_b;
    int gx, gy, g;
    for (row = 0; row < WINDOW_ROWS; row++) {
        for (col = 0; col < WINDOW_COLS; col++) {
            if (row == 0) {
                gy_r = window->pixels[row+1][col].R;
                gy_g = window->pixels[row+1][col].G;
                gy_b = window->pixels[row+1][col].B;
            } else if (row == WINDOW_ROWS-1) {
                gy_r = window->pixels[row-1][col].R;
                gy_g = window->pixels[row-1][col].G;
                gy_b = window->pixels[row-1][col].B;
            } else{
                gy_r = window->pixels[row+1][col].R - window->pixels[row-1][col].R;
                gy_g = window->pixels[row+1][col].G - window->pixels[row-1][col].G;
                gy_b = window->pixels[row+1][col].B - window->pixels[row-1][col].B;
            }
            if (col == 0) {
                gx_r = window->pixels[row][col+1].R;
                gx_g = window->pixels[row][col+1].G;
                gx_b = window->pixels[row][col+1].B;
            } else if (col == WINDOW_COLS-1) {
                gx_r = window->pixels[row][col-1].R;
                gx_g = window->pixels[row][col-1].G;
                gx_b = window->pixels[row][col-1].B;
            } else{
                gx_r = window->pixels[row][col+1].R - window->pixels[row][col-1].R;
                gx_g = window->pixels[row][col+1].G - window->pixels[row][col-1].G;
                gx_b = window->pixels[row][col+1].B - window->pixels[row][col-1].B;
            }
            g_r = sqrt(pow(gx_r, 2)+pow(gy_r, 2));
            g_g = sqrt(pow(gx_g, 2)+pow(gy_g, 2));
            g_b = sqrt(pow(gx_b, 2)+pow(gy_b, 2));
            if (g_r > g_g && g_r > g_b) {
                gx = gx_r;
                gy = gy_r;
                g = g_r;
            } else if (g_g > g_r && g_g > g_b) {
                gx = gx_g;
                gy = gy_g;
                g = g_g;
            } else {
                gx = gx_b;
                gy = gy_b;
                g = g_b;
            }
            Iy[row][col] = gy;
            Ix[row][col] = gx;
            imagegradient->pixels[row][col].magnitude = g;
            imagegradient->pixels[row][col].angle = atan((float)gy / gx);
        }
    }
    return imagegradient;
}

histogram88_t* computeHistograms88(imagegradient_t* imagegradient) {
    histogram88_t* histogram88 = (histogram88_t*) myMalloc(sizeof(histogram88_t));
    int row, col, hRow, hCol, startRow, endRow, startCol, endCol;
    int magnitude, angle;
    int index1, index2;
    int remainder; // This will become a float if angles are in radians
    float ratio;
    for (hRow = 0; hRow < HISTOGRAM88ROWS; hRow++) {
        for (hCol = 0; hCol < HISTOGRAM88COLS; hCol++) {
            // Define bounds of 8x8 patch
            startRow = hRow * CELLSIZE;
            startCol = hCol * CELLSIZE;
            endRow = startRow + CELLSIZE - 1;
            endCol = startCol + CELLSIZE - 1;
            // Compute histogram of 8x8 patch
            for (row = startRow; row <= endRow; row++) {
                for (col = startCol; col <= endCol; col++) {
                    angle = (imagegradient->pixels[row][col].angle + 90); // CRITICAL: adjust range to fit in [0 180)
                    // Determine which 2 buckets the magnitude is distributed across
                    index1 = (int) floor(angle / ANGLE_INTERVAL);
                    index2 = (index1 + 1) % NBINS;
                    // Calculate how much goes in each bucket
                    remainder = angle % ANGLE_INTERVAL; // This will not work if angles are in radians
                    ratio = (float)remainder / ANGLE_INTERVAL;
                    magnitude = imagegradient->pixels[row][col].magnitude;
                    // Populate histogram
                    histogram88->histogram_grid[hRow][hCol][index1] = histogram88->histogram_grid[hRow][hCol][index1] + magnitude*(1.0-ratio);
                    histogram88->histogram_grid[hRow][hCol][index2] = histogram88->histogram_grid[hRow][hCol][index2] + magnitude*(ratio);
                }
            }
        }
    }
    return histogram88;
}

histogramnorm_t* computeHistogramnorms(histogram88_t* histogram88) {
    histogramnorm_t* histogramnorm = (histogramnorm_t*) myMalloc(sizeof(histogramnorm_t));
    int row, col, bin;
    float total;
    for (row = 0; row < HISTOGRAMNORMROWS; row++) {
        for (col = 0; col < HISTOGRAMNORMCOLS; col++) {
            // Concatenate 4 8x8 patches' histograms
            for (bin = 0; bin < NBINS; bin++) {
                histogramnorm->histogram_grid[row][col][NBINS*0+bin] = histogram88->histogram_grid[row][col][bin];
                histogramnorm->histogram_grid[row][col][NBINS*1+bin] = histogram88->histogram_grid[row][col+1][bin];
                histogramnorm->histogram_grid[row][col][NBINS*2+bin] = histogram88->histogram_grid[row+1][col][bin];
                histogramnorm->histogram_grid[row][col][NBINS*3+bin] = histogram88->histogram_grid[row+1][col+1][bin];
            }
            // Normalize concatenated vector
            total = 0.0;
            for (bin = 0; bin < 4*NBINS; bin++) {
                total = total + pow(histogramnorm->histogram_grid[row][col][bin], 2.0);
            }
            total = sqrt(total);
            for (bin = 0; bin < 4*NBINS; bin++) {
                histogramnorm->histogram_grid[row][col][bin] = histogramnorm->histogram_grid[row][col][bin] / total;
            }
        }
    }
    return histogramnorm;
}

hog_t* computeHOG(histogramnorm_t* histogramnorm) {
    hog_t* hog = (hog_t*) myMalloc(sizeof(hog_t));
    int row, col, bin;
    int index = 0;
    // Concatenate all the normalized histograms
    for (row = 0; row < HISTOGRAMNORMROWS; row++) {
        for (col = 0; col < HISTOGRAMNORMCOLS; col++) {
            for (bin = 0; bin < 4*NBINS; bin++) {
                hog->hog_vector[index] = histogramnorm->histogram_grid[row][col][bin];
                index++;
            }
        }
    }
    return hog;
}

bool classify(hog_t* hog) {
    // Just simulate a dot product happening.
    int i;
    int a;
    for (i = 0; i < HOG_DIM; i++) {
        a = hog->hog_vector[i] * hog->hog_vector[i];
    }
    return (a > 0);
}

int main() {
    clock_t t = clock();
    int row, col;
    int startRow, endRow, startCol, endCol;    
    int stepsize = 32; 
    image_t* image = read_image();
    for (row = 0; row < IMAGE_ROWS-WINDOW_ROWS+1; row = row + stepsize) {
        for (col = 0; col < IMAGE_COLS-WINDOW_COLS+1; col = col + stepsize) {
            startRow = row;
            endRow = row+WINDOW_ROWS-1; 
            startCol = col;
            endCol = col+WINDOW_COLS-1;
            window_t* window = get_window(image, startRow, endRow, startCol, endCol);
            imagegradient_t* imagegradient = get_image_gradient(window);
            histogram88_t* histograms88 = computeHistograms88(imagegradient);
            histogramnorm_t* histogramnorm = computeHistogramnorms(histograms88);
            hog_t* hog = computeHOG(histogramnorm);
            bool isPerson = classify(hog);
        }
    }
    t = clock() - t;
    printf ("It took %lu clicks (%f seconds) to process one frame.\n",t,((float)t)/CLOCKS_PER_SEC);
    return 0;
    // I should probably free all the stuff I malloc'd lol
}