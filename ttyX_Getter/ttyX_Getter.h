#ifndef TTYX_GETTER_H
#define TTYX_GETTER_H

#include <vector>
#include <utility>
#include <string>
using namespace std;

vector<pair<string,string>> FindDevicesFromFile(string FilePath);
string FindDeviceFromResult(vector<pair<string,string>> vpsDev, string TargetName);

#endif // TTYX_GETTER_H