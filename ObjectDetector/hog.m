function [HOG_vector] = hog(img) 
% Ensure image dimensions are multiples of 16

%% Compute gradient
imgGradient = computeGradient(img);

%% Compute grid of 8x8 HOGs
histograms88 = computeHistograms88(imgGradient);

%% Normalize across 16x16 block (i.e. 4 8x8 blocks) 
% Every 16x16 block is comprised of 4 8x8 blocks. We just concatenate each
% of their histograms into one longer histogram and normalize that entire
% vector
[imgRows, imgCols, ~] = size(img);
cellRows = 8;
cellCols = 8;
% Calculate how many 16x16 blocks we have in our image
blockRows = (imgRows / cellRows) - 1;
blockCols = (imgCols / cellCols) - 1;
nbins = 9;
% Prepare to store all normalized histograms
histograms2 = zeros(blockRows, blockCols, nbins*4); % should be 'double'
for row = 1:blockRows
    for col = 1:blockCols
        histogram = zeros(1, 36);
        % Concatenate each 8x8 block's histograms to each other
        for i = 1:9
           histogram(0+i) = histograms88(row, col, i);
           histogram(9+i) = histograms88(row, col+1, i);
           histogram(18+i) = histograms88(row+1, col, i);
           histogram(27+i) = histograms88(row+1, col+1, i);
        end
        % Normalize the entire concatenated vector
        total = 0;
        for i = 1:36
            total = total + histogram(i)^2;
        end
        total = sqrt(total);
        for i = 1:36
            histograms2(row, col, i) = histogram(i) / total; % There was a bug in this line of code
        end
    end
end

%% Compute final HOG vector
% Go through each 16x16 block's normalized histogram and concatenate them
% to form the final giant HOG vector
HOG_vector = zeros(1, nbins*4*(blockRows)*(blockCols)); % should be 'double'
index = 1;
for row = 1:blockRows
    for col = 1:blockCols
        for j = 1:(4*nbins)
            HOG_vector(index) = histograms2(row, col, j);
            index = index + 1;
        end
    end
end

end
