#include <stdafx.h>
#include <application.h>
#include <game_object.h>
#include <game_object_manager.h>


// The template-specialization to declare the singleton variable for the Application class
template<> ssuge::Application* ssuge::Singleton<ssuge::Application>::msSingleton = NULL;


ssuge::Application::Application()  : OgreBites::ApplicationContext("ssuge"), mLogManager(NULL), mScriptManager(NULL), mGameObjectManager(NULL), mSoundManager(NULL)
{
	addInputListener(this);
}


ssuge::Application::~Application()
{
	// We'll do most of our clean-up in shutdown
}



void ssuge::Application::setup()
{
	// do not forget to call the base first
	OgreBites::ApplicationContext::setup();

	// get a pointer to some already-created objects and create a scene manager
	mRoot = getRoot();
	mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC);
	mWindow = getRenderWindow();

	// register our scene with the RTSS
	Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(mSceneManager);

	// Create a light source (this will change once we have a LightComponent)
	Ogre::Light* light = mSceneManager->createLight("MainLight");
	Ogre::SceneNode* lightNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	lightNode->setPosition(0, 50, 0);
	lightNode->attachObject(light);

	// This will also change once we have a CameraComponent
	Ogre::SceneNode* camNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	camNode->setPosition(0, 25, 65);
	camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_PARENT);
	Ogre::Camera* cam = mSceneManager->createCamera("myCam");
	cam->setNearClipDistance(0.1f);
	cam->setFarClipDistance(1000.0f);
	cam->setAutoAspectRatio(true);
	camNode->attachObject(cam);
	mWindow->addViewport(cam);

	// Setup shadows
	mSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
	mSceneManager->setAmbientLight(Ogre::ColourValue(0.3f, 0.3f, 0.3f));


	// Set up the overlay system
	//mOverlaySystem = new Ogre::OverlaySystem();
	mSceneManager->addRenderQueueListener(getOverlaySystem());

	// Create our "managers"
	mLogManager = new LogManager("ssuge.log", 25);
	mScriptManager = new ScriptManager();
	mGameObjectManager = new GameObjectManager();
	mSoundManager = new SoundManager();

	// Miscellaneous setup
	createDebugPanel();

	// Load our sample script
	//SCRIPT_MANAGER->loadScript("..\\Media\\my_script.py");
	GAME_OBJECT_MANAGER->loadGroup("..\\Media\\simple_level.lvl");

	// Run our test python script
	SCRIPT_MANAGER->loadScript("..\\Media\\test_script.py");
}


void ssuge::Application::shutdown()
{
	// Do any cleanup of our objects

	// Delete our managers
	if (mSoundManager)
		delete mSoundManager;
	if (mGameObjectManager)
		delete mGameObjectManager;
	if (mLogManager)
		delete mLogManager;
	if (mScriptManager)
		delete mScriptManager;

	// Let the ApplicationContext do its thing
	OgreBites::ApplicationContext::shutdown();
}


bool ssuge::Application::keyPressed(const OgreBites::KeyboardEvent & evt)
{

	switch (evt.keysym.sym)
	{
	case SDLK_ESCAPE:
		getRoot()->queueEndRendering();
		return false;
	case SDLK_F11:
		if (mDebugOverlay)
		{
			if (mDebugOverlay->isVisible())
				mDebugOverlay->hide();
			else
				mDebugOverlay->show();
		}
		break;
	default:
		LOG_MANAGER->log("Keypress: " + std::to_string(evt.keysym.sym), 3.0f);
		break;
	}

	return true;
}


bool ssuge::Application::frameStarted(const Ogre::FrameEvent & evt)
{
	if (!ApplicationContext::frameStarted(evt))
		return false;

	// Update the stats panel
	float spacing = 16.0f;
	if (mDebugOverlay && mDebugOverlay->isVisible())
	{
		Ogre::OverlayManager * mgr = Ogre::OverlayManager::getSingletonPtr();
		Ogre::TextAreaOverlayElement * text = (Ogre::TextAreaOverlayElement*)mgr->getOverlayElement("DebugStatsGUI\\Text0");
		text->setCaption("Average FPS: " + std::to_string(mWindow->getAverageFPS()));
		text->setPosition(mgr->getViewportWidth() - 200.0f, 0.0f);
		text->setCharHeight(spacing);

		text = (Ogre::TextAreaOverlayElement*)mgr->getOverlayElement("DebugStatsGUI\\Text1");
		text->setCaption("Triangle Count: " + std::to_string(mWindow->getTriangleCount()));
		text->setPosition(mgr->getViewportWidth() - 200.0f, spacing);
		text->setCharHeight(spacing);
	}
	LOG_MANAGER->update(evt.timeSinceLastFrame);

	// (TEMPORARY!) Just for Lab7...
	float dt = evt.timeSinceLastFrame;
	GameObject * r1 = GAME_OBJECT_MANAGER->getGameObject(0, "rotater1");
	r1->rotateWorld(dt * 45.0f, 0, 1, 0);
	//GameObject * r2 = GAME_OBJECT_MANAGER->getGameObject(0, "rotater2");
	//r2->rotateWorld(dt * 20.0f, 0, 1, 0);

	// Update the GOM, which will in turn update all game objects
	GAME_OBJECT_MANAGER->update(evt.timeSinceLastFrame);

	return true;
}



void ssuge::Application::createDebugPanel()
{
	Ogre::OverlayManager * mgr = Ogre::OverlayManager::getSingletonPtr();
	mDebugOverlay = mgr->create("DebugStatsGUI");

	// Create a panel to hold all text elements
	Ogre::OverlayContainer * panel = (Ogre::OverlayContainer*)(mgr->createOverlayElement("Panel", "DebugStatsGUI\\Panel"));
	mDebugOverlay->add2D(panel);

	// Create the text elements
	for (unsigned int i = 0; i < 2; i++)
	{
		Ogre::TextAreaOverlayElement * text = (Ogre::TextAreaOverlayElement*)mgr->createOverlayElement("TextArea", "DebugStatsGUI\\Text" + std::to_string(i));
		text->setMetricsMode(Ogre::GMM_PIXELS);
		text->setFontName("SdkTrays/Value");
		text->setColourBottom(Ogre::ColourValue(1.0f, 0.4f, 0.4f));
		text->setColourTop(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
		panel->addChild(text);
	}

	// Show the overlay
	mDebugOverlay->show();
}