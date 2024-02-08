#ifndef SCHERMO_LIB_H
#define SCHERMO_LIB_H
#include <U8g2lib.h>

#define fontSmall u8g2_font_5x7_tf
#define fontHead  u8g2_font_ncenB08_tr
#define fontHello u8g2_font_lubBI19_tf

class My_Screen{
  private:
  public:
    U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2;
    char classe[6];
    My_Screen() : u8g2(U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C(U8G2_R0, SCL, SDA, U8X8_PIN_NONE)) {
    }

    void begin(const char* nome) {
      // Copia il contenuto della stringa costante 'nome' nell'array 'classe'
      strncpy(classe, nome, sizeof(classe) - 1);
      classe[sizeof(classe) - 1] = '\0'; // Assicura la terminazione nulla

      u8g2.begin();
    }


    void InitDisplay(){
      char buf[20];
      u8g2.begin();
      u8g2.setFont(fontHello);
      sprintf (buf, "Ciao %s", classe);
      u8g2.drawStr(0,32, buf);	// write something to the internal memory
      u8g2.sendBuffer();					// transfer internal memory to the display
    }

    void printHum(float hud){
      char buf[20];
      u8g2.setFont(fontSmall);

      sprintf (buf, "%d%s",int(hud), "%");
      u8g2.drawStr(59,30, "   ");
      u8g2.drawStr(59,30,buf);
      u8g2.sendBuffer();
    }

    void printTmp(float tmp){
      char buf[20];
      u8g2.setFont(fontSmall);
      
      sprintf (buf, "%dC",int(tmp));
      u8g2.drawStr(17,30, "   ");
      u8g2.drawStr(17,30,buf);
      u8g2.sendBuffer();
    }

    void printPlant(float plant){
      char buf[20];
      u8g2.setFont(fontSmall);
      
      sprintf (buf, "%d%s",int(plant), "%");
      u8g2.drawStr(101,30, "   ");
      u8g2.drawStr(101,30, buf);
      u8g2.sendBuffer();
    }

    void printBattery(float battery){
      char buf[20];
      u8g2.setFont(fontSmall);
      sprintf (buf, "%d.%d%s", (int)battery, (int)(battery*10)%10 , "V");
      Serial.println(buf);
      u8g2.drawStr(101,8, "      ");
      u8g2.drawStr(101,10, buf);
      u8g2.sendBuffer();
    }

    void printBaseScreen(){
      u8g2.clearBuffer();					// clear the internal memory
      char buf[20];
      sprintf (buf, "Smart plant %s", classe);
      u8g2.setFont(fontHead);	// choose a suitable font
      u8g2.drawStr(0,10, buf);	// write something to the internal memory
      u8g2.setFont(u8g2_font_unifont_t_weather);
      u8g2.drawGlyph(0,32,49);
      u8g2.drawGlyph(42,32,50);
      u8g2.setFont(u8g2_font_unifont_t_77);
      u8g2.drawGlyph(84,32,9969);
      u8g2.sendBuffer();					// transfer internal memory to the display
    }
};
/*
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C defineScreen();
void printInfo(U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2, char classe[4], float tmp, float hud, float plant);
void InitDisplay(U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2, char classe[3]);
void printHum(U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2, float hud);
void printTmp(U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2, float tmp);
void printPlant(U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2, float plant);
void printBattery(U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2, float battery);
*/
#endif