# 2022-WGCTF-badge-botnet-emulator

This code can be used to emulate a 2022 WatchGuard CTF badge. Badges can cross infect each other as part of the botnet game, using infrared signals to communicate. Cross infection interactions give your badge additional mutations. Mutations unlock additional LED bling modes in the Settings menu. 

I borrowed the botnet cross infection code from the official badge source code, which can be found [here](https://github.com/WatchGuard-Threat-Lab/2022-WGCTF-Badge).

## Instructions

1. To use this code, compile it with VS Code + PlatformIO and upload it to an arduino. I used an Arduino Uno. If you use something else, you may have to adjust the platformio.ini file accordingly. You can use the Arduino IDE as well, but you'll have to track down the library dependencies manually.
2. Connect to the arduino via a serial terminal, the one built-in to PlatformIO works great.
3. The serial terminal will prompt you to enter your own badge's ID, for acknowledgement purposes in the cross infection botnet game. This ID can be found in the Settings > Badge Info menu on your badge. Only enter the first 4 (of 8 total) hex characters into the serial prompt.
4. On your badge, enter the BOTNET menu, then Cross Infect, then OK to begin cross infection.
5. On the serial terminal, hit the enter key to begin cross infection and ack.
6. The arduino will blast out a greeting message from a random badge ID and type, then an ack message destined for the badge ID you previously entered. It will do this 3 times in a row for redundancy.
7. If the greet/ack handshake between badge and arduino were successful, you should see a success message on the badge's screen.
8. The arduino will now pause, waiting for you to hit the space bar.
9. Press OK on the badge, then return to the Cross Infect menu, then press OK again.
10. Press the space bar in your serial terminal to blast out another greet/ack handshake.
11. Repeat steps 6 through 10 as many times as needed to unlock all mutations.

Enjoy the unlocked LED bling!


## Hardware

- Arduino Uno
- Infrared LED: 
`GND --- (-) LED (+) --- 100 Ω resistor --- Pin D3`
