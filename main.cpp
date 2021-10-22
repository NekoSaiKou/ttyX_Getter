#include <ttyX_Getter.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
    vector<pair<string,string>> vpsDev = FindDevicesFromFile(string(getenv("PWD"))+"/DeviceMap.cfg");
    cout << "------------ Search Result ------------" << endl;
    for(auto psDev: vpsDev)
    {
        cout << psDev.first << " : " << psDev.second << endl;
    }

    cout << "------------ Search Result2 ------------" << endl;
    string dev = FindDeviceFromResult(vpsDev, string("USB2TTL1"));
    std::cout << "USB2TTL1 : " << dev << endl;
}
