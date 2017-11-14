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
  bool redraw = false;
  bool render2d = true;
  int dimension = 3, parameters = 3;


  /*The following variables are used in 3D mode*/
  bool perspectiveProjection = true;
  math::static_vector<double, 3> cameraPosition;

  /*The direction that the camera is pointing in. This does not have to be a unit vector*/
  math::static_vector<double, 3> cameraDirection;


  math::static_matrix<float, 4,4> generate_view_matrix() {
    /*We first compute the right, up and direction unit vectors*/
    math::static_vector<double, 3> up;
    up[0] = 0;
    up[1] = 1;
    up[2] = 0;

    math::static_vector<double, 3> rightVector(math::cross(cameraDirection, up));
    math::static_vector<double, 3> upVector(math::cross(cameraDirection, rightVector));
    cameraDirection.normalize();
    rightVector.normalize();
    upVector.normalize();

    math::static_matrix<float, 4,4> translation(0);
    translation[0][0] = 1;
    translation[1][1] = 1;
    translation[2][2] = 1;
    translation[3][3] = 1;
    translation[0][3] = -cameraPosition[0];
    translation[1][3] = -cameraPosition[1];
    translation[2][3] = -cameraPosition[2];

    math::static_matrix<float, 4,4> rotation(0);
    rotation[3][3] = 1;

    rotation[0][0] = rightVector[0];
    rotation[0][1] = rightVector[1];
    rotation[0][2] = rightVector[2];

    rotation[1][0] = upVector[0];
    rotation[1][1] = upVector[1];
    rotation[1][2] = upVector[2];

    rotation[2][0] = cameraDirection[0];
    rotation[2][1] = cameraDirection[1];
    rotation[2][2] = cameraDirection[2];

    return rotation * translation;
  }

  math::static_matrix<float, 4,4> generate_projection_matrix() {
    /*We first calculate the coordinates defining the near plane (which is orthogonal to and intersects
  the -z axis as well as the distance from the origin to this plane and to the far plane)*/
    double nearDistance = 0.1, farDistance = 500, leftCoordinate=-0.1, rightCoordinate=0.1, topCoordinate=0.1, bottomCoordinate=-0.1;
    math::static_matrix<float, 4,4> mat(0);
    if(perspectiveProjection) {
      mat[0][0] = 2*nearDistance/(rightCoordinate-leftCoordinate);
      mat[0][2] = (rightCoordinate + leftCoordinate)/(rightCoordinate - leftCoordinate);
      mat[1][1] = 2*nearDistance/(topCoordinate - bottomCoordinate);
      mat[1][2] = (topCoordinate + bottomCoordinate)/(topCoordinate - bottomCoordinate);
      mat[2][2] = -(farDistance + nearDistance)/(farDistance - nearDistance);
      mat[2][3] = -2*farDistance*nearDistance/(farDistance - nearDistance);
      mat[3][2] = -1;
    } else {
      /*We calculate an orthogonal projection*/
      mat[0][0] = 2/(rightCoordinate - leftCoordinate);
      mat[0][3] = -(rightCoordinate + leftCoordinate)/(rightCoordinate - leftCoordinate);
      mat[1][1] = 2/(topCoordinate - bottomCoordinate);
      mat[1][3] = -(topCoordinate + bottomCoordinate)/(topCoordinate - bottomCoordinate);
      mat[2][2] = -2/(farDistance - nearDistance);
      mat[2][3] = -(farDistance + nearDistance)/(farDistance - nearDistance);
      mat[3][3] = 1;
    }
    return mat;
  }

  math::static_matrix<float, 4,4> generate_model_matrix() {
    math::static_matrix<float, 4,4> identity(0);
    identity[0][0] = 1;
    identity[1][1] = 1;
    identity[2][2] = 1;
    identity[3][3] = 1;
    return identity;
  }

  /*An array of 3 ints that mark which variable corresponds to which axes.
  The variable number in axesVariable[0] is corresponds to the x axis, then the y, then the z axis.
  Note that in 2d mode, only the first two entries are used.*/
  int axesVariable[3];

  /*Generates the matrix that maps values to -1,1 for opengl's use*/
  math::static_matrix<float, 4,4> generate_2d_transformation_matrix() {
    /*We essentially translate x,y by (-a,-b) and the scale by 2/span in each coordinate*/
    math::static_matrix<float, 4,4> mat(0);
    mat[0][0] = 2.0/viewportSpanX;
    mat[0][3] = -viewportCenterX*mat[0][0];

    mat[1][1] = 2.0/viewportSpanY;
    mat[1][3] = -viewportCenterY*mat[1][1];

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
