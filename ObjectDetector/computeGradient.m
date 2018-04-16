function [imgGradient] = computeGradient(img)

[imgRows, imgCols, ~] = size(img);

imgGradient = zeros(imgRows, imgCols, 2); % dim-1 can be unsigned, dim-2 must be signed

Iy = zeros(imgRows, imgCols);
Ix = zeros(imgRows, imgCols);

img = double(img); % this is critical for our calculations

for row = 1:imgRows
    for col = 1:imgCols
        % Compute x and y gradients for each channel
        if (row == 1)
            gy_r = img(row+1, col, 1);
            gy_g = img(row+1, col, 2);
            gy_b = img(row+1, col, 3);
        elseif (row == imgRows)
            gy_r = -img(row-1, col, 1);
            gy_g = -img(row-1, col, 2);
            gy_b = -img(row-1, col, 3);
        else
            gy_r = img(row+1, col, 1) - img(row-1, col, 1);
            gy_g = img(row+1, col, 2) - img(row-1, col, 2);
            gy_b = img(row+1, col, 3) - img(row-1, col, 3);
        end
        if (col == 1)
            gx_r = img(row, col+1, 1);
            gx_g = img(row, col+1, 2);
            gx_b = img(row, col+1, 3);
        elseif (col == imgCols)
            gx_r = -img(row, col-1, 1);
            gx_g = -img(row, col-1, 2);
            gx_b = -img(row, col-1, 3);
        else
            gx_r = img(row, col+1, 1) - img(row, col-1, 1);
            gx_g = img(row, col+1, 2) - img(row, col-1, 2);
            gx_b = img(row, col+1, 3) - img(row, col-1, 3);
        end
        % Compute final gradient for each channel
        g_r = sqrt(gx_r^2+gy_r^2);
        g_g = sqrt(gx_g^2+gy_g^2);
        g_b = sqrt(gx_b^2+gy_b^2);
        max_val = max([g_r, g_g, g_b]);
        % Final gradient is max gradient of each channel
        if (max_val == g_r)
            gx = gx_r;
            gy = gy_r;
            g = g_r;
        elseif (max_val == g_g)
            gx = gx_g;
            gy = gy_g;
            g = g_g;
        else
            gx = gx_b;
            gy = gy_b;
            g = g_b;
        end
        % Compute angle corresponding to max gradient
        Iy(row, col) = gy;
        Ix(row, col) = gx;
        imgGradient(row, col, 1) = g;
        angle_in_radians = atan(Iy(row, col) / Ix(row, col));
        imgGradient(row, col, 2) = int16(radtodeg(angle_in_radians)); % degrees
    end
end


%% If you want to save the magnitude/gradient to file
% imgToSave = uint8(imgGradient(:,:,1));
% imwrite(imgToSave, 'ringo_gradient.jpg');

end