#include "Defines.h"
#include "ResourceManager.h"
#include <algorithm>
#include <cctype>

namespace Lan
{
	tstring trim(tstring s)
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
			return !std::isspace(ch);
		}));
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
			return !std::isspace(ch);
		}).base(), s.end());
		return s;
	}

	ResourceManager::ResourceManager()
	{
		std::ifstream resInfo("./Resources.dsr");
		
		resInfo.seekg(0, std::ios_base::end);
		int32 len = static_cast<int32>(resInfo.tellg());
		resInfo.seekg(0, std::ios_base::beg);

		char * buffer = new char[len];

		resInfo.read(buffer, len);

		tstring str(buffer);

		ParseRes(str);

		delete[] buffer;
	}

	ResourceManager::~ResourceManager()
	{

	}

	void ResourceManager::ParseRes(tstring dataString)
	{
		tstringstream stream(dataString);
		std::map<tstring, tstring> val;

		char* buffer = new char[dataString.length() + 1];

		while (!stream.eof())
		{
			stream.getline(buffer, dataString.length());
			tstring line(buffer);

			tsize commentPos = line.find("//");
			if (commentPos != tstring::npos)
			{
				line.replace(commentPos, tstring::npos, ""); //�ּ� ����
			}

			commentPos = line.find("/*");/* ���� �� �ּ� ���� */
			if (commentPos != tstring::npos)
			{
				tsize commentEndPos = line.find("*/", commentPos);
				if (commentEndPos != tstring::npos)
				{
					line.replace(commentPos, commentEndPos - commentPos + 2, "");
				}
				else
				{
					do
					{
						stream.getline(buffer, dataString.length());
						line = buffer;
						commentEndPos = line.find("*/");
					} while (commentEndPos == tstring::npos);
					line.replace(0, commentEndPos + 2, "");
				}
			}

			tsize colPos = line.find(':');
			if (colPos != tstring::npos)
			{
				tstring name = line.substr(0, colPos);
				tstring path = line.substr(colPos + 1, tstring::npos);
				InsertPath(name, path, val);
			}

		}
	}

	void ResourceManager::InsertPath(tstring& rawName, tstring& rawPath, std::map<tstring, tstring>& val)
	{
		tstring name = trim(rawName);
		tstring path = trim(rawPath);

		if (path.at(0) == '\"') 
		{
			path.replace(0, 1, "");
		}

		if (path.at(path.length() - 1) == '\"')
		{
			path.replace(path.length() - 1, tstring::npos, "");
		}

		if (tstring::npos == name.find('|'))
		{
			while (tstring::npos != path.find_first_of('<'))
			{
				tsize Start = path.find_first_of('<');
				tsize end = path.find_first_of('>');
				if (end == tstring::npos)
				{
					LOG(LogLevel::Error, "���ҽ� ���� �м� ����");
					break;
				}

				path.replace(Start, end - Start + 1, val[path.substr(Start + 1, end - Start - 1)]);
			}

			m_Paths[name] = path;
		}
		else
		{
			tsize nameStart = name.find('<');
			tsize nameEnd = name.find('>');
			tsize nameMid = name.find('|');

			tstring startNumStr = name.substr(nameStart + 1, nameMid - nameStart - 1);
			int32 startNum = std::stoi(startNumStr);

			tstring endNumStr = name.substr(nameMid + 1, nameEnd - nameMid - 1);
			int32 endNum = std::stoi(endNumStr);

			tsize pathMid = path.find('|');
			tsize pathStart = path.find_last_of('<', pathMid);
			tsize pathEnd = path.find('>', pathMid);

			if (endNum <= startNum)
			{
				LOG(LogLevel::Error, "���ҽ� ���� ����");
				return;
			}

			for (int i = startNum; i < endNum; i++)
			{
				tstring tname = name;
				tstring tpath = path;
				InsertPath(tname.replace(nameStart, nameEnd - nameStart + 1, std::to_string(i)),
					tpath.replace(pathStart, pathEnd - pathStart + 1, std::to_string(i)),
					val);
			}
		}
	}

	bool ResourceManager::LoadResource(tstring name)
	{
		if (m_Paths.count(name) < 1)
		{
			LOG(LogLevel::Warning, "���� �̸��� ���ҽ� �ε� �õ�");
			return false;
		}

		if (m_Resources.count(name) > 0)
		{
			LOG(LogLevel::Warning, "�̹� �ε�� ���ҽ� �ε� �õ�");
			return false;
		}

		tstring path = m_Paths[name];

		tsize dotPos = path.find_last_of('.');
		if (dotPos == tstring::npos)
		{
			LOG(LogLevel::Error, "���ҽ� ������ Ȯ���� ����");
			return false;
		}

		tstring extension = path.substr(dotPos + 1, tstring::npos);

		std::ifstream stream(path, std::ios_base::binary);
		stream.seekg(0, std::ios_base::end);
		tsize len = static_cast<tsize>(stream.tellg());
		stream.seekg(0, std::ios_base::beg);

		int8 * buffer = new int8[len];

		stream.read(reinterpret_cast<char *>(buffer), len);
		stream.close();

		Resource * resource = 
			reinterpret_cast<Resource *>(m_Factories[extension]->Alloc(buffer, len, path, extension));

		m_Resources[name] = resource;

		delete[] buffer;

		return true;
	}

	void ResourceManager::UnloadResource(tstring name)
	{
		Resource * resource = m_Resources[name];

		m_Factories[resource->m_Extension]->Free(resource);

		m_Resources.erase(name);
	}

	void ResourceManager::AddResourceType(tstring extension, Factory& factory)
	{
		if (m_Factories.count(extension) > 0)
		{
			LOG(LogLevel::Warning, "���ҽ� ���丮 �ߺ� �߰�");
			return;
		}

		m_Factories[extension] = &factory;
	}

	Resource* ResourceManager::GetResource(tstring name)
	{
		if (m_Resources.count(name) < 1)
		{
			LOG(LogLevel::Warning, "���� ���ҽ� ����");
			return nullptr;
		}

		return m_Resources[name];
	}
}