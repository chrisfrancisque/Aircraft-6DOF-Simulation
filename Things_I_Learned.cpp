//CODE:
//
//	what is the & sign doing in this statement: const std::vector<double>& x?
//
//	Reference (&)
//	1. This means that x is not a copy of the std::vector<double> passed to the function or variable.
//	
//	2. Instead, it refers to the same memory location as the original vector.
//
//	3. This avoids copying the data, which can be computationally expensive for large vectors, making the code more efficient.
//
//
//	Const (const):
//	
//	1. The const qualifier indicates that x cannot modify the referenced vector.
//	
//	2. You can read from the vector but cannot modify its contents or structure.
//
//	const std::vector<double>& x ensures the vector is passed efficiently (by reference) and prevents it from being modified in the vector
//
//	
//	
//	What is a std::unordered_map in C++?
//
//	- an unordered_map is a similar data structure as a dictionary in Python
//
//	
//	
//	
//	What is <Eigen/Dense> ?
//	- <Eigen/Dense> is a header file from the Eigen Library, a popular template library for linear algebra operations. 
//	This libary is widley used for tasks such as matrix and vector arithmetic, numerical solvers and more.
//
//	What is <cmath>
//	- <cmath> is a very similar library to math in python
//
//	In C++ it is standard practice to declare a header file (.h) and define them in a corresponding source file
//	(.cpp) with the same name. This allows other source files to access the function via the header.
//
//	size_t is a type in c++ that is commonly used for representing sizes and indicies of objects in memory. 
//	It is an unsigned integer type that is guaranteed to be large enough to represent the size of any
//	object in memory.
//
//	matplotlibcpp is a c++ library that provides a simple interface to use Python's popular matplotlib
//	plotting library directly within c++ programs. 
//
//	The code was orginally written in Python, rewriting it in C++ required frequent debugging to address challenges with data structures and logic. 
//	Through this process, I developed strong skills in efficiently using the debugger to diagnose and resolve issues.
//
//
//	AERODYNAMICS:
//
//	Aerodynamic instability refers to a condition in which an object or structure experiences unsteady or uncontrollable forces due to interactions with airflow, 
//	leading to oscillations, divergence, or other undesired behaviors. This instability occurs when aerodynamic forces and moments amplify rather than dampen motion, 
//	often resulting in structural failure or loss of control.
// 	
//
//
//
//
//
//
//
//
//
//
//
