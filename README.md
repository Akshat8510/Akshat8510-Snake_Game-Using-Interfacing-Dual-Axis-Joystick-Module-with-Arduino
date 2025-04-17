Abstract

This project presents a modern take on the classic Snake Game
using Arduino, joystick control, a MAX7219 LED matrix, and
EEPROM for high score tracking. Designed with a humorous
and sarcastic twist, this version of Snake enhances the gaming
experience with savage commentary and pixelated effects
displayed via Serial Monitor and matrix visuals. The game
maintains a playful tone while introducing gradually increasing
difficulty and preserving user performance statistics.

Working Principle

Snake movement is controlled using analog values from the
joystick (X and Y axes). Direction is mapped and updated unless reversing. 
Collision detection with walls (optional) or self ends the game. 
Eating food increases length and optionally speed.
Game over screen is displayed once until reset via button press.

Software Logic

Directional logic prevents reversal. 
Game difficulty increases after every few food collections. 
Serial monitor adds sarcastic feedback like: “ You lost. Again. Classic.”
                                             “ Cannibalism? Snake ate itself.”
                                             “ Use joystick. Don’t be a noob.” 

Challenges Faced

Handling fast joystick response time without delays
Ensuring wall wrap and wall collision logic coexist smoothly
Preventing message spamming on loss
Generating emojis consistently in serial monitor
