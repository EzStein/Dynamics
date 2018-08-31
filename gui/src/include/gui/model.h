#ifndef DYNSOLVER_GUI_MODEL_H_
#define DYNSOLVER_GUI_MODEL_H_

#include <vector>
#include <unordered_map>
#include <map>
#include <stdexcept>

#include <glad/glad.h>

#include "gui/common.h"
#include "compiler/ast/AST.h"
#include "compiler/function.h"
#include "math/window2d.h"
#include "math/window3d.h"
#include "gl/buffer.h"
#include "gl/program.h"
#include "gl/vertex_array.h"
#include "gl/font.h"
#include "gl/text_renderer.h"
#include "math/util.h"
#include "math/eigenvalue.h"

class wxGLCanvas;
class wxGLContext;

namespace dynsolver {
namespace math {
class square_matrix;
}
namespace gui {

class app;

class color {
private:
  double r, g, b, a;
public:
  // Constructs a color from RGBA values given in the range,
  // 0 to 1.
  color(double, double, double, double);

  // Retrieve the r g b and a values.
  double get_red() const;
  double get_green() const;
  double get_blue() const;
  double get_alpha() const;

  void set_red(double);
  void set_green(double);
  void set_blue(double);
  void set_alpha(double);

  // Inverts this color.
  void invert();
};

struct solution_specs {
  solution_specs();

  // The color that should be used to render this solution.
  color glColor;
  
  // The starting value of the dynamical variables. This does not include
  // the variable of differentiation, and it is ordered according to
  // the dynamicalVarNames vector.
  ::math::vector init;

  // This is the starting value of the variable of differentiation.
  double tStart;

  // Invariant:
  // tMax must be strictly greater than tMin. initial[0] must be
  // strictly between tMax and tMin
  // increment is strictly positive. 
  
  // The bounds on how far tMax and tMin should run.
  double tMax;
  double tMin;

  // Currently we use standard euler's method. This indicates the standard
  // increment.
  double inc;
};

// A specification for drawing a solution through an initial point.
struct solution {
  solution_specs specs;
  std::list<::math::dynamical_point> data;
};

struct singular_point_specs {
  singular_point_specs();
  // The initial point used to compute the position of the singular point
  // in newtons method.
  ::math::vector init;
  color glColor;
};

struct singular_point {
  enum class type {
    STABLE_NODE,  // All real and same sign -
    UNSTABLE_NODE, // All real and same sign +
    SADDLE, // All real, opposate signs.
    
    STABLE_FOCUS, // All Non-real, neagitve real part
    UNSTABLE_FOCUS, // All non-real positive real part
    SADDLE_FOCUS, // All non-real mixed real part
    
    STABLE_FOCUS_NODE, // Mixed real and non-real, same real part sign, -
    UNSTABLE_FOCUS_NODE, // Mixed real and non-real, same real part sign, +
    SADDLE_FOCUS_NODE // Mixed real and non-real, different real part signs.
    
  };
  singular_point(const singular_point_specs& specs = singular_point_specs(),
	  const ::math::vector& position = ::math::vector(),
	  const std::vector<::math::eigenvalue>& eigenvalues = std::vector<::math::eigenvalue>(),
	  const type& type = type::STABLE_NODE) :
	  specs(specs), position(position), eigenvalues(eigenvalues), type(type) { }
  singular_point_specs specs;
  ::math::vector position;
  
  // A list of eigenvalues and their corresponding eigenvectors
  // for the linearization of the vector field at this point.
  std::vector<::math::eigenvalue> eigenvalues;

  type type;
};

struct isocline_specs {
  isocline_specs();
  // This is the seed used in newtons method to find the isocline.
  // It is a vector of dynamical variables ordered according to
  // dynamicalVarNames.
  ::math::vector init;

  // This is the direction that the vector field points in on this isocline.
  // It is also ordered according to dynamicalVarNames.
  ::math::vector direction;

  // This is the increment (in some direction) that is used when
  // computing the isocline.
  double inc;

  // This is the number of vertices to compute in either direction of the
  // starting point.
  int span;

  // This is the radius about the init points for which we attempt to
  // find an isocline. If the the isocline falls outside of this radius,
  // we may still find it. However we did not find the isocline,
  // it is likely not in this radius. The granularity of search,
  // is set by, searchInc;
  double searchRadius;
  double searchInc;

  color glColor;
};

struct isocline {
  isocline_specs specs;

  // This is the data representing the isocline. It is ordered so that it
  // may be drawn in a single GL_LINE_STRIP primative.
  std::list<::math::vector> data;
};

struct separatrix_specs {
  separatrix_specs();
  
  // Whether this is a stable or unstable separatrix.
  enum class type {
    STABLE, UNSTABLE
  };
  type type;
  
