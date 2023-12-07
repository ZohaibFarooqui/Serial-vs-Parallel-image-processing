
Welcome to my OpenMP-based Parallel Image Processing project! 🚀

This project showcases the implementation of a Box Blur algorithm for image processing with parallelization using OpenMP. The goal is to efficiently blur images by harnessing the power of shared-memory parallelism.

Key Features:
🔍 Parallel Execution: Leveraging OpenMP for parallelism to enhance image processing speed.
🖼️ Box Blur Algorithm: Implementation of a simple yet effective blurring algorithm to create visually appealing results.
💻 Optimized Code: Carefully crafted code to balance parallelism and data dependencies, ensuring optimal performance.
📈 Scalability: Demonstrates scalability by varying the number of threads for parallel execution.

FOR SERIAL 
mpic++ -o seq_image seq_image.cpp -std=c++11

FOR PARALLEL
g++ -o image image.cpp -std=c++11 -fopenmp -O3 -Wall -Wextra -lstdc++
