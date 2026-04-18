# Photon - AI Inference Platform

## Requirements 
```
CPP20
```
## How to run

```
cmake -S . build -DINFERENCE_ENABLE_ONNX=ON
cmake --build build
```
## To run examples

```
# 1. Basic inference
./build/bin/basic_inference

# 2. Logistic Regeression (mnist)
./build/bin/logistic_regression
```
