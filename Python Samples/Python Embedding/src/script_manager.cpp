#include <stdafx.h>
#include <script_manager.h>
#include <script_functions.h>
#include <log_manager.h>
#include <script_GameObjectClass.h>

extern PyTypeObject script_GameObjectClassType;

// SSUGE Module Definitions
struct PyModuleDef ssuge_module_definition =
{
	PyModuleDef_HEAD_INIT,
	"SSUGE Extension Module",
	"Python extension for SSUGE embedded interpreter.",
	-1,
	ssuge::ssuge_script_functions
};

// SSUGE Function Defintions
PyMethodDef ssuge::ssuge_script_functions[] =
{
	{ "log", log, METH_VARARGS, "Logs something." },
	{ "createGroup", createGroup, METH_VARARGS, "Create object group." },
	{ NULL, NULL, 0, NULL }
};

// SSUGE Module Initialization
PyMODINIT_FUNC ssuge::PyInit_ssuge(void)
{
	PyObject * module = PyModule_Create(&ssuge_module_definition);
	if (module == NULL)
		return NULL;

	if (PyType_Ready(&script_GameObjectClassType) < 0)
		return NULL;

	Py_INCREF(&script_GameObjectClassType);
	PyModule_AddObject(module, "GameObject", (PyObject *) &script_GameObjectClassType);

	return module;
}

// The template-specialization to declare the singleton variable for the ssuge::LogManager class
template<> ssuge::ScriptManager* ssuge::Singleton<ssuge::ScriptManager>::msSingleton = NULL;

ssuge::ScriptManager::ScriptManager()
{
	// Initializes and attaches the Python Interpreter to this object
	PyImport_AppendInittab("ssuge", &PyInit_ssuge);
	Py_Initialize();
}

ssuge::ScriptManager::~ScriptManager()
{
	// De-Constructs the Python Interpreter attached to this object
	Py_Finalize();
}

void ssuge::ScriptManager::loadScript(std::string filename)
{
	FILE * fp;
	int err;

	if (filename.substr(filename.length() - 3) == ".py")
	{
		fp = fopen(filename.c_str(), "r");
		if (fp == NULL)
		{
			LOG_MANAGER->log("ERROR: running script " + filename + " -> file does not exist", 5.0f);
			return;
		}

		// https://docs.python.org/3/c-api/veryhigh.html
		err = PyRun_SimpleFile(fp, filename.c_str()); 
		if (err)
		{
			LOG_MANAGER->log("ERROR: " + filename + " -> script run failure", 5.0f);
		}
		
	}
	else
	{
		LOG_MANAGER->log("ERROR: running script " + filename + " -> file must be Python (.py)", 5.0f);
	}
	return;
}