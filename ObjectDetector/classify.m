function [] = classify()
%% Wrapper script to calculate hog vector for an image

% Ensure image dimensions are multiples of 16
imgName = 'ringo.jpg'; 
img = imread(imgName);

HOG_vector = hog(img);

end