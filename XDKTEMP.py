import serial
import matplotlib.pyplot as plt
import time
import win32com.client

import twitter as twt


XDKPort = serial.Serial('COM5', 9600, timeout=1)

xdata = []
ydata = []
zdata = []
plt.show()

axes = plt.gca()
axes.set_autoscale_on(True)



for i in range(50):


    data = XDKPort.readline()
    sep = data.split()
    ydata.append(int(sep[0]))
    xdata.append(int(sep[1]))
    zdata.append(int(sep[2]))
    print(sep)

    if int(sep[0]) > 28000:
        line, = axes.plot(xdata, ydata,'r-')
    else:
        line, = axes.plot(xdata, ydata,'b-')
    line.set_xdata(xdata)
    line.set_ydata(ydata)

    axes.relim()
    axes.autoscale_view(True,True,True)
    LIMITE = int(sep[2])
    plt.figtext(0.50, 0.96, ' TEMPERATURA ', fontsize='large', color='R', ha ='center')

    plt.xlabel('TIEMPO(S)')
    #La linea 45 guarda imagen cada paso de tiempo
    #plt.savefig(str(i) + '.png')
    plt.pause(1e-17)
    time.sleep(0.1)

    if int(sep[0]) > int(sep[2]):
        mensaje = 'Tu dispositivo esta en peligro la temperatura es {}' .format(sep[0])
        twt.ejecutar(mensaje)

# add this if you don't want the window to disappear at the end
plt.show()


XDKPort.close()