  // The singular point associated with this separatrix.
  singular_point_id singularPoint;

  // Euler's method stuff
  // Positive increment for euler's method.
  double inc;

  // If an unstable separatrix this is the amount of
  // positive time to run the separatrix. For a stable separatrix,
  // this is the amount of negative time to run the separatrix.
  double time;

  // This indicates which of the two solutions on the invariant we intend
  // to draw. True indicates along the unstable eigenvector and false is
  // along in the opposate direction.
  bool direction;

  color glColor;
};

struct separatrix {
  separatrix_specs specs;
  // A list of vectors where the vectors hold the dynamical variables
  // in the order given by dynamicalVarNames. Does not hold the variable
  // of differentiation.
  std::list<::math::vector2d> data;
};

// Represent a point in the dynamical and parameter space.
// Does not include the variable of differentiation t.
struct bifurcation_point {
  ::math::vector dynamicalVars;
  ::math::vector parameters;
};

struct gen_singular_point_ret {
  singular_point singularPoint;
  bool success;

  gen_singular_point_ret();
  gen_singular_point_ret(const singular_point& singularPoint, bool success);
};

struct gen_separatrix_ret {
  singular_point singularPoint;
  separatrix sep;
  bool success;

  gen_separatrix_ret();
  gen_separatrix_ret(const singular_point& singularPoint,
		     const separatrix& sep,
		     bool success);
};

struct find_separatrix_intersection_ret {
  // The new singular point position.
  ::math::vector2d intersection;
  bool success;

  find_separatrix_intersection_ret();
  find_separatrix_intersection_ret(const ::math::vector2d intersection,
				   bool success);
};

class no_intersection_found_exception : public std::runtime_error {
public:
  no_intersection_found_exception() : std::runtime_error("") { }
};

struct hopf_bifurcation_specs {
  hopf_bifurcation_specs();
  bifurcation_point init;
  
  double inc;

  int span;

  double searchRadius;
  double searchInc;

  color glColor;
};

struct hopf_bifurcation {
  hopf_bifurcation_specs specs;
  std::list<::math::vector> data;
};

struct saddle_node_bifurcation_specs {
  saddle_node_bifurcation_specs();
  bifurcation_point init;
  
  double inc;
  
  int span;

  double searchRadius;
  double searchInc;

  color glColor;
};

struct saddle_node_bifurcation {
  saddle_node_bifurcation_specs specs;
  std::list<::math::vector> data;
};

struct saddle_connection_bifurcation_specs {
  saddle_connection_bifurcation_specs();

  // Holds the initial parameter condition values.
  ::math::vector init;

  // The two separatrices which we are attempting to connect.
  separatrix_id separatrix1;
  separatrix_id separatrix2;

  // These two vectors define a line in the dynamical space which is transverse
  // to the saddle separatrices. We minimize the distance between the two
  // separatrices on this line.
  ::math::vector2d transversalA;
  ::math::vector2d transversalB;
  
  double inc;

  int span;

  double searchRadius;
  double searchInc;

  color glColor;
};

struct saddle_connection_bifurcation {
  saddle_connection_bifurcation_specs specs;
  std::list<::math::vector> data;
};

struct limit_cycle_bifurcation_specs {
  limit_cycle_bifurcation_specs();
  // Holds the initial parameter conditions.
  ::math::vector init;

  periodic_solution_id cycle1;
  periodic_solution_id cycle2;

  // These two vectors define a line in the dynamical space which is transverse
  // to the limit cycles. We minimize the distance between the two
  // cycles on this line.
  ::math::vector transversalA;
  ::math::vector transversalB;
  
  double inc;

  int span;

  double searchRadius;
  double searchInc;

  color glColor;
};

struct limit_cycle_bifurcation {
  limit_cycle_bifurcation_specs specs;
  std::list<::math::vector> data;
};

// Used to pass to and from the dynamical window dialog to obtain
// imformation on the viewport and axes for generating or changing a dynamical
// window.
struct dynamical_specs {
  bool is3d;

  // 2d
  std::string horzAxisVar;
  std::string vertAxisVar;
  ::math::window2d viewport2d;

  // 3d
  ::math::window3d viewport3d;
  std::string xAxisVar;
  std::string yAxisVar;
  std::string zAxisVar;
};

struct parameter_specs {
  std::string horizAxisVar;
  std::string vertAxisVar;
  ::math::window2d viewport;
};

// This class is used to hold all the data behind the GUI system.
// In general, the data can only be accessed using getters and setters.
// This class almost completely decoupled from the view. It is the
// responsibility of the view to update the user interface when it expects that
// a value in the model has changed. The exception is that all opengl work
// occurs in this file.
//
// NOTE: The model may NOT be constructed until an opengl context is set
// current, and the opengl function pointers are loaded.
class model {
 private:
  // Nested Inner classes
  // A tuple which associates an abstract syntax tree with its compiled
  // function.
  struct expression {
    AST ast;
    compiler::function<double, const double*> function;
  };

