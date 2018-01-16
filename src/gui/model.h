#ifndef MODEL_GUARD
#define MODEL_GUARD

/*
* This class is used to hold all the data behind the GUI system.
* In general, the data can only be accessed using getters and setters.
* In order to avoid using a complicated callback system, this class
* will retain a pointer to the VIEW (the top_frame) and will use
* this pointer update the UI for setters
*/

struct model {
private:
  top_frame* view;

  /*The current dimension is the number of equations used in this system of ODE's
  likewise, parameterCount is the number of parameters that may appear in those equations*/
  int dimension, parameterCount;

  /*Parameters stores the current value that each parameter (in order) holds
  initialValues stores what each variable including variable t holds as its initial value*/
  std::vector<double> parameters, initialValues;

  /*The max min and increment values for variable t*/
  double tMax, tMin, tInc;

  /*
  * These indicate the variable be represented on the axes for the given planes.
  * In the parameter plane, a 0 indicates the first parameter a, 1 for b, 2 for c, etc...
  * In the dynamical plane, 0 indicates the first variable t, 1 for x, 2 for y, etc...
  */
  int parameterPlaneAxisOneVariable, parameterPlaneAxisTwoVariable;
  int dynamicalPlaneAxisOneVariable, dynamicalPlaneAxisTwoVariable, dynamicalPlaneAxisThreeVariable;

  /*Indicates the current rendering mode. 0 indicates 2d rendering, 1 indicates 3d perspective rendering*/
  int renderMode;

  /*This is a list of the equations that will be compiled into syntax trees on a call to compile*/
  std::vector<std::string> equationStrings;

  /*A list of the abstract syntax tree representations of the equations*/
  std::vector<AST> equationASTs;

  /*
  * This is a list of the pointers to the compiled equations in executable memory.
  */
  std::vector<driver::double_func_t> equationFunctionPointers;

  /*
  * A list of functions pointers to the where each element is an entry in row-major order
  * of a function of the jacobian of the vector field. It is actually the jacobian of the provided
  * vector field with the additional equation prepended to the front: t = 0
  */
  std::vector<driver::double_func_t> jacobianFunctionPointers;

};

#endif
