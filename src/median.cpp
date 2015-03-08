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
#include <iomanip>

void toLowerCase(std::string &str)
{
  for(int i=0; i<str.size(); ++i) 
    str[i] = std::tolower(str[i]);
}

double median(std::vector<int> const& v)
{
  unsigned const n = v.size();
  if (n % 2) // if odd
    return v[ (n-1)/2  ];
  else if (n > 1)
    return .5*(v[n/2] + v[n/2-1]);
  else return 0.;
}

// Sorted insertion. I use this function to maintain a vector always sorted.
template<class V, class T>
static
typename V::iterator insert_sort(V &vec, T value)
{
  typename V::iterator it = std::lower_bound(vec.begin(), vec.end(), value);
  return vec.insert(it, value);
}

int main(int argc, char* argv[])
{
  if (argc !=3)
  {
    std::cout << "usage:\n";
    std::cout << "./median <input dir> <output dir>\n";
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


  std::ofstream wc_result(argv[2]);
  wc_result << std::fixed << std::setprecision(1);
  std::ifstream file;
  std::vector<int> v; // num words .. it will be a sorted vector

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
      int n_words = 0;
      while (is >> word)
      {
        ++n_words;
      }
      insert_sort(v, n_words);
      wc_result << median(v) << std::endl;
    }

    file.close();
  }  

  wc_result.close();

}


