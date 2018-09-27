#include "stdafx.h"
#include <log_manager.h>
#include <script_functions.h>
#include <game_object_manager.h>

PyObject * log(PyObject * self, PyObject * args)
{
	const char * message;

	if (!PyArg_ParseTuple(args, "s", &message) || !PyTuple_Size(args) || PyTuple_Size(args) > 1)
	{
		return (PyObject *) NULL;
	}
	
	LOG_MANAGER->log(std::string(message), 3.0f);

	return Py_BuildValue("");
}

PyObject * createGroup(PyObject * self, PyObject * args)
{
	const char * group_name;

	if (!PyArg_ParseTuple(args, "s", &group_name) || !PyTuple_Size(args) || PyTuple_Size(args) > 1)
	{
		return (PyObject *) NULL;
	}

	GAME_OBJECT_MANAGER->createGroup(std::string(group_name));

	return Py_BuildValue("");
}