
function [] = createVideo()

vid = VideoReader('town_center_converted.m4v');
vidout = VideoWriter('testing_pipeline.avi');
open(vidout);
fig = figure;

prev_img = imresize(readFrame(vid),2/3);
%while hasFrame(vid) % Uncomment this line to run through the entire video
for a = 1:10
    a
    img = imresize(readFrame(vid),2/3);

    [imgRows, imgCols, ~] = size(img);
    windowSize = [128 64];

    stepsize = 32;

    positiveWindows = [];

    for row = 1:stepsize:imgRows-windowSize(1)+1
        for col = 1:stepsize:imgCols-windowSize(2)+1
            imgPatch = img(row:row+windowSize(1)-1, col:col+windowSize(2)-1, :);
            prev_imgPatch = prev_img(row:row+windowSize(1)-1, col:col+windowSize(2)-1, :);
            if mean(mean(mean(imgPatch - prev_imgPatch))) < 1
                continue
            end
            hog_vector = hog(imgPatch);
            class = classify(hog_vector);

            if (class)
                positiveWindows = [positiveWindows; row col];
            end
        end
    end
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
    
    frame = getframe(fig);
    writeVideo(vidout,frame.cdata);
    prev_img = img;
end
close(vidout);

end