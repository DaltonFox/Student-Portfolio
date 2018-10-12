#pragma once
#include "stdafx.h"
#include <Python.h>

/// Top-Level Python Module Functions

// Envokes ssuge::LogManager logging from Python
PyObject * log(PyObject * self, PyObject * args); 

// Adds an object group to ssuge::GameObjectManager
PyObject * createGroup(PyObject * self, PyObject * args);