  class parameter_window {
  private:
    // A reference to the model.
    const model& modelData;

    // The specification for this window.
    parameter_specs specs;

    double realTickDistanceX;
    double realTickDistanceY;
    gl::buffer axesVbo;
    int ticksPerLabel;
    double tickFontSize;
    gl::buffer crossVbo;
    GLsizei crossVboVertices;

    // Represents the render data for a bifurcation.
    struct render_data {
      gl::buffer vbo;
      GLsizei vertices;
    };

    std::unordered_map<hopf_bifurcation_id, render_data> hopfBifurcationData;
    std::unordered_map<saddle_node_bifurcation_id, render_data>
    saddleNodeBifurcationData;
    std::unordered_map<limit_cycle_bifurcation_id, render_data>
    limitCycleBifurcationData;
    std::unordered_map<saddle_connection_bifurcation_id, render_data>
    saddleConnectionBifurcationData;
    
  public:
    parameter_window(model&, const parameter_specs& spec);

    bool on_parameter_position(const ::math::vector2d& pos) const;

    // Update
    void set_specs(const parameter_specs& spec);

    // Sets the 2d viewport.
    void set_viewport(const ::math::window2d&);

    // Renders the window to the currently bound context.
    void paint();

    // Called when the parameter window is resized. The new
    // width and height is given.
    void resize(int width, int height);

    // Read
    // Returns true if the point given in pixels is on the vertical/horizontal
    // axis.
    bool on_vert_axis(const ::math::vector2d&) const;
    bool on_horiz_axis(const ::math::vector2d&) const;
    
    // Gets and sets the specifications.
    const parameter_specs& get_specs() const;

    void gen_hopf_bifurcation_vbo(hopf_bifurcation_id);
    void gen_saddle_node_bifurcation_vbo(saddle_node_bifurcation_id);
    void gen_limit_cycle_bifurcation_vbo(limit_cycle_bifurcation_id);
    void gen_saddle_connection_bifurcation_vbo(saddle_connection_bifurcation_id);

    void delete_hopf_bifurcation(hopf_bifurcation_id);
    void delete_saddle_node_bifurcation(saddle_node_bifurcation_id);
    void delete_limit_cycle_bifurcation(limit_cycle_bifurcation_id);
    void delete_saddle_connection_bifurcation(saddle_connection_bifurcation_id);

    bool select_hopf_bifurcation(int x, int y, hopf_bifurcation_id*);
    bool select_saddle_node_bifurcation(int x, int y, saddle_node_bifurcation_id*);
    bool select_limit_cycle_bifurcation(int x, int y, limit_cycle_bifurcation_id*);
    bool select_saddle_connection_bifurcation(int x, int y, saddle_connection_bifurcation_id*);
    
    ::math::vector get_point(const ::math::vector2d&) const;

  private:
    void draw_hopf_bifurcations();
    void draw_saddle_node_bifurcations();
    void draw_limit_cycle_bifurcations();
    void draw_saddle_connection_bifurcations();
    
  };
  
  // Represents information associated with an instance of a dynamical window.
  // Multiple dynamical windows may be open at any given time.
  class dynamical_window {
  private:
    // A reference to the containing model. Since this is an inner class of
    // model, it may access all of its private variables.
    const model& modelData;

    // The specification from which VBO's and other data is generated.
    dynamical_specs specs;

    // The VBO for storing the axes vertex info.
    gl::buffer axesVbo;

    // Each solution is associated with render data containing a VBO each for
    // its 2d and 3d rendering.
    struct render_data {
      // The 2d vbo is stores the projection of a solution onto the plane that
      // is currently being viewed. Each vertex contains one attribute of 2
      // floats. Thus the stride is also 2 floats.
      gl::buffer vbo2d;

      // Contains the projection onto the 3d space currently associated with
      // this window. Each vertex contains one attribute of 3 floats. The
      // stride is 3 floats.
      gl::buffer vbo3d;

      // The number of vertices is the same for both VBO's
      size_t vertices;
    };

    // Maps each solution to its render data.
    std::unordered_map<solution_id, render_data> solutionData;

    std::unordered_map<isocline_id, render_data> isoclineData;

    std::unordered_map<separatrix_id, render_data> separatrixData;

    // The number of ticks between each labeled tick.
    int ticksPerLabel;

    // The font size of the labels on the ticks.
    double tickFontSize;

