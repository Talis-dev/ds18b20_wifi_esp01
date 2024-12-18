# Sistema de Monitoramento com ESP8266 e Sensores DS18B20

## Descrição do Projeto

Este projeto utiliza um ESP8266 (ESP-01) conectado a um cabo blindado de aproximadamente 130 metros, ao longo do qual estão distribuídos 8 sensores de temperatura DS18B20. O sistema é projetado para realizar medições precisas e comunicar os dados via MQTT para um servidor que plota os valores em um gráfico na web.

O cabo é alimentado por uma fonte integrada para reforçar o sinal ao longo do percurso. Além disso, um potenciômetro foi implementado no circuito do ESP-01 para ajustar o "pull-up" do sinal, permitindo maior estabilidade na comunicação, independentemente do peso do cabo ou da quantidade de sensores conectados.

## Características Principais

1. **Cabo Blindado de 3 vias 1mm² com malha de 130 metros**:
   - Fornece isolamento para comunicação estável em ambientes ruidosos.

2. **Sensores DS18B20**:
   - Conectados ao longo do cabo, permitem medição de temperatura em múltiplos pontos.

3. **Reforço de Sinal**:
   - Fonte de alimentação 3.3v 2A integrada ao cabo para garantir a entrega do sinal por toda a extensão.

4. **Potenciômetro multivoltas 5k Ajustável**:
   - Regula o "pull-up" do sinal, adaptando-se à configuração do sistema (número de sensores ou comprimento do cabo).

5. **Integração MQTT**:
   - Publica os dados dos sensores em um tópico para visualização e análise na web.

## Funcionamento

1. **Inicialização**:
   - O ESP-01 conecta-se a uma rede Wi-Fi especificada e ao servidor MQTT.
   - Detecta e lista os sensores DS18B20 conectados ao cabo.

2. **Leitura de Temperatura**:
   - O ESP-01 solicita leituras de todos os sensores e converte os dados em formato JSON.

3. **Envio de Dados**:
   - Os valores das temperaturas, associados aos endereços dos sensores, são publicados no tópico MQTT `sensors_ds18b20/address`.

4. **Ajuste Dinâmico**:
   - O potenciômetro permite ajustes no "pull-up" do sinal para garantir comunicação estável.

5. **Visualização na Web**:
   - Um servidor MQTT coleta os dados publicados e os plota em gráficos para monitoramento em tempo real.

## Configurações Necessárias

1. **Rede Wi-Fi**:
   - SSID: `nome da rede`
   - Senha: `*********`

2. **Servidor MQTT**:
   - IP: `192.168.1.100`
   - Porta: `1883`

## Código-Fonte

O código-fonte deste projeto está implementado em C++ e pode ser encontrado neste repositório. Ele utiliza as bibliotecas:

- `ESP8266WiFi` para conexão Wi-Fi.
- `WiFiManager` para configurações dinâmicas.
- `PubSubClient` para integração MQTT.
- `ArduinoJson` para manipulação de dados JSON.
- `OneWire` e `DallasTemperature` para comunicação com os sensores DS18B20.

## Dependências

Certifique-se de instalar as seguintes bibliotecas na IDE Arduino:

1. ESP8266WiFi
2. WiFiManager
3. PubSubClient
4. ArduinoJson
5. OneWire
6. DallasTemperature

## Esquema Elétrico

- O cabo blindado conecta os sensores DS18B20 em série.
- A alimentação e o potenciômetro estão localizados próximos ao ESP-01.

## Exemplo de Resultado (JSON Publicado)

```json
{
  "28FF6A4C731603D3": 24.5,
  "28FF6A4C73160456": 25.0,
  "28FF6A4C73160589": 24.8
}
```

## Contribuições

Contribuições são bem-vindas! Envie um pull request com suas melhorias ou ideias para discussão.

## Licença

Este projeto é licenciado sob a Licença MIT. Consulte o arquivo LICENSE para mais detalhes.

