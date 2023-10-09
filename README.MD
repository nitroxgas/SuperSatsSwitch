# SuperSatsSwitch
**SuperSatsSwitch - An IoT switch to SuperSats.tv or LNPay wallets.**

As you get satoshis from the audience, you may turn on anything you want, including lighting, articulated dolls, and other real-world things, using a switch similar to the BitcoinSwitch.

This is a free and open source project!

## Requeirements

- ESP line of processors
- A relay or other device that could be activated via a digital pin

## Instructions

- Compile and upload the code using VSCode with Platformio;
- Access the device portal with a cellphone or other wifi-enabled device;
    - SSID: SuperSatsSwitch
    - Password: MineYourCoins

- Enter the needed information:
    - Wifi credentials;
    - The LNPay server; (or leave the deault)
    - The LNPay API path; (or leave the default)
    - Your wallet public key; (starts with pak_ )
    - Your wallet read key; (starts with wakr_ )
    - Relay 1 pin; (wich digital pin should be used)
    - Relay 1 Sats; (minimal amount of sats received to activate the realy)
    - Relay 1 Time; (How much time in seconds the relay need to be activated)
    - Relay 1 pulse; (How many times the activation should be repeated - not yet...)
    - Awtrix address; (If you have an Awtrix display, the message could be displayed there; Use IP or .local awtrix name)

The current version was tested with a Nodemcu - ESP8266

TO-DO:
- Polish the code. B'p
- Finish the installation instructions.
- Implement one more relay.
- Polish the documentation.
- Publish a binary and portal for an easier installation process.
- ....


### Heavily inspired by:

- <a href="https://github.com/lnbits/bitcoinswitch">Bitcoin Switch</a>
- <a href="https://github.com/BitMaker-hub/NerdMiner_v2">NerdMinerV2</a>
