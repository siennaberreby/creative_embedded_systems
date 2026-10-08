# Module 1

<img width="360" height="640" alt="Project demonstration on the TTGO T-Display" src="https://github.com/user-attachments/assets/acd0f774-4db6-4a92-b4bb-a72422b39b74" />

A generative artwork for the original ESP32 TTGO T-Display: six neon blooms expand over a star field, restarting with new positions, speeds, sizes, and color combinations.

[Design blog and demo video on Notion](https://www.notion.so/3f35804c3855802c9766f99a3ee09156)

## Inspiration and design goals

I am French, I love cooking, and Ratatouille is my favorite movie. The specific inspiration is Anton Ego’s flashback: tasting ratatouille brings him back to his childhood and his mother’s cooking. Its connection between food, memory, and comfort was especially touching to me.

Neon Garden expresses this inspiration through abstract color and movement. Expanding circles suggest a small sensation opening into a larger feeling. A fixed six-color palette and repeated shapes provide continuity, while independently randomized growth creates changing compositions.

## Hardware and software

- Original ESP32 TTGO T-Display with its built-in 135 × 240 TFT screen, used in landscape orientation.
- USB data cable and computer for programming and power.
- Battery connected to the ESP32 for the enclosed installation.
- Arduino IDE with the **esp32 by Espressif Systems** board package.
- **TFT_eSPI by Bodmer**, configured with **Setup25_TTGO_T_Display.h**.

This sketch targets the original board, not the T-Display S3. No external display wiring or sensors are required.

## Files

- [Arduino sketch](neon_garden/neon_garden.ino)
- The demo GIF and envelope photograph are embedded as GitHub attachments.

## Setup and upload

1. Install Arduino IDE and the **esp32 by Espressif Systems** package through Boards Manager. Follow [Espressif’s installation instructions](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html) if the package is not listed.
2. Install **TFT_eSPI by Bodmer** through Library Manager.
3. In the library’s **User_Setup_Select.h**, comment out the default include for **User_Setup.h** and enable **#include <User_Setups/Setup25_TTGO_T_Display.h>**. Keep only one display setup selected. See the [setup selector](https://github.com/Bodmer/TFT_eSPI/blob/master/User_Setup_Select.h). Recheck this selection after library updates.
4. Download this repository and open **neon_garden/neon_garden.ino** in Arduino IDE.
5. Connect the original TTGO T-Display using a USB data cable. Choose **ESP32 Dev Module** and the board’s serial port. Use the board profile’s default settings.
6. Upload the sketch. It should display “NEON GARDEN,” expanding colored circles, gray stars, and “LAB 1 / randomly growing light.” No button press is needed to start.

**Installed library environment:** ESP32 board package 3.3.12 and TFT_eSPI 2.5.43, with Setup25_TTGO_T_Display.h enabled. These versions were checked on the development computer; they are not a claim of a new upload test.

## How it works

- Six Bloom objects store independent positions, radii, speeds, size limits, and palette offsets.
- resetBloom() chooses a new center, a speed of 0.35–0.84 pixels per frame, a size limit of 24–48 pixels, and a starting palette color.
- Each bloom contains up to four rings, spaced seven pixels apart, around a white center.
- Twenty-eight gray stars are generated at startup and remain stationary.
- Each frame is drawn into a 16-bit offscreen sprite, then sent to the display to reduce flicker.
- The minimum frame interval is 33 milliseconds, targeting approximately 30 FPS; actual performance depends on drawing and transfer time.
- Opaque title and caption bands cover circles that cross into the text areas.

The program generates images at runtime rather than replaying stored frames. New random parameters are selected whenever a bloom reaches its limit, and the random sequence is seeded from esp_random() at startup.

## Customization

Edit BLOOM_COUNT to change the number of blooms, PALETTE to change their colors, or the ranges in resetBloom() to change speed and size. Arduino random(min, max) excludes the upper bound. Keep WIDTH and HEIGHT matched to the display orientation.

## Visual and installation documentation

The demo GIF is embedded above, and the [demo video](https://www.youtube.com/watch?v=eXNiJIZaBdE) is embedded in the linked Notion blog.

<img width="540" alt="Hand-decorated envelope enclosing the ESP32, with a rectangular opening for the display" src="https://github.com/user-attachments/assets/0cbc033d-982a-48be-8020-ba776858dd0e" />

The ESP32 is placed inside a hand-decorated paper envelope, with the display aligned behind a rectangular opening. A battery plugged into the ESP32 powers the enclosed device. The envelope’s drawings include cooking utensils, cheese, wine glasses, a French flag, and a rat, connecting the physical presentation to my French background, my love of cooking, and Ratatouille.

This photograph documents the enclosure design with the device inside. It is the available installation photograph; it does not show the device hanging in the exhibition space.
