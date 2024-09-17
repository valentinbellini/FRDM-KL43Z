#ifndef HTML_H
#define HTML_H

const char HTML_FORM[] = R"rawliteral(
<html><body>
  <h1>Ingrese los datos de la red WiFi a conectar</h1>
  <form action="/connect" method="POST">
    SSID: <input type="text" name="ssid"><br>
    Password: <input type="password" name="password"><br>
    Extra Data: <input type="text" name="extraData"><br>
    <input type="submit" value="Conectar">
  </form>
</body></html>
)rawliteral";

#endif
