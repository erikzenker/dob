#include <iostream>
#include <boost/filesystem.hpp>

void dump(boost::filesystem::path path, int level)
{
    try
    {
        std::cout << (boost::filesystem::is_directory(path) ? 'D' : ' ') << ' ';
        std::cout << (boost::filesystem::is_symlink(path) ? 'L' : ' ') << ' ';
        for(int i = 0; i < level; ++i)
            std::cout << ' ';
	if(boost::filesystem::is_directory(path))
	  std::cout << path.string() << std::endl;
	else if(boost::filesystem::is_regular_file(path))
	  std::cout << path.filename() << std::endl;
	  
        //std::cout << path.parent_path().string() << "/" << path.filename().string() << std::endl;
        //std::cout << path.string() << std::endl;p
    }
    catch(boost::filesystem::filesystem_error& fex)
    {
        std::cout << fex.what() << std::endl;
    }
}

int main(int argc, char **argv){

  if(argc < 2){
    std::cout << "Usage: a.out [path]" << std::endl;
    return 1;
  }

  boost::filesystem::path p(argv[1]);

  if(boost::filesystem::exists(p)){
    if(boost::filesystem::is_directory(p)){
      std::cout << p << " is an directory" << std::endl;

    }
    else if(boost::filesystem::is_regular_file(p)){
      std::cout << p << " is an regular file" << std::endl;

    }
    else{
      std::cout << p << " does exist but is neighter regular file nor directory" << std::endl;

    }

  }
  else{
    std::cout << p << " does not exist" << std::endl;
  }


  dump(p, 0);
  boost::filesystem::recursive_directory_iterator it(p);
  boost::filesystem::recursive_directory_iterator end;
 
  while(it != end) // 2.
    {
      dump(*it, it.level()); // 3.
 
      if(boost::filesystem::is_directory(*it) && boost::filesystem::is_symlink(*it)) // 4.
	it.no_push();
 
      try
        {
	  ++it; // 5.
        }
      catch(std::exception& ex)
        {
	  std::cout << ex.what() << std::endl;
	  it.no_push(); // 6.
	  try { ++it; } catch(...) { std::cout << "!!" << std::endl; return 1; } // 7.
        }
    }


  return 0;
  
}
