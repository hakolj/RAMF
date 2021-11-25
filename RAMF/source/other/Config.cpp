#include "pch.h"
#include "Config.h"  

using namespace std;


Config::Config(string filename, string keyword, string delimiter,
	string comment)
	: m_Delimiter(delimiter), m_Comment(comment), m_keyword(keyword)
{
	// Construct a Config, getting keys and values from given file  

	std::ifstream in(filename.c_str());

	if (!in) throw File_not_found(filename);

	in >> (*this);
}


Config::Config()
	: m_Delimiter(string(1, '=')), m_Comment(string(1, '#')), m_keyword(string("keyword"))
{
	// Construct a Config without a file; empty  
}



bool Config::KeyExists(const string& key) const
{
	// Indicate whether key is found  
	mapci p = m_Contents.find(key);
	return (p != m_Contents.end());
}


/* static */
void Config::Trim(string& inout_s)
{
	// Remove leading and trailing whitespace  
	static const char whitespace[] = " \n\t\v\r\f";
	inout_s.erase(0, inout_s.find_first_not_of(whitespace));
	inout_s.erase(inout_s.find_last_not_of(whitespace) + 1U);
}


std::ostream& operator<<(std::ostream& os, const Config& cf)
{
	// Save a Config to os  
	for (Config::mapci p = cf.m_Contents.begin();
		p != cf.m_Contents.end();
		++p)
	{
		os << p->first << " " << cf.m_Delimiter << " ";
		os << p->second << std::endl;
	}
	return os;
}

void Config::Remove(const string& key)
{
	// Remove key and its value  
	m_Contents.erase(m_Contents.find(key));
	return;
}

std::istream& operator>>(std::istream& is, Config& cf)
{
	// Load a Config from is  
	// Read in keys and values, keeping internal whitespace  
	typedef string::size_type pos;
	const string& delim = cf.m_Delimiter;  // separator  
	const string& comm = cf.m_Comment;    // comment  
	const string& keyword = cf.m_keyword; //keyword
	const pos skip = delim.length();        // length of separator  
	bool isection = false;

	string nextline = "";  // might need to read ahead to see where value ends  

	while (is || nextline.length() > 0)
	{
		// Read an entire line at a time  
		string line;
		if (nextline.length() > 0)
		{
			line = nextline;  // we read ahead; use it now  
			nextline = "";
		}
		else
		{
			std::getline(is, line);
		}

		// Ignore comments  
		line = line.substr(0, line.find(comm));
		Config::Trim(line); //trim line
		string word = line.substr(0, 3);
		if (word == "SET") {
			line.replace(0, 3, "");
			Config::Trim(line);
			if (line == "-" + keyword) {
				isection = true; //start section
				continue;
			}
		}

		if (!isection) continue;
		
		word = line.substr(0, 6);
		if (word == "ENDSET"){
			line.replace(0, 6, "");
			Config::Trim(line);
			if (line == "-"+keyword) {
				isection = false;
				//cout << "end reading " + keyword + "\n";
				break;
			}
			else {
				cout << "Error: Reach unknown comment: 'ENDSET " + line + " in config file.";
				exit(0);
			}
		}
		
		//else if (line == "end " + keyword) {
		//	if (!isection) {
		//		cout << "Error: Reach 'end " + keyword + "' without finding '" + keyword + "' in config file.";
		//		exit(0);
		//	}
		//}
		


		// Parse the line if it contains a delimiter  
		pos delimPos = line.find(delim);
		if (delimPos < string::npos)
		{
			// Extract the key  
			string key = line.substr(0, delimPos);
			line.replace(0, delimPos + skip, "");

			// See if value continues on the next line  
			// Stop at blank line, next line with a key, end of stream,  or end section
			// or end of file sentry  
			bool terminate = false;
			while (!terminate && is)
			{
				std::getline(is, nextline);
				terminate = true;

				string nlcopy = nextline;
				Config::Trim(nlcopy);
				if (nlcopy == "") continue; // blank line

				nextline = nextline.substr(0, nextline.find(comm));
				if (nextline.find(delim) != string::npos) // with a key
					continue;

				if (nlcopy.substr(0,6) == "ENDSET") continue; //end section

				nlcopy = nextline;
				Config::Trim(nlcopy);
				if (nlcopy != "") line += "\n"; //value continues
				line += nextline;
				terminate = false;
			}

			// Store key and value  
			Config::Trim(key);
			Config::Trim(line);
			cf.m_Contents[key] = line;  // overwrites if key is repeated  
		}
	}

	return is;
}
bool Config::FileExist(std::string filename)
{
	bool exist = false;
	std::ifstream in(filename.c_str());
	if (in)
		exist = true;
	return exist;
}

void Config::ReadFile(string filename, string delimiter,
	string comment)
{
	m_Delimiter = delimiter;
	m_Comment = comment;
	std::ifstream in(filename.c_str());

	if (!in) throw File_not_found(filename);

	in >> (*this);
}