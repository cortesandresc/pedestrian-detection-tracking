function [class] = classify(hog_vector)
% Load the parameters in. Path assumes you are running from ObjectDetector/
load('../data/SVM_Params.mat');
% Transpose weights and hog vector. This step should be optimized for the
% FPGA. The 0.5 represents medium confidence the HOG is a person. This
% value can range from 0 (low confidence) to >1 (high confidence)
class = (weights' * hog_vector' + bias) > 0.6;

end