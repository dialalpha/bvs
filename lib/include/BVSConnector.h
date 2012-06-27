#ifndef BVSCONNECTOR_H
#define BVSCONNECTOR_H

#include<map>
#include<memory>
#include<string>



// Forward declaration
class BVSConnector;



/** Connector Types.
 * IN  - used for input
 * OUT - used for output
 */
enum class BVSConnectorType { IN, OUT};



/** Connector map definition. */
typedef std::map<std::string, BVSConnector*, std::less<std::string>> BVSConnectorMap;



/** The BVS connection between modules.
 * This class provides access to creating connections between different modules
 * by creating a connector on each side and then pushing data through it like a
 * pipe.
 */
class BVSConnector
{
	public:
		/** Constructs a connector.
		 * @param connectorName The connector's name.
		 * @param connectorType The connector's type.
		 */
		BVSConnector(const std::string& connectorName, BVSConnectorType connectorType);

		// TODO needs to be private, create access functions that also take care of synchronization
		// maybe: get(where to store to) and get returns bool indicating success or failure
		// remember queue design...
		std::shared_ptr<int> data;
		
	private:
		std::string identifier; /**< The connector's identifier. */
		BVSConnectorType type; /**< The connector's type. */

		/** Map of connectors, used upon creating to register connector. */
		static BVSConnectorMap connectors;
		
		friend class BVSLoader;
};
#endif //BVSCONNECTOR_H
