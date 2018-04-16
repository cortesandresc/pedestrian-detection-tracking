#include <cstddef>
#include <iostream>
#include <math.h>

using namespace std;

#define ROWS 128
#define COLS 64

#define CELLSIZE 8

#define NBINS 9
#define ANGLE_INTERVAL (180 / NBINS)

#define HISTOGRAM88ROWS (ROWS / CELLSIZE)
#define HISTOGRAM88COLS (COLS / CELLSIZE)

#define HISTOGRAMNORMROWS ((ROWS / CELLSIZE) - 1)
#define HISTOGRAMNORMCOLS ((COLS / CELLSIZE) - 1)

typedef struct GRADIENT {
    int magnitude;
    int angle; // Assumes angles are in degrees
} gradient_t;

typedef struct IMAGEGRADIENT {
    gradient_t pixel[ROWS][COLS];
} imagegradient_t;

typedef struct HISTOGRAMS88 {
    float histogram_grid[HISTOGRAM88ROWS][HISTOGRAM88COLS][NBINS];
} histogram88_t;

typedef struct HISTOGRAMSNORM {
    float histogram_grid[HISTOGRAMNORMROWS][HISTOGRAMNORMCOLS][4*NBINS];
} histogramnorm_t;

typedef struct HOG_VECTOR {
    float hog[4*NBINS*HISTOGRAMNORMROWS*HISTOGRAMNORMCOLS];
} hog_t;

void* myMalloc(size_t bytes) {
    void* ptr = malloc(bytes);
    if (ptr == NULL) {
        throw "Unable to dynamically allocate memory.";
    }
    return ptr;
}

imagegradient_t* get_image_gradient() {
    imagegradient_t* imagegradient = (imagegradient_t*) myMalloc(sizeof(imagegradient_t));
    // Read in image gradient here
    return imagegradient;
}

histogram88_t* computeHistograms88(imagegradient_t* imagegradient) {
    histogram88_t* histogram88 = (histogram88_t*) myMalloc(sizeof(histogram88_t));
    int row, col, hRow, hCol, startRow, endRow, startCol, endCol;
    int magnitude, angle;
    int index1, index2;
    int remainder;
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
                    angle = (imagegradient->pixel[row][col].angle + 90); // CRITICAL: adjust range to fit in [0 pi)
                    // Determine which 2 buckets the magnitude is distributed across
                    index1 = (int) floor(angle / ANGLE_INTERVAL);
                    index2 = (index1 + 1) % NBINS;
                    // Calculate how much goes in each bucket
                    remainder = angle % ANGLE_INTERVAL;
                    ratio = (float)remainder / ANGLE_INTERVAL;
                    magnitude = imagegradient->pixel[row][col].magnitude;
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
    return hog;
}

int main() {
    imagegradient_t* imagegradient = get_image_gradient();
    histogram88_t* histograms88 = computeHistograms88(imagegradient);
    histogramnorm_t* histogramnorm = computeHistogramnorms(histograms88);
    hog_t* hog = computeHOG(histogramnorm);
    return 0;
}