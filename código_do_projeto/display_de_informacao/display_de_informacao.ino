#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

char entrada;
const byte numChars = 32;
char receivedChars[numChars];

boolean newData = false;

void setup() {  
    Serial.begin(4800);
    Serial.print("Serial Ready");
    lcd.init(); // Inicia o display
    lcd.backlight(); // liga backlight
    lcd.clear(); // limpa o display
  
}

void loop() {
    if (Serial.available() <= 0){ // verifica se tem comunicação com a serial
        lcd.setCursor(0,0);
        lcd.print("Conectar Serial");
    }
    else { // se tiver comunicação
        recvWithStartEndMarkers(); // recebe os caracteres
        showNewData(); // formata e mostra no display
    }
}

void recvWithStartEndMarkers() { // função pra armazenar os caracteres
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // termina a string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }
        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }  
}

void showNewData() {
    String saida = String(receivedChars); // transforma a lista em uma string

    if (newData == true) {
        int index = saida.indexOf('|');
        // divide a string
        String cpuInfo = "CPU:" + saida.substring(0,index) + "%";
        String ramInfo = "RAM:" + saida.substring(index + 1) + "%";
        // mostra no display
        lcd.setCursor(0,0);
        lcd.print(cpuInfo);
        lcd.setCursor(0,1);
        lcd.print(ramInfo);

        newData = false;
        delay(1000);
        lcd.clear();
    }
}
