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
    gradient_t pixel[WINDOW_ROWS][WINDOW_COLS];
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
    image_t* image = (image_t*) myMalloc(sizeof(image_t));
    return image;
}

window_t* get_window(image_t* image, int startRow, int endRow, int startCol, int endCol) {
    window_t* window = (window_t*) myMalloc(sizeof(window_t));
    // Load in image(startRow:endRow, startCol:endCol, :);
    return window;
}

imagegradient_t* get_image_gradient(window_t* window) {
    imagegradient_t* imagegradient = (imagegradient_t*) myMalloc(sizeof(imagegradient_t));
    // Read in image gradient here
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
                    angle = (imagegradient->pixel[row][col].angle + 90); // CRITICAL: adjust range to fit in [0 180)
                    // Determine which 2 buckets the magnitude is distributed across
                    index1 = (int) floor(angle / ANGLE_INTERVAL);
                    index2 = (index1 + 1) % NBINS;
                    // Calculate how much goes in each bucket
                    remainder = angle % ANGLE_INTERVAL; // This will not work if angles are in radians
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

void classify(hog_t* hog) {
    // Don't return anything, just simulate a dot product happening.
    int i;
    int a;
    for (i = 0; i < HOG_DIM; i++) {
        a = hog->hog_vector[i] * hog->hog_vector[i];
    }
    return;
}

int main() {
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
            classify(hog);
        }
    }
    return 0;
}
