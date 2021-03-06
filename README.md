# VSTReader
Simple JUCE application to read/print vst's paramenters


![image](https://user-images.githubusercontent.com/55757804/141980044-4914f64c-9230-46be-88c4-e5990f036270.png)

# I/0 configuration
The audio/midi in/out nodes allow to create a channel betwenn audio channel setted in the settings that you can open by right-clicking on a node and the click "show plugin GUI" or "configure audio I/O"

![image](https://user-images.githubusercontent.com/55757804/141977255-eac0a3ba-49f1-48aa-a539-cc466e1a7873.png)

Now you can connect node's end points (called pins) allowing you begin play some music.
Obviouslly you must put some instrument between them to generet music from midi message and then effects and ather to manipulate the sound generated.
The virtual midi keyboard allows you to generate midi messages even if you do not have an external controller directly connected


# Add a plugin
Just right-click on the panel... in this way it will apper a drop-menu that shows you the list of avaiable plugin.
Just click on them to add the selected plugin to the window. (You can have multiple instance of the same plugin)
![image](https://user-images.githubusercontent.com/55757804/141980150-25a96315-1ad1-4e38-95ef-5bf2a98ad2fb.png)


# Scan Plugin
In the main command bar , click on "Plugin" and then "Edit the list of avaiable plugin"... it will apper the list of the current avaiable plugin with some information such as the format, the category ecc...

![image](https://user-images.githubusercontent.com/55757804/141980355-2ea8b519-5698-46cf-9ce2-5c0d3fbc2385.png)

![image](https://user-images.githubusercontent.com/55757804/141980424-e0800584-df64-4cf8-a12a-99e39d0f9d47.png)

Now clicking on "Option" and then "Scan for new or updated VST3 plugin" it will appear another window contains the paths where the application search for VST3 plugin...

![image](https://user-images.githubusercontent.com/55757804/141980685-3cc3abf6-8dc9-4511-8289-8ecfbc82d74f.png)

CLicking on the "+" or "-" buttons you can add/remove those paths.

![image](https://user-images.githubusercontent.com/55757804/141980783-433e4b8b-ff58-400a-a7b8-221448692116.png)

Once upon you have added your local pluing paths , click on "scan" and the application will recognize plugins and add them to the current avaible plugin...
If the operation succeed now you can add the plugin in the same way showed before.

![image](https://user-images.githubusercontent.com/55757804/141980940-2a1f08ea-6f14-4850-b083-77af1106c74a.png)

# Plugin properties
Right-click on plugin will show you a menu that allows you to :
1. Delete the filter 
2. Disconnect all pins
3. Bypass the filter 
4. Show the GUI Plugin
5. Show all plugin's programs
6. Show all plugin's parameters
7. Show degub log
8. ecc...

![image](https://user-images.githubusercontent.com/55757804/141983325-963800f2-01d4-4f12-8dd5-9d31bdc3e565.png)

Focus on the "6" it will show a windows contains all the parameters that you can change by sliders/buttons ( basing on the parameter's type).

![image](https://user-images.githubusercontent.com/55757804/141983184-a9ef9fca-7022-4a90-aaee-8073dfcc2f7d.png)














