# XDK sensor de temperatura.

El proposito de este repositorio es el de usar el sensor de temperatura del XDK, obteniendo los datos a través del puerto serial
y graficandolos con ayuda de Python 2.7, tiene la característica que cuando se llega a una temperatura límite envía un tweet
de alerta.

# Prerequisitos.

* [Anaconda](https://www.anaconda.com/download/)
* [Twitter Consumer Keys](http://codygo.es/redes-sociales/conseguir-las-consumer-keys-de-twitter/)



# Instalando

```
git clone https://github.com/joakinCC/XDKTEMP.git
```

```
pip install -r requirements.txt
```

Es importante crear el documento credenciales.py en el cual tendrá las consumer Keys
```
CONSUMER_KEY = ""
CONSUMER_SECRET =""


ACCESS_TOKEN = ""
ACCESS_TOKEN_SECRET = ""
```

Para los siguientes pasos es necesario abrir el workbench y el código de prueba ExtensionBusTemperatureIOBoard y sustituir el archivo
ExtensionBusTemperatureIOBoard.c

# Ejecutando

Una vez que el workbench se está ejecutando es necesario ejecutar el script.


```
python XDKTEMP.py
```
