#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <Python.h>
#include <game_object.h>

typedef struct
{
	PyObject_HEAD
	char * groupName;
	char * objectName;
} script_GameObject;