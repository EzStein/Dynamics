/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   render_data.h
 * Author: ezra
 *
 * Created on October 16, 2017, 1:39 PM
 */

#ifndef RENDER_DATA_H
#define RENDER_DATA_H

/*
 * Contains information on what should be rendered to the screen.
 * For instance, it contains a list of solutions, a list of nullclines
 * and other isoclines, etc...
 */
template<int DIM>
struct render_data {
  std::vector<solution<DIM> > solutions;
  std::vector<isocline<DIM> > isoclines;
};

/*
 * A class that contains a list of points representing a solution to
 * to be drawn on the screen.
 * The list is ordered by time which is useful when drawing the solutions
 * onto a plane. It also contains the initial conditions that produced the curve.
 * It is templated according to the dimension of this curve.
 */

template<int DIM>
struct solution {
  /*
   * The initial conditions of this curve
   */
  math::vector<double, DIM> initConditions;
  
  /*
   * A list of points in this curve ordered
   * according to the value of t (the first element of the vector)
   */
  std::list<math::vector<double, DIM> > points;
};

template<int DIM>
struct solution {


#endif /* RENDER_DATA_H */

