#ifndef FILESTATE_H
#define FILESTATE_H

#include <boost/filesystem.hpp>

struct FileState {
 FileState(boost::filesystem::path path, unsigned modtime, unsigned inode, bool is_dir) :
   path(path), modtime(modtime), inode(inode), is_dir(is_dir){
  
  }
  
  FileState():
    path(""), modtime(0), inode(0), is_dir(false){
  }

  boost::filesystem::path path;
  unsigned modtime;
  unsigned inode;
  bool is_dir;


};

// boost::filesystem::path resolvePath(const boost::filesystem::path& p, const boost::filesystem::path& base = boost::filesystem::current_path()){
//     boost::filesystem::path abs_p = boost::filesystem::absolute(p,base);
//     boost::filesystem::path result;
//     for(auto it = abs_p.begin(); it!=abs_p.end(); ++it){
//         if(*it == ".."){
//             // /a/b/.. is not necessarily /a if b is a symbolic link
//             if(boost::filesystem::is_symlink(result) )
//                 result /= *it;
//             // /a/b/../.. is not /a/b/.. under most circumstances
//             // We can end up with ..s in our result because of symbolic links
//             else if(result.filename() == "..")
//                 result /= *it;
//             // Otherwise it should be safe to resolve the parent
//             else
//                 result = result.parent_path();
//         }
//         else if(*it == ".") {
//             // Ignore
//         }
//         else {
//             // Just cat other path entries
//             result /= *it;
//         }
//     }
//     return result;
// }

#endif /* FILESTATE */
