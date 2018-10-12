#pragma once
#include <stdafx.h>
#include <singleton.h>
#include <Python.h>

#define SCRIPT_MANAGER ssuge::ScriptManager::getSingletonPtr()

namespace ssuge
{
	/// The ScriptManager is used to embed a Python Interpreter into SSUGE for executing Python scripts (note: using C-Python)
	class ScriptManager : public Singleton<ScriptManager>
	{
			// @@@@@ ATTRIBUTES @@@@@
		protected:
		
			// @@@@@ CONSTRUCTORS / DESTRUCTORS @@@@@
		public:
			/// Constructor
			ScriptManager();

			/// Destructor
			~ScriptManager();

			// @@@@@ METHODS @@@@@
		public:
			/// Takes the path to a Python script and executes it
			virtual void loadScript(std::string filename);
	};

	/// SSUGE Module Declarations
	PyMethodDef ssuge_script_functions[];
	PyMODINIT_FUNC PyInit_ssuge(void);
}
