function error = svm_optimization_function(w)
  % C is the slack variable - hyperparameter
  C = 0.7;
  
  % Calculate the margin error by computing 1/2 * norm(w)
  margin_error = 1/2 * (w * w');
  
  % Load in the data set  
  x = load('../data/features');
  y = load('../data/labels');
  x = x.features;
  y = y.labels;
  
  % Calculate the label error (if the label is < 1)
  labels = y .* (x * w');
  label_error = labels;
  label_error( labels >= 1) = 0;
  label_error( labels < 1) = abs( 1 - labels(labels < 1));
  label_error = C * sum(label_error); 
  
  error = margin_error + label_error;
end
