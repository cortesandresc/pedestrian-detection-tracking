function [] = createVideo()


vid = VideoReader('town_center_converted.m4v');
vidout = VideoWriter('testing_pipeline.avi');
open(vidout);
fig = figure('visible','off');
%fig = figure;
% Store locations are in [ [right bottom width height] ; [...];]
analytics.stores = [ 325 375 324 285;
                    555 250 230 210;
                    860 70 345 69;
                    1200 725 160 400;
                    1235 350 70 185];
                
% Contains the amount of people standing near each store
analytics.counts = [ 0 ;
                  0 ;
                  0 ;
                  0 ;
                  0];
              
% Number of frames seen so far
analytics.numFrames = 0;

% Number of people seen TOTAL (includes same person many times)
analytics.numPeople = 0;

prev_img = imresize(readFrame(vid),2/3);
%while hasFrame(vid) % Uncomment this line to run through the entire video

for a = 1:100
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
    analytics = updateAnalytics(positiveWindows,analytics);
    imshow(img);
    hold on;
    
    for i = 1:size(positiveWindows,1)
        row = positiveWindows(i, 1);
        col = positiveWindows(i, 2);
        x = col;
        y = row;
        rectangle('Position', [x y 64 128], 'EdgeColor', 'r');
    end
    
    % Remove this comment to displaying where store locations are
    %{
    for i = 1: size(analytics.stores,1)
        i
        x = analytics.stores(i, 1);
        y = analytics.stores(i, 2);

        w = analytics.stores(i,3);
        h = analytics.stores(i,4);
        position = [(x-w) (y-h) w h];
        rectangle('Position', position, 'EdgeColor', 'g');
    end 
    %}
    title('People in Image');
    hold off;
    
    frame = getframe(fig);
    writeVideo(vidout,frame.cdata);
    prev_img = img;
end
close(vidout);

end