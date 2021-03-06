
function [] = slidingwindow()

imgName = 'composite_img.jpg'; 
% The images in slide_images/ are:
% 1) crossing 2) crossing2 3) snow 4) shore
imgName = 'slide_images/snow.jpg'; 
img = imread(imgName);

[imgRows, imgCols, ~] = size(img);
windowSize = [128 64];

stepsize = 20;

positiveWindows = [];

for row = 1:stepsize:imgRows-windowSize(1)+1
    for col = 1:stepsize:imgCols-windowSize(2)+1
        imgPatch = img(row:row+windowSize(1)-1, col:col+windowSize(2)-1, :);
        hog_vector = hog(imgPatch);
        class = classify(hog_vector);
        
        if (class)
            positiveWindows = [positiveWindows; row col];
        end
    end
end

figure;
imshow(img);
hold on;
for i = 1:size(positiveWindows,1)
    row = positiveWindows(i, 1);
    col = positiveWindows(i, 2);
    x = col;
    y = row;
    rectangle('Position', [x y 64 128], 'EdgeColor', 'r');
end
title('People in Image');
hold off;

end