function [class] = classify(hog_vector)

r = rand * 100;

class = (r > 95); % replace this with SVM classification based on hog_vector

end