    // The current tick distance in real coordnate values.
    double realTickDistanceX;
    double realTickDistanceY;
    
  public:
    dynamical_window(model&, const dynamical_specs& spec);

    // Update
    void set_specs(const dynamical_specs& spec);

    // Sets the 2d viewport.
    void set_viewport_2d(const ::math::window2d&);

    // Sets the 3d viewport.
    void set_viewport_3d(const ::math::window3d&);

    // Renders the window to the currently bound context.
    void paint();

    // Called when the dynamical window is resized. The new
    // width and height is given.
    void resize(int width, int height);

    // Read
    // Returns true if the point given in pixels is on the vertical/horizontal
    // axis.
    bool on_vert_axis(const ::math::vector2d&) const;
    bool on_horiz_axis(const ::math::vector2d&) const;
    // Gets and sets the specifications.
    const dynamical_specs& get_specs() const;

    // Delete
    // Deletes the VBO associated with the given solution.
    void delete_solution(solution_id);

    // Deletes the isocline's render data.
    void delete_isocline(isocline_id);

    void delete_separatrix(separatrix_id);

    // Select
    // Attempts to select the solution near the mouse cursor at position,
    // x and y. Returns the selected solution through id. If no solution is
    // found, returns false and does not modify id.
    bool select_solution(int x, int y, solution_id* id);
    bool select_isocline(int x, int y, isocline_id* id);
    bool select_singular_point(int x, int y, singular_point_id* id);
    bool select_separatrix(int x, int y, separatrix_id* id);

    ::math::dynamical_point get_point(const ::math::vector2d& pos) const;

    // Generates a VBO for the corresponding solution data and id.
    // Replaces the VBO for that ID if it already exists.
    void gen_solution_vbo(solution_id);

    // Generates a VBo for the corresponding isocline data and id.
    // Replaces the VBO for that ID if it already exists.
    void gen_isocline_vbo(isocline_id);

    void gen_separatrix_vbo(separatrix_id);

  private:
    // Draw
    void draw_solutions_3d();
    void draw_isoclines_3d();
    void draw_singular_points_3d();
    void draw_axes_3d();

    void draw_solutions_2d();
    void draw_isoclines_2d();
    void draw_singular_points_2d();
    void draw_separatrices_2d();
    
    
    typedef std::unordered_map<solution_id, render_data>::const_iterator
    solution_data_const_iter;

    typedef std::unordered_map<solution_id, render_data>::const_iterator
    isocline_data_const_iter;

    typedef std::unordered_map<separatrix_id, render_data>::const_iterator
    separatrix_data_const_iter;
  };

  // Model Class
  // Static constants used for opengl shaders.
  static const std::string k2dVertexShaderFilePath;
  static const GLuint k2dPositionAttribute;
  static const GLuint k2dVertexBinding;
  static const std::string k2dTransformationUniform;
  
  static const std::string k2dFragmentShaderFilePath;
  static const std::string k2dColorUniform;

  static const std::string kPath3dVertexShaderFilePath;
  static const GLuint kPath3dPositionAttribute;
  static const GLuint kPath3dVertexBinding;
  static const std::string kPath3dTransformationUniform;
  
  static const std::string kPath3dFragmentShaderFilePath;
  static const std::string kPath3dColorUniform;

  // A font and text renderer
  gl::font font;
  gl::text_renderer textRenderer;
  
  // The number of dynamical variables involved excluding the variable of
  // differentiation, t. Should always be 1 or greater. This number shall be
  // denoted as d.
  int dynamicalVars;

  // Indicates the number of parameters used.
  // Must be nonnegative.
  int parameters;

  // A list of expressions corresponding to the abstract syntax trees.
  // The size is the number of dynamical variables, and it is ordered according
  // to the ordering in dynamicalVarNames.
  std::vector<std::string> expressions;


  // This maps symbols in the compiled expression to the index that they are
  // associated with. That is, the index of the double array that is passed,
  // to the compiled functions.
  std::map<std::string, int> symbolsToIndex;

  // This is the transpose of the above symbols to index map.
  std::unordered_map<int, std::string> indexToSymbols;

  // Each symbol is also either a dynamical variable, parameter,
  // or the variable of differentiation. That information along with the
  // ordering of parameters and dynamical variables is stored in these vectors.
  std::vector<std::string> dynamicalVarNames;
  // The index of the dynamical variable in the dynamicalVarNames vector.
  std::map<std::string, int> dynamicalVarIndex;
  std::vector<std::string> parameterNames;
  std::map<std::string, int> parameterIndex;

  // Maps the parameterIndex to its symbol index.
  std::vector<int> parameterIndexToSymbol;
  std::vector<int> dynamicalIndexToSymbol;
  std::string varDiffName;
  int varDiffIndex;


