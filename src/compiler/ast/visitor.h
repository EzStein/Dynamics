/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   visitor.h
 * Author: ezra
 *
 * Created on September 26, 2017, 4:26 PM
 *
 * Used to implement the visitor pattern. This is an interface class that
 * declares a virtual visit method for each type of node in the ast.
 * The default behavior is to do nothing. A subclass
 * may override parts of methods if it wishes to do something else.
 */

#ifndef VISITOR_H
#define VISITOR_H
/*#include "compiler/ast/binary_addition_operator_node.h"
#include "compiler/ast/binary_multiplication_operator_node.h"
#include "compiler/ast/binary_subtraction_operator_node.h"
#include "compiler/ast/division_operator_node.h"
#include "compiler/ast/exponentiation_operator_node.h"
#include "compiler/ast/integer_number_leaf_node.h"
#include "compiler/ast/number_leaf_node.h"
#include  "compiler/ast/unary_minus_operator_node.h"
#include "compiler/ast/polyadic_addition_operator_node.h"
#include "compiler/ast/polyadic_multiplication_operator_node.h"
#include "compiler/ast/variable_leaf_node.h"*/

class binary_addition_operator_node;
class binary_subtraction_operator_node;
class binary_multiplication_operator_node;
class unary_minus_operator_node;
class exponentiation_operator_node;
class division_operator_node;
class polyadic_addition_operator_node;
class polyadic_multiplication_operator_node;
class integer_number_leaf_node;
class number_leaf_node;
class variable_leaf_node;

class visitor {
public:
    virtual void visit(binary_addition_operator_node*) { };
    virtual void visit(binary_subtraction_operator_node*) { };
    virtual void visit(binary_multiplication_operator_node*) { };
    virtual void visit(unary_minus_operator_node*) { };
    virtual void visit(exponentiation_operator_node*) { };
    virtual void visit(division_operator_node*) { };
    virtual void visit(polyadic_addition_operator_node*) { };
    virtual void visit(polyadic_multiplication_operator_node*) { };
    virtual void visit(integer_number_leaf_node*) { };
    virtual void visit(number_leaf_node*) { };
    virtual void visit(variable_leaf_node*) { };
};


#endif /* VISITOR_H */
