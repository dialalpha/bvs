#ifndef BVS_LOADER_H
#define BVS_LOADER_H

#include<string>
#include<thread>
#include<vector>

#include "bvs/bvsinfo.h"
#include "bvs/config.h"
#include "bvs/logger.h"
#include "control.h"
#include "moduledata.h"



/** BVS namespace, contains all library stuff. */
namespace BVS
{
	// Forward declaration.
	class Control;



	/** Module Map. */
	typedef std::map<std::string, std::shared_ptr<ModuleData>, std::less<std::string>> ModuleMap;

	/** Module Vector.*/
	typedef std::vector<std::shared_ptr<ModuleData>> ModuleVector;


	/** The system loader: loads, unloads and controls modules. */
	class Loader
	{
		public:
			/** Constructor for loader.
			 * @param[in] control Reference to control mechanism.
			 * @param[in] info Reference to info struct.
			 */
			Loader(Control& control, const Info& info);

			/** Registers a module.
			 * @param[in] id Name of module.
			 * @param[in] module Pointer to module.
			 */
			static void registerModule(const std::string& id, Module* module);

			/** Load the given module.
			 * Executes bvsRegisterModule function in module to register it with the
			 * system.
			 * @param[in] moduleTraits The name and traits of the module.
			 * @param[in] asThread Whether to load the module inside a thread or not.
			 * @return Reference to object.
			 */
			Loader& load(const std::string& moduleTraits, const bool asThread);

			/** Unload the given module.
			 * @param[in] moduleName The name of the module.
			 * @param[in] eraseFromMap Whether to delete all associated metadata.
			 * @return Reference to object.
			 */
			Loader& unload(const std::string& moduleName, const bool eraseFromMap = true);

			/** Unload all modules.
			 * @return Reference to object.
			 */
			Loader& unloadAll();

			/** Connect all modules.
			 * @param[in] connectorTypeMatching Whether to try to match associated
			 * connectors.
			 * @return Reference to object.
			 */
			Loader& connectAllModules(const bool connectorTypeMatching = true);

			/** Connect selected module.
			 * Connects the selected module by checking its desired configuration
			 * options and processing them piece by piece.
			 * @param[in] id Module id.
			 * @param[in] connectorTypeMatching Whether to try to match associated
			 * connectors.
			 * @return Reference to object.
			 */
			Loader& connectModule(const std::string& id, const bool connectorTypeMatching = true);

		private:
			/** Check input Connector.
			 * Checks input connector for existence, type etc.
			 * @param[in] module Module data for selected module.
			 * @param[in] inputName Name of desired input.
			 * @return True if all checks passed.
			 */
			Loader& checkModuleInput(const ModuleData* module, const std::string& inputName);

			/** Check output Connector.
			 * Checks output Connector for existence, type etc.
			 * @param[in] module Module data for selected module.
			 * @param[in] targetModule Name of desired module.
			 * @param[in] targetOutput Name of desried output.
			 */
			Loader& checkModuleOutput(const ModuleData* module, const std::string& targetModule, const std::string& targetOutput);

			/** Print Connector list.
			 * Prints a list of all Connectors defined by given module.
			 * @param[in] module Module data for selected module.
			 * @return Reference to object.
			 */
			Loader& printModuleConnectors(const ModuleData* module);

			/** Map of registered modules and their metadata. */
			static ModuleMap modules;

			/** Vector of modules executed by master. */
			static ModuleVector masterModules;

			/** Reference to control mechanism, needed to load und unload threaded modules. */
			Control& control;

			Logger logger; /**< Logger metadata. */
			const Info& info; /**< Info reference. */

			Loader(const Loader&) = delete; /**< -Weffc++ */
			Loader& operator=(const Loader&) = delete; /**< -Weffc++ */

			friend class Control;
	};
} // namespace BVS



#endif //BVS_LOADER_H
