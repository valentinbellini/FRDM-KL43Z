# Proyecto ESP32: AP+STA+HTTP REQUEST para Consulta de Precios de Criptomonedas

Este proyecto utiliza un ESP32 para crear un Access Point (AP) y permitir que los usuarios se conecten a una red WiFi mediante una interfaz web. Además, el dispositivo se conecta a una red WiFi como estación (STA) y consulta los precios de varias criptomonedas usando la API de CoinGecko. Los precios se muestran en una pantalla OLED SSD1306 y alternan cada 30 segundos.

## Características

- **Modo AP+STA**: El ESP32 actúa como Access Point para permitir la conexión del usuario, y luego como estación (STA) para conectarse a la red WiFi del usuario.
- **Interfaz web simple**: Permite ingresar la SSID y contraseña de la red WiFi desde un formulario HTML.
- **Consultas a la API de CoinGecko**: Se obtienen los precios de criptomonedas como Bitcoin, Ethereum y Solana.
- **Pantalla OLED SSD1306**: Muestra de forma clara y rotativa los precios de las criptomonedas en intervalos de tiempo.

## Requisitos de Hardware

- **ESP32**: Microcontrolador que soporta WiFi.
- **Pantalla OLED SSD1306**: Display I2C para mostrar los datos.
- **Conexión WiFi**: Para que el ESP32 pueda acceder a internet y consultar los precios.

## Dependencias de Software

Instalar las siguientes bibliotecas en el entorno de Arduino:

- **WiFi.h**: Librería para la conexión WiFi del ESP32.
- **Ticker.h**: Para implementar timers basados en interrupciones.
- **ArduinoJson.h**: Librería para manejar respuestas en formato JSON de la API de CoinGecko.
- **Adafruit_SSD1306.h**: Librería para manejar la pantalla OLED.
- **Adafruit_GFX.h**: Librería gráfica auxiliar para el OLED.

## Instalación

1. **Clona el repositorio o descarga el código**:
   ```bash
   git clone https://github.com/usuario/proyecto-esp32-ap-sta.git
   ```
2. Instala las dependencias desde el gestor de bibliotecas de Arduino (o usa `Sketch -> Include Library -> Manage Libraries...`):
    - Busca e instala ArduinoJson.
    - Busca e instala Adafruit SSD1306.
    - Busca e instala Adafruit GFX.
    - Asegúrate de tener la librería WiFi.h para el ESP32.
3. Carga el código en tu ESP32 utilizando el IDE de Arduino.
4. Conecta la pantalla OLED SSD1306 a los pines I2C de tu ESP32:
    - SDA → GPIO 21
    - SCL → GPIO 22

## Estructura del proyecto
/proyecto-esp32-ap-sta/
│
├── /src/                  # Código fuente del proyecto
│   ├── main.ino           # Código principal
│   ├── wifi_manager.h     # Funciones de manejo WiFi (AP + STA)
│   ├── crypto.h           # Funciones de consulta a la API de CoinGecko
│   ├── ssd1306.h          # Funciones para controlar la pantalla OLED
│   └── html.h             # Formulario HTML para la configuración de WiFi
│
├── README.md              # Documentación del proyecto
└── LICENSE                # Licencia del proyecto

## API Utilizada
CoinGecko API: https://www.coingecko.com/es/api

### Limites de la API
La API gratuita permite realizar hasta 100 consultas por minuto.


