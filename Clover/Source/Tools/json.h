#ifndef _JSON_H
#define _JSON_H 1

#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <string> 

#include <unordered_map>

/*
int main()
{
    // Part 1

    // 1
    JSonObject  j("Part2.json");
    j.Print();
    std::cout << std::endl;


    // 3
    std::cout << "With Operator: " << j("Paul") << std::endl;

    std::cout << std::endl;
    // 4
    std::cout << "Count: " << j.Count() << std::endl;

    std::pair<std::string, std::string> p = j.GetKeyValuePair(0);

    std::cout << "GetKeyValuePair: " << p.second << std::endl;


    // 5
    std::cout << "Count: " << j.Count() << std::endl;

    std::cout << std::endl;
    j.Print();

    std::cout << std::endl;


    std::cout << "Count Objects: " << j.CountObjects() << std::endl;

    // 6
    JSonObject key = j.GetObject("Software Engineering");
    std::cout << "Extract Object: " << key("Pascale") << std::endl;

    // 7
    std::cout << "Count Objects: " << j.CountObjects() << std::endl;


    std::cout << std::endl;
    j.Print();
}
 */

const char openObject = '{';
const char closeObject = '}';

const char openName = '"';

const char changeToValue = ':';
const char changeToKey = ',';

class JSonObject
{
public:
    typedef std::pair<std::string, std::string> Pair;
    typedef std::unordered_map<int, Pair> Object;

    typedef std::unordered_map<std::string, Object> ObjectMap;

    

    JSonObject(const char* filepath)
    {
		std::ifstream file_(filepath);
        std::ostringstream temp_data;
        temp_data << file_.rdbuf();
        data_ = temp_data.str();

		// 2
        process();
    }

    ~JSonObject()
    {}
    
    std::string operator()(const std::string& key)
    {
		for (auto& pair : objectMap_)
		{
			for (auto& pair2 : pair.second)
			{
				if (pair2.second.first == key)
				{
					return pair2.second.second;
				}
			}
		}
        return "";
    }

    int Count() { return maxCount; }

    int CountObjects()
    {
        return objectMap_.size();
    }

    Pair GetKeyValuePair(int index)
    {
        Pair result;
        for (auto& objects : objectMap_)
        {
            if (objects.second.find(index) != objects.second.end())
            {
                result = objects.second[index];

                objects.second.erase(index);
                --maxCount;

                return result;
            }
        }
        return result;
    }

    JSonObject GetObject(const std::string& key)
    {
        Object result;
		if (objectMap_.find(key) != objectMap_.end())
		{
			result = objectMap_[key];
            maxCount -= result.size();

            objectMap_.erase(key);
			return result;
		}
		return result;
    }

    void Print()
    {
		for (auto& pair : objectMap_)
		{
            std::cout << pair.first << " : "<< std::endl;
			for (auto& pair2 : pair.second)
			{
				std::cout << "    " << pair2.second.first << " : " << pair2.second.second << std::endl;
			}
		}
       
    }

private:
	JSonObject(const Object& map)
	{
		addObject("Root", map);
	}

	void process()
	{
		checkIfHasChilds();
		std::string::iterator iterator = data_.begin();


		if (!hasChilds_)
		{
			processObject(iterator);
			addObject("Root", map_);
			return;
		}

		std::string objectName;

		for (iterator = data_.begin(); iterator != data_.end(); ++iterator)
		{
			char current = *iterator;

			switch (current)
			{
			case openObject:
				break;

			case openName:
				objectName = getName(++iterator);
				break;
			case changeToValue:
				processObject(++iterator);
				if (objectName == "") break;
				addObject(objectName, map_);
				objectName = "";
				break;

			default:
				break;
			}

		}
		map_.clear();
        data_.clear();
	}

	void add(const int index, const std::string& key, const std::string& value)
	{
		map_[index] = Pair(key, value);
		++count;
	}

	void addObject(const std::string& key, Object object)
	{
		objectMap_[key] = object;
		maxCount += object.size();
        map_.clear();
	}

    // Checks if has childs without processing the entire string
    void checkIfHasChilds()
    {
        std::string::iterator iterator;
        int isOpen = false;
        for (iterator = data_.begin(); iterator != data_.end(); ++iterator)
        {
            char current = *iterator;
            if (current == openObject)
            {
                if (isOpen)
                {
                    hasChilds_ = true;
                    return;
                }
                isOpen = true;
            }

            if (current == changeToKey)
            {
                if (isOpen)
                {
                    hasChilds_ = false;
                    return;
                }
            }
        }
    }

    // Process the childs if any or the entire JSon is not childs found
    void processObject(std::string::iterator& iterator)
    {
		std::string key, value;
		bool begin_name = false, is_Value = false, is_Key = true;
		
		for (; iterator != data_.end(); ++iterator)
		{
			char current = *iterator;

			switch (current)
			{
			case openObject:
				break;

			case closeObject: 
                    // Exit this object
                    return;
                break;

			case openName:
				if (is_Key)
				{
					key = getName(++iterator);
				}
				if (is_Value)
				{
					value = getName(++iterator);
					add(count, key, value);
				}
				break;

			case changeToValue:
				is_Key = false;
				is_Value = true;
				break;

			case changeToKey:
				is_Key = true;
				is_Value = false;
				break;

			default:
				break;
			}

		}
    }

    std::string getName(std::string::iterator& iterator)
    {
        std::string name;
        for (;iterator != data_.end(); ++iterator)
        {
            if (*iterator == '"')
            {
                //++iterator;
                return name;
            }

            name += *iterator;
        }
    }
	
    std::string data_;
    bool hasChilds_ = false;
    Object map_;
    Object allKeys_;
    ObjectMap objectMap_;
    int count = 0;
    int maxCount = 0;
};
 
#endif