#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <map>
#include <sstream>
#include <cctype>

void toLowerCase(std::string &str)
{
  for(int i=0; i<str.size(); ++i) 
    str[i] = std::tolower(str[i]);
}


int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << "usage:\n";
    std::cout << "./count <input dir> <output dir>\n";
    return 1;
  }

  std::cout << "executing " << argv[0] << std::endl;

  std::string path(argv[1]);
 
  //--------------------------------------------------------------------
  // find all files in wc_input
  //

  std::vector<std::string> ls; // list of files in wc_input

  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir ( path.c_str()  )) != NULL)
  {
    //std::cout << "files in wc_input:\n";
    while ((ent = readdir (dir)) != NULL)
    {
      if ( !strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..") )
        continue;
      //std::cout << ent->d_name << std::endl;
      ls.push_back(ent->d_name);
    }
    closedir (dir);
  }
  else
  {
    std::cerr << "ERROR: directory not found\n";
    return 1;
  }
  //std::cout << std::endl;
  
  // -----------------------------------------------------------------


  std::map<std::string, int> table; 
  std::ofstream wc_result(argv[2]);
  std::ifstream file;

  // for each file ...
  for (unsigned ff = 0; ff<ls.size(); ++ff)
  {
    file.open((path+"/"+ls[ff]).c_str());
    if (!file.is_open())
    { 
      std::cerr << "ERROR: file not found "  << path+ls[ff] << std::endl;
      throw;
    }
    if (!wc_result.is_open())
    {
      std::cerr << "ERROR: output not valid" << std::endl;
      throw;
    };

    std::string line;
    while (std::getline(file, line))
    {
      // remove punctuations
      line.erase(std::remove(line.begin(), line.end(), '.'), line.end());
      line.erase(std::remove(line.begin(), line.end(), '-'), line.end());
      line.erase(std::remove(line.begin(), line.end(), '?'), line.end());
      line.erase(std::remove(line.begin(), line.end(), ','), line.end());
      //std::cout << line << std::endl;
      std::istringstream is(line);
      std::string word;
      while (is >> word)
      {
        toLowerCase(word);
        std::map<std::string, int>::iterator here = table.find(word);
        if (here == table.end())
          table.insert(std::make_pair(word, 1));
        else
         (*here).second ++;
      }
    }

    file.close();
  }  

  std::map<std::string, int>::iterator it = table.begin();

  for ( ; it != table.end(); ++it)
  {
    wc_result << (*it).first << " " << (*it).second << std::endl;
  }

  wc_result.close();

}


