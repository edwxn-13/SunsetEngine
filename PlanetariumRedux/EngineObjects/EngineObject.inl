
//Inlinefile!! 

template <class T>
inline T* EngineObject::getComponentOfType()
{
	for (int i = 0; i <component_list.size(); i++) {
		Component* component = component_list[i];
		if (typeid(*component) == typeid(T)) 
		{
			T* temp = dynamic_cast<T*>(component);
			return temp;

		}
	}
	return nullptr;
}

template <class T>
inline std::vector<T*> EngineObject::getComponentsOfType()
{
	std::vector<Component*> list = {};
	for (int i = 0; i < component_list.size(); i++) {
		Component* component = component_list[i];
		if (typeid(*component) == typeid(T)))

			T* temp = dynamic_cast<T*>(component);
			list.push_back(temp);
	}
	return list;
}

