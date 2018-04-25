% This file finds the base scene for the video we are using to demo.
% To do this, we sum the first numFrames images and divide by numFrames
scaling_factor = 2/3;

vid = VideoReader('town_center_converted.m4v');
numFrames = 3500;% Number of images to add | must be value between [1,7495]
summedImages = uint32(imresize(readFrame(vid),scaling_factor));
%while hasFrame(vid)
for a = 1:numFrames
    a
    img = imresize(readFrame(vid),scaling_factor);
    summedImages = summedImages + uint32(img);
end

baseImg = uint8(summedImages./numFrames);
