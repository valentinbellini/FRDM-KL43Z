# ESP32 Captive Portal

Este proyecto utiliza una **ESP32** para crear un **portal cautivo** que permite a los invitados conectarse a tu red Wi-Fi a través de un punto de acceso (AP) creado por la ESP32. La configuración básica incluye la conexión en modo dual (AP + STA) y el uso de un servidor web para presentar el portal cautivo.

## Estructura del Proyecto

```plaintext
/ESP32_Captive_Portal/
│
├── /lib/                  
│   ├── ESPAsyncWebServer/  # Librería para manejar el servidor web asíncrono
│   └── AsyncTCP/           # Librería necesaria para ESPAsyncWebServer
│
├── /data/                 
│   ├── index.html          # Página principal del portal cautivo
│   ├── styles.css          # Estilos CSS para el portal
│   └── script.js           # Funcionalidades JS para el portal (futuras implementaciones)
│
├── /src/                  
│   ├── captive_portal.ino  # Archivo principal del proyecto
│   ├── wifi_config.h       # Configuración de la red Wi-Fi (NO SUBIR A GIT)
│   └── wifi_config_example.h # Ejemplo para otros usuarios que deseen configurar su Wi-Fi
│
└── .gitignore              # Archivo que asegura que las credenciales no se suban a Git
└── platformio.ini          # Archivo de configuración de PlatformIO (si usas PlatformIO)
```

## Configuración Inicial

### Clona este repositorio

```bash
git clone https://github.com/tu_usuario/ESP32_Captive_Portal.git
cd ESP32_Captive_Portal
```

### Configura credenciales Wi-Fi
- Copia wifi_config_example.h y renómbralo como wifi_config.h.
- Edita wifi_config.h con las credenciales de tu red Wi-Fi.

```bash
const char* ssid_sta = "YOUR_SSID";
const char* password_sta = "YOUR_PASSWORD";
```

### Sube el código a la ESP32:

- Abre captive_portal.ino en tu IDE.
- Compila y sube el código a la ESP32.

### Accede al Portal Cautivo:

- Conecta un dispositivo al punto de acceso MiPortalAP con la contraseña 12345678.
- Una vez conectado, abre un navegador e ingresa la dirección **http://192.168.4.1** para acceder al portal cautivo.

### Libraries

- https://github.com/me-no-dev/ESPAsyncWebServer
- https://github.com/me-no-dev/AsyncTCP