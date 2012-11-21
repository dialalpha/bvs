#ifndef BVSEXAMPLEMODULE_H
#define BVSEXAMPLEMODULE_H

#include "bvs/module.h"



/** This is the BVSExampleModule module.
 * Please add sufficient documentation to enable others to use it.
 * Include information about:
 * - Dependencies
 * - Inputs
 * - Outputs
 * - Configuration Options
 */
class BVSExampleModule : public BVS::Module
{
	public:
		/** Your module constructor.
		 * Please do not change the signature, as it will be called by the
		 * framework.
		 * You can use the constructor/destructor pair to create/destroy your data.
		 * @param[in] id Your modules unique identifier, will be set by framework.
		 * @param[in] conf Your module configuration.
		 * @param[in] bvs Reference to framework info for e.g. config option retrieval.
		 */
		BVSExampleModule(const std::string id, const std::string conf, BVS::Info& bvs);

		/** Your module destructor. */
		~BVSExampleModule();

		/** Execute function doing all the work.
		 * This function is executed exactly once and only once upon each started
		 * round/step of the framework. It is supposed to contain the actual work
		 * of your module.
		 */
		BVS::Status execute();

		/** UNUSED
		 * @return Module's status.
		 */
		BVS::Status debugDisplay();

	private:
		const std::string id; /**< Your unique module id, set by framework. */

		/** Your logger instance.
		 * @see Logger
		 */
		BVS::Logger logger;

		/** Your Info reference;
		 * @see Info
		 */
		const BVS::Info& bvs;

		/** Example Connector used to retrieve/send data from/to other modules.
		 * @see Connector
		 */
		//BVS::Connector<int> input;
		//BVS::Connector<std::string> output;

		BVSExampleModule(const BVSExampleModule&) = delete; /**< -Weffc++ */
		BVSExampleModule& operator=(const BVSExampleModule&) = delete; /**< -Weffc++ */
};



/** This calls a macro to create needed module utilities. */
BVS_MODULE_UTILITIES(BVSExampleModule)



#endif //BVSEXAMPLEMODULE_H

