function [] = classify()

% Ensure image dimensions are multiples of 16
imgName = 'ringo.jpg';
img = imread(imgName);
%img = img(17:144, 17:80,:);

HOG_vector = hog(img);

figure;
imagesc(HOG_vector);
title('HOG');

figure;
imshow(img);
title('Original Image');

end