  // Stores the current values of all the parameters.
  ::math::vector parameterPosition;

  // Invariant on the above members maintains that the size of
  // dynamicalVarNames and parameterNames are consistent with
  // dynamicalVariables and parameters. In addition, the indexes mapped
  // in indexToSymbols form a contiguous range from 0 to
  // dynamicalVars + parameters exclusive. And the strings
  // that they are mapped to are precisely those strings contained in the
  // vectors.
  
  // The following variables are updated on every recompile.
  
  // The following stores CAS syntax tree info.
  // The system of equations as a syntax tree.
  // These two variables are updated everytime a string is compiled.
  // Variables are ordered from 0 to d.
  // We require that jacobian has size of d * d.
  // System.size() is equal to d.
  // Each index in the system corresponds to the order that the dynamical
  // variable appears in dynamicalVarNames.
  std::vector<expression> system;

  // A double array of partial derivatives of the expressions.
  // The first index indicates the expression as ordered in
  // dynamicalVarIndex. The second index indicates the variable
  // as ordered in symbolToIndex.
  std::vector<std::vector<expression>> partials;

  // The first index is the index of the dynamical expression.
  // The second two indexes refer to the two variables of
  // differentiation. This first (leftmost) is the first
  // partial application,
  // and the second (rightmost) is the second partial application.
  std::vector<std::vector<std::vector<expression>>> doublePartials;

  // Each dynamical and parameter window will have an id. The id is associated
  // with the some information in the following maps. The id is also used to
  // lookup the appropriate frames in the app class.
  // Consider using strong typdefs
  std::unordered_map<dynamical_id, dynamical_window> dynamicalWindows;
  std::unordered_map<parameter_id, parameter_window> parameterWindows;


  // The following variables are updated on each change to parameterPosition
  // and to each change to system or jacobian (on any recompilation).
  
  // The following variables demonstrate what to draw on the dynamical plane.
  // They are specifications for what to draw, and the actually pixel data.
  // Each entry in the following maps is given by a unique id.
  std::unordered_map<solution_id, solution> solutions;
  std::unordered_map<singular_point_id, singular_point> singularPoints;
  std::unordered_map<isocline_id, isocline> isoclines;
  std::unordered_map<separatrix_id, separatrix> separatrices;
  std::unordered_map<hopf_bifurcation_id, hopf_bifurcation> hopfBifurcations;
  std::unordered_map<saddle_node_bifurcation_id, saddle_node_bifurcation>
  saddleNodeBifurcations;
  std::unordered_map<saddle_connection_bifurcation_id, saddle_connection_bifurcation>
  saddleConnectionBifurcations;
  std::unordered_map<limit_cycle_bifurcation_id, limit_cycle_bifurcation>
  limitCycleBifurcations;

  // This value indicates which object is currently selected. If the value is
  // set to one of the kNoId constants above, then no object of that type is
  // selected.
  solution_id selectedSolutionId;
  singular_point_id selectedSingularPointId;
  isocline_id selectedIsoclineId;
  separatrix_id selectedSeparatrixId;
  hopf_bifurcation_id selectedHopfBifurcationId;
  saddle_node_bifurcation_id selectedSaddleNodeBifurcationId;
  limit_cycle_bifurcation_id selectedLimitCycleBifurcationId;
  saddle_connection_bifurcation_id selectedSaddleConnectionBifurcationId;

  // These unique id's are all greater than or equal to any id's mapped in the
  // corresponding maps. When adding a new value, the unique id is incremented
  // and then the new value is inserted with the new id.
  dynamical_id uniqueDynamicalId;
  parameter_id uniqueParameterId;
  solution_id uniqueSolutionId;
  singular_point_id uniqueSingularPointId;
  isocline_id uniqueIsoclineId;
  separatrix_id uniqueSeparatrixId;
  hopf_bifurcation_id uniqueHopfBifurcationId;
  saddle_node_bifurcation_id uniqueSaddleNodeBifurcationId;
  limit_cycle_bifurcation_id uniqueLimitCycleBifurcationId;
  saddle_connection_bifurcation_id uniqueSaddleConnectionBifurcationId;
  
  // True if the model is currently representing a compiled system.
  // False if no system is being viewed. If false, the following variables,
  // have no meaning and do not need to satisfy any invariants.
  bool compiled;

  // Opengl variables. There are two programs, the 2d and Path3d.
  gl::vertex_array vao2d;
  gl::program program2d;

  gl::vertex_array vaoPath3d;
  gl::program programPath3d;
  
  const GLuint k2dTransformationUniformLocation;
  const GLuint k2dColorUniformLocation;
  const GLuint kPath3dTransformationUniformLocation;
  const GLuint kPath3dColorUniformLocation;

