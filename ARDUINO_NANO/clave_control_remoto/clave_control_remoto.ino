/*
    Copyright © Daniel T. Borelli <daltomi@disroot.org>
    This software is provided 'as-is', without any express or implied warranty.
    In no event will the authors be held liable for any damages arising from
    the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not claim
      that you wrote the original software. If you use this software in a product,
      an acknowledgment in the product documentation would be appreciated but is
      not required.

    2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/


/* ARDUINO NANO */

/* https://github.com/s00500/CuteBuzzerSounds   */
#include <CuteBuzzerSounds.h>

/* https://github.com/z3t0/Arduino-IRremote     */
#include <IRremote.h>

#define PIN_IR_RECEIVE              11
#define PIN_BUZZER                  8
#define PIN_LED_ROJO_00             (1 << PIND2)
#define PIN_LED_ROJO_01             (1 << PIND3)
#define PIN_LED_ROJO_02             (1 << PIND4)
#define PIN_LED_VERDE               (1 << PIND5)

#define SND_TECLA                   S_DISCONNECTION
#define SND_MAL                     S_OHOOH
#define SND_BIEN                    S_CONNECTION

#define CLAVE_00                    0xA /* tecla 6 \           */
#define CLAVE_01                    0xD /* tecla 8  -> 684     */
#define CLAVE_02                    0x8 /* tecla 4 /           */

#define ENCENDER_LED(L)             PORTD |= ((L))
#define APAGAR_LED(L)               PORTD &= ~((L))

#define ENCENDER_LED_ROJOS          ENCENDER_LED(PIN_LED_ROJO_00 | PIN_LED_ROJO_01 | PIN_LED_ROJO_02)
#define APAGAR_LED_ROJOS            APAGAR_LED(PIN_LED_ROJO_00 | PIN_LED_ROJO_01 | PIN_LED_ROJO_02)

#define ENCENDER_LED_VERDE          ENCENDER_LED(PIN_LED_VERDE)
#define APAGAR_LED_VERDE            APAGAR_LED(PIN_LED_VERDE)

#define TITILAR(ACCION_A,ACCION_B) do {     \
    for (byte i = 0; i < 5; ++i) {          \
        (ACCION_A);                         \
        delay(200);                         \
        (ACCION_B);                         \
        delay(200);                         \
    }                                       \
} while(0)                                  \

/* ------------------------------------------------------------------ */

byte indice = 0;
byte clave[3];
unsigned long tiempo_ms = millis();

/* ------------------------------------------------------------------ */


void setup()
{
#ifdef USB_SERIAL
    Serial.begin(9600);
#endif
    IrReceiver.begin(PIN_IR_RECEIVE, DISABLE_LED_FEEDBACK);
    cute.init(PIN_BUZZER);
    DDRD |= (PIN_LED_ROJO_00 | PIN_LED_ROJO_01 | PIN_LED_ROJO_02 | PIN_LED_VERDE);
    PORTD = 0B0;
}


void reproducir_sonido(byte const snd_id)
{
    IrReceiver.stop();
    cute.play(snd_id);
    delay(10);
    IrReceiver.start();
    IrReceiver.resume();
}


void verificar_clave(void)
{
    bool const coincide = ( (clave[0] == CLAVE_00) &&
                            (clave[1] == CLAVE_01) &&
                            (clave[2] == CLAVE_02) );
    if (coincide) {
        APAGAR_LED_ROJOS;
        reproducir_sonido(SND_BIEN);
        TITILAR(APAGAR_LED_VERDE, ENCENDER_LED_VERDE);
    } else {
        reproducir_sonido(SND_MAL);
        TITILAR(ENCENDER_LED_ROJOS, APAGAR_LED_ROJOS);
    }
}


void loop()
{
    if (IrReceiver.decode()) {

        if ((millis() - tiempo_ms) > 250) {

            reproducir_sonido(SND_TECLA);

            clave[indice] = IrReceiver.decodedIRData.command;

#ifdef USB_SERIAL
            Serial.println(clave[indice], HEX);
#endif
            APAGAR_LED(PIN_LED_VERDE);

            switch(indice) {
            case 0 : ENCENDER_LED(PIN_LED_ROJO_00); break;
            case 1 : ENCENDER_LED(PIN_LED_ROJO_01); break;
            case 2 : {
                    ENCENDER_LED(PIN_LED_ROJO_02);
                    delay(200);
                    verificar_clave();
                    break;
                }
            }
            tiempo_ms = millis();
            (++indice) %= 3;
        }
        IrReceiver.resume();
    }
}
