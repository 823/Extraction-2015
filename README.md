# Extraction-2015
code for a paper (2015) in CAGEO

%This is the code for a paper in CAGEO.

%%%%%%%%%%%%%%%%%%%%%%%%%

Title of the paper: Extraction of Microcracks in Rock Images based on Heuristic Graph Searching and Application

Co-authors: Zhihua Luo, Zhende Zhu, Huaining Ruan, Chong Shi.

Email: lzhhhu@163.com, hnruan@hhu.edu.cn

%%%%%%%%%%%%%%%%%%%%%%%

This directory contains the code to extract the cracks from images of rocks, and it gives some feature values of the crack. 
The basic idea of the approach presented in this paper is as follows: once a crack is identified by the user, only its two endpoints are needed, and then the algorithm will solve the crack result automatically.

Notes：
1. The Executable file folder includes the compiled program code, and it runs on Windows OS requiring .net framework 4.0 or above(You can download it from http://www.microsoft.com/en-US/download/details.aspx?id=17718)

2. The Source code files folder includes all the source codes.
ImageProcess.sln is the Microsoft Visual Studio 2010 solution file and it contains the other three projects.
ImageProWPF folder is the project for the software interface and it is coded in C# language with WPF environment (based on .net framework 4.0) and it needs 
NativeCCode folder is the project for the core algorithms and it is coded in C++ language and it requires the opencv2.1 library(it's an open source computer vision library and you can download it from http://opencv.org/).
WrapCPathfind folder is the .net wrapper project which can be used in C# for the NativeCCode codes and it requires the emgucv2.1.0.793 library (it's a cross platform .Net wrapper to the OpenCV library and you download it from )

3. The Test images folder includes some example images for test.

Enjoy it!