  // Contains vertex data for rendering a circle of radius 1.
  gl::buffer circleVbo;
  // The number of vertices stored in the circle vbo.
  GLsizei circleVboVertices;

  gl::buffer triangleVbo;
  GLsizei triangleVboVertices;

  gl::buffer squareVbo;
  GLsizei squareVboVertices;

  gl::buffer starVbo;
  GLsizei starVboVertices;

  // Private functions.
  std::vector<gl::shader> build_shaders_2d();
  std::vector<gl::shader> build_shaders_path_3d();

  // (Re)Generates the solution data for the given id using the
  // specification associated with that solution.
  void generate_solution_data(solution_id id);

  // (Re)Generates the isocline data for the given id using the
  // specification associated with that solution. If it fails to
  // find an isocline for that data, returns false.
  bool generate_isocline_data(isocline_id id);

  // (Re)Generates the singular point data for the given id using the
  // specification associated with that solution. If it fails to
  // find an isocline for that data, returns false.
  bool generate_singular_point_data(singular_point_id id);

  // Attempts to generate the separatrix data. This may fail, if
  // the singular point associated with the separatrix no longer exists or
  // if the singular point is no longer a saddle.
  // If so, this function returns false and does not modify the separatrix.
  bool generate_separatrix_data(separatrix_id id);

  // For the given singular point specification and a given separatrix
  // specification, this method first finds a singular point, and then
  // computes its corresponding separatrix at the provided parameter position.
  // If the singular point could not be found, or it is not a saddle, this
  // function returns a struct with the success flag set to false. Otherwise,
  // it returns the separatrix data.
  gen_separatrix_ret gen_separatrix(const singular_point_specs& singularPointSpecs,
				    const separatrix_specs& separatrixSpecs,
				    ::math::vector parameters);

  // Using the given singular point sepecification, this generates a singular
  // point with the given parameters.
  gen_singular_point_ret gen_singular_point(const singular_point_specs&,
					    const ::math::vector& parameters);

  bool generate_hopf_bifurcation_data(hopf_bifurcation_id);
  bool generate_saddle_node_bifurcation_data(saddle_node_bifurcation_id);

  bool generate_saddle_connection_bifurcation_data(saddle_connection_bifurcation_id);
  bool generate_limit_cycle_bifurcation_data(limit_cycle_bifurcation_id);

  // Computes the intersection between the provided separatrix and the
  // transversal at the given parameter position. start is the starting
  // condition used to search for the new singular point position with the new
  // parameters.
  find_separatrix_intersection_ret
  find_separatrix_intersection(double paramA,
			       double paramB,
			       double sing1,
			       double sing2,
			       const ::math::vector2d& transA,
			       const ::math::vector2d& transB,
			       const separatrix_id& id);
  
  typedef std::unordered_map<solution_id, solution>::const_iterator
  solution_const_iter;
  typedef std::unordered_map<dynamical_id, dynamical_window>::const_iterator
  dynamical_const_iter;
  typedef std::unordered_map<parameter_id, parameter_window>::const_iterator
  parameter_const_iter;
  typedef std::unordered_map<singular_point_id, singular_point>::const_iterator
  singular_point_const_iter;
  typedef std::unordered_map<isocline_id, isocline>::const_iterator
  isocline_const_iter;
  typedef std::unordered_map<separatrix_id, separatrix>::const_iterator
  separatrix_const_iter;

  typedef std::unordered_map<solution_id, solution>::iterator
  solution_iter;
  typedef std::unordered_map<dynamical_id, dynamical_window>::iterator
  dynamical_iter;
  typedef std::unordered_map<parameter_id, parameter_window>::iterator
  parameter_iter;
  typedef std::unordered_map<singular_point_id, singular_point>::iterator
  singular_point_iter;
  typedef std::unordered_map<isocline_id, isocline>::iterator
  isocline_iter;
  typedef std::unordered_map<separatrix_id, separatrix>::iterator
  separatrix_iter;
  
public:
  // This value is never mapped. Usually this is set to 0.
  static const dynamical_id kNoDynamicalId;
  static const parameter_id kNoParameterId;
  static const solution_id kNoSolutionId;
  static const singular_point_id kNoSingularPointId;
  static const isocline_id kNoIsoclineId;
  static const separatrix_id kNoSeparatrixId;
  static const hopf_bifurcation_id kNoHopfBifurcationId;
  static const saddle_node_bifurcation_id kNoSaddleNodeBifurcationId;
  static const limit_cycle_bifurcation_id kNoLimitCycleBifurcationId;
  static const saddle_connection_bifurcation_id kNoSaddleConnectionBifurcationId;
  
