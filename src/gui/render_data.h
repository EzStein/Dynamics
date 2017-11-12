#ifndef RENDER_DATA_GUARD
#define RENDER_DATA_GUARD
#include <list>
#include <vector>
#include "math/vector.h"
#include "math/static_matrix.h"
#include <climits>

/*A data structure representing a curve in n-space*/
struct solution {


  /*It is essentially just a list of coordinate vectors.
  Note that this is not a very efficient data structure
  since the imformation is not stored contiguously. This may
  be changed later. Vectors are stored in the order that
  they should be connected by lines*/
  std::list<math::vector<double> > points;

  /*Stores the set of initial conditions associated with this solution*/
  math::vector<double> initVals;
};

/*Contains all the information
needed to render to the openGL canvas.*/
struct render_data {
  /*Contains a list of solutions in no particular order*/
  std::vector<solution> solutions;

  /*FOR 2D calculations ONLY*/
  /*Indicates the span of the entire viewport in the x and y directions for 2d calculations*/
  double viewportSpanX, viewportSpanY;
  double viewportCenterX, viewportCenterY;

  /*Generates the matrix that maps values to -1,1 for opengl's use*/
  math::static_matrix<float, 4,4> generate_2d_transformation_matrix() {
    /*We essentially translate x,y by (-a,-b) and the scale by 2/span in each coordinate*/
    math::static_matrix<float, 4,4> mat(0);
    mat[0][0] = 2.0/viewportSpanX;
    mat[0][3] = -viewportCenterX*mat[0][0];

    mat[1][1] = 2.0/viewportSpanY;
    mat[1][3] = -viewportCenterY*mat[1][0];

    mat[2][2] = 1;
    mat[3][3] = 1;
    return mat;
  }

  /*Returns the inverse transformation matrix used to get the value of
  the mouse position based on its location on the gl canvas*/
  math::static_matrix<double, 4,4> generate_2d_inverse_transformation_matrix() {
    math::static_matrix<double, 4,4> mat(0);
    mat[0][0] = viewportSpanX/2;
    mat[0][3] = viewportCenterX;
    mat[1][1] = viewportSpanY/2;
    mat[1][3] = viewportCenterY;
    mat[2][2] = 1;
    mat[3][3] = 1;
    return mat;
  }
};

#endif
