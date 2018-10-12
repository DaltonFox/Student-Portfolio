#include "stdafx.h"
#include <Python.h>
#include <stdio.h>
#include <script_GameObjectClass.h>
#include <game_object_manager.h>
#include <log_manager.h>
#include <structmember.h>

ssuge::GameObject * getInstance(char * groupName, char * objectName)
{
	std::string sgroupName = std::string(groupName);
	std::string sobjectName = std::string(objectName);
	int groupID = GAME_OBJECT_MANAGER->getGroupID(sgroupName);
	return GAME_OBJECT_MANAGER->getGameObject(groupID, sobjectName);
}

PyObject * script_GameObject_new(PyTypeObject * type, PyObject * args, PyObject * kwargs)
{
	script_GameObject * obj = (script_GameObject*)type->tp_alloc(type, 0);
	if (obj != NULL)
	{
		obj->groupName = "";
		obj->objectName = "";
	}

	return (PyObject*) obj;
}


int script_GameObject_init(script_GameObject * self, PyObject* args, PyObject * kwargs)
{
	if (!PyTuple_Check(args) || PyTuple_Size(args) != 2 ||
		!PyUnicode_Check(PyTuple_GetItem(args, 0)))
	{
		PyErr_SetString(PyExc_TypeError, "You must pass two string arguments for groupName and objectName.");
		return -1;
	}

	static char *kwlist[] = { "groupName", "objectName", NULL };
	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|ss", kwlist, &self->groupName, &self->objectName))
		return -1;
	
	GAME_OBJECT_MANAGER->createGameObject(std::string(self->groupName), std::string(self->objectName));

	return 0;
}

void script_GameObject_dealloc(script_GameObject * self)
{
	Py_TYPE(self)->tp_free((PyObject*)self);
	return;
}

PyObject * script_GameObject_str(script_GameObject * self)
{
	char temps[512];

	sprintf_s(temps, 511, "(Game Object \"%s\" \"%s\")", self->groupName, self->objectName);

	return PyUnicode_FromString(temps);
}

// ERR
void script_GameObject_setScale(script_GameObject * self, PyObject* args)
{
	float sx, sy, sz;
	PyArg_ParseTuple(args, "f", &sx, &sy, &sz);

	script_GameObject * obj = (script_GameObject*) self;
	ssuge::GameObject * ssuge_obj = getInstance(obj->groupName, obj->objectName);
	ssuge_obj->setScale(sx, sy, sz);

	return;
}

// ERR
void script_GameObject_setPosition(script_GameObject * self, PyObject* args)
{
	float px, py, pz;
	PyArg_ParseTuple(args, "f", &px, &py, &pz);

	script_GameObject * obj = (script_GameObject*)self;
	ssuge::GameObject * ssuge_obj = getInstance(obj->groupName, obj->objectName);
	ssuge_obj->setScale(px, py, pz);

	return;
}

// ERR
void script_GameObject_rotate(script_GameObject * self, PyObject* args)
{
	float rd, rx, ry, rz;
	PyArg_ParseTuple(args, "f", &rd, &rx, &ry, &rz);

	script_GameObject * obj = (script_GameObject*)self;
	ssuge::GameObject * ssuge_obj = getInstance(obj->groupName, obj->objectName);
	ssuge_obj->setOrientation(rd, rx, ry, rz);

	return;
}

// ERR
void script_GameObject_createMeshComponent(script_GameObject * self, PyObject* args)
{
	std::string mesh_name = std::string(PyUnicode_AsUTF8(PyTuple_GetItem(args, 0)));

	script_GameObject * obj = (script_GameObject*)self;
	ssuge::GameObject * ssuge_obj = getInstance(obj->groupName, obj->objectName);
	ssuge_obj->createMeshComponent(mesh_name);

	return;
}

// ERR
void script_GameObject_createCameraComponent(script_GameObject * self, PyObject* args)
{
	script_GameObject * obj = (script_GameObject*)self;
	ssuge::GameObject * ssuge_obj = getInstance(obj->groupName, obj->objectName);
	ssuge_obj->createCameraComponent();
	return;
}

PyMethodDef script_GameObject_methods[] =
{
	{ "setScale", (PyCFunction) script_GameObject_setScale, METH_VARARGS, "" },
	{ "setPosition", (PyCFunction) script_GameObject_setPosition, METH_VARARGS, "" },
	{ "rotate", (PyCFunction) script_GameObject_rotate, METH_VARARGS, "" },
	{ "createMeshComponent", (PyCFunction) script_GameObject_createMeshComponent, METH_VARARGS, "" },
	{ "createCameraComponent", (PyCFunction) script_GameObject_createCameraComponent, METH_VARARGS, "" },
	{ NULL, NULL, 0, NULL }
};

static PyMemberDef script_GameObject_members[] =
{
	{ "groupName", T_STRING, offsetof(script_GameObject, groupName), 0,
	  "groupName"},
	{ "objectName", T_STRING, offsetof(script_GameObject, objectName), 0,
	  "objectName"},
	{ NULL }
};

PyTypeObject script_GameObjectClassType =
{
	PyVarObject_HEAD_INIT(NULL, 0)					// Initializes the header-stuff
	"SSUGE GameObjectType",							// tp_name
	sizeof(script_GameObject),						// tp_basicsize
	0,												// tp_itemsize
	(destructor) script_GameObject_dealloc,			// tp_dealloc
	0,												// tp_print
	0,												// tp_getattr
	0,												// tp_setattr
	0,												// tp_reserved
	0,												// tp_repr
	0,												// tp_as_number
	0,												// tp_as_sequence
	0,												// tp_as_mapping
	0,												// tp_hash
	0,												// tp_call
	(reprfunc) script_GameObject_str,				// tp_str
	0,												// tp_getattro
	0,												// tp_setattro
	0,												// tp_as_buffer
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,		// tp_flags
	"GameObject",									// tp_doc
	0,												// tp_traverse
	0,												// tp_clear
	0,												// tp_richcompare
	0,												// tp_weaklistoffset
	0,												// tp_iter
	0,												// tp_iternext
	script_GameObject_methods,						// tp_methods
	script_GameObject_members,						// tp_members
	0,												// tp_getset
	0,												// tp_base
	0,												// tp_dict
	0,												// tp_descr_get
	0,												// tp_descr_set
	0,												// tp_dictoffset
	(initproc) script_GameObject_init,				// tp_init
	0,												// tp_alloc
	script_GameObject_new,							// tp_new
};
