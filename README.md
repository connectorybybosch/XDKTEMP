# Sensor temperature on Cross Domain Development Kit

* DESCRIPTION

\tHow to use the temperature sensor BME280 embedded on the XDK (Cross Domain Development Kit), an IoT device developed by Bosch Connected Devices and Solutions. These scripts allow reading over USB port the temperature data and draw a graph with Python 2.7. 

\tIf the value exceeds a predetermined temperature boundary, the program sends a Twitter notification. The temperature limit can be modified using the push-buttons included on the XDK; Button * or 1 increases the boundary, Button ** or 2  decreases the limit.

* OBJECTIVE

\tIntroduction to XDK programming and how to use other programming languages like Python to develop IoT solutions quickly and efficiently in a workshop for Talent Land Guadalajara 2018.

* DEVELOPMENT

\tXDK scripts called "LedsAndButtons" and "extbusTempIOBoard" are combined to generate an only script called "TemperaturaSerial". 

\tThe script sends data temperature every 1 second over USB to a computer and a file on Python called "XDKTEMP" reads the data and automatically and draws a graph on real time.

\tIf the temperature exceeds the boundary, it executes Python script "twitter" and sends a push/notification to the profile that was configured on the Python script "credenciales".

# Requirements

* [Python 2.7](https://www.python.org/downloads/)
* [How to configure your Twitter API and Consumer Keys](http://codygo.es/redes-sociales/conseguir-las-consumer-keys-de-twitter/)
* [Where to configure your Consumer Keys](https://apps.twitter.com/)
* [Workbench (Windows x64)](https://drive.google.com/open?id=1uuvDiIS0DkNt-izWRVDEpGVKzJ4VjGV7)
* [Workbench (MAC)](https://drive.google.com/open?id=1DgjtimoCSGjLahTD1-V6X2Bso7JbYmEJ)

# Downloading and installing libraries and scripts

```
git clone https://github.com/connectorybybosch/XDKTEMP.git
```

```
pip install -r requirements.txt
```

If the libraries do not install or get an error in your CMD window, try by your own the libraries installation:

```
pip install pyserial==3.4
pip install matplotlib
pip install pypiwin32
pip install tweepy
```

It is essential to modify the script "credenciales.py" with your Consumer Keys and Access Tokens between the "" and not leave white spaces in your code, if it happens, the program does not work correctly and the script will not send any error or warning message.

In the COM variable you need to write the name and number of ypur USB port, ypu can see it on your XDK Workbench when you connect your device over USB to your computer.

In the COM variable, you need to write the name and number of your USB port, and you can see it on your XDK Workbench when you connect your device over USB to your computer.

```
CONSUMER_KEY = ""
CONSUMER_SECRET =""


ACCESS_TOKEN = ""
ACCESS_TOKEN_SECRET = ""

COM = ""
```

# Execute

Create a new project on the XDK Workbench called "TemperaturaSerial" and delete all its content and copy the entire script "temperaturaSerial" of this repository in your XDK project.

Compile and flash your XDK device, after you see on XDK Workbench Console the information of temperature, make a click on the green icon located on the right of you XDK image in the Workbench, the figure needs to change to blue.

After that, execute "XDKTEMP.py".

```
python XDKTEMP.py
```

# Enjoy!

Get fun using the buttons and pay attention to your Twitter notifications!
