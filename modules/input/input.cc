#include "input.h"



// This is your module's constructor.
// Please do not change its signature as it is called by the framework (so the
// framework actually creates your module) and the framework assigns the unique
// identifier and gives you access to the its config.
// However, you might use it to create your data structures etc., or you can use
// the onLoad() and onClose() functions, just be consistent in order to avoid
// weird errors.
input::input(const std::string id, const BVSConfig& config)
	: BVSModule()
	, id(id)
	, logger(id)
	, config(config)
	, output("image", BVSConnectorType::OUTPUT)
	, capture()
{
	capture.open(0);
	if(!capture.isOpened()) exit(1);

	//capture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	//capture.set(CV_CAP_PROP_FRAME_HEIGHT, 800);
}



// This is your module's destructor.
// See the constructor for more info.
input::~input()
{

}



// Executed at module load.
BVSStatus input::onLoad()
{
LOG(2, "loaded input module!");

	return BVSStatus::OK;
}



// UNUSED
BVSStatus input::preExecute()
{
	return BVSStatus::OK;
}



// Put all your work here.
BVSStatus input::execute()
{
	LOG(2, "Execution of " << id << "!");

	capture >> output.set();

	return BVSStatus::OK;
}



// UNUSED
BVSStatus input::postExecute()
{
	return BVSStatus::OK;
}



BVSStatus input::debugDisplay()
{
	return BVSStatus::OK;
}



// Executed at module unload.
BVSStatus input::onClose()
{
	return BVSStatus::OK;
}



// This function is called by the framework upon creating a module instance of
// this class. It creates the module and registers it within the framework.
// DO NOT CHANGE OR DELETE
extern "C" {
	// register with framework
	int bvsRegisterModule(std::string id, BVSConfig& config)
	{
		registerModule(id, new input(id, config));

		return 0;
	}
}