  // The maximal distance between two axis ticks in pixels. If the number of
  // axis tickets jumps above the maximum, then the number of tickets are
  // doubled, and the real distance between each tick is halved. If it falls
  // below the minimum, the distance between each tick is doubled.
  static const int minPixelTickDist;
  static const int maxPixelTickDist;
  
  model();

  // Draws the 2d axes to the bound context. Updates
  // the realTickDistances accordingly.
  void draw_axes_2d(const ::math::window2d& viewport,
		    int ticksPerLabel,
		    double tickFontSize,
		    double& realTickDistanceX,
		    double& realTickDistanceY,
		    gl::buffer& axesVbo) const;

  // Called to clear what is currently compiled.
  void set_no_compile();

  // Tells opengl to paint the given dynamical window according to the state
  // contained in this model. Note the the opengl context must be bound to the
  // appropriate window before calling this method, and the buffers must
  // be swapped after calling this method.
  void paint_dynamical(dynamical_id id);
  void paint_parameter(parameter_id id);

  // Resizes the dynamical window so that the width and height are given by
  // the new values in pixels.
  void resize_dynamical(dynamical_id id, int width, int height);
  void resize_parameter(parameter_id id, int width, int height);
  

  // Resets most variables and compiles the array of strings in newExpressions
  // into a vector field system of ODE's.  Returns true
  // on success. In the future, this function will return information
  // on why the compilation failed if it does fail.
  // Requires that the vector has size 1 or more.
  bool compile(const std::string& newVarDiffName,
	       const std::vector<std::string>& newDynamicalVarNames,
	       const std::vector<std::string>& newParameterNames,
	       const std::vector<std::string>& newExpressions);

  // Add
  // Adds the dynamical window according to the specification provided.
  dynamical_id add_dynamical(const dynamical_specs&);

  parameter_id add_parameter(const parameter_specs&);

  // Adds the initial value solution. The solution is added according to the
  // specification provided.
  void add_solution(const solution_specs&);
  
  // Attempts find and add the singular point. If newtons method fails,
  // this method returns false. If the singular point already exists,
  // nothing more is done.
  bool add_singular_point(const singular_point_specs&);

  // Attempts to add the isocline. Returns true on success, and false
  // if the isocline could not be found with the given conditions.
  bool add_isocline(const isocline_specs& specs);

  void add_separatrix(const separatrix_specs& specs);

  bool add_hopf_bifurcation(const hopf_bifurcation_specs&);
  bool add_saddle_node_bifurcation(const saddle_node_bifurcation_specs&);
  bool add_limit_cycle_bifurcation(const limit_cycle_bifurcation_specs&);
  bool add_saddle_connection_bifurcation(const saddle_connection_bifurcation_specs&);


  // Delete
  // Deletes all the data associated with the given window and removes
  // it from the model.
  void delete_dynamical(dynamical_id id);

  void delete_parameter(parameter_id id);

  // Deletes the solution.
  void delete_solution(solution_id id);

  // Deletes this singular point.
  void delete_singular_point(singular_point_id id);

  // Deletes the isocline.
  void delete_isocline(isocline_id);

  void delete_separatrix(separatrix_id);

  void delete_hopf_bifurcation(hopf_bifurcation_id);
  void delete_saddle_node_bifurcation(saddle_node_bifurcation_id);
  void delete_limit_cycle_bifurcation(limit_cycle_bifurcation_id);
  void delete_saddle_connection_bifurcation(saddle_connection_bifurcation_id);

  // Select
  // Attempts to select the solution below the cursor at x, y in the dynamical
  // window given by id. On success, returns true. The solution found can be
  // checked throught get_selected_solution_id().
  // This function will work regardless of whether
  // the dynamical plane is viewing in 2 or 3 dimensions.
  bool select_solution(dynamical_id id, int x, int y);

  // Performs the same as select_solution except operates on isoclines and
  // singular points
  bool select_isocline(dynamical_id, int x, int y);
  bool select_singular_point(dynamical_id, int x, int y);
  bool select_separatrix(dynamical_id, int x, int y);
  bool select_hopf_bifurcation(parameter_id, int, int);
  bool select_saddle_node_bifurcation(parameter_id, int, int);
  bool select_limit_cycle_bifurcation(parameter_id, int, int);
  bool select_saddle_connection_bifurcation(parameter_id, int, int);

  // Selects the object directly
  void select_solution(solution_id id);
  void select_singular_point(singular_point_id id);
  void select_isocline(isocline_id id);
  void select_separatrix(separatrix_id id);
  void select_hopf_bifurcation(hopf_bifurcation_id);
  void select_saddle_node_bifurcation(saddle_node_bifurcation_id);
  void select_limit_cycle_bifurcation(limit_cycle_bifurcation_id);
  void select_saddle_connection_bifurcation(saddle_connection_bifurcation_id);

