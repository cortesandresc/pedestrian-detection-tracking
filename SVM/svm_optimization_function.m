function error = svm_optimization_function(w)
  % C is the slack variable - hyperparameter
  C = 0.9;
  
  bias = w(end);
  w = w(1:end-1);
  
  global x y;
  N = 1 / size(y,1);
  
  % Calculate the margin error by computing 1/2 * norm(w)
  margin_error = 1/2 * (w * w');
  
  % Calculate the label error (if the label is < 1)
  labels = y .* ((x * w') + bias);
  label_error = labels;
  label_error( labels >= 1) = 0;
  label_error( labels < 1) = abs( 1 - labels(labels < 1));
  label_error = C * N * sum(label_error); 
  
  error = margin_error + label_error;
end
