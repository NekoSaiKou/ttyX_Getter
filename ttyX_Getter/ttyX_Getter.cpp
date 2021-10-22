/**
 * @file GetDevice.cpp
 * @author cyyang
 * @brief This program will scan all tty device which is using usb bus, and
 *        print their DEVNAME and several property.
 * 
 */

#include "ttyX_Getter.h"
#include <libudev.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class udev_device
{
    public:
        udev_device(string Dev_name)
        {
            mDev_name = Dev_name;
        }

        void add_property(const string Property_name, const string Property_value)
        {
            pair<string,string> pProperty = make_pair(Property_name, Property_value);
            mvpProperties.push_back(pProperty);
        }

        string mDev_name;
        vector<pair<string,string>> mvpProperties;

    public:
        static void GetDevicesPath(const vector<udev_device*> &vpDev, vector<pair<string,string>> &vpsDevPath)
        {
            vpsDevPath.clear();
            vpsDevPath.reserve(vpDev.size());

            struct udev *udev;
            struct udev_enumerate *enumerate;
            struct udev_list_entry *devices, *dev_list_entry;
            struct udev_device *dev;

            udev = udev_new();

            enumerate = udev_enumerate_new(udev);
            if(!enumerate)
            {
                cout << "[Device Search] No Device Found" << endl;
                return;
            }
            udev_enumerate_add_match_subsystem(enumerate, "tty");
            udev_enumerate_add_match_property(enumerate, "ID_BUS", "usb");
            udev_enumerate_scan_devices(enumerate);
            devices = udev_enumerate_get_list_entry(enumerate);
            if(!devices)
            {
                cout << "[Device Search] No ttyUSB Device Found" << endl;
                return;
            }

            for(int i=0; i < vpDev.size(); i++)
            {
                udev_device* pDev = vpDev[i];
                cout << "[Device Search] Looking for device: " << pDev->mDev_name << " ..."<< endl;
                pair<string,string> pDevice = make_pair(pDev->mDev_name, string(""));
                vpsDevPath.push_back(pDevice);

                bool found_device = false;
                udev_list_entry_foreach(dev_list_entry, devices) 
                {
                    const char *path;
                    path = udev_list_entry_get_name(dev_list_entry);
                    dev = udev_device_new_from_syspath(udev, path);

                    bool matched = true;
                    for(pair<string,string> pProperty : pDev->mvpProperties)
                    {
                        const char *property_value = udev_device_get_property_value(dev, pProperty.first.c_str()); 
                        matched &= property_value && (pProperty.second.compare(string(property_value))==0);
                    }
                    if(matched && pDev->mvpProperties.size() > 0)
                    {
                        found_device = true;
                        const char *devname = udev_device_get_property_value(dev, "DEVNAME");
                        vpsDevPath.back().second = string(devname);
                        break;
                    }
                }
                udev_device_unref(dev);

                if(!found_device)
                {
                    cout << "[Device Search] " << vpsDevPath.back().first << " not found" << endl;
                }
                else
                {
                    cout << "[Device Search] " << vpsDevPath.back().first << " found, DEVNAME: " << vpsDevPath.back().second  << endl;
                }
            }
            udev_enumerate_unref(enumerate);
            udev_unref(udev);
        }

};

/**
 * @brief 
 * 
 * @param FilePath Find devices from the configuration file
 * @return vector<pair<string,string>> 
 */
vector<pair<string,string>> FindDevicesFromFile(string FilePath)
{
    vector<udev_device*> vpDev;
    vector<pair<string,string>> vpsDevPath;

    // Parse File
    ifstream cfg_file(FilePath.c_str());
    string s;
    bool NextFoundDevName = true;
    bool NextSearchProperty = false;
    while (getline (cfg_file, s))
    {
        if (s.rfind("#", 0) == 0) 
        {
            continue;
        }
        if(s.empty())
        {
            NextFoundDevName = true;
            NextSearchProperty = false;
            continue;
        }

        // Start with Device
        if (s.rfind("DeviceName", 0) == 0 && NextFoundDevName)
        {
            // Extract the device
            string sHead, sSplit, sDevname;
            istringstream(s) >> sHead >> sSplit >> sDevname;

            if(!sDevname.empty())
            {
                NextFoundDevName = false;
                NextSearchProperty = true;

                udev_device * dev = new udev_device(sDevname);
                vpDev.push_back(dev);
            }
            continue;
        }

        // Start with Property
        if (s.rfind("Property", 0) == 0 && NextSearchProperty)
        {
            // Extract the property
            string sHead, sSplit, sProperty_name, sSplit2, sProperty_value;
            istringstream(s) >> sHead >> sSplit >> sProperty_name >> sSplit2 >> sProperty_value;
            if(!sProperty_name.empty() && !sProperty_value.empty())
            {
                // Save to vector
                vpDev.back()->add_property(sProperty_name, sProperty_value);
            }
            continue;
        }
    }
    cfg_file.close();
    
    cout << "------------ Load Device cfg ------------" << endl;
    for(int i=0; i < vpDev.size(); i++)
    {
        udev_device * dev = vpDev[i];
        cout << "[Device Search] " << dev->mDev_name << endl;
        for(auto property : dev->mvpProperties)
        {
            cout << "[Device Search] " << property.first << " : " << property.second << endl;
        }
    }
    cout << endl;

    cout << "------------ Search Device ------------" << endl;
    udev_device::GetDevicesPath(vpDev, vpsDevPath);
    cout << endl;
    
    return vpsDevPath;

}

/**
 * @brief Get device using DeviceName in cfg file from the result of @ref FindDevicesFromFile
 * 
 * @param vpsDev 
 * @param TargetName 
 * @return string 
 */
string FindDeviceFromResult(vector<pair<string,string>> vpsDev, string TargetName)
{
    for(auto psDev : vpsDev)
    {
        if(TargetName.compare(psDev.first) == 0)
        {
            return psDev.second;
        }
    }
    return string("");
}
