#include <vector>
#include <string>
#include <dbg_print.h>
#include <Profile.h>

class ProfileManager {
 public:
  ProfileManager(std::vector<Profile>* pProfiles);
  ~ProfileManager();
  bool StartProfile(std::string profileName);
  bool StopProfile(std::string profileName);
  bool RestartProfile(std::string profileName);

 private:
  vector<Profile>* mpProfiles;

};