  // Deselects the solution that is currently selected. Does nothing if no
  // solution is selected.
  void deselect_solution();

  // Does the same as the above functions but operates on isoclines or
  // singular points.
  void deselect_isocline();
  void deselect_singular_point();
  void deselect_separatrix();
  void deselect_hopf_bifurcation();
  void deselect_saddle_node_bifurcation();
  void deselect_limit_cycle_bifurcation();
  void deselect_saddle_connection_bifurcation();


  // Update: Some specs need to be dynamically updated. More fine grained
  // control is provided if such functions are common.
  // Sets the dynamical window specification. This will update the
  // necessary VBO's if that is needed.
  void set_dynamical_specs(dynamical_id id, const dynamical_specs& spec);
  
  // Sets the 2d viewport of the dynamical window without altering the rest
  // of the specification.
  void set_dynamical_viewport_2d(dynamical_id id,
				 const ::math::window2d& window);

  // Sets the 2d viewport of the dynamical window without altering the rest of
  // the specification.
  void set_dynamical_viewport_3d(dynamical_id id,
				 const ::math::window3d& window);

  void set_parameter_specs(parameter_id, const parameter_specs&);
  
  void set_parameter_viewport(parameter_id,
			      const ::math::window2d& window);

  // Sets the solution specification. Updates VBO's if necessary.
  void set_solution_specs(solution_id id, const solution_specs& spec);


  // Read
  const dynamical_specs& get_dynamical_specs(dynamical_id id) const;
  const parameter_specs& get_parameter_specs(parameter_id id) const;

  const std::unordered_map<solution_id, solution>& get_solutions() const;
  
  const std::unordered_map<singular_point_id, singular_point>&
  get_singular_points() const;
  const std::unordered_map<isocline_id, isocline>& get_isoclines() const;
  const std::unordered_map<separatrix_id, separatrix>& get_separatrices() const;
  const std::unordered_map<hopf_bifurcation_id, hopf_bifurcation>&
  get_hopf_bifurcations() const;
  const std::unordered_map<saddle_node_bifurcation_id, saddle_node_bifurcation>&
  get_saddle_node_bifurcations() const;
  const std::unordered_map<saddle_connection_bifurcation_id, saddle_connection_bifurcation>&
  get_saddle_connection_bifurcations() const;
  const std::unordered_map<limit_cycle_bifurcation_id, limit_cycle_bifurcation>&
  get_limit_cycle_bifurcations() const;

  // Returns the selected id for the given object or kNoId if no such
  // object is selected.
  solution_id get_selected_solution() const;
  isocline_id get_selected_isocline() const;
  singular_point_id get_selected_singular_point() const;
  separatrix_id get_selected_separatrix() const;
  hopf_bifurcation_id get_selected_hopf_bifurcation() const;
  saddle_node_bifurcation_id get_selected_saddle_node_bifurcation() const;
  saddle_connection_bifurcation_id get_selected_saddle_connection_bifurcation() const;
  limit_cycle_bifurcation_id get_selected_limit_cycle_bifurcation() const;
  

  // True if the given position in pixels lies on the vertical or horizontal
  // axes respectively for the given dynamical_window
  bool on_dynamical_vert_axis(dynamical_id id, const ::math::vector2d&) const;
  bool on_dynamical_horiz_axis(dynamical_id id, const ::math::vector2d&) const;

  bool on_parameter_vert_axis(parameter_id id, const ::math::vector2d&) const;
  bool on_parameter_horiz_axis(parameter_id id, const ::math::vector2d&) const;

  // Returns the dynamical dimension.
  // The dynamical dimension is one less than the number of dynamical variables.
  int get_dynamical_dimension() const;

  int get_parameters() const;

  const std::vector<std::string>& get_dynamical_names() const;
  const std::vector<std::string>& get_parameter_names() const;
  const std::string& get_var_diff_name() const;

  // Returns the dynamical point associated with the given mouse position
  // in this dynamical window.
  ::math::dynamical_point get_dynamical_point(dynamical_id, const ::math::vector2d& pos) const;
  ::math::vector get_parameter_point(parameter_id, const ::math::vector2d& pos) const;

  // Sets the parameter position and updates all solutions, isoclines, and
  // singular points and their vbo's.
  void set_parameter_position(parameter_id, const ::math::vector2d&);

  // Returns true if the mouse cursor specified by pos is near the parameter
  // position.
  bool on_parameter_position(parameter_id id, const ::math::vector2d& pos) const;

  const ::math::vector& get_parameter_position() const;
};
} // namespace gui
} // namespace dynsolver
#endif

