function [] = classify()

% Ensure image dimensions are multiples of 16
imgName = '70X134H96/Test/pos/person_and_bike_190d.png';
img = imread(imgName);
img = img(17:144, 17:80,:);

HOG_vector = hog(img);
figure;
imagesc(HOG_vector);

end