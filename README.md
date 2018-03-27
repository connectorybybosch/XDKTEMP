# XDK sensor de temperatura.

El proposito de este repositorio es el de usar el sensor de temperatura del XDK, obteniendo los datos a través del puerto serial
y graficandolos con ayuda de Python 2.7, tiene la característica que cuando se llega a una temperatura límite envía un tweet de alerta.

# Prerequisitos.

* [Python 2.7](https://www.python.org/downloads/)
* [Como conseguir los Twitter Consumer Keys](http://codygo.es/redes-sociales/conseguir-las-consumer-keys-de-twitter/)
* Habilitar los Tokens de twitter [aquí](https://apps.twitter.com/)
* [Workbench (Windows x64)](https://drive.google.com/open?id=1uuvDiIS0DkNt-izWRVDEpGVKzJ4VjGV7)
* [Workbench (MAC)](https://drive.google.com/open?id=1DgjtimoCSGjLahTD1-V6X2Bso7JbYmEJ)

# Instalando

```
git clone https://github.com/BlackZuN/XDKEjemplos.git
```

```
pip install -r requirements.txt
```

Es importante modificar el documento credenciales.py en el cual tendrá las consumer Keys y el numero de puerto serial de la XDK
```
CONSUMER_KEY = ""
CONSUMER_SECRET =""


ACCESS_TOKEN = ""
ACCESS_TOKEN_SECRET = ""

COM = ""
```

Para los siguientes pasos es necesario abrir el workbench, copiar TemperaturaSerial.c al archivo .c de un proyecto nuevo creado con el mismo nombre del programa, ahi quitar el codigo original y sustituirlo por el actual.

# Ejecutando

Una vez que el workbench se está ejecutando hay que dar click en COM para deshabilitar el puerto serial en el Workbench y posteriormente es necesario ejecutar el script de python.


```
python XDKTEMP.py
```
