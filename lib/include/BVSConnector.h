#ifndef BVSCONNECTOR_H
#define BVSCONNECTOR_H

#include<map>
#include<memory>
#include<string>



/** Connector Types.
 * NOOP - not assigned
 * IN   - used for input
 * OUT  - used for output
 */
enum class BVSConnectorType { NOOP, IN, OUT};


struct BVSConnectorData
{
	std::string id;
	BVSConnectorType type;
	bool active;
	void* pointer;
};

/** Connector map definition. */
typedef std::map<std::string, std::shared_ptr<BVSConnectorData>, std::less<std::string>> BVSConnectorMap;

struct BVSConnectorDataCollector
{
	static BVSConnectorMap connectors;
};



/** The BVS connection between modules.
 * This class provides access to creating connections between different modules
 * by creating a connector on each side and then pushing data through it like a
 * pipe.
 */
template<typename T> class BVSConnector
{
	public:
		/** Constructs a connector.
		 * @param connectorName The connector's name.
		 * @param connectorType The connector's type.
		 */
		BVSConnector(const std::string& connectorName, BVSConnectorType connectorType);
		~BVSConnector();

		// TODO needs to be private, create access functions that also take care of synchronization
		// maybe: get(where to store to) and get returns bool indicating success or failure
		// remember queue design...
		T* connection;

		// TODO comments
		void set(T* input);
		T* get();
		
	private:
		std::shared_ptr<BVSConnectorData> data;

		BVSConnector(const BVSConnector&); /**< -Weffc++ */
		BVSConnector operator=(const BVSConnector&); /**< -Weffc++ */
		
		friend class BVSLoader;
};



template<typename T> BVSConnector<T>::BVSConnector(const std::string& connectorName, BVSConnectorType connectorType)
	: connection(nullptr)
	, data(std::shared_ptr<BVSConnectorData>(new BVSConnectorData{connectorName, connectorType, false, nullptr}))
{
	BVSConnectorDataCollector::connectors[connectorName] = data;
	
	if (data->type == BVSConnectorType::OUT)
	{
		connection = new T();
		data->pointer = connection;
		data->active = true;
		printf("%p\n", (void*)connection);
		printf("%p\n", (void*)BVSConnectorDataCollector::connectors[connectorName]->pointer);
	}
}

template<typename T> BVSConnector<T>::~BVSConnector()
{
	if (data->type == BVSConnectorType::IN) delete connection;
}



template<typename T> void BVSConnector<T>::set(T* input)
{
	//only for input

	data->pointer = input;
}



template<typename T> T* BVSConnector<T>::get()
{
	//only for output

	if (connection == nullptr && data->pointer != nullptr && data->active)
	{
		connection = static_cast<T*>(data->pointer);
		data->active = true;
	}

	return connection;
}



#endif //BVSCONNECTOR_